#include "../minishell.h"

int ft_cd(char **args, int *i)
{
	char *path;

	if (args[1] == NULL)
	{
		path = getenv("HOME");
		if (path == NULL)
		{
			perror("cd");
			return 1;
		}
	}
	else
	{
		path = args[1];
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		return 1;
	}
	char *new_pwd = getcwd(NULL, 0);  // Pega o diretório atual após o chdir
	if (new_pwd == NULL)
	{
		perror("getcwd");
		return 1;
	}
	setenv("PWD", new_pwd, 1);
	free(new_pwd);
	(*i)++;
	return 0;
}
