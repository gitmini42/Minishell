#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

int g_exit_status = 0;

char	*ft_strndup(const char *src, size_t n)
{
	size_t	len;
	char	*dest;
	size_t	i;

	len = 0;
	i = 0;
	while (len < n && src[len] != '\0')
		len++;
	dest = malloc(len + 1);
	if (!dest)
		return NULL;
	while (i < len)
	{
		dest[i] = src[i];
		i++;
	}
	dest[len] = '\0';
	return (dest);
}

char	*ft_strcpy(char *dest, const char *src)
{
	char	*original_dest;

	original_dest = dest;
	while (*src != '\0')
	{
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
	return (original_dest);
}

void	sigint_handler(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    if (rl_done == 0)
        rl_redisplay();
}

/* void	sigquit_handler()
{
    printf("\nSIGQUIT received. Exiting minishell...\n");
    exit(0);
} */
void	free_args(char **args2, CommandData *data)
{
	int	i;

	i = 0;
	while (args2[i] != NULL)
	{
		free(args2[i]);
		i++;
	}
	free(args2);
	free_command_data(data);
}

void	parse_and_validate_input(const char *input, char *const envp[])
{
	CommandData	data;
	char		**args2;
	int			count;

	args2 = parse_command(input);
	if (args2 == NULL)
	{
		write(STDERR_FILENO, "Error: Failed to parse input\n", 29);
		return;
	}
	count = 0;
	while (args2[count] != NULL)
		count++;
	parse_input(args2, count, &data);
	execute_commands(&data, envp);
	free_args(args2, &data); //add funcao para reduzir as linhas..
}

char	**copy_envp(char **envp)
{
	int		i;
	char	**new_envp;

	i = 0;
	while (envp[i])
		i++;
	new_envp = malloc((i + 1) * sizeof(char *));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		new_envp[i] = ft_strdup(envp[i]);
		i++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}
void	free_envp(char **envp)
{
	int	i;

	if (envp == NULL)
		return ;
	i = 0;
	while (envp[i] != NULL)
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

int	main(int argc, char *argv[], char *envp[])
{
    (void)argc;
    (void)argv;
	char **my_envp = copy_envp(envp);
    char *input;
    struct sigaction sa;

    ft_memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGINT, &sa, NULL) == -1)
	{
        perror("sigaction");
        exit(1);
    }
    ft_memset(&sa, 0, sizeof(sa));
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
            //printf("Exiting minishell...\n");
            break;
        }
        if (*input == '\0' || strcmp(input, "exit") == 0)
		{
            free(input);
            continue;
        }
        parse_and_validate_input(input, my_envp);
        add_history(input);
        free(input);
    }
	free_envp(my_envp);
    return (0);
}
