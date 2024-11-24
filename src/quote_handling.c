/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imqandyl <imqandyl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 19:09:22 by imqandyl          #+#    #+#             */
/*   Updated: 2024/11/24 08:12:43 by imqandyl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Handle quote parsing
int handle_quotes(char *input, int *i, char *buffer, int *j)
{
    if (!input || !i || !buffer || !j)
        return QUOTE_ERROR;
    
    char quote = input[*i];
    (*i)++;  // Skip opening quote
    
    while (input[*i] && input[*i] != quote)
    {
        // For single quotes, copy everything literally
        if (quote == '\'')
        {
            buffer[(*j)++] = input[(*i)++];
        }
        // For double quotes, allow $ expansion but prevent other special interpretations
        else if (quote == '"')
        {
            // Special handling for $ inside double quotes
            if (input[*i] == '$' && isalpha(input[*i + 1]))
            {
                char var_name[256] = {0};
                int k = 0;
                (*i)++;  // Skip $
                
                while (input[*i] && (isalnum(input[*i]) || input[*i] == '_')) //lik echo "$USER_NAME" without _ it would miss Nam
                    var_name[k++] = input[(*i)++];
                
                char *value = getenv(var_name);
                if (value)
                {
                    for (k = 0; value[k]; k++)
                        buffer[(*j)++] = value[k];
                }
                (*i)--; // Adjust for upcoming increment
            }
            else
            {
                buffer[(*j)++] = input[(*i)++];
            }
        }
    }
    
    if (!input[*i])  // Quote was not closed
        return QUOTE_ERROR;
    
    (*i)++;  // Skip closing quote
    buffer[*j] = '\0';  // Null terminate the buffer
    return 0;
}

// Syntax error checking
int check_syntax_error(t_token *tokens)
{
    t_token *current = tokens;
    
    while (current)
    {
        // Check for pipe errors
        if (current->type == TOKEN_PIPE)
        {
            if (!current->next || !current->previous) //pipe at start or end
                return SYNTAX_ERROR;
            if (current->next->type == TOKEN_PIPE) //double pipe    
                return SYNTAX_ERROR;
        }
        // Check for redirection errors
        if (current->type == TOKEN_REDIR_IN || 
            current->type == TOKEN_REDIR_OUT || 
            current->type == TOKEN_APPEND)
        {
            if (!current->next || current->next->type != TOKEN_WORD)
                return SYNTAX_ERROR;
        }
        current = current->next;
    }
    return 0;
}