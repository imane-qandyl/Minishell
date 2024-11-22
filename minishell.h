/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imqandyl <imqandyl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 21:15:25 by imqandyl          #+#    #+#             */
/*   Updated: 2024/11/18 10:24:56 by imqandyl         ###   ########.fr       */
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
# include <readline/readline.h>
# include <readline/history.h>

typedef enum e_token_type
{
	TOKEN_WORD,		/* Commands and arguments */
	TOKEN_PIPE,		/* | */
	TOKEN_REDIR_IN,	/* < */
	TOKEN_REDIR_OUT,	/* > */
	TOKEN_APPEND,	/* >> */
	TOKEN_HEREDOC,	/* << */
	TOKEN_SPACE		/* Space or tabs */
}	t_token_type;
typedef struct s_token
{
	char		*value;
	t_token_type	type;
	struct s_token	*next;
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
	char				*infile;	/* Input redirection (optional) */
	char				*outfile;	/* Output redirection (optional) */
	int					append_mode;	/* For >> redirection */
	struct s_command	*next;		/* Points to the next command in the list */
}	t_command;

int     builtin_cd(t_command *cmd);
int     builtin_echo(t_command *cmd);
int     builtin_env(t_command *cmd);
int     builtin_exit(t_command *cmd);
int     builtin_export(t_command *cmd);
int     builtin_unset(t_command *cmd);

#endif