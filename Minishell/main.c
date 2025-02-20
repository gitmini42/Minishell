#include "parser_pipex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

void	sigint_handler()
{
    rl_replace_line("", 0);
    printf("\n");
    rl_on_new_line();
    rl_redisplay();
    fflush(stdout);
}

/* void	sigquit_handler()
{
    printf("\nSIGQUIT received. Exiting minishell...\n");
    exit(0);
} */

void	parse_and_validate_input(const char *input, char *const envp[])
{
    CommandData data;
    char **args2;
    int count;
    int i;

    args2 = parse_command(input);
    if (args2 == NULL)
	{
        fprintf(stderr, "Error: Failed to parse input\n");
        return;
    }
    count = 0;
    while (args2[count] != NULL)
        count++;
    parse_input(args2, count, &data);
    execute_commands(&data, envp);
    i = 0;
    while (args2[i] != NULL)
	{
        free(args2[i]);
        i++;
    }
    free(args2);
    free_command_data(&data);
}

int	main(int argc, char *argv[], char *envp[])
{
    (void)argc;
    (void)argv;
    char *input;
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) == -1)
	{
        perror("sigaction");
        exit(1);
    }
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
        perror("sigaction");
        exit(1);
    }
    while (1)
	{
        input = readline("minishell> ");
        if (!input)
		{
            printf("Exiting minishell...\n");
            break;
        }

        if (*input == '\0')
		{
            free(input);
            continue;
        }
        if (strcmp(input, "exit") == 0)
		{
            free(input);
            break;
        }
        parse_and_validate_input(input, envp);
        add_history(input);
        free(input);
    }
    return (0);
}
