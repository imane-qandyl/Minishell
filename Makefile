NAME = minishell

# Source files
PARSE_SRC = main.c commands_manage.c quote_handling.c token_structure.c token_to_cmd.c syntax_error.c parser_tests.c
EXEC_SRC = cd.c echo.c env.c exit.c export.c init_environ.c pwd.c unset.c execute_commands.c

# Object files
PARSE_OBJ = $(PARSE_SRC:.c=.o)
EXEC_OBJ = $(EXEC_SRC:.c=.o)

# Paths
PARSE_PATH = ./parsing/
EXEC_PATH = ./execution/
OBJ_PATH = ./obj/

# Full paths for source and object files
PARSE_SRC_FILES = $(addprefix $(PARSE_PATH), $(PARSE_SRC))
EXEC_SRC_FILES = $(addprefix $(EXEC_PATH), $(EXEC_SRC))
PARSE_OBJ_FILES = $(addprefix $(OBJ_PATH), $(PARSE_OBJ))
EXEC_OBJ_FILES = $(addprefix $(OBJ_PATH), $(EXEC_OBJ))

# Combine all object files
OBJ_FILES = $(PARSE_OBJ_FILES) $(EXEC_OBJ_FILES)

CFLAGS = -Wall -Wextra -Werror -g -pthread -I./include
CC = gcc

HEADER = ./include/minishell.h
LDFLAGS = -lreadline

all: $(NAME)

$(NAME): $(OBJ_FILES) $(HEADER)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ_FILES) $(LDFLAGS)

$(OBJ_PATH)%.o: $(PARSE_PATH)%.c $(HEADER)
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_PATH)%.o: $(EXEC_PATH)%.c $(HEADER)
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_PATH)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
