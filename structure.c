/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structure.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imqandyl <imqandyl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 10:24:29 by imqandyl          #+#    #+#             */
/*   Updated: 2024/11/18 10:29:22 by imqandyl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Add this structure at the top
typedef struct s_parser_state
{
    int error;
    char *error_message;
    int line_number;
} t_parser_state;

// Token list management functions:
t_token *create_token(char *value, t_token_type type)
{
    t_token *new_token = malloc(sizeof(t_token));
    if (!new_token) return NULL;
    new_token->value = strdup(value);
    new_token->type = type;
    new_token->next = NULL;
    return new_token;
}

void add_token(t_token **list, t_token *new_token)
{
    if (!*list)
    {
        *list = new_token;
        return;
    }
    t_token *current = *list;
    while (current->next) current = current->next;
    current->next = new_token;
}

void free_tokens(t_token *list)
{
    while (list)
    {
        t_token *temp = list;
        list = list->next;
        free(temp->value);
        free(temp);
    }
}

// Command list management functions:
t_command *create_command(void)
{
    t_command *cmd = malloc(sizeof(t_command));
    if (!cmd) return NULL;

    cmd->cmd_name = NULL;
    cmd->args = NULL;
    cmd->infile = NULL;
    cmd->outfile = NULL;
    cmd->append_mode = 0;
    cmd->next = NULL;
    return cmd;
}

void add_argument(t_command *cmd, char *arg_value)
{
    t_arg *new_arg = malloc(sizeof(t_arg));
    if (!new_arg) return;

    new_arg->value = strdup(arg_value);
    new_arg->next = NULL;

    if (!cmd->args)
    {
        cmd->args = new_arg;
    }
    else
    {
        t_arg *current = cmd->args;
        while (current->next) current = current->next;
        current->next = new_arg;
    }
}

void add_command(t_command **list, t_command *new_cmd)
{
    if (!*list)
    {
        *list = new_cmd;
        return;
    }
    t_command *current = *list;
    while (current->next) current = current->next;
    current->next = new_cmd;
}

// Tokenizer function:
t_token *tokenize_input(char *input)
{
    t_token *token_list = NULL;
    int i = 0;
    char buffer[1024];
    int j = 0;

    while (input[i])
    {
        // Handle spaces
        if (input[i] == ' ' || input[i] == '\t')
        {
            if (j > 0)
            {
                buffer[j] = '\0';
                add_token(&token_list, create_token(buffer, TOKEN_WORD));
                j = 0;
            }
            i++;
            continue;
        }
        // Handle pipes
        else if (input[i] == '|')
        {
            if (j > 0)
            {
                buffer[j] = '\0';
                add_token(&token_list, create_token(buffer, TOKEN_WORD));
                j = 0;
            }
            add_token(&token_list, create_token("|", TOKEN_PIPE));
            i++;
            continue;
        }
        // Handle redirections
        else if (input[i] == '>' || input[i] == '<')
        {
            if (j > 0)
            {
                buffer[j] = '\0';
                add_token(&token_list, create_token(buffer, TOKEN_WORD));
                j = 0;
            }
            if (input[i] == '>' && input[i + 1] == '>')
            {
                add_token(&token_list, create_token(">>", TOKEN_APPEND));
                i += 2;
            }
            else if (input[i] == '<' && input[i + 1] == '<')
            {
                add_token(&token_list, create_token("<<", TOKEN_HEREDOC));
                i += 2;
            }
            else if (input[i] == '>')
            {
                add_token(&token_list, create_token(">", TOKEN_REDIR_OUT));
                i++;
            }
            else
            {
                add_token(&token_list, create_token("<", TOKEN_REDIR_IN));
                i++;
            }
            continue;
        }
        // Handle quotes
        else if (input[i] == '"' || input[i] == '\'')
        {
            char quote = input[i++];
            while (input[i] && input[i] != quote)
            {
                buffer[j++] = input[i++];
            }
            if (input[i] == quote) i++;  // Skip closing quote
            continue;
        }
        buffer[j++] = input[i++];
    }
    if (j > 0)
    {
        buffer[j] = '\0';
        add_token(&token_list, create_token(buffer, TOKEN_WORD));
    }
    return token_list;
}

// Convert tokens to commands:
t_command *tokenize_input_to_commands(char *input)
{
    t_token *tokens = tokenize_input(input);
    t_command *command_list = NULL;
    t_command *current_command = NULL;
    t_token *token = tokens;

    while (token)
    {
        if (token->type == TOKEN_WORD)
        {
            // If it's the first token of the command, create a new command
            if (!current_command)
            {
                current_command = create_command();
                current_command->cmd_name = strdup(token->value); // Set the command name
            }
            else
            {
                // Add the token as an argument to the current command
                add_argument(current_command, token->value);
            }
        }
        else if (token->type == TOKEN_PIPE)
        {
            if (current_command)
            {
                add_command(&command_list, current_command);
                current_command = NULL;
            }
        }
        else if (token->type == TOKEN_REDIR_IN)
        {
            token = token->next;
            if (token && token->type == TOKEN_WORD)
                current_command->infile = strdup(token->value);
        }
        else if (token->type == TOKEN_REDIR_OUT)
        {
            token = token->next;
            if (token && token->type == TOKEN_WORD)
            {
                current_command->outfile = strdup(token->value);
                current_command->append_mode = 0;
            }
        }
        else if (token->type == TOKEN_APPEND)
        {
            token = token->next;
            if (token && token->type == TOKEN_WORD)
            {
                current_command->outfile = strdup(token->value);
                current_command->append_mode = 1;
            }
        }

        token = token->next;
    }

    // Add the last command to the list if it exists
    if (current_command)
        add_command(&command_list, current_command);

    free_tokens(tokens); // Free the token list

    return command_list;
}

// Print command list and arguments for testing
void print_commands(t_command *cmd_list)
{
    t_command *cmd = cmd_list;
    while (cmd)
    {
        printf("Command: %s\n", cmd->cmd_name);

        t_arg *arg = cmd->args;
        while (arg)
        {
            printf("  Arg: %s\n", arg->value);
            arg = arg->next;
        }

        cmd = cmd->next;
    }
}

int validate_command(t_command *cmd)
{
    if (!cmd->cmd_name)
        return 0;  // Invalid command
    
    // Check for valid file descriptors
    if (cmd->infile && access(cmd->infile, R_OK) != 0)
        return 0;  // Input file not accessible
    
    if (cmd->outfile && cmd->append_mode && access(cmd->outfile, W_OK) != 0)
        return 0;  // Output file not writable
    
    return 1;  // Command is valid
}

int main(void)
{
    char *input;
    t_command *commands;

    while (1)
    {
        input = readline("minishell> ");
        if (!input)
            break; // Exit on EOF (Ctrl+D)

        if (input[0] != '\0') // Don't process empty input
        {
            add_history(input); // Add to history
            commands = tokenize_input_to_commands(input); // Tokenize input into commands and arguments

            // Print the parsed commands and arguments
            print_commands(commands);

            // Free the command list and its arguments
            t_command *cmd = commands;
            while (cmd)
            {
                t_command *temp_cmd = cmd;
                cmd = cmd->next;

                // Free arguments
                t_arg *arg = temp_cmd->args;
                while (arg)
                {
                    t_arg *temp_arg = arg;
                    arg = arg->next;
                    free(temp_arg->value);
                    free(temp_arg);
                }

                free(temp_cmd->cmd_name);
                free(temp_cmd);
            }
        }

        free(input); // Free the input string
    }

    return 0;
}
