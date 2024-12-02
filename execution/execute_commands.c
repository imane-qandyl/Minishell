/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iliastepanov <iliastepanov@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 14:04:57 by iliastepano       #+#    #+#             */
/*   Updated: 2024/12/02 15:27:51 by iliastepano      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


// int main()
// {
//     init_environ();
//     custom_pwd();
//     cleanup_environ();
//     return (0);
// }

void execute_commands(t_command *cmd_list)
{
    while (cmd_list)
    {
        // custom_cd(cmd_list);
        // custom_echo(cmd_list);
        custom_env(cmd_list);
        // custom_exit(cmd_list);
        // custom_export(cmd_list);
        custom_pwd(cmd_list);
        // custom_unset(cmd_list);
        cmd_list = cmd_list->next;
    }
}