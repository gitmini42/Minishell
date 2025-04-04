#include "../minishell.h" // Adjust to your project's header file

void ft_env(char **envp)
{
    int i;

    i = 0;
    while (envp[i])
    {
        write(STDOUT_FILENO, envp[i], ft_strlen(envp[i]));
        write(STDOUT_FILENO, "\n", 1);
        i++;
    }
    g_exit_status = 0;
}