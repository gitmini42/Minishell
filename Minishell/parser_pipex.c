#include "minishell.h"

void	execute_command(char *command, char **args, char *const envp[])
{
	char	*full_path;

	
	full_path = find_command_path(command, envp);
	if (!full_path)
	{
		ft_putstr_fd("Command not found: ", 2);
		ft_putstr_fd(command, 2);
		ft_putstr_fd("\n", 2);
		g_exit_status = 127; // Command not found
		exit(127);
	}

	execve(full_path, args, envp);
	perror("execve");

	/* // Handle specific `execve` failures
	if (errno == EACCES) // Permission denied
		g_exit_status = 126;
	else
		g_exit_status = 1; // General failure */

	free(full_path);
	exit(g_exit_status);
}

void	setup_pipes_and_redirections(int i, int num_commands,
			int *pipefd, int *prev_pipe_read, CommandData *data)
{
	int	fd_in;
	int	fd_out;
	int	flags;

	if (i == 0 && data->input_file)
	{
		fd_in = open(data->input_file, O_RDONLY);
		if (fd_in < 0)
		{
			perror("open input file");
			exit(1);
		}
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	if (i == num_commands - 1 && data->output_file)
	{
		flags = O_WRONLY | O_CREAT;
		if (data->append_output)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		fd_out = open(data->output_file, flags, 0644);
		if (fd_out < 0)
		{
			perror("open output file");
			exit(1);
		}
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
	if (i > 0)
	{
		dup2(*prev_pipe_read, STDIN_FILENO);
		close(*prev_pipe_read);
	}
	if (i < num_commands - 1)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
	}
}

void execute_commands(CommandData *data, char *const envp[])
{
    int pipefd[2];
    int prev_pipe_read = -1;
    int i;
    pid_t pid;
    pid_t *pids;
    int status;
	int is_builtin_cmd;
    struct sigaction sa;

    pids = malloc(sizeof(pid_t) * data->num_commands);
    if (!pids)
    {
        perror("malloc");
        exit(1);
    }
	ft_memset(pids, 0, sizeof(pid_t) * data->num_commands);
    i = 0;
    while (i < data->num_commands)
    {
        if (i < data->num_commands - 1)
        {
            if (pipe(pipefd) == -1)
            {
                perror("pipe");
                g_exit_status = 1;
                return;
            }
        }
        if (data->commands[i])
    		is_builtin_cmd = check_builtin(data->commands[i]);
		else
    		is_builtin_cmd = 0;
        if (data->num_commands > 1 || !is_builtin_cmd)
        {
            pid = fork();
            if (pid == 0)
            {
                ft_memset(&sa, 0, sizeof(sa));
                sa.sa_handler = SIG_DFL;
                sigemptyset(&sa.sa_mask);
                sa.sa_flags = 0;
                if (sigaction(SIGQUIT, &sa, NULL) == -1)
                {
                    perror("sigaction");
                    exit(1);
                }
                setup_pipes_and_redirections(i, data->num_commands, pipefd, &prev_pipe_read, data);
                if (is_builtin_cmd)
                {
                    execute_builtin(data->commands[i], data->arguments[i], &i, (char **)envp);
                    exit(g_exit_status);
                }
                else
                {
                    execute_command(data->commands[i], data->arguments[i], envp);
                    exit(1);
                }
            }
            else if (pid > 0)
            {
                pids[i] = pid;
                if (i > 0)
                    close(prev_pipe_read);
                if (i < data->num_commands - 1)
                {
                    prev_pipe_read = pipefd[0];
                    close(pipefd[1]);
                }
            }
            else
            {
                perror("fork");
                g_exit_status = 1;
                free(pids);
                return;
            }
        }
        else if (is_builtin_cmd)
        {
            execute_builtin(data->commands[i], data->arguments[i], &i, (char **)envp);
        }
        i++;
    }
    i = 0;
    while (i < data->num_commands)
    {
        if (pids[i] > 0)
        {
            if (waitpid(pids[i], &status, 0) > 0)
            {
                if (WIFEXITED(status))
                    g_exit_status = WEXITSTATUS(status);
                else if (WIFSIGNALED(status))
                    g_exit_status = 128 + WTERMSIG(status);
            }
        }
        i++;
    }

    free(pids);
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

    // Execute the commands
    char *envp[] = {NULL}; // Empty environment (can be customized)
    execute_commands(&data, envp);

    // Free allocated memory
    free_command_data(&data);

    return 0;
} */
