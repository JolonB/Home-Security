#include "explorer.h"
#include "arguments.h"

#include <stdio.h>
#include <string.h>
#include "esp_console.h"
#include "esp_log.h"
#include "dirent.h"

const char *EX_TAG = "explorer";
static sdmmc_card_t* mounted_card;


void file_explorer_init(sdmmc_card_t* card) {
    mounted_card = card;
}

void file_explorer_run(void) {
    ESP_LOGI(EX_TAG, "Starting explorer");
    // Request user input and parse it

    // esp_console_register_help_command();
    esp_console_repl_t *repl = NULL;
    esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
    esp_console_dev_uart_config_t uart_config = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
    esp_console_new_repl_uart(&uart_config, &repl_config, &repl);

    file_explorer_console_init();
    for (int i = 0; i < file_explorer_console_cmds_len; i++) {
        esp_console_cmd_register(&file_explorer_console_cmds[i]);
    }
    esp_console_register_help_command();

    esp_console_start_repl(repl);

    ESP_LOGI(EX_TAG, "Ending explorer");
}

static int file_explorer_ls(int argc, char **argv) {
    int nerrors = arg_parse(argc, argv, (void **) &file_explorer_console_ls_args);
    printf("Hello World!\nErrors: %d\n", nerrors);
    printf("File: %s\n", file_explorer_console_ls_args.file->sval[0]);
    return 0;
    /*
    if (mounted_card == NULL) {
        ESP_LOGE(EX_TAG, "Card not mounted");
        return NULL;
    }
    DIR* dir = opendir(directory_path);
    if (dir == NULL) {
        ESP_LOGE(EX_TAG, "Failed to open directory");
        return NULL;
    }
    struct dirent* entry;
    char* result = malloc(sizeof(char) * 1024);
    strcpy(result, "");
    while ((entry = readdir(dir)) != NULL) {
        strcat(result, entry->d_name);
        strcat(result, "\n");
    }
    closedir(dir);
    return result;
    */
}

static int file_explorer_cd(int argc, char** argv) {return 0;}
static int file_explorer_pwd(int argc, char** argv) {return 0;}
static int file_explorer_mkdir(int argc, char** argv) {return 0;}
static int file_explorer_rmdir(int argc, char** argv) {return 0;}
static int file_explorer_rm(int argc, char** argv) {return 0;}
static int file_explorer_touch(int argc, char** argv) {return 0;}
static int file_explorer_cat(int argc, char** argv) {return 0;}
static int file_explorer_copy(int argc, char** argv) {return 0;}
static int file_explorer_move(int argc, char** argv) {return 0;}
static int file_explorer_rename(int argc, char** argv) {return 0;}
