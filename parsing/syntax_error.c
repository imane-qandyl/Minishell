/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imqandyl <imqandyl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 08:44:59 by imqandyl          #+#    #+#             */
/*   Updated: 2024/11/27 13:11:12 by imqandyl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
// Check for invalid starting tokens
static int check_start_token(t_token *token)
{
    if (!token)
        return 0;
    if (token->type == TOKEN_PIPE || 
        token->type == TOKEN_REDIR_OUT || 
        token->type == TOKEN_REDIR_IN ||
        token->type == TOKEN_APPEND)
    {
        // printf("syntax error near unexpected token '%s'\n", token->value);
        return SYNTAX_ERROR;
    }
    return 0;
}

static int check_unsupported_operators(t_token *token)
{
    if (token->type == TOKEN_WORD)
    {
        if (strcmp(token->value, "&&") == 0 ||
            strcmp(token->value, "&") == 0)
        {
            // printf("syntax error near unexpected token '%s'\n", token->value);
            return SYNTAX_ERROR;
        }
    }
    return 0;
}

static int check_semicolon_syntax(t_token *token)
{

        // Check if it's the first token
        if (token->type == TOKEN_SEMICOLON)
        {
        // Check if there's a next token and it's not another semicolon
            // if (!token->next || token->next->type != TOKEN_WORD)
            return SYNTAX_ERROR;
        }
    return 0;
}
// Check pipe syntax
static int check_pipe_syntax(t_token *token)
{
    if (token->type == TOKEN_PIPE)
    {
        if (!token->previous) {
            return SYNTAX_ERROR;
        }
        if (!token->next || 
            token->next->type == TOKEN_PIPE ||
            token->next->type == TOKEN_REDIR_OUT ||
            token->next->type == TOKEN_REDIR_IN ||
            token->next->type == TOKEN_APPEND)
        {
            // printf("syntax error near unexpected token '|'\n");
            return SYNTAX_ERROR;
        }
        // t_token *next = token->next;
        // while (next && next->type != TOKEN_PIPE)
        // {
        //     if (next->type == TOKEN_WORD) // Ensure there's a command after the pipe
        //         return 0; // Valid pipe syntax
        //     next = next->next;
        // }
        // return SYNTAX_ERROR;
    }
    return 0;
}

// Check redirection syntax
static int check_redirection_syntax(t_token *token)
{
    if (token->type == TOKEN_REDIR_IN || 
        token->type == TOKEN_REDIR_OUT || 
        token->type == TOKEN_APPEND)
    {
        if (!token->next || token->next->type != TOKEN_WORD)
        {
            // printf("syntax error near unexpected token 'newline'\n");
            return SYNTAX_ERROR;
        }
    }
    return 0;
}

// Check for multiple redirections in a command
static int check_multiple_redirections(t_token *token)
{
    if (token->type == TOKEN_WORD && token->next)
    {
        t_token *next = token->next;
        int input_count = 0;
        int output_count = 0;
        
        while (next && next->type != TOKEN_PIPE)
        {
            if (next->type == TOKEN_REDIR_IN)
                input_count++;
            if (next->type == TOKEN_REDIR_OUT || next->type == TOKEN_APPEND)
                output_count++;
            if (input_count > 1 || output_count > 1)
            {
                // printf("syntax error: multiple redirections\n");
                return SYNTAX_ERROR;
            }
            next = next->next;
        }
    }
    return 0;
}

// Check last token
static int check_last_token(t_token *tokens)
{
    t_token *current = tokens;
    while (current && current->next)
        current = current->next;
        
    if (current->type == TOKEN_PIPE || 
        current->type == TOKEN_REDIR_OUT || 
        current->type == TOKEN_REDIR_IN ||
        current->type == TOKEN_APPEND)
    {
        // printf("syntax error near unexpected token 'newline'\n");
        return SYNTAX_ERROR;
    }
    return 0;
}

// Main syntax error checking function
int check_syntax_error(t_token *tokens)
{
    t_token *current = tokens;

    if (!current)
        return 0;
    if (check_start_token(current))
        return SYNTAX_ERROR;
    
    while (current)
    {
        if (check_unsupported_operators(current))
            return SYNTAX_ERROR;
        if (check_pipe_syntax(current))
            return SYNTAX_ERROR;
        if (check_redirection_syntax(current))
            return SYNTAX_ERROR;
        if (check_multiple_redirections(current))
            return SYNTAX_ERROR;
        if (check_semicolon_syntax(current))
            return SYNTAX_ERROR;
        current = current->next;
    }
    
    if (check_last_token(tokens))
        return SYNTAX_ERROR;
    
    return 0;
}