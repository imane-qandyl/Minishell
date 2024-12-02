/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iliastepanov <iliastepanov@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 14:04:57 by iliastepano       #+#    #+#             */
/*   Updated: 2024/12/02 18:54:45 by iliastepano      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_args(t_arg *args)
{
    if (strcmp(args->value, "-n") == 0)
       args = args->next;
    while (args)
	{
        printf("%s", args->value);
        args = args->next;
        if (args)
            printf(" ");
    }
}

static void	print_newline(t_arg *args)
{
	if (strcmp(args->value, "-n") == 0)
		return ;
	printf("\n");
}

void custom_echo(t_command *cmd_list)
{
    if (strcmp(cmd_list->cmd_name, "echo") != 0 || !(cmd_list->args) || !(cmd_list->args->value))
        return ;
    print_args(cmd_list->args);
    print_newline(cmd_list->args);
}

// int main(int argc, char **argv)
// {
// 	custom_echo(argc, argv);
// 	return (0);
// }