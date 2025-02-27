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
}

void	free_command_data(CommandData *data)
{
	int	i;
	int	j;

	i = 0;
	while (i < data->num_commands)
	{
		free(data->commands[i]);
		j = 0;
		while (data->arguments[i][j] != NULL)
		{
			free(data->arguments[i][j]);
			j++;
		}
		free(data->arguments[i]);\
		i++;
	}
	free(data->commands);
	free(data->arguments);
	free(data->input_file);
	free(data->output_file);
}
void	is_pipe(CommandData **data, int *command_index, int *arg_index)
{
	(*data)->num_pipes++;
	(*command_index)++;
	*arg_index = 0;
}
void	redirect_input(char **args, CommandData *data, int *i)
{
	if (args[*i + 1] == NULL) // Verifica se há um argumento após "<"
	{
		fprintf(stderr, "Error: No input file specified after '<'\n");
		return;
	}

	data->input_file = strdup(args[*i + 1]);
	(*i)++;
}

void	redirect_output(char **args, CommandData *data, int *i)
{
	if (args[*i + 1] == NULL)
	{
		fprintf(stderr, "Error: No output file specified after '>'\n");
		return;
	}
	data->output_file = strdup(args[*i + 1]);
	(*i)++;
}

void	redirect_output_append(char **args, CommandData *data, int *i)
{
	if (args[*i + 1] == NULL)
	{
		fprintf(stderr, "Error: No output file specified after '>>'\n");
		return;
	}

	data->output_file = strdup(args[*i + 1]);
	data->append_output = 1;
	(*i)++;
}

void	parse_input(char **args, int count, CommandData *data)
{
	int	i;
	int	command_index;
	int	arg_index;

	data->num_commands = 0;
	data->num_pipes = 0;
	data->input_file = NULL;
	data->output_file = NULL;
	data->append_output = 0;
	data->commands = malloc(count * sizeof(char *));
	data->arguments = malloc(count * sizeof(char **));
	command_index = 0;
	arg_index = 0;
	i = 0;
	while (i < count)
	{
		if (strcmp(args[i], "|") == 0)
			is_pipe(&data, &command_index, &arg_index);
		else if (strcmp(args[i], "<") == 0)
			redirect_input(args, data, &i);
		else if (strcmp(args[i], ">") == 0)
			redirect_output(args, data, &i);
		else if (strcmp(args[i], ">>") == 0)
			redirect_output_append(args, data, &i);
		else
		{
			if (arg_index == 0)
			{
				data->commands[command_index] = strdup(args[i]);
				data->arguments[command_index] = malloc((count + 1)
						* sizeof(char *));
			}
			data->arguments[command_index][arg_index] = strdup(args[i]);
			arg_index++;
		}
		i++;
	}
	i = 0;
	while (i <= command_index)
	{
		data->arguments[i][arg_index] = NULL;
		i++;
	}
	data->num_commands = command_index + 1;
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
