SHELL		=	/bin/bash
NAME = philo
CC = cc
FLAGS = -Wall -Wextra -Werror -pthread
OBJECTS = $(SOURCES:.c=.o)
ECHO		= echo -e

GREEN		=	\033[1;92m
YELLOW		=	\033[1;93m
DEF_COLOR  =	\033[0m

SOURCES =	$(wildcard ./sources/philosophers_actions/*.c) \
			$(wildcard ./sources/philosophers_utils/*.c) \
			$(wildcard ./sources/user_arguments_check/*.c) \
			$(wildcard ./sources/*.c)


all: $(NAME)

$(NAME): $(OBJECTS)
	@$(ECHO) -n "$(YELLOW)[Philos]$(DEF_COLOR)"
	@$(CC) $(FLAGS) $(OBJECTS) -o $(NAME)
	@$(ECHO) "$(GREEN)  Success!$(DEF_COLOR)"

$(OBJECTS): %.o: %.c
	@$(CC) $(FLAGS) -c $< -o $@

clean:
	@$(ECHO) -n "$(YELLOW)[Objects]$(DEF_COLOR)"
	@rm -f $(OBJECTS)
	@$(ECHO) "$(GREEN) Cleaned!$(DEF_COLOR)"

fclean: clean
	@$(ECHO) -n "$(YELLOW)[Philos]$(DEF_COLOR)"
	@rm -f $(NAME)
	@$(ECHO) "$(GREEN)  Cleaned!$(DEF_COLOR)"

re: fclean all
