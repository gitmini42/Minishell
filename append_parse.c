#include "parser_pipex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	count_arguments(char **input)
{
	int	output_size;
	int	is_first_argument;
	int	is_second_argument;
	int	i;

	output_size = 0;
	is_first_argument = 1;
	is_second_argument = 0;
	i = 0;
	while (input[i] != NULL)
	{
		if (ft_strcmp(input[i], "|") == 0)
		{
			output_size++;
			is_first_argument = 1;
			is_second_argument = 0;
		}
		else if (is_first_argument)
		{
			output_size++;
			is_first_argument = 0;
			is_second_argument = 1;
		}
		else if (is_second_argument)
		{
			output_size++;
			is_second_argument = 0;
		}
		i++;
	}
	return (output_size);
}

char	**parse_command_line(char **input)
{
	int		output_size;
	char	**output;
	int		output_index;
	char	temp[1024];
	int		is_first_argument;
	int		i;

	output_size = count_arguments(input);
	output = malloc((output_size + 1) * sizeof(char *));
	output_index = 0;
	temp[0] = '\0';
	is_first_argument = 1;
	i = 0;
	while (input[i] != NULL)
	{
		if (ft_strcmp(input[i], "|") == 0)
		{
			if (ft_strlen(temp) > 0)
			{
				output[output_index++] = ft_strdup(temp);
				temp[0] = '\0';
			}
			output[output_index++] = ft_strdup(input[i]);
			is_first_argument = 1;
		}
		else if (is_first_argument)
		{
			output[output_index++] = ft_strdup(input[i]);
			is_first_argument = 0;
		}
		else
		{
			if (ft_strlen(temp) > 0)
				ft_strcat(temp, " ");
			ft_strcat(temp, input[i]);
		}
		i++;
	}
	if (ft_strlen(temp) > 0)
		output[output_index++] = ft_strdup(temp);
	output[output_index] = NULL;
	return (output);
}

/* int main() {
    char* input[] = {"ls", "-l", "a", "b", "c", "d", "|", "la", "-b", "a", "b", NULL};

    char** output = parse_command_line(input);

    for (int i = 0; output[i] != NULL; i++) {
        printf("output[%d]: %s\n", i, output[i]);
    }

    // Free the allocated memory
    for (int i = 0; output[i] != NULL; i++) {
        free(output[i]);
    }
    free(output);

    return 0;
} */
