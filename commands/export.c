/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iliastepanov <iliastepanov@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 14:04:57 by iliastepano       #+#    #+#             */
/*   Updated: 2024/11/24 16:44:27 by iliastepano      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_identifier(const char *name)
{
    int	i;

    if (!name || !isalpha(name[0]) && name[0] != '_')
        return 0;
	i = -1;
    while (name[++i])
        if (!isalnum(name[i]) && name[i] != '_')
            return 0;
    return 1;
}

static void	print_all_env_vars_if_not_arg(char *arg)
{
	int	i;

	if (arg)
		return ;
	i = 0;
	while (g_environ[0][i])
	{
		if (g_environ[1][i])
			printf("declare -x %s=\"%s\"\n", g_environ[0][i], g_environ[1][i]);
		else
			printf("declare -x %s\n", g_environ[0][i]);
		i++;
	}
}

static char *create_line(char *name, char *value, char *sep)
{
	char	*new_var;
	int		len_sep;

	len_sep = strlen(sep);
	new_var = malloc(strlen(name) + strlen(value) + len_sep + 1);
	if (!new_var)
		return NULL;
	strcpy(new_var, name);
	if (sep)
		strcat(new_var, sep);
	strcat(new_var, value);
	return (new_var);
}

static int	find_env_var_index(char *arg)
{
	int	i;

	i = 0;
	while (g_environ[0][i] && strcmp(g_environ[0][i], arg) != 0)
		i++;
	return i;
}

static void	update_existing_value(int var_index, char *name, char *value)
{
	if (!g_environ[0][var_index])
		return ;
	free(g_environ[0][var_index]);
	free(g_environ[1][var_index]);
	g_environ[0][var_index] = strdup(name);
	g_environ[1][var_index] = strdup(value);
}

static void	create_new_key(int var_index, char *name, char *value)
{
	char **new_keys;
	char **new_values;

	if (g_environ[0][var_index])
		return ;
		
	new_keys = realloc(g_environ[0], sizeof(char*) * (var_index + 2));
	new_values = realloc(g_environ[1], sizeof(char*) * (var_index + 2));
	if (!new_keys || !new_values)
	{
		free(new_keys);
		free(new_values);
		return;
	}
	g_environ[0] = new_keys;
	g_environ[1] = new_values;
	g_environ[0][var_index] = strdup(name);
	g_environ[1][var_index] = strdup(value);
	g_environ[0][var_index + 1] = NULL;
	g_environ[1][var_index + 1] = NULL;
}

static void	handle_with_equal_sign(char *arg, char *equal_sign)
{
	char	*value;
	int		var_index;

	if (!equal_sign || !arg)
		return ;
	*equal_sign = '\0';
	value = equal_sign + 1;
	if (!is_valid_identifier(arg))
	{
		perror("export: not a valid identifier\n");
		return ;
	}
	var_index = find_env_var_index(arg);
	update_existing_value(var_index, arg, value);
	create_new_key(var_index, arg, value);
}

static void	handle_without_equal_sign(char *arg, char *equal_sign) 
{
	char	*value;
	int		var_index;

	if (equal_sign || !arg)
		return ;
	if (!is_valid_identifier(arg))
	{
		fprintf(stderr, "export: `%s': not a valid identifier\n", arg);
		return;
	}
	value = "";
	var_index = find_env_var_index(arg);
	update_existing_value(var_index, arg, value);
	create_new_key(var_index, arg, value);
}

void custom_export(char *arg)
{
	char *equal_sign;

	if (!arg)
	{
		print_all_env_vars_if_not_arg(NULL);
		return;
	}
    equal_sign = strchr(arg, '=');
	handle_with_equal_sign(arg, equal_sign);
	handle_without_equal_sign(arg, equal_sign);
}

// int main(int argc, char **argv)
// {
// 	init_environ();
// 	if (argc < 2)
// 		custom_export(NULL);
// 	else
// 		custom_export(argv[1]);
// 	cleanup_environ();
// 	return (0);
// }
