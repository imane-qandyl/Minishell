/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imqandyl <imqandyl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 17:15:25 by imqandyl          #+#    #+#             */
/*   Updated: 2024/12/16 17:15:37 by imqandyl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	builtin_echo(int argc, char **argv)
{
	int	i;
	int	n_flag;

	n_flag = 0;
	i = 1;
	if (argc > 1 && !strcmp(argv[1], "-n"))
	{
		n_flag = 1;
		i++;
	}
	while (i < argc)
	{
		printf("%s", argv[i]);
		if (i < argc - 1)
			printf(" ");
		i++;
	}
	if (!n_flag)
		printf("\n");
	return (0);
}
