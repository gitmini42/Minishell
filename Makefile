NAME = minishell
CC = gcc
CFLAGS = -g -Werror -Wall -Wextra 
LIBFT = ./libft/libft.a
RM = rm -rf

SRCS = find_command_path.c parser_pipex.c main.c append_parse.c
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

