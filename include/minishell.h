/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imqandyl <imqandyl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 21:15:25 by imqandyl          #+#    #+#             */
/*   Updated: 2024/12/16 17:36:00 by imqandyl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <ctype.h>
# include <stdbool.h> 
# include <readline/readline.h>
# include <readline/history.h>
#include "../libft/libft.h"
# include <stdbool.h>

#define SYNTAX_ERROR 1
#define QUOTE_ERROR 2
#define ERROR_MEMORY 3
#define ERROR_BUFFER_OVERFLOW 4
#define ERROR_INVALID_REDIRECT 5


extern char **environ;
char ***g_environ;

typedef enum e_token_type
{
	TOKEN_WORD,		/* Commands and arguments */
	TOKEN_PIPE,		/* | */
	TOKEN_REDIR_IN,	/* < */
	TOKEN_REDIR_OUT,	/* > */
	TOKEN_APPEND,	/* >> */
	TOKEN_HEREDOC,	/* << */
	TOKEN_SPACE,	/* Space or tabs */
	TOKEN_SEMICOLON
}	t_token_type;
typedef struct s_token
{
	char		*value;
	t_token_type	type;
	int			position;
	int			line_number;
	struct s_token	*next;
	struct s_token	*previous;
}	t_token;

typedef struct s_arg
{
	char			*value;
	struct s_arg	*next;
}	t_arg;

typedef struct s_command
{
	char				*cmd_name;	/* The command name (e.g., "echo") */
	t_arg				*args;		/* A linked list of arguments */
	char				**argv;		/* Array of arguments for execve */
	int					argc;		/* Count of arguments */
	char				*infile;	/* Input redirection (optional) */
	char				*outfile;	/* Output redirection (optional) */
	int					append_mode;	/* For >> redirection */
	int					heredoc_mode;	/* For << redirection */
	struct s_command	*next;		/* Points to the next command in the list */
}	t_command;

typedef struct s_parser_state
{
    int error;
    char *error_message;
    int line_number;
} t_parser_state;

typedef struct s_error
{
    int code;
    char *message;
    int line;
    int column;
} t_error;

typedef struct s_pipeline {
    int pipe_count;
    int **pipes;      // Array of pipe file descriptors
    pid_t *pids;      // Array of process IDs
    int status;       // Exit status of last command
} t_pipeline;

t_token *create_token(char *value, t_token_type type);
void add_token(t_token **list, t_token *new_token);
void free_tokens(t_token *list);

// Function prototypes for command management
t_command *create_command(void);
void add_argument(t_command *cmd, char *arg_value);
void add_command(t_command **list, t_command *new_cmd);
void print_commands(t_command *cmd_list);
int validate_command(t_command *cmd);

// Tokenization and parsing functions
t_token *tokenize_input(char *input);
t_command *tokenize_input_to_commands(char *input);
int check_syntax_error(t_token *tokens);
int handle_quotes(char *input, int *i, char *buffer, int *j);

void free_command_list(t_command *cmd_list);
void run_parser_tests(void);

char *expand_env_vars(char *input, int *exit_status);

// Execution system
int     execute_command_line(t_command *cmd_list);
int     execute_single_command(t_command *cmd);
int     execute_pipeline(t_command *cmd, t_pipeline *pipeline);
void    setup_pipeline(t_pipeline *pipeline, t_command *cmd_list);
void    cleanup_pipeline(t_pipeline *pipeline);

// Builtin commands
int     is_builtin(const char *cmd);
int     execute_builtin(t_command *cmd);
int     builtin_echo(int argc, char **argv);
int     builtin_cd(int argc, char **argv);
int     builtin_pwd(void);
int     builtin_export(char **argv);
int     builtin_unset(char **argv);
int     builtin_env(void);
int     builtin_exit(char **argv);

// Redirections
void    setup_redirections(t_command *cmd);
void    restore_redirections(int stdin_backup, int stdout_backup);
int     handle_heredoc(const char *delimiter);




#endif