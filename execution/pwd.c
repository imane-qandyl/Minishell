/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iliastepanov <iliastepanov@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 21:32:16 by iliastepano       #+#    #+#             */
/*   Updated: 2024/12/02 15:28:00 by iliastepano      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void custom_pwd(t_command *cmd_list)
{
    char *cwd;

    if (strcmp(cmd_list->cmd_name, "pwd") != 0)
        return ;
    cwd = getcwd(NULL, 0);
    if (cwd != NULL)
    {
        printf("%s\n", cwd);
        free(cwd);
    }
}
