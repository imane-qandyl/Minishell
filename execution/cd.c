/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iliastepanov <iliastepanov@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 14:04:57 by iliastepano       #+#    #+#             */
/*   Updated: 2024/12/02 14:44:31 by iliastepano      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cd_with_one_arg(const char *path, int argc)
{
	int i;

	if (argc != 2)
		return ;
	i = -1;
	while (g_environ[0][++i])
	{
		if (strcmp(g_environ[0][i], "PWD") != 0) // dependency
			continue ;
		if (chdir(path) == -1)
		{
			perror("error: cd");
			return ;
		}
		free(g_environ[1][i]);
		g_environ[1][i] = strdup(path);
		return ;
	}
	perror("error: PWD not found in environment");
}

static void	cd_with_no_arg(int argc)
{
	int i;

	if (argc != 1)
		return ;
	i = -1;
	while (g_environ[0][++i])
	{
		if (strcmp(g_environ[0][i], "HOME") != 0)
			continue ;
		cd_with_one_arg(g_environ[1][i], argc);
		return ;
	}
	perror("error: HOME not found in environment");
}

static void	cd_with_many_arg(int argc)
{
	if (argc <= 2)
		return ;
	perror("error: cd too many arguments");
}

void custom_cd(int argc, char **argv)
{
	cd_with_no_arg(argc);
	cd_with_one_arg(argv[1], argc);
    cd_with_many_arg(argc);
}

// int main(int argc, char *argv[])
// {
// 	custom_pwd();
// 	custom_cd(argc, argv);
// 	custom_pwd();
// 	return 0;
// }