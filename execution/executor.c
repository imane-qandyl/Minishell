/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imqandyl <imqandyl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 17:11:25 by imqandyl          #+#    #+#             */
/*   Updated: 2024/12/16 17:35:53 by imqandyl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char *find_command_path(char *cmd)
{
    char *path = getenv("PATH");
    char *path_copy = strdup(path);
    char *dir = strtok(path_copy, ":");
    char *cmd_path;

    while (dir)
    {
        cmd_path = malloc(strlen(dir) + strlen(cmd) + 2);
        sprintf(cmd_path, "%s/%s", dir, cmd);
        if (access(cmd_path, X_OK) == 0)
        {
            free(path_copy);
            return cmd_path;
        }
        free(cmd_path);
        dir = strtok(NULL, ":");
    }
    free(path_copy);
    return NULL;
}

int execute_single_command(t_command *cmd)
{
    int stdin_backup = dup(STDIN_FILENO);
    int stdout_backup = dup(STDOUT_FILENO);
    int status = 0;

    if (!cmd || !cmd->cmd_name)
        return 1;

    if (is_builtin(cmd->cmd_name))
        return execute_builtin(cmd);

    pid_t pid = fork();
    if (pid == 0)
    {
        setup_redirections(cmd);
        char *cmd_path = find_command_path(cmd->cmd_name);
        if (!cmd_path)
        {
            fprintf(stderr, "minishell: command not found: %s\n", cmd->cmd_name);
            exit(127);
        }
        execve(cmd_path, cmd->argv, environ);
        perror("minishell");
        exit(1);
    }
    else if (pid < 0)
    {
        perror("minishell: fork");
        return 1;
    }

    waitpid(pid, &status, 0);
    restore_redirections(stdin_backup, stdout_backup);
    return WEXITSTATUS(status);
}

int execute_pipeline(t_command *cmd, t_pipeline *pipeline)
{
    t_command *current = cmd;
    int cmd_index = 0;

    while (current)
    {
        pipeline->pids[cmd_index] = fork();
        if (pipeline->pids[cmd_index] == 0)
        {
            // Set up pipes for child process
            if (cmd_index > 0)
                dup2(pipeline->pipes[cmd_index - 1][0], STDIN_FILENO);
            if (cmd_index < pipeline->pipe_count)
                dup2(pipeline->pipes[cmd_index][1], STDOUT_FILENO);

            // Close all pipe fds in child
            for (int i = 0; i < pipeline->pipe_count; i++)
            {
                close(pipeline->pipes[i][0]);
                close(pipeline->pipes[i][1]);
            }

            setup_redirections(current);
            if (is_builtin(current->cmd_name))
                exit(execute_builtin(current));

            char *cmd_path = find_command_path(current->cmd_name);
            if (!cmd_path)
            {
                fprintf(stderr, "minishell: command not found: %s\n", 
                    current->cmd_name);
                exit(127);
            }
            execve(cmd_path, current->argv, environ);
            exit(1);
        }
        current = current->next;
        cmd_index++;
    }

    // Close all pipe fds in parent
    for (int i = 0; i < pipeline->pipe_count; i++)
    {
        close(pipeline->pipes[i][0]);
        close(pipeline->pipes[i][1]);
    }

    // Wait for all children
    for (int i = 0; i < cmd_index; i++)
        waitpid(pipeline->pids[i], &pipeline->status, 0);

    return WEXITSTATUS(pipeline->status);
}

int execute_command_line(t_command *cmd_list)
{
    t_pipeline pipeline;
    int status;

    if (!cmd_list)
        return 0;

    if (!cmd_list->next)
        return execute_single_command(cmd_list);

    setup_pipeline(&pipeline, cmd_list);
    status = execute_pipeline(cmd_list, &pipeline);
    cleanup_pipeline(&pipeline);

    return status;
}
