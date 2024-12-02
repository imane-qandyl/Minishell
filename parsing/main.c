/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iliastepanov <iliastepanov@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 19:24:32 by imqandyl          #+#    #+#             */
/*   Updated: 2024/12/02 15:27:49 by iliastepano      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int main(void)
{
    char *input;
    t_command *commands;
    //  run_parser_tests();
    init_environ();
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
                execute_commands(commands);
                free_command_list(commands);
            }
        }

        free(input); // Free the input string
    }
    cleanup_environ();
    printf("DONE");
    return 0;
}
