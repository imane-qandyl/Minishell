/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iliastepanov <iliastepanov@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 21:32:16 by iliastepano       #+#    #+#             */
/*   Updated: 2024/11/24 16:37:25 by iliastepano      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void custom_pwd()
{
    int i;

    i = -1;
    while (g_environ[0][++i])
    {
        if (strcmp(g_environ[0][i], "PWD") != 0)
            continue ;
        printf("%s\n", g_environ[1][i]);
        return;
    }
    perror("error: PWD not found in environment");
}

// int main()
// {
//     init_environ();
//     custom_pwd();
//     cleanup_environ();
//     return (0);
// }
