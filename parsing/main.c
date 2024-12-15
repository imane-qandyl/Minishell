/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imqandyl <imqandyl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 19:24:32 by imqandyl          #+#    #+#             */
/*   Updated: 2024/12/15 16:54:44 by imqandyl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int main(void)
{
    // run_parser_tests1();
    run_parser_tests();

    char *input;
    t_command *commands;
    // signal(SIGINT, handle_sigint);   // Ctrl-C
    // signal(SIGQUIT, handle_sigquit); // Ctrl-
    // signal(SIGTERM, handle_sigterm); // Ctrl-D
    while (1)
    {
        input = readline("minishell> ");
        if (!input)
            break;
        if (input[0] != '\0') // Don't process empty input
        {
            add_history(input); // Add to history
            commands = tokenize_input_to_commands(input);

            if (commands)
            {
                print_commands(commands);
                free_command_list(commands);
            }
        }

        free(input); // Free the input string
    }
   
    return 0;
}