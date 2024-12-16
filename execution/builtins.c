/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imqandyl <imqandyl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 18:20:25 by imqandyl          #+#    #+#             */
/*   Updated: 2024/12/16 18:20:42 by imqandyl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_builtin(const char *cmd)
{
	static const char *builtins[] = {
		"echo", "cd", "pwd", "export",
		"unset", "env", "exit", NULL
	};
	int	i;

	i = 0;
	if (!cmd)
		return (0);
	while (builtins[i])
	{
		if (!ft_strncmp(cmd, builtins[i], strlen(builtins[i]) + 1))
			return (1);
		i++;
	}
	return (0);
}

int	execute_builtin(t_command *cmd)
{
	if (!cmd || !cmd->cmd_name)
		return (1);

	if (!ft_strncmp(cmd->cmd_name, "echo", strlen("echo") + 1))
		return (builtin_echo(cmd->argc, cmd->argv));
	if (!ft_strncmp(cmd->cmd_name, "cd", strlen("cd") + 1))
		return (builtin_cd(cmd->argc, cmd->argv));
	if (!ft_strncmp(cmd->cmd_name, "pwd", strlen("pwd") + 1))
		return (builtin_pwd());
	if (!ft_strncmp(cmd->cmd_name, "export", strlen("export") + 1))
		return (builtin_export(cmd->argv));
	if (!ft_strncmp(cmd->cmd_name, "unset", strlen("unset") + 1))
		return (builtin_unset(cmd->argv));
	if (!ft_strncmp(cmd->cmd_name, "env", strlen("env") + 1))
		return (builtin_env());
	if (!ft_strncmp(cmd->cmd_name, "exit", strlen("exit") + 1))
		return (builtin_exit(cmd->argv));
	return (1);
}

