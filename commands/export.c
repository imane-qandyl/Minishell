/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imqandyl <imqandyl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 17:15:25 by imqandyl          #+#    #+#             */
/*   Updated: 2024/12/16 17:26:12 by imqandyl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	print_exports(void)
{
	char	**env;
	char	*equal_sign;

	env = environ;
	while (*env)
	{
		printf("declare -x ");
		equal_sign = strchr(*env, '=');
		if (equal_sign)
		{
			printf("%.*s=\"%s\"\n", (int)(equal_sign - *env), *env, equal_sign + 1);
		}
		else
			printf("%s\n", *env);
		env++;
	}
}

int	builtin_export(char **argv)
{
	int		i;
	char	*equal_sign;

	if (!argv[1])
	{
		print_exports();
		return (0);
	}
	i = 1;
	while (argv[i])
	{
		equal_sign = strchr(argv[i], '=');
		if (equal_sign)
		{
			*equal_sign = '\0';
			setenv(argv[i], equal_sign + 1, 1);
			*equal_sign = '=';
		}
		else
			setenv(argv[i], "", 1);
		i++;
	}
	return (0);
}
