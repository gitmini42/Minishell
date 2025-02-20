#include "parser_pipex.h"

static void	print_command_info(char **args, char *command_path)
{
	int	i;

	printf("valid command: %s\n", args[0]);
	printf("command path: %s\n", command_path);
	if (args[1])
	{
		printf("command arguments:");
		i = 1;
		while (args[i])
		{
			printf(" %s", args[i]);
			i++;
		}
		printf("\n");
	}
}

void	validate_and_print_command(char **args, char *const envp[])
{
	char	*command_path;

	if (!args || !args[0])
		return ;
	command_path = find_command_path(args[0], envp);
	if (command_path)
	{
		print_command_info(args, command_path);
		free(command_path);
	}
	else
		printf("invalid command: %s\n", args[0]);
}

void execve_args(char **args, char *command_path, char *const envp[])
{
	pid_t pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		if (execve(command_path, args, envp) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		int status;
		waitpid(pid, &status, 0);
	}
	}

void builtin_echo(char **args, int *i)
{

	if(args[*i + 1] && ft_strcmp(args[*i + 1], "-n") == 0)
		printf("%s", args[*i+1]);
	else if(args[*i + 1])
		printf("%s\n", args[*i+1]);

}

int is_builtins(char **args, int *i)
{
	if (ft_strcmp(args[*i], "echo") == 0)
		return (builtin_echo(args, i), 1);
	else if (ft_strcmp(args[*i], "cd") == 0)
		return (1);
	else if (ft_strcmp(args[*i], "pwd") == 0)
		return (1);
	else if (ft_strcmp(args[*i], "export") == 0)
		return (1);
	else if (ft_strcmp(args[*i], "unset") == 0)
		return (1);
	else if (ft_strcmp(args[*i], "env") == 0)
		return (1);
	else if (ft_strcmp(args[*i], "exit") == 0)
		return (1);
	return 0;
}
static void process_command(char **args, int *i, char *const envp[])
{
	char *command_path;

	command_path = find_command_path(args[*i], envp);
	if(is_builtins(args, i) == 1)
		;
	else if(command_path)
		execve_args(&args[*i], command_path, envp);
	else
		ft_putstr_fd("Error: command not found\n", STDERR_FILENO);
	(*i)++;
	if (args[*i] && strcmp(args[*i], "|") != 0)
		(*i)++;
}


void	parse_and_validate_input(const char *input, char *const envp[])
{
	char	**args2;
	char	**args;
	int		i;
	int		j;

	args2 = parse_command(input);
	args = parse_command_line(args2);
	i = 0;
	while (args[i] != NULL)
	{
		if (ft_strcmp(args[i], "|") == 0)
		{
			printf("Pipe:  |\n");
			i++;
			continue ;
		}
		process_command(args, &i, envp);
	}
	j = 0;
	while (args[j] != NULL)
		free(args[j++]);
	free(args);
}


int main(int argc, char *argv[], char *envp[])
{
    (void)argc;
    (void)argv;
    char *input;

    while (1)
    {
        input = readline("minishell> ");
        if (!input)
        {
            printf("Exiting minishell...\n");
            break;
        }
        if (*input)
            add_history(input);
        if (strcmp(input, "exit") == 0)
        {
            free(input);
            break;
        }
        parse_and_validate_input(input, envp);
        free(input);
    }
    return (0);
}
