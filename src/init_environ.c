/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_environ.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iliastepanov <iliastepanov@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 14:04:57 by iliastepano       #+#    #+#             */
/*   Updated: 2024/11/17 14:23:13 by iliastepano      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void init_environ(void)
{
	int	i;
    int	env_count;

	// if (g_environ)
    //     return;
    env_count = 0;
    while (environ[env_count])
        env_count++;
    g_environ = malloc(sizeof(char*) * (env_count + 1));
    if (!g_environ)
        return;

    i = -1;
    while (++i < env_count)
	{
        g_environ[i] = strdup(environ[i]);
        if (!g_environ[i])
		{
            while (i > 0)
                free(g_environ[--i]);
            free(g_environ);
            g_environ = NULL;
            return;
        }
    }
    g_environ[env_count] = NULL;
}

void cleanup_environ(void)
{
    if (!g_environ)
        return;
        
    int i = 0;
    while (g_environ[i])
    {
        free(g_environ[i]);
        i++;
    }
    free(g_environ);
    g_environ = NULL;
}
