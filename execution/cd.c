/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iliastepanov <iliastepanov@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 14:04:57 by iliastepano       #+#    #+#             */
/*   Updated: 2024/11/17 14:05:27 by iliastepano      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cd_with_one_arg(const char *path, int argc)
{
	if (argc != 2)
		return ;
    if (chdir(path) == -1)
        perror("error: cd");
}

static void	cd_with_no_arg(int argc)
{
	const char *home_dir;
	if (argc != 1)
		return ;
	home_dir = getenv("HOME");
	if (home_dir == NULL)
		return perror("error: cd HOME environment variable not set");
	cd_with_one_arg(home_dir, argc);
}

static void	cd_with_many_arg(int argc)
{
	if (argc <= 2)
		return ;
	perror("error: cd too many arguments");
}

void custom_pwd()
{
    char *cwd;

    cwd = getcwd(NULL, 0);
    if (cwd == NULL)
        return perror("error: pwd");
    printf("%s\n", cwd);
    free(cwd);
}

// void custom_cd(int argc, char **argv)
// {
// 	custom_pwd();
// 	cd_with_no_arg(argc);
// 	cd_with_one_arg(argv[1], argc);
//     cd_with_many_arg(argc);
// 	custom_pwd();
// }

// int main(int argc, char *argv[])
// {
// 	custom_cd(argc, argv);
// 	return 0;
// }
