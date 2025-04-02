#include "../minishell.h"

#include "../minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int	ft_set_env(char **envp, const char *name, const char *value)
{
    size_t  name_len;
    size_t  value_len;
    int     i;
    char    *new_entry;

    name_len = ft_strlen(name);
    value_len = ft_strlen(value);
    i = 0;
    while (envp[i])
    {
        if (ft_strncmp(envp[i], name, name_len) == 0 && envp[i][name_len] == '=')
        {
            new_entry = malloc(name_len + value_len + 2);
            if (!new_entry)
                return (-1);
            ft_strcpy(new_entry, name);
            new_entry[name_len] = '=';
            ft_strcpy(new_entry + name_len + 1, value);
            free(envp[i]);
            envp[i] = new_entry;
            return (0);
        }
        i++;
    }
    return (-1);
}

int ft_cd(char **args, int *i, char **envp)
{
    char *path;
    char *old_pwd;
	char *new_pwd;

	old_pwd = getcwd(NULL, 0);
    if (!args[1])
    {
        path = getenv("HOME");
        if (!path)
        {
			write(STDERR_FILENO, "cd: HOME not set\n", 17);
            g_exit_status = 1;
            return 1;
        }
    }
    else
        path = args[1];
    if (chdir(path) != 0)
    {
        perror("cd");
        g_exit_status = 1;
        free(old_pwd);
        return 1;
    }
    new_pwd = getcwd(NULL, 0);
    if (!new_pwd)
    {
        perror("getcwd");
        g_exit_status = 1;
        free(old_pwd);
        return 1;
    }
    if (old_pwd)
        ft_set_env(envp, "OLDPWD", old_pwd);
    ft_set_env(envp, "PWD", new_pwd);
    free(old_pwd);
    free(new_pwd);
    (*i)++;
    g_exit_status = 0;
    return 0;
}




