NAME = minishell
LIBFT = libft/libft.a

PARSING_DIR = parsing
EXECUTION_DIR = execution
COMMANDS_DIR = commands

SRCS = $(PARSING_DIR)/token_to_cmd.c \
       $(PARSING_DIR)/quote_handling.c \
       $(PARSING_DIR)/token_structure.c \
       $(PARSING_DIR)/commands_manage.c \
       $(PARSING_DIR)/main.c \
       $(PARSING_DIR)/syntax_error.c \
       $(PARSING_DIR)/env_expansion.c \
       $(EXECUTION_DIR)/signals.c \
       $(EXECUTION_DIR)/redirections.c \
       $(EXECUTION_DIR)/pipes.c \
       $(EXECUTION_DIR)/executor.c \
       $(EXECUTION_DIR)/builtins.c \
       $(COMMANDS_DIR)/cd.c \
       $(COMMANDS_DIR)/echo.c \
       $(COMMANDS_DIR)/env.c \
       $(COMMANDS_DIR)/exit.c \
       $(COMMANDS_DIR)/export.c \
       $(COMMANDS_DIR)/pwd.c \
       $(COMMANDS_DIR)/unset.c

OBJS = $(SRCS:.c=.o)

CFLAGS = -Wall -Wextra -Werror -g -pthread -I./include
CC = gcc

HEADER = ./include/minishell.h
LDFLAGS = -lreadline -L./libft -lft

all: $(LIBFT) $(NAME)

$(LIBFT):
	@make -C libft

$(NAME): $(OBJS) $(HEADER) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)
	@make -C libft clean

fclean: clean
	@rm -f $(NAME)
	@make -C libft fclean

re: fclean all

.PHONY: all clean fclean re