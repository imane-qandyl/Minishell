/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iliastepanov <iliastepanov@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 16:38:14 by iliastepano       #+#    #+#             */
/*   Updated: 2024/11/17 14:08:19 by iliastepano      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>



#define ESC 53


extern char **environ;
char **g_environ;

typedef struct s_command
{
    char *command;          // The main command, e.g., "echo", "cd"
    char **args;            // Array of argument strings
    int args_count;         // Number of arguments in the args array
    char *redirect;         // Redirection type: ">", ">>", "<", "<<"
    char *filename;         // File associated with redirection
} t_command;


void custom_cd(int argc, char **argv);
void custom_echo(int argc, char **argv);
void init_environ(void);
void cleanup_environ(void);





#endif // MINISHELL_H
