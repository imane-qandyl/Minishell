/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imqandyl <imqandyl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 08:44:59 by imqandyl          #+#    #+#             */
/*   Updated: 2024/12/06 12:58:35 by imqandyl         ###   ########.fr       */
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
        if (token->type == TOKEN_WORD)
        {
            if (token->value[0] == '"' || token->value[0] == '\'')
                    return 0;
                
                // Check for semicolons in unquoted words
                if (strchr(token->value, ';'))
                    return SYNTAX_ERROR;
        }
    return 0;
    
}

// Check pipe syntax
static int check_pipe_syntax(t_token *token)
{
    if (token->type == TOKEN_PIPE)
    {
        if (!token->previous || token->previous->type == TOKEN_PIPE) {
            return SYNTAX_ERROR;
        }
            if (!token->next || 
            token->next->type == TOKEN_PIPE ||
            token->next->type == TOKEN_REDIR_OUT ||
            token->next->type == TOKEN_REDIR_IN ||
            token->next->type == TOKEN_APPEND)
        {
            return SYNTAX_ERROR;
        }
    }
    return 0;
}

// Check redirection syntax
static int check_redirection_syntax(t_token *token)
{
    if (token->type == TOKEN_REDIR_IN || 
        token->type == TOKEN_REDIR_OUT || 
        token->type == TOKEN_APPEND ||
        token->type == TOKEN_HEREDOC)
    {
        // Reject heredoc since it's not supported
        if (token->type == TOKEN_HEREDOC)
            return SYNTAX_ERROR;

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
        int heredoc_count = 0;
        
        while (next && next->type != TOKEN_PIPE)
        {
            if (next->type == TOKEN_REDIR_IN)
                input_count++;
            if (next->type == TOKEN_REDIR_OUT || next->type == TOKEN_APPEND)
                output_count++;
            if (next->type == TOKEN_HEREDOC)
                heredoc_count++;
                
            if (heredoc_count > 0 || input_count > 1 || output_count > 1)
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

// Add this function to check for multiple pipes
// static int check_multiple_pipes(t_token *token)
// {
//     if (token->type == TOKEN_PIPE)
//     {
//         // Count pipes in the entire command
//         t_token *current = token;
//         int pipe_count = 0;
        
//         // Count pipes before current token
//         while (current->previous)
//         {
//             if (current->previous->type == TOKEN_PIPE)
//                 pipe_count++;
//             current = current->previous;
//         }
        
//         // Count pipes after current token
//         current = token->next;
//         while (current)
//         {
//             if (current->type == TOKEN_PIPE)
//                 pipe_count++;
//             current = current->next;
//         }
        
//         // If we found any additional pipes, return error
//         if (pipe_count > 0)
//             return SYNTAX_ERROR;
//     }
//     return 0;
// }

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
        // if (check_multiple_pipes(current))
        //     return SYNTAX_ERROR;
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
