#include "explorer.h"

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "dirent.h"
#include "esp_console.h"
#include "esp_log.h"
#include "freertos/task.h"

#include "arguments.h"
#include "colortext.h"

#define EXPLORER_MAX_PATH_LENGTH 128
#define EXPLORER_MAX_CAT_LENGTH_KB 10
#define EXPLORER_TEXT_DIR COLORTEXT_BLUE
#define EXPLORER_TEXT_FILE COLORTEXT_YELLOW

typedef uint32_t filesize_t;

const char *EX_TAG = "explorer";
static sdmmc_card_t *mounted_card;
char *current_path = NULL;

bool is_valid_dir(const char *path) {
  ESP_LOGD(EX_TAG, "Checking if %s is a valid directory", path);
  // If the path ends with a slash, remove it
  int path_len = strlen(path);

  char *path_copy;
  if (path[path_len - 1] == '/') {
    path_copy = strndup(path, path_len - 1);
  } else {
    path_copy = strdup(path);
  }

  DIR *dir = opendir(path_copy);
  if (dir == NULL) {
    ESP_LOGD(EX_TAG, "Directory was not valid");
    free(path_copy);
    return false;
  }
  ESP_LOGD(EX_TAG, "Directory was valid");
  closedir(dir);
  free(path_copy);
  return true;
}

bool is_valid_file(const char *path) {
  // Check if the file exists
  ESP_LOGD(EX_TAG, "Checking if %s exists", path);
  if (access(path, F_OK) == 0) {
    ESP_LOGD(EX_TAG, "File exists");
    return true;
  } else {
    ESP_LOGD(EX_TAG, "File does not exist");
    return false;
  }
}

filesize_t get_file_size(const char *path) {
  struct stat st;
  if (stat(path, &st) == 0) {
    return (filesize_t)st.st_size;
  } else {
    return 0;
  }
}

char *file_explorer_absolute_directory(const char *path) {
  ESP_LOGD(EX_TAG, "Current path %s", current_path);
  char *new_path = malloc(sizeof(char) * EXPLORER_MAX_PATH_LENGTH);
  if (strncmp(path, "/", 1) == 0) {
    strcpy(new_path, path);
  } else if (strcmp(path, "..") == 0) {
    // TODO(jolonb): allow going up multiple directories
    // Go up one directory
    strcpy(new_path, current_path);
    int path_length = strlen(new_path);
    if (path_length <= 1) {
      strcpy(new_path, "/");
    } else {
      // If the last character is a slash, we should remove it
      if (new_path[path_length - 1] == '/') {
        new_path[path_length - 1] = '\0';
      }
      // Find the last slash
      char *last_slash = strrchr(new_path, '/');
      // If there is no slash, or the slash is the first character, we are
      // at the root
      if (last_slash == NULL || last_slash == new_path) {
        strcpy(new_path, "/");
      } else {
        *last_slash = '\0';
      }
    }
  } else if (strcmp(path, ".") == 0) {
    strcpy(new_path, current_path);
  } else {
    // Append path to current path
    strcpy(new_path, current_path);
    // Add a slash to the path if there isn't already one
    const int len = strlen(current_path);
    if (len > 0 && new_path[len - 1] != '/') {
      strcat(new_path, "/");
    }
    strcat(new_path, path);
  }

  // If the path ends with a slash, remove it, unless the path is the root
  int path_len = strlen(new_path);
  if (path_len > 1 && new_path[path_len - 1] == '/') {
    new_path[path_len - 1] = '\0';
  }
  return new_path;
}

void set_path(const char *path) {
  strcpy(current_path, path);
  // If the path doesn't end with a slash, add one
  const int len = strlen(current_path);
  if (len > 0 && current_path[len - 1] != '/') {
    strcat(current_path, "/");
  }
}

void file_explorer_init(sdmmc_card_t *card, const char *default_path) {
  mounted_card = card;
  current_path = malloc(sizeof(char) * EXPLORER_MAX_PATH_LENGTH);

  // If no path is specified, use the root directory
  if (default_path != NULL) {
    set_path(default_path);
  } else {
    set_path("/");
  }

  ESP_LOGI(EX_TAG, "File explorer initialised with root at %s", current_path);
}

void file_explorer_run(void) {
  ESP_LOGI(EX_TAG, "Starting explorer");

  esp_console_repl_t *repl = NULL;
  esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
  esp_console_dev_uart_config_t uart_config =
      ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
  esp_console_new_repl_uart(&uart_config, &repl_config, &repl);

  file_explorer_console_init();
  for (int8_t i = 0; i < file_explorer_console_cmds_len; i++) {
    esp_console_cmd_register(&file_explorer_console_cmds[i]);
  }
  esp_console_register_help_command();

  esp_console_start_repl(repl);
}

static int file_explorer_ls(int argc, char **argv) {
  int nerrors = arg_parse(argc, argv, (void **)&file_explorer_console_ls_args);
  if (nerrors != 0) {
    arg_print_errors(stderr, file_explorer_console_ls_args.end, argv[0]);
    return 1;
  }

  // Set the path to the stored path only if one path was given
  const char *path;
  if (file_explorer_console_ls_args.directory->count == 1) {
    path = file_explorer_console_ls_args.directory->sval[0];
  } else {
    path = current_path;
  }

  // Get absolute path
  char *absolute_path = file_explorer_absolute_directory(path);
  ESP_LOGI(EX_TAG, "Listing %s\n", absolute_path);

  // Check if path is valid
  if (is_valid_dir(absolute_path)) {
    // List files
    DIR *dir = opendir(absolute_path);
    struct dirent *ent;
    char *file_name;
    file_name =
        malloc(sizeof(char) * EXPLORER_MAX_PATH_LENGTH + COLORTEXT_MAX_LENGTH);
    while ((ent = readdir(dir)) != NULL) {
      if (ent->d_type == DT_DIR) {
        colortext_color_str(ent->d_name, COLORTEXT_BLUE, file_name);
        printf("%s/\n", file_name);
      } else {
        colortext_color_str(ent->d_name, COLORTEXT_YELLOW, file_name);
        printf("%s\n", file_name);
      }
    }
    // Clean up
    closedir(dir);
    free(absolute_path);
    return 0;
  } else {
    ESP_LOGW(EX_TAG, "Path %s is not a valid directory", absolute_path);
    free(absolute_path);
    return 1;
    // TODO(jolon): allow ls of files
  }
}

static int file_explorer_cd(int argc, char **argv) {
  int nerrors = arg_parse(argc, argv, (void **)&file_explorer_console_cd_args);
  if (nerrors != 0) {
    arg_print_errors(stderr, file_explorer_console_cd_args.end, argv[0]);
    return 1;
  }
  // Get requested directory
  const char *directory = file_explorer_console_cd_args.directory->sval[0];
  // Get absolute directory
  char *absolute_path = file_explorer_absolute_directory(directory);

  // Check if directory is valid
  if (!is_valid_dir(absolute_path)) {
    ESP_LOGW(EX_TAG, "Failed to open directory at %s", absolute_path);
    free(absolute_path);
    return 1;
  }
  // Change to directory if it is valid
  ESP_LOGI(EX_TAG, "Changing directory to %s", absolute_path);
  set_path(absolute_path);
  free(absolute_path);
  return 0;
}

static int file_explorer_pwd(int argc, char **argv) {
  printf("%s\n", current_path);
  return 0;
}

int explorer_mkdir(char *path) {
  // Check if directory is valid
  if (is_valid_dir(path)) {
    return 1;
  }
  // Create directory if it is valid
  ESP_LOGI(EX_TAG, "Creating directory %s", path);
  if (mkdir(path, 0777) != 0) {
    return 2;
  }
  return 0;
}

int explorer_mkdir_recursive(char *path) {
  bool success = false;

  if (strlen(path) == 0) {
    return 1;
  }

  // Start at 1 because the first char should always be a /
  int i = 1;
  while (*(path + i) != '\0') {
    // If we find a slash, create a directory
    if (*(path + i) == '/') {
      // Set the slash to a string terminator
      *(path + i) = '\0';
      // Create the directory
      if (explorer_mkdir(path) == 0) {
        success = true;
      } else if (success) {
        // If the directory was partially created, but the rest of the
        // path failed, return an error
        return 1;
      }
      // Add the slash back to the path
      *(path + i) = '/';
    }
    i++;
  }

  // Check if the directory was created
  if (success) {
    return 0;
  } else {
    return 1;
  }
}

static int file_explorer_mkdir(int argc, char **argv) {
  int nerrors =
      arg_parse(argc, argv, (void **)&file_explorer_console_mkdir_args);
  if (nerrors != 0) {
    arg_print_errors(stderr, file_explorer_console_mkdir_args.end, argv[0]);
    return 1;
  }
  // Get requested directory
  const char *directory = file_explorer_console_mkdir_args.directory->sval[0];

  // Get absolute directory
  char *absolute_path = file_explorer_absolute_directory(directory);

  int result = -1;
  if (file_explorer_console_mkdir_args.recursive->count > 0) {
    result = explorer_mkdir_recursive(absolute_path);
    if (result == 1) {
      ESP_LOGW(EX_TAG, "Failed to create directory %s", absolute_path);
    }
  } else {
    result = explorer_mkdir(absolute_path);
    if (result == 1) {
      ESP_LOGW(EX_TAG, "Directory %s already exists", absolute_path);
    } else if (result == 2) {
      ESP_LOGW(EX_TAG,
               "Failed to create directory %s."
               " Try specifying the --parents flag.",
               absolute_path);
    }
  }

  free(absolute_path);
  return result;
}

static int file_explorer_rm(int argc, char **argv) {
  int nerrors = arg_parse(argc, argv, (void **)&file_explorer_console_rm_args);
  if (nerrors != 0) {
    arg_print_errors(stderr, file_explorer_console_rm_args.end, argv[0]);
    return 1;
  }

  // Get requested file
  const char *file = file_explorer_console_rm_args.file->sval[0];

  // Get absolute file path
  char *absolute_path = file_explorer_absolute_directory(file);

  // Delete the file
  ESP_LOGI(EX_TAG, "Deleting file %s", absolute_path);
  if (remove(absolute_path) != 0) {
    ESP_LOGW(EX_TAG, "Failed to delete file %s", absolute_path);
    free(absolute_path);
    return 1;
  }
  free(absolute_path);
  return 0;
}

static int file_explorer_touch(int argc, char **argv) {
  int nerrors =
      arg_parse(argc, argv, (void **)&file_explorer_console_touch_args);
  if (nerrors != 0) {
    arg_print_errors(stderr, file_explorer_console_touch_args.end, argv[0]);
    return 1;
  }
  // Get requested file
  const char *file = file_explorer_console_touch_args.file->sval[0];

  // Get absolute file path
  char *absolute_path = file_explorer_absolute_directory(file);

  // Create an empty file at absolute_path
  ESP_LOGI(EX_TAG, "Creating file %s", absolute_path);
  FILE *f = fopen(absolute_path, "a");
  if (f == NULL) {
    ESP_LOGW(EX_TAG, "Failed to create file %s", absolute_path);
    free(absolute_path);
    return 1;
  }

  fclose(f);
  free(absolute_path);
  return 0;
}

static int file_explorer_cat(int argc, char **argv) {
  int nerrors = arg_parse(argc, argv, (void **)&file_explorer_console_cat_args);
  if (nerrors != 0) {
    arg_print_errors(stderr, file_explorer_console_cat_args.end, argv[0]);
    return 1;
  }
  // Check if max_size flag is set
  int max_size = EXPLORER_MAX_CAT_LENGTH_KB;
  if (file_explorer_console_cat_args.max_size->count == 1) {
    max_size = file_explorer_console_cat_args.max_size->ival[0];
  }

  // Get requested file
  const char *file = file_explorer_console_cat_args.file->sval[0];

  // Get absolute file path
  char *absolute_path = file_explorer_absolute_directory(file);

  // Open the file
  filesize_t file_len;
  filesize_t i = 0;
  FILE *f = fopen(absolute_path, "rb");
  if (f == NULL) {
    ESP_LOGW(EX_TAG, "Failed to open file %s", absolute_path);
    free(absolute_path);
    return 1;
  }

  // Get file length
  file_len = get_file_size(absolute_path);

  // Check if file is too large
  if (file_len > 1024 * max_size) {
    ESP_LOGW(EX_TAG,
             "File %s is more than %i KB so will not be displayed."
             " Increase this by setting the --max-size flag.",
             absolute_path, max_size);
    fclose(f);
    free(absolute_path);
    return 1;
  }

  // Print each character
  while (i < file_len) {
    signed char c = fgetc(f);
    printf("%c", c);
    i++;
  }

  // Clean up
  fclose(f);
  free(absolute_path);

  return 0;
}

int write_to_file(FILE *input, FILE *output, filesize_t max_size) {
  filesize_t i = 0;
  // Print each character
  while (i < max_size) {
    fputc(fgetc(input), output);
    i++;
  }

  return 0;
}

static int file_explorer_copy(int argc, char **argv) {
  int nerrors =
      arg_parse(argc, argv, (void **)&file_explorer_console_copy_args);
  if (nerrors != 0) {
    arg_print_errors(stderr, file_explorer_console_copy_args.end, argv[0]);
    return 1;
  }
  // Get requested file
  const char *file = file_explorer_console_copy_args.src->sval[0];
  // Get requested destination
  const char *dest = file_explorer_console_copy_args.dest->sval[0];

  // Get absolute file path
  char *absolute_path = file_explorer_absolute_directory(file);
  // Get absolute destination path
  char *absolute_dest = file_explorer_absolute_directory(dest);

  // Read the contents of the file and write them to the destination
  ESP_LOGI(EX_TAG, "Copying file %s to %s", absolute_path, absolute_dest);
  FILE *f_src = fopen(absolute_path, "rb");
  if (f_src == NULL) {
    ESP_LOGW(EX_TAG, "Failed to open source file %s", absolute_path);
    free(absolute_path);
    free(absolute_dest);
    return 1;
  }
  FILE *f_dest = fopen(absolute_dest, "wb");
  if (f_dest == NULL) {
      ESP_LOGW(EX_TAG, "Failed to open destination file %s", absolute_dest);
    free(absolute_path);
    free(absolute_dest);
    fclose(f_src);
    return 1;
  }

  // Get file length
  filesize_t file_len = get_file_size(absolute_path);
  filesize_t i = 0;

  // Print each character
  while (i < file_len) {
    fputc(fgetc(f_src), f_dest);
    i++;
  }

  free(absolute_path);
  free(absolute_dest);
  fclose(f_src);

  // Let the user know if the buffer couldn't be flushed
  if (fflush(f_dest) != 0) {
    ESP_LOGW(EX_TAG,
             "Failed to flush file buffer for %s."
             " You may not have enough space on the device.",
             absolute_dest);
    fclose(f_dest);
    return 1;
  }

  fclose(f_dest);

  return 0;
}

static int file_explorer_move(int argc, char **argv) {
  int nerrors =
      arg_parse(argc, argv, (void **)&file_explorer_console_move_args);
  if (nerrors != 0) {
    arg_print_errors(stderr, file_explorer_console_move_args.end, argv[0]);
    return 1;
  }
  // Get requested file
  const char *file = file_explorer_console_move_args.src->sval[0];
  // Get requested destination
  const char *dest = file_explorer_console_move_args.dest->sval[0];

  // Get absolute file path
  char *absolute_path = file_explorer_absolute_directory(file);
  // Get absolute destination path
  char *absolute_dest = file_explorer_absolute_directory(dest);

  // Move the file
  ESP_LOGI(EX_TAG, "Moving file %s to %s", absolute_path, absolute_dest);
  if (rename(absolute_path, absolute_dest) != 0) {
    ESP_LOGW(EX_TAG, "Failed to move file %s to %s", absolute_path,
             absolute_dest);
    free(absolute_path);
    free(absolute_dest);
    return 1;
  }

  return 0;
}

static int file_explorer_stat(int argc, char **argv) {
  int nerrors =
      arg_parse(argc, argv, (void **)&file_explorer_console_stat_args);
  if (nerrors != 0) {
    arg_print_errors(stderr, file_explorer_console_stat_args.end, argv[0]);
    return 1;
  }
  // Get requested file
  const char *file = file_explorer_console_stat_args.file->sval[0];

  // Get absolute file path
  char *absolute_path = file_explorer_absolute_directory(file);

  // Get file stats
  struct stat s;
  if (stat(absolute_path, &s) != 0) {
    ESP_LOGW(EX_TAG, "Failed to get stats for file %s", absolute_path);
    free(absolute_path);
    return 1;
  }

  // Print file stats
  ESP_LOGI(EX_TAG, "File %s", absolute_path);
  ESP_LOGI(EX_TAG, "  Size: %u bytes", (filesize_t) s.st_size);
  ESP_LOGI(EX_TAG, "  Permissions: %o", (int)s.st_mode);
  ESP_LOGI(EX_TAG, "  Owner: %d", (int)s.st_uid);
  ESP_LOGI(EX_TAG, "  Group: %d", (int)s.st_gid);
  // We remove the line break character to make things tidier
  char *last_time = malloc(64);
  ctime_r(&s.st_mtime, last_time);
  last_time[strlen(last_time) - 1] = '\0';
  ESP_LOGI(EX_TAG, "  Last modified: %s", last_time);
  ctime_r(&s.st_atime, last_time);
  last_time[strlen(last_time) - 1] = '\0';
  ESP_LOGI(EX_TAG, "  Last accessed: %s", last_time);
  ctime_r(&s.st_ctime, last_time);
  last_time[strlen(last_time) - 1] = '\0';
  ESP_LOGI(EX_TAG, "  Last changed: %s", last_time);

  free(absolute_path);

  return 0;
}
