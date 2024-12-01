/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imqandyl <imqandyl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 12:01:15 by imqandyl          #+#    #+#             */
/*   Updated: 2024/11/30 23:40:25 by imqandyl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

char input_buffer[1024]; // Buffer to hold user input

void clear_input_buffer() {
    memset(input_buffer, 0, sizeof(input_buffer)); // Clear the buffer
}

void handle_sigint(int sig) {
    // Handle Ctrl-C
    printf("\n"); // New line
    clear_input_buffer(); // Clear buffer logic here
    printf("minishell> "); // Display new prompt
    fflush(stdout); // Ensure prompt is displayed immediately
}

void handle_sigquit(int sig) {
    // Handle Ctrl-\
    // Do nothing
}

void handle_sigterm(int sig) {
    // Handle Ctrl-D
    // Quit logic here
    // You may want to implement a flag to indicate quitting
    printf("\nExiting minishell...\n");
    exit(0); // Exit the shell
}

void setup_signal_handlers() {
    signal(SIGINT, handle_sigint);   // Ctrl-C
    signal(SIGQUIT, handle_sigquit); // Ctrl-\
    signal(SIGTERM, handle_sigterm); // Ctrl-D
}

int main() {
    setup_signal_handlers(); // Set up signal handlers

    while (1) {
        printf("minishell> ");
        fflush(stdout); // Ensure prompt is displayed immediately

        // Read input from user
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            // Handle EOF (Ctrl-D)
            handle_sigterm(0);
        }

        // Process the input (you would implement your command processing here)
        // For now, just clear the buffer after processing
        clear_input_buffer();
    }

    return 0;
}
