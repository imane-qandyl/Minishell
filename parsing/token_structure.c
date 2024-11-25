/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_structure.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imqandyl <imqandyl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 10:24:29 by imqandyl          #+#    #+#             */
/*   Updated: 2024/11/25 09:15:29 by imqandyl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Create a new token
t_token *create_token(char *value, t_token_type type)
{
    t_token *new_token = malloc(sizeof(t_token));
    if (!new_token) return NULL;
    new_token->value = strdup(value);
    if (!new_token->value)
    {
        free(new_token);
        return NULL;
    }
    new_token->type = type;
    new_token->position = 0;
    new_token->line_number = 1;
    new_token->next = NULL;
    new_token->previous = NULL;
    return new_token;
}

// Add a token to the end of the list
void add_token(t_token **list, t_token *new_token)
{
    if (!*list)
    {
        *list = new_token;
        new_token->previous = NULL;
        return;
    }
    t_token *current = *list;
    while (current->next) current = current->next;
    current->next = new_token;
    new_token->previous = current;
}

// Free all tokens in a list
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