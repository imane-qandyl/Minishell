/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iliastepanov <iliastepanov@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 14:04:57 by iliastepano       #+#    #+#             */
/*   Updated: 2024/11/17 16:07:52 by iliastepano      ###   ########.fr       */
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
	char **env;

	if (arg)
		return ;
	env = g_environ;
	while (*env != NULL)
	{
		printf("declare -x %s\n", *env);
			env++;
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
	while (g_environ[i] && strncmp(g_environ[i], arg, strlen(arg)) != 0)
		i++;
	return i;
}

static void	update_existing_value(var_index, new_var)
{
	if (!g_environ[var_index])
		return ;
	free(g_environ[var_index]);
	g_environ[var_index] = new_var;
}

static void	create_new_key(var_index, new_var)
{
	char **new_environ;

	if (g_environ[var_index])
		return ;
	new_environ = realloc(g_environ, sizeof(char*) * (var_index + 2));
	if (!new_environ)
	{
		free(new_var); // return error code instead
		return;
	}
	g_environ = new_environ;
	g_environ[var_index] = new_var;
	g_environ[var_index + 1] = NULL;
}

static void	handle_with_equal_sign(char *arg, char *equal_sign)
{
	char	*value;
	char	*new_var;
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
	new_var = create_line(arg, value, "=");
	if (!new_var)
		return ;
	var_index = find_env_var_index(arg);
	update_existing_value(var_index, new_var);
	create_new_key(var_index, new_var);
}

static void	handle_without_equal_sign(char *arg, char *equal_sign) 
{
	char	*value;
	char	*new_var;
	int		var_index;

	if (equal_sign || !arg)
		return ;
	if (!is_valid_identifier(arg))
	{
		fprintf(stderr, "export: `%s': not a valid identifier\n", arg);
		return;
	}
	new_var = create_line(arg, value, "");
	if (!new_var)
		return ;
	var_index = find_env_var_index(arg);
	update_existing_value(var_index, new_var);
	create_new_key(var_index, new_var);
}

void custom_export(char *arg)
{
	char *equal_sign;
// check arg here
    print_all_env_vars_if_not_arg(arg);
    equal_sign = strchr(arg, '=');
	handle_with_equal_sign(arg, equal_sign);
	handle_without_equal_sign(arg, equal_sign);
	print_all_env_vars_if_not_arg(NULL);

}

int main(int argc, char **argv)
{
	init_environ();
	if (argc < 2)
		custom_export(NULL);
	else
		custom_export(argv[1]);
	cleanup_environ();
	return (0);
}

