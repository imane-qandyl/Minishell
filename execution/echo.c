/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iliastepanov <iliastepanov@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 14:04:57 by iliastepano       #+#    #+#             */
/*   Updated: 2024/12/02 16:36:26 by iliastepano      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	check_option_n(t_arg *args, bool *is_newline)
{
    *is_newline = false;
	if (strcmp(args->value, "-n") != 0)
		return ;
	*is_newline = true;
}

static void	print_args(t_arg *args, bool is_newline)
{
    if (is_newline)
       args = args->next;
    while (args)
	{
        printf("%s", args->value);
        args = args->next;
        if (args)
            printf(" ");
    }
}

static void	print_newline(bool is_newline)
{
	if (is_newline)
		return ;
	printf("\n");
}

void custom_echo(t_command *cmd_list)
{
    bool is_newline;

    if (strcmp(cmd_list->cmd_name, "echo") != 0 || !cmd_list->args->value)
        return ;
    check_option_n(cmd_list->args, &is_newline);
    print_args(cmd_list->args, is_newline);
    print_newline(is_newline);
}

// int main(int argc, char **argv)
// {
// 	custom_echo(argc, argv);
// 	return (0);
// }