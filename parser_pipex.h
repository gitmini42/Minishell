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

size_t ft_strlen(const char *str);
void ft_putchar_fd(char c, int fd);
void ft_putstr_fd(char *s, int fd);
char *ft_substr(const char *s, unsigned int start, size_t len);
char **parse_command(const char *input);
char	*find_command_path(char *command, char *const envp[]);
char** parse_command_line(char** input);
void execve_args_with_pipe(char **args, char *command_path, char *const envp[], int *pipefd);

#endif
