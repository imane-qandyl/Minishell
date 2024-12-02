/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iliastepanov <iliastepanov@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 14:04:57 by iliastepano       #+#    #+#             */
/*   Updated: 2024/12/02 16:09:38 by iliastepano      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void custom_exit(t_command *cmd_list)
{
    if (strcmp(cmd_list->cmd_name, "exit") != 0)
        return ;
    exit(0); // incorrect
}

// int main()
// {
//     custom_exit();
//     return 0;
// }
