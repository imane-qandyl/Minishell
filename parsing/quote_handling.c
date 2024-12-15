/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imqandyl <imqandyl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 19:09:22 by imqandyl          #+#    #+#             */
/*   Updated: 2024/12/15 21:30:52 by imqandyl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
                
                while (input[*i] && (isalnum(input[*i]) || input[*i] == '_')) 
                {
                    var_name[k++] = input[(*i)++];
                }
                // char *value = getenv(var_name);
                // if (value)
                // {
                //     for (k = 0; value[k] && *j < 255; k++) // Prevent buffer overflow
                //     {
                //         buffer[(*j)++] = value[k];
                //     }
                // }
                // If we want empty string for environment variables in double quotes
                buffer[*j] = '\0';  // Just add null terminator without copying anything
            }
            else
            {
                buffer[(*j)++] = input[(*i)++];
            }
        }

        // Prevent buffer overflow
        if (*j >= 255) {
            buffer[*j] = '\0'; // Null terminate
            return QUOTE_ERROR; // Indicate an error
        }
    }
    
    if (!input[*i])  // Quote was not closed
        return QUOTE_ERROR;
    
    (*i)++;  // Skip closing quote
    buffer[*j] = '\0';  // Null terminate the buffer
    return 0;
}
