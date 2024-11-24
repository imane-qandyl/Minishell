/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iliastepanov <iliastepanov@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 14:04:57 by iliastepano       #+#    #+#             */
/*   Updated: 2024/11/24 18:46:07 by iliastepano      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void custom_env(void)
{
    int i;

    i = -1;
    while (g_environ[0][++i] != NULL)
        printf("%s=%s\n", g_environ[0][i], g_environ[1][i]);
}

int main()
{
    init_environ();
    custom_env();
    cleanup_environ();
    return (0);
}

