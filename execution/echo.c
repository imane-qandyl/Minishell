/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iliastepanov <iliastepanov@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 14:04:57 by iliastepano       #+#    #+#             */
/*   Updated: 2024/11/17 14:05:31 by iliastepano      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_variables(bool *no_newline, int *index)
{
	*no_newline = false;
	*index = 1;
}

static void	check_option_n(int argc, char **argv, bool *no_newline, int *index)
{
	if (argc <= 1 || strcmp(argv[1], "-n") != 0)
		return ;
	*no_newline = true;
	*index = 2;
}

static void	print_args(int argc, char **argv, int index)
{
    while (index < argc)
	{
        printf("%s", argv[index]);
        if (index < argc - 1)
            printf(" ");
        index++;
    }
}

static void	print_newline(bool no_newline)
{
	if (no_newline)
		return ;
	printf("\n");
}

void custom_echo(int argc, char **argv)
{
    bool no_newline;
    int index;

	init_variables(&no_newline, &index);
    check_option_n(argc, argv, &no_newline, &index);
    print_args(argc, argv, index);
    print_newline(no_newline);
}

// int main(int argc, char **argv)
// {
// 	custom_echo(argc, argv);
// 	return (0);
// }
