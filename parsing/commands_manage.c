/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_manage.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imqandyl <imqandyl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 19:08:46 by imqandyl          #+#    #+#             */
/*   Updated: 2024/11/25 13:23:25 by imqandyl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_command *create_command(void)
{
    t_command *cmd = malloc(sizeof(t_command));
    if (!cmd) return NULL;
 
    cmd->cmd_name = NULL;
    cmd->args = NULL;
    cmd->infile = NULL;
    cmd->outfile = NULL;
    cmd->append_mode = 0;
    cmd->next = NULL;
    return cmd;
}

void add_argument(t_command *cmd, char *arg_value)
{
    t_arg *new_arg = malloc(sizeof(t_arg));
    if (!new_arg) return;

    new_arg->value = strdup(arg_value);
    new_arg->next = NULL;

    if (!cmd->args)
    {
        cmd->args = new_arg;
    }
    else
    {
        t_arg *current = cmd->args;
        while (current->next) current = current->next;
        current->next = new_arg;
    }
}

void add_command(t_command **list, t_command *new_cmd)
{
    if (!*list)
    {
        *list = new_cmd;
        return;
    }
    t_command *current = *list;
    while (current->next) current = current->next;
    current->next = new_cmd;
}

void print_commands(t_command *cmd_list)
{
    t_command *cmd = cmd_list;
    while (cmd)
    {
        printf("Command: %s\n", cmd->cmd_name);

        t_arg *arg = cmd->args;
        while (arg)
        {
            printf("  Arg: %s\n", arg->value);
            arg = arg->next;
        }

        cmd = cmd->next;
    }
}

int validate_command(t_command *cmd)
{
    if (!cmd->cmd_name)
        return 0;  // Invalid command
    
    // Check for valid file descriptors
    if (cmd->infile && access(cmd->infile, R_OK) != 0)
        return 0;  // Input file not accessible
    
    if (cmd->outfile && cmd->append_mode && access(cmd->outfile, W_OK) != 0)
        return 0;  // Output file not writable
    
    return 1;  // Command is valid
}

void free_command_list(t_command *cmd_list)
{
    t_command *current = cmd_list;
    while (current)
    {
        t_command *next = current->next;
        
        // Free arguments
        t_arg *arg = current->args;
        while (arg)
        {
            t_arg *next_arg = arg->next;
            free(arg->value);
            free(arg);
            arg = next_arg;
        }
        
        // Free command fields
        free(current->cmd_name);
        free(current->infile);
        free(current->outfile);
        free(current);
        current = next;
    }
}