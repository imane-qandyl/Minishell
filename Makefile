NAME = minishell

SRC = main.c commands_manage.c quote_handling.c token_structure.c token_to_cmd.c syntax_error.c env_expansion.c
OBJ = $(SRC:.c=.o)

PATH_SRC = ./parsing/
PATH_OBJ = ./obj/
SRC_FILES = $(addprefix $(PATH_SRC), $(SRC))
OBJ_FILES = $(addprefix $(PATH_OBJ), $(OBJ))

CFLAGS = -Wall -Wextra -Werror -g -pthread -I./include
CC = gcc

HEADER = ./include/minishell.h
LDFLAGS = -lreadline

all: $(NAME)

$(NAME): $(OBJ_FILES) $(HEADER)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ_FILES) $(LDFLAGS)

$(PATH_OBJ)%.o: $(PATH_SRC)%.c $(HEADER)
	@mkdir -p $(PATH_OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(PATH_OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
