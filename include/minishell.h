/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imqandyl <imqandyl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 21:15:25 by imqandyl          #+#    #+#             */
/*   Updated: 2024/12/06 15:17:19 by imqandyl         ###   ########.fr       */
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

typedef struct s_exec_info
{
    int     pipe_fd[2];
    pid_t   pid;
    int     status;
    int     in_fd;
    int     out_fd;
    char    **env;
    int     exit_status;
} t_exec_info;

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

void handle_sigint(int sig);
void handle_sigquit(int sig);
void handle_sigterm(int sig);

char *expand_env_vars(char *input, int *exit_status);

void custom_pwd(void);
void custom_env(void);
void custom_pwd(void);

void init_environ(void);
void cleanup_environ(void);

// Add these prototypes
void build_argv_array(t_command *cmd);
char *resolve_command_path(t_command *cmd);
bool is_builtin(char *cmd);

#endif