/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iliastepanov <iliastepanov@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 21:32:16 by iliastepano       #+#    #+#             */
/*   Updated: 2024/11/14 20:51:27 by iliastepano      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void custom_pwd()
{
    char *cwd;

    cwd = getcwd(NULL, 0);
    if (cwd == NULL)
        return perror("error: pwd");
    printf("%s\n", cwd);
    free(cwd);
}

// int main(int argc, char **argv)
// {
// 	custom_pwd();
// 	return (0);
// }
