#include "minishell.h"


typedef struct s_parse
{
	char		**args;
	int			args_count;
	bool		in_quotes;
	char		quote_char;
	size_t		i;
	size_t		start;
	int			brace_count;
	const char	*cmd;
}	t_parse;

// excluí ft_strlen, ft_putchar_fd, ft_putstr_fd e *ft_substr
// pois já estão no libft

char	*clean_backslashes(const char *arg)
{
	size_t	i;
	size_t	j;
	size_t	len;
	char	*cleaned;

	i = 0;
	j = 0;
	len = ft_strlen(arg);
	cleaned = malloc(len + 1);
	if (!cleaned)
	{
		ft_putstr_fd("Error: malloc failed\n", STDERR_FILENO);
		exit(1);
	}
	while (arg[i])
	{
		if (arg[i] == '\\' && arg[i + 1] != '\0')
			i++;
		cleaned[j] = arg[i];
		j++;
		i++;
	}
	cleaned[j] = '\0';
	return (cleaned);
}

void	add_argument(t_parse *state)
{
	char	*arg;
	char	*cleaned_arg;
	char	**new_args;
	int		i;

	arg = ft_substr(state->cmd, state->start, state->i - state->start);
	if (!arg)
	{
		ft_putstr_fd("Error: malloc failed\n", STDERR_FILENO);
		exit(1);
	}
	cleaned_arg = clean_backslashes(arg);
	free(arg);
	new_args = malloc(sizeof(char *) * (state->args_count + 2));
	if (!new_args)
	{
		ft_putstr_fd("Error: malloc failed\n", STDERR_FILENO);
		exit(1);
	}
	i = -1;
	while (++i < state->args_count)
		new_args[i] = state->args[i];
	new_args[state->args_count] = cleaned_arg;
	new_args[state->args_count + 1] = NULL;
	free(state->args);
	state->args = new_args;
	state->args_count++;
}

void	handle_non_quoted_space(t_parse *state)
{
	if (state->i > state->start)
		add_argument(state);
	while (state->cmd[state->i] == ' ')
		state->i++;
	state->start = state->i;
}

char	**finalize_args(t_parse *state)
{
	state->args[state->args_count] = NULL;
	return (state->args);
}

void	handle_end_of_quoted_string(t_parse *state)
{
	state->in_quotes = false;
	add_argument(state);
	state->i++;
	state->start = state->i;
	state->quote_char = '\0';
}

static void	initialize_state(t_parse *state, const char *cmd)
{
	state->args = malloc(sizeof(char *) * 2);
	if (!state->args)
	{
		ft_putstr_fd("Error: malloc failed\n", STDERR_FILENO);
		exit(1);
	}
	state->args_count = 0;
	state->in_quotes = false;
	state->quote_char = '\0';
	state->i = 0;
	state->start = 0;
	state->brace_count = 0;
	state->cmd = cmd;
}

static void	handle_quoted_string(t_parse *state)
{
	state->in_quotes = true;
	state->quote_char = state->cmd[state->i];
	state->i++;
	state->start = state->i;
}

static void	handle_braces_and_escapes(t_parse *state)
{
	if (state->cmd[state->i] == '{')
		state->brace_count++;
	else if (state->cmd[state->i] == '}')
		state->brace_count--;
	if (state->cmd[state->i] == '\\' && state->cmd[state->i + 1] != '\0')
	{
		state->i++;
		state->start = state->i;
	}

	state->i++;
}


char	**parse_command(const char *cmd)
{
	t_parse	state;

	initialize_state(&state, cmd);
	while (state.cmd[state.i])
	{
		if ((state.cmd[state.i] == '\''
				|| state.cmd[state.i] == '"') && !state.in_quotes)
			handle_quoted_string(&state);
		else if (state.cmd[state.i] == state.quote_char
			&& state.in_quotes && state.brace_count == 0)
			handle_end_of_quoted_string(&state);
		else if (state.in_quotes)
			handle_braces_and_escapes(&state);
		else if (state.cmd[state.i] == ' ' && !state.in_quotes)
			handle_non_quoted_space(&state);
		else
			state.i++;
	}
	if (state.i > state.start)
		add_argument(&state);
	return (finalize_args(&state));
}

/* int main(void)
{
    //char **args = parse_command("echo 'Hello, World!' \"This is a test\" arg3 \\{escaped\\}");
	char **args = parse_command("echo \"Hello, World!\" | grep \"World\" | wc -l");

    for (int i = 0; args[i] != NULL; i++)
    {
        printf("Argument %d: %s\n", i, args[i]);
        free(args[i]);
    }
    free(args);
    return 0;
} */
