/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imqandyl <imqandyl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 17:09:25 by imqandyl          #+#    #+#             */
/*   Updated: 2024/12/16 17:09:58 by imqandyl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void setup_pipeline(t_pipeline *pipeline, t_command *cmd_list)
{
    int cmd_count = 0;
    t_command *current = cmd_list;

    while (current)
    {
        cmd_count++;
        current = current->next;
    }

    pipeline->pipe_count = cmd_count - 1;
    pipeline->pipes = malloc(sizeof(int *) * pipeline->pipe_count);
    pipeline->pids = malloc(sizeof(pid_t) * cmd_count);

    for (int i = 0; i < pipeline->pipe_count; i++)
    {
        pipeline->pipes[i] = malloc(sizeof(int) * 2);
        pipe(pipeline->pipes[i]);
    }
}

void cleanup_pipeline(t_pipeline *pipeline)
{
    for (int i = 0; i < pipeline->pipe_count; i++)
    {
        close(pipeline->pipes[i][0]);
        close(pipeline->pipes[i][1]);
        free(pipeline->pipes[i]);
    }
    free(pipeline->pipes);
    free(pipeline->pids);
}