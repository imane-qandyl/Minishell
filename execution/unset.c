/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iliastepanov <iliastepanov@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 14:04:57 by iliastepano       #+#    #+#             */
/*   Updated: 2024/12/02 15:04:34 by iliastepano      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_identifier(const char *name)
{
    int	i;

    if (!name || (!isalpha(name[0]) && name[0] != '_'))
        return 0;
	i = 0;
    while (name[++i])
        if (!isalnum(name[i]) && name[i] != '_')
            return 0;
    return 1;
}

void unset_variable(const char *var)
{
    int i;
    int j;

    if (!is_valid_identifier(var))
    {
        fprintf(stderr, "unset: `%s': not a valid identifier\n", var);
        return;
    }

    i = 0;
    while (g_environ[0][i])
    {
        if (strcmp(g_environ[0][i], var) == 0)
        {
            free(g_environ[0][i]);
            free(g_environ[1][i]);
            j = i;
            while (g_environ[0][j + 1])
            {
                g_environ[0][j] = g_environ[0][j + 1];
                g_environ[1][j] = g_environ[1][j + 1];
                j++;
            }
            g_environ[0][j] = NULL;
            g_environ[1][j] = NULL;
            return;
        }
        i++;
    }
}

// int main(int argc, char **argv)
// {
// 	init_environ();
//     custom_env();
// 	if (argc < 2)
// 		perror("unset error");
// 	else
// 		unset_variable(argv[1]);
//     custom_env();
// 	cleanup_environ();
// 	return (0);
// }