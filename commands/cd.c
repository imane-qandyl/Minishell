/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imqandyl <imqandyl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 17:15:25 by imqandyl          #+#    #+#             */
/*   Updated: 2024/12/16 18:21:27 by imqandyl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	handle_home_cd(void)
{
	char *home_path;

	home_path = getenv("HOME");
	if (!home_path)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		return (1);
	}
	if (chdir(home_path) == -1)
	{
		perror("minishell: cd");
		return (1);
	}
	return (0);
}

int	builtin_cd(int argc, char **argv)
{
	if (!argv)
		return (1);

	if (argc == 1)
		return (handle_home_cd());
	if (argc > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	if (!argv[1])
		return (1);
	if (chdir(argv[1]) == -1)
	{
		perror("minishell: cd");
		return (1);
	}
	return (0);
}
