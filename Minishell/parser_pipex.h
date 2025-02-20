#ifndef PARSER_PIPEX_H
#define PARSER_PIPEX_H

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
# include "libft/libft.h"

typedef struct {
    char **commands;      // Array of commands
    char ***arguments;    // Array of argument arrays
    int num_commands;     // Number of commands
    int num_pipes;        // Number of pipes
    char *input_file;     // Input redirection file
    char *output_file;    // Output redirection file
    int append_output;    // Flag for append mode (>>)
} CommandData;

size_t ft_strlen(const char *str);
void ft_putchar_fd(char c, int fd);
void ft_putstr_fd(char *s, int fd);
char *ft_substr(const char *s, unsigned int start, size_t len);
char **parse_command(const char *input);
char** parse_command_line(char** input);
void parse_input(char **tokens, int count, CommandData *data);
void free_command_data(CommandData *data);
void execute_commands(CommandData *data, char *const envp[]);
char	*find_command_path(char *command, char *const envp[]);
#endif