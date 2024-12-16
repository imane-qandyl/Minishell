/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imqandyl <imqandyl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 16:45:10 by imqandyl          #+#    #+#             */
/*   Updated: 2024/12/16 17:39:34 by imqandyl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void    setup_redirections(t_command *cmd)
{
    if (cmd->infile)
    {
        int fd;
        
        if (cmd->heredoc_mode)
            fd = handle_heredoc(cmd->infile);
        else
            fd = open(cmd->infile, O_RDONLY);
            
        if (fd < 0)
        {
            perror("minishell");
            exit(1);
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }

    if (cmd->outfile)
    {
        int flags = O_WRONLY | O_CREAT;
        flags |= cmd->append_mode ? O_APPEND : O_TRUNC;
        int fd = open(cmd->outfile, flags, 0644);
        if (fd < 0)
        {
            perror("minishell");
            exit(1);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
}

void    restore_redirections(int stdin_backup, int stdout_backup)
{
    dup2(stdin_backup, STDIN_FILENO);
    dup2(stdout_backup, STDOUT_FILENO);
    close(stdin_backup);
    close(stdout_backup);
}

int handle_heredoc(const char *delimiter)
{
    // Temporary fix to avoid unused parameter warning
    (void)delimiter;
    
    // Temporary implementation that returns a valid file descriptor
    // You should replace this with actual heredoc implementation
    return dup(STDIN_FILENO);
} 