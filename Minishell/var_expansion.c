#include "minishell.h"

char *expand_variables(char *arg)
{
    char	*new_arg;
	int		i;
	int		j;
	char	*var_value;
	int		new_size;
	char	*tmp;
	char	*var_start;
	int		var_len;
	char	*var_name;

	i = 0;
	j = 0;
	new_size = strlen(arg) + 1;
	new_arg = malloc(new_size);
    if (!new_arg)
        return NULL;
    while (arg[i])
    {
        if (arg[i] == '$')
        {
            if (arg[i + 1] == '?')
            {
                var_value = ft_itoa(g_exit_status);
                if (!var_value)
				{
                    free(new_arg);
                    return (NULL);
                }
                new_size += ft_strlen(var_value) - 2;
                tmp = realloc(new_arg, new_size);  //preciso mudar realloc
                if (!tmp)
				{
                    free(var_value);
                    free(new_arg);
                    return (NULL);
                }
                new_arg = tmp;
                ft_strcpy(&new_arg[j], var_value);
                j += strlen(var_value);
                i += 2;
                free(var_value);
                continue ;
            }
            else if (isalpha(arg[i + 1]) || arg[i + 1] == '_')
            {
                var_start = &arg[i + 1];
                var_len = 0;
                while (isalnum(var_start[var_len]) || var_start[var_len] == '_')
                    var_len++;
                var_name = ft_strndup(var_start, var_len);
                if (!var_name)
				{
                    free(new_arg);
                    return NULL;
                }
                var_value = getenv(var_name);
                free(var_name);
                if (!var_value)
                    var_value = "";
                new_size += ft_strlen(var_value) - (var_len + 1);
                tmp = realloc(new_arg, new_size); //preciso mudar realloc
                if (!tmp)
				{
                    free(new_arg);
                    return (NULL);
                }
                new_arg = tmp;
                ft_strcpy(&new_arg[j], var_value);
                j += strlen(var_value);
                i += var_len + 1;
                continue ;
            }
        }
        new_arg[j++] = arg[i++];
    }
    new_arg[j] = '\0';
    return (new_arg);
}

/* void expand_arguments(char **args)
{
    int i = 0;
    char *expanded;

    while (args[i])
    {
        expanded = expand_variables(args[i]);
        if (expanded)
        {
            free(args[i]);  // Free old argument
            args[i] = expanded; // Replace with expanded value
        }
        i++;
    }
} */