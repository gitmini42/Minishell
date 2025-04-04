#include "minishell.h"

void	initialize_command_data(CommandData *data)
{
	data->commands = NULL;
	data->arguments = NULL;
	data->num_commands = 0;
	data->num_pipes = 0;
	data->input_file = NULL;
	data->output_file = NULL;
	data->append_output = 0;
	data->heredoc_delim = NULL;
}

void	free_command_data(CommandData *data)
{
	int	i;
	int	j;

	if (!data)
		return ;
	if (data->commands)
	{
		i = 0;
		while (i < data->num_commands)
		{
			if (data->commands[i])
				free(data->commands[i]);
			i++;
		}
		free(data->commands);
	}
	if (data->arguments)
	{
		i = 0;
		while (i < data->num_commands)
		{
			if (data->arguments[i])
			{
				j = 0;
				while (data->arguments[i][j])
				{
					free(data->arguments[i][j]);
					j++;
				}
				free(data->arguments[i]);
			}
			i++;
		}
		free(data->arguments);
	}
	if (data->input_file)
		free(data->input_file);
	if (data->output_file)
		free(data->output_file);
	if (data->heredoc_delim)
		free(data->heredoc_delim);
	data->num_commands = 0;
	data->num_pipes = 0;
}
void	is_pipe(CommandData **data, int *command_index, int *arg_index)
{
	(*data)->num_pipes++;
	(*command_index)++;
	*arg_index = 0;
}
void	redirect_input(char **args, CommandData *data, int *i)
{
	if (args[*i + 1] == NULL)
	{
		write(STDERR_FILENO, "Error: No input file specified after '<'\n", 41);
		return;
	}
	data->input_file = ft_strdup(args[*i + 1]);
	(*i)++;
}

void	redirect_output(char **args, CommandData *data, int *i)
{
	if (args[*i + 1] == NULL)
	{
		write(STDERR_FILENO, "Error: No output file specified after '>'\n", 42);
		return ;
	}
	data->output_file = ft_strdup(args[*i + 1]);
	(*i)++;
}

void	redirect_output_append(char **args, CommandData *data, int *i)
{
	if (args[*i + 1] == NULL)
	{
		write(STDERR_FILENO, "Error: No output file specified after '>>'\n", 43);
		return ;
	}
	data->output_file = ft_strdup(args[*i + 1]);
	data->append_output = 1;
	(*i)++;
}

static int is_operator(const char *token)
{
    return (ft_strcmp(token, "|") == 0 ||
            ft_strcmp(token, "<") == 0 ||
            ft_strcmp(token, ">") == 0 ||
            ft_strcmp(token, ">>") == 0 ||
            ft_strcmp(token, "<<") == 0);
}

static void handle_operator(char **args, CommandData *data, int *i, int *command_index)
{
    const char *token = args[*i];

    if (ft_strcmp(token, "|") == 0)
    {
        data->num_pipes++;
        (*command_index)++;
    }
    else if (ft_strcmp(token, "<") == 0)
    {
        if (args[*i + 1])
        {
            free(data->input_file);
            data->input_file = ft_strdup(args[*i + 1]);
            (*i)++;
        }
    }
    else if (ft_strcmp(token, ">") == 0)
    {
        if (args[*i + 1])
        {
            free(data->output_file);
            data->output_file = ft_strdup(args[*i + 1]);
            data->append_output = 0;
            (*i)++;
        }
    }
    else if (ft_strcmp(token, ">>") == 0)
    {
        if (args[*i + 1])
        {
            free(data->output_file);
            data->output_file = ft_strdup(args[*i + 1]);
            data->append_output = 1;
            (*i)++;
        }
    }
    else if (ft_strcmp(token, "<<") == 0)
    {
        if (args[*i + 1])
        {
            free(data->heredoc_delim);
            data->heredoc_delim = ft_strdup(args[*i + 1]);
            (*i)++;
        }
        else
        {
            write(STDERR_FILENO, "Error: No delimiter specified after '<<'\n", 42);
        }
    }
    (*i)++;
}

void parse_input(char **args, int count, CommandData *data)
{
    int		arg_index;
    int		*arg_counts;
    int		command_index;
    int		i;

    ft_memset(data, 0, sizeof(CommandData));
    arg_counts = malloc(count * sizeof(int));
    if (!arg_counts)
        return;
    ft_memset(arg_counts, 0, sizeof(int) * count);
    command_index = 0;
    i = 0;
    while (i < count)
    {
        if (is_operator(args[i]))
            handle_operator(args, data, &i, &command_index);
        else
        {
            arg_counts[command_index]++;
            i++;
        }
    }
    data->num_commands = command_index + 1;
    data->commands = malloc(data->num_commands * sizeof(char *));
    data->arguments = malloc(data->num_commands * sizeof(char **));
    if (!data->commands || !data->arguments)
    {
        free_command_data(data);
        free(arg_counts);
        return;
    }
    ft_memset(data->commands, 0, data->num_commands * sizeof(char *));
    ft_memset(data->arguments, 0, data->num_commands * sizeof(char **));
    command_index = 0;
    arg_index = 0;
    i = 0;
    while (i < count)
    {
        if (is_operator(args[i]))
        {
            handle_operator(args, data, &i, &command_index);
            arg_index = 0;
        }
        else
        {
            //printf("Parsing token %d: '%s'\n", i, args[i]);
            if (arg_index == 0)
            {
                data->commands[command_index] = ft_strdup(args[i]);
                data->arguments[command_index] = malloc((arg_counts[command_index] + 1) * sizeof(char *));
                if (!data->commands[command_index] || !data->arguments[command_index])
                {
                    free_command_data(data);
                    free(arg_counts);
                    return;
                }
            }
            data->arguments[command_index][arg_index] = ft_strdup(args[i]);
            if (!data->arguments[command_index][arg_index])
            {
                free_command_data(data);
                free(arg_counts);
                return;
            }
            arg_index++;
            i++;
        }
    }
    i = 0;
    while (i < data->num_commands)
    {
        if (data->arguments[i])
            data->arguments[i][arg_counts[i]] = NULL;
        i++;
    }
    free(arg_counts);
}


/* int main() {
    // Example input: "ls -l | grep .c > output.txt"
    char *tokens[] = {"ls", "-l", "|", "grep", ".c", ">", "output.txt"};
    int count = sizeof(tokens) / sizeof(tokens[0]);

    CommandData data;
    parse_input(tokens, count, &data);

    // Print parsed data
    printf("Number of commands: %d\n", data.num_commands);
    printf("Number of pipes: %d\n", data.num_pipes);
    printf("Input file: %s\n", data.input_file ? data.input_file : "None");
    printf("Output file: %s\n", data.output_file ? data.output_file : "None");
    printf("Append output: %d\n", data.append_output);

    for (int i = 0; i < data.num_commands; i++) {
        printf("Command %d: %s\n", i, data.commands[i]);
        for (int j = 0; data.arguments[i][j] != NULL; j++) {
            printf("  Argument %d: %s\n", j, data.arguments[i][j]);
        }
    }

    // Free allocated memory
    free_command_data(&data);

    return 0;
} */
