#include "../minishell.h"

int check_builtin(char *command)
{
    if (command == NULL)
        return (0);
    if (ft_strcmp(command, "echo") == 0 ||
        ft_strcmp(command, "pwd") == 0 ||
        ft_strcmp(command, "cd") == 0)
        return (1);
    return (0);
}

int execute_builtin(char *command, char **args, int *i, char **envp)
{
    if (ft_strcmp(command, "echo") == 0)
    {
        ft_echo(args);
        return (1);
    }
    if (ft_strcmp(command, "pwd") == 0)
    {
        ft_pwd();
        return (1);
    }
    if (ft_strcmp(command, "cd") == 0)
    {
        ft_cd(args, i, envp);
        return (1);
    }
    return (0);
}
