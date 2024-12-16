/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imqandyl <imqandyl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 17:30:25 by imqandyl          #+#    #+#             */
/*   Updated: 2024/12/16 17:27:13 by imqandyl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void initialize_shell(void)
{
    char *prompt;

    prompt = getenv("USER");
    if (prompt)
        printf("\n\nWelcome %s to minishell\n\n", prompt);
    else
        printf("\n\nWelcome to minishell\n\n");
}

static void handle_input(char *input, int *exit_status)
{
    t_command *cmd_list;

    if (!input)
    {
        printf("exit\n");
        exit(*exit_status);
    }
    if (*input)
    {
        add_history(input);
        input = expand_env_vars(input, exit_status);
        cmd_list = tokenize_input_to_commands(input);
        if (cmd_list)
        {
            *exit_status = execute_command_line(cmd_list);
            free_command_list(cmd_list);
        }
        free(input);
    }
}

int main(void)
{
    char *input;
    int exit_status;

    initialize_shell();
    exit_status = 0;
    while (1)
    {
        input = readline("minishell$ ");
        handle_input(input, &exit_status);
    }
    return (0);
}