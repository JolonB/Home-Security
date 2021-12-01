
#ifndef COMPONENTS_FILE_MANAGE_ARGUMENTS_H_
#define COMPONENTS_FILE_MANAGE_ARGUMENTS_H_

#include "esp_console.h"
#include "argtable3/argtable3.h"

/* Function declarations */
static int file_explorer_ls(int, char**);
static int file_explorer_cd(int, char**);
static int file_explorer_pwd(int, char**);
static int file_explorer_mkdir(int, char**);
static int file_explorer_rm(int, char**);
static int file_explorer_touch(int, char**);
static int file_explorer_cat(int, char**);
static int file_explorer_copy(int, char**);
static int file_explorer_move(int, char**);
static int file_explorer_stat(int, char**) 


static struct {
    struct arg_str *directory;
    struct arg_end *end;
} file_explorer_console_ls_args;

static struct {
    struct arg_str *directory;
    struct arg_end *end;
} file_explorer_console_cd_args;

static struct {
    struct arg_end *end;
} file_explorer_console_pwd_args;

static struct {
    struct arg_str *directory;
    struct arg_lit *recursive;
    struct arg_end *end;
} file_explorer_console_mkdir_args;

static struct {
    struct arg_str *file;
    struct arg_end *end;
} file_explorer_console_rm_args;

static struct {
    struct arg_str *file;
    struct arg_end *end;
} file_explorer_console_touch_args;

static struct {
    struct arg_str *file;
    struct arg_end *end;
} file_explorer_console_cat_args;

static struct {
    struct arg_str *src;
    struct arg_str *dest;
    struct arg_end *end;
} file_explorer_console_copy_args;

static struct {
    struct arg_str *src;
    struct arg_str *dest;
    struct arg_end *end;
} file_explorer_console_move_args;

static struct {
    struct arg_str *file;
    struct arg_end *end;
} file_explorer_console_stat_args;

static void file_explorer_console_init() {
    file_explorer_console_ls_args.directory = arg_str0(NULL, NULL, "<file>", "Directory to read");
    file_explorer_console_ls_args.end = arg_end(2);

    file_explorer_console_cd_args.directory = arg_str1(NULL, NULL, "<directory>", "Directory to change to");
    file_explorer_console_cd_args.end = arg_end(2);

    file_explorer_console_pwd_args.end = arg_end(1);

    file_explorer_console_mkdir_args.directory = arg_str1(NULL, NULL, "<directory>", "Directory to create");
    file_explorer_console_mkdir_args.recursive = arg_lit0("p", "parents", "Create directories recursively");
    file_explorer_console_mkdir_args.end = arg_end(2);

    file_explorer_console_rm_args.file = arg_str1(NULL, NULL, "<file>", "File to remove");
    file_explorer_console_rm_args.end = arg_end(2);

    file_explorer_console_touch_args.file = arg_str1(NULL, NULL, "<file>", "File to create");
    file_explorer_console_touch_args.end = arg_end(2);

    file_explorer_console_cat_args.file = arg_str1(NULL, NULL, "<file>", "File to read");
    file_explorer_console_cat_args.end = arg_end(2);
}


const esp_console_cmd_t file_explorer_console_cmds[] = {
    {
        .command = "ls",
        .help = "list directory contents",
        .hint = NULL,
        .func = &file_explorer_ls,
        .argtable = &file_explorer_console_ls_args,
    },
    {
        .command = "cd",
        .help = "change the working directory",
        .hint = NULL,
        .func = &file_explorer_cd,
        .argtable = &file_explorer_console_cd_args,
    },
    {
        .command = "pwd",
        .help = "print name of current/working directory",
        .hint = NULL,
        .func = &file_explorer_pwd,
        .argtable = &file_explorer_console_pwd_args,
    },
    {
        .command = "mkdir",
        .help = "make directories",
        .hint = NULL,
        .func = &file_explorer_mkdir,
        .argtable = &file_explorer_console_mkdir_args,
    },
    {
        .command = "rm",
        .help = "remove files or directories",
        .hint = NULL,
        .func = &file_explorer_rm,
    },
    {
        .command = "touch",
        .help = "change file timestamps",
        .hint = NULL,
        .func = &file_explorer_touch,
    },
    {
        .command = "cat",
        .help = "concatenate files and print on the standard output",
        .hint = NULL,
        .func = &file_explorer_cat,
    },
    {
        .command = "cp",
        .help = "copy files and directories",
        .hint = NULL,
        .func = &file_explorer_copy,
    },
    {
        .command = "mv",
        .help = "move (rename) files",
        .hint = NULL,
        .func = &file_explorer_move,
    },
    {
        .command = "stat",
        .help = "get file information",
        .hint = NULL,
        .func = &file_explorer_stat,
    },
};

short file_explorer_console_cmds_len = sizeof(file_explorer_console_cmds) / sizeof(file_explorer_console_cmds[0]);

#endif // COMPONENTS_FILE_MANAGE_ARGUMENTS_H_