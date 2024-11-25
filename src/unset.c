/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iliastepanov <iliastepanov@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 14:04:57 by iliastepano       #+#    #+#             */
/*   Updated: 2024/11/17 14:05:50 by iliastepano      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_valid_identifier(const char *name) {
    if (!name || !isalpha(name[0]) && name[0] != '_')
        return 0;

    for (int i = 1; name[i] != '\0'; i++) {
        if (!isalnum(name[i]) && name[i] != '_')
            return 0;
    }
    return 1;
}

void unset_variable(const char *var) {
    if (!is_valid_identifier(var)) {
        fprintf(stderr, "unset: `%s': not a valid identifier\n", var);
        return;
    }

    // Attempt to unset the environment variable
    if (unsetenv(var) != 0) {
        perror("unset");  // Print error if unsetenv fails
    }
}

// Example usage
int main() {
    unset_variable("VAR1");
    return 0;
}
