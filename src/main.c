/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iliastepanov <iliastepanov@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 21:18:37 by iliastepano       #+#    #+#             */
/*   Updated: 2024/11/11 21:34:01 by iliastepano      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command *create_command_node(const char *command, char **args, int args_count, const char *redirect, const char *filename)
{
    t_command *node = malloc(sizeof(t_command));
    if (!node)
        return NULL;
    node->command = strdup(command);
    node->args = malloc(args_count * sizeof(char *));
    for (int i = 0; i < args_count; i++) {
        node->args[i] = strdup(args[i]);
    }
    node->args_count = args_count;
    node->redirect = redirect ? strdup(redirect) : NULL;
    node->filename = filename ? strdup(filename) : NULL;

    return node;
}


void free_command_node(t_command *node)
{
    if (!node)
        return;
    free(node->command);
    for (int i = 0; i < node->args_count; i++)
    {
        free(node->args[i]);
    }
    free(node->args);
    free(node->redirect);
    free(node->filename);
    free(node);
}

int main()
{
    builtin_pwd();
    return (0);
}