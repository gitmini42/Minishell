#include "../minishell.h"

int	is_builtin(char *command, char **args)
{
	if (ft_strcmp(command, "echo") == 0)
		return (ft_echo(args), 1);
	if (ft_strcmp(command, "pwd") == 0)
		return (ft_pwd());
	return (0);
}


