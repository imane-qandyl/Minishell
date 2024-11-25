/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_to_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imqandyl <imqandyl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 19:09:57 by imqandyl          #+#    #+#             */
/*   Updated: 2024/11/25 09:16:29 by imqandyl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
// just categorizes them
static void handle_buffer_flush(t_token **token_list, char *buffer, int *j)
{
    if (*j > 0)
    {
        buffer[*j] = '\0';
        add_token(token_list, create_token(buffer, TOKEN_WORD));
        *j = 0;
    }
}

static int handle_redirection_tokens(t_token **token_list, char *input, int *i)
{
    if (input[*i] == '>' && input[*i + 1] == '>')
    {
        add_token(token_list, create_token(">>", TOKEN_APPEND));
        *i += 2;
    }
    else if (input[*i] == '<' && input[*i + 1] == '<')
    {
        add_token(token_list, create_token("<<", TOKEN_HEREDOC));
        *i += 2;
    }
    else if (input[*i] == '>')
    {
        add_token(token_list, create_token(">", TOKEN_REDIR_OUT));
        (*i)++;
    }
    else
    {
        add_token(token_list, create_token("<", TOKEN_REDIR_IN));
        (*i)++;
    }
    return 0;
}

t_token *tokenize_input(char *input)
{
    t_token *token_list = NULL;
    int i = 0;
    char buffer[1024];
    int j = 0;

    while (input[i])
    {
        if (input[i] == '\\' || input[i] == ';')
        {
            i++;
            continue;
        }
        if (input[i] == ' ' || input[i] == '\t')
        {
            handle_buffer_flush(&token_list, buffer, &j);
            i++;
            continue;
        }
        else if (input[i] == '|')
        {
            handle_buffer_flush(&token_list, buffer, &j);
            add_token(&token_list, create_token("|", TOKEN_PIPE));
            i++;
            continue;
        }
        else if (input[i] == '>' || input[i] == '<')
        {
            handle_buffer_flush(&token_list, buffer, &j);
            handle_redirection_tokens(&token_list, input, &i);
            continue;
        }
        else if (input[i] == '"' || input[i] == '\'')
        {
            handle_buffer_flush(&token_list, buffer, &j);
            if (handle_quotes(input, &i, buffer, &j) != 0)
            {
                free_tokens(token_list);
                return NULL;
            }
            continue;
        }
        buffer[j++] = input[i++];
    }
    
    handle_buffer_flush(&token_list, buffer, &j);
    return token_list;
}

static int handle_word_token(t_command **current_cmd, t_token *token)
{
    if (!*current_cmd)
    {
        *current_cmd = create_command();
        if (!*current_cmd)
            return 1;
        (*current_cmd)->cmd_name = strdup(token->value);
        if (!(*current_cmd)->cmd_name)
        {
            free(*current_cmd);
            *current_cmd = NULL;
            return 1;
        }
    }
    else if (!(*current_cmd)->cmd_name)
    {     
        (*current_cmd)->cmd_name = strdup(token->value);
        (*current_cmd)->cmd_name = strdup(token->value);
        if (!(*current_cmd)->cmd_name)
            return 1;
    }
    else
        add_argument(*current_cmd, token->value);
    return 0;
}

static int handle_redirection(t_command *cmd, t_token **token, int type)
{
    *token = (*token)->next;
    if (!*token || (*token)->type != TOKEN_WORD)
        return 1;

    if (type == TOKEN_REDIR_IN || type == TOKEN_HEREDOC)
    {
        if (cmd->infile)
            free(cmd->infile);
        cmd->infile = strdup((*token)->value);
        cmd->heredoc_mode = (type == TOKEN_HEREDOC);
    }
    else
    {
        if (cmd->outfile)
            free(cmd->outfile);
        cmd->outfile = strdup((*token)->value);
        cmd->append_mode = (type == TOKEN_APPEND);
    }
    return 0;
}

static void cleanup_commands(t_command *command_list, t_command *current_command, t_token *tokens)
{
    if (current_command && current_command != command_list)
        free_command_list(current_command);
    if (command_list)
        free_command_list(command_list);
    if (tokens)
        free_tokens(tokens);
}

t_command *tokenize_input_to_commands(char *input)
{
    t_token *tokens = tokenize_input(input);
    if (!tokens || check_syntax_error(tokens))
    {
        printf("syntax error\n");
        free_tokens(tokens);
        return NULL;
    }
    
    t_command *command_list = NULL;
    t_command *current_command = NULL;
    t_token *token = tokens;

    while (token)
    {
        if (token->type == TOKEN_WORD)
        {
            if (handle_word_token(&current_command, token))
            {
                cleanup_commands(command_list, current_command, tokens);
                return NULL;
            }
        }
        else if (token->type == TOKEN_PIPE)
        {
            if (!current_command)
            {
                printf("syntax error");
                cleanup_commands(command_list, current_command, tokens);
                return NULL;
            }
            add_command(&command_list, current_command);
            current_command = NULL;
        }
        else if (token->type >= TOKEN_REDIR_IN && token->type <= TOKEN_HEREDOC)
        {
            if (!current_command || handle_redirection(current_command, &token, token->type))
            {
                printf("syntax error\n");
                cleanup_commands(command_list, current_command, tokens);
                return NULL;
            }
        }
        token = token->next;
    }

    if (current_command)
        add_command(&command_list, current_command);

    free_tokens(tokens);
    return command_list;
}
/*// Let's test with: echo "Hello World" > output.txt | grep "Hello"

// First, tokenize_input() will create these tokens:
t_token *tokens = tokenize_input("echo \"Hello World\" > output.txt | grep \"Hello\"");

// The tokens would look like this:
tokens = [
    {value: "echo", type: TOKEN_WORD}
    {value: "Hello World", type: TOKEN_WORD}  // Note: quotes are handled
    {value: ">", type: TOKEN_REDIR_OUT}
    {value: "output.txt", type: TOKEN_WORD}
    {value: "|", type: TOKEN_PIPE}
    {value: "grep", type: TOKEN_WORD}
    {value: "Hello", type: TOKEN_WORD}
]

// Then, tokenize_input_to_commands() will create command structures:
t_command *commands = tokenize_input_to_commands("echo \"Hello World\" > output.txt | grep \"Hello\"");

// The commands would look like this:
commands = [
    Command1 {
        cmd_name: "echo"
        args: ["Hello World"]
        outfile: "output.txt"
        append_mode: 0
        infile: NULL
    },
    Command2 {
        cmd_name: "grep"
        args: ["Hello"]
        outfile: NULL
        append_mode: 0
        infile: NULL
    }
]*/