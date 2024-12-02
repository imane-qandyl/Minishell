/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_environ.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iliastepanov <iliastepanov@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 14:04:57 by iliastepano       #+#    #+#             */
/*   Updated: 2024/12/01 19:23:10 by iliastepano      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int malloc_environ(void)
{
    int env_count;

    env_count = 0;
    while (environ[env_count])
        env_count++;

    g_environ = malloc(sizeof(char**) * 2);
    if (!g_environ)
        return (-1);

    g_environ[0] = malloc(sizeof(char*) * (env_count + 1));
    g_environ[1] = malloc(sizeof(char*) * (env_count + 1));
    if (!g_environ[0] || !g_environ[1])
    {
        free(g_environ[0]);
        free(g_environ[1]);
        free(g_environ);
        return (-1);
    }
    return (env_count);
}

void free_environ(int i)
{
    if (g_environ[0][i] && !g_environ[1][i])
        free(g_environ[0][i]);
    while (i > 0)
    {
        free(g_environ[0][--i]);
        free(g_environ[1][i]);
    }
    free(g_environ[0]);
    free(g_environ[1]);
    free(g_environ);
    g_environ = NULL;
    return;
}

void init_environ(void)
{
    int i;
    int env_count;
    char *equal_sign;

    env_count = malloc_environ();
    if (env_count == -1)
        return; // add exit code
    i = -1;
    while (++i < env_count)
    {
        equal_sign = strchr(environ[i], '=');
        if (!equal_sign)
            continue;

        g_environ[0][i] = strndup(environ[i], equal_sign - environ[i]);
        g_environ[1][i] = strdup(equal_sign + 1);

        if (!g_environ[0][i] || !g_environ[1][i])
            free_environ(i); // add exit code
    }
    g_environ[0][env_count] = NULL;
    g_environ[1][env_count] = NULL;
}

void cleanup_environ(void)
{
    if (!g_environ)
        return; // add exit code
        
    int i = 0;
    while (g_environ[0][i])
    {
        free(g_environ[0][i]);
        free(g_environ[1][i]);
        i++;
    }
    free(g_environ[0]);
    free(g_environ[1]);
    free(g_environ);
    g_environ = NULL;
}

// int main()
// {
//     int i = -1;
//     int j = -1;
//     init_environ();

//     while (g_environ[0][++i])
//     {
//         printf("key: %s, value: %s\n", g_environ[0][i], g_environ[1][i]);
//     }
//     cleanup_environ();
//     return (0);
// }