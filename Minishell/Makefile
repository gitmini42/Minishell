NAME = minishell
CC = gcc
CFLAGS = -g -Wall -Werror -Wextra
LIBFT = ./libft/libft.a
RM = rm -rf

SRCS = main.c parser_pipex.c pipes_exec.c token_handling.c find_command_path.c
OBJS = $(SRCS:.c=.o)

$(NAME) : $(OBJS)
	$(MAKE) -C ./libft            # Build the libft library first
	gcc $(CFLAGS) $(OBJS) -lreadline -o $(NAME) $(LIBFT)  # Link with libft.a

all : $(NAME)

clean :
	$(MAKE) clean -C ./libft
	$(RM) $(OBJS)

fclean : clean
	$(MAKE) fclean -C ./libft
	$(RM) $(NAME)

re : fclean all

