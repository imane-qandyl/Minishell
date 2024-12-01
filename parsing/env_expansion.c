#include <minishell.h>

//When get_var_name is called, it will return "USER" and update i to point to the character after USER.
static char *get_var_name(char *str, int *i)
{
    char var_name[256] = {0};
    int j = 0;
    
    (*i)++; // Skip the $
    if (str[*i] == '?') {
        (*i)++;
        return strdup("?");
    }
    
    while (str[*i] && (isalnum(str[*i]) || str[*i] == '_')) {
        var_name[j++] = str[(*i)++];
    }
    (*i)--; // Adjust for the main loop increment
    if(j > 0)
        return strdup(var_name);
    return NULL;
}

//When expand_var is called, it will return "Imane_q" if the environment variable USER is set to "john_doe".
static char *expand_var(char *var_name, int *exit_status)
{
    if (!var_name)
        return strdup("$");
    
    if (strcmp(var_name, "?") == 0)
    {
        char num[12];
        printf("%d", *exit_status);
        return strdup(num);
    }
    
    char *value = getenv(var_name);
    if(value)
        return strdup(value);
    return strdup("");
}

char *expand_env_vars(char *input, int *exit_status)
{
    char *result = malloc(4096);
    int i = 0, j = 0;
    
    while (input[i])
    {
        if (input[i] == '$' && input[i + 1] != '\0')
        {
            char *var_name = get_var_name(input, &i);
            char *value = expand_var(var_name, exit_status);
            
            strcpy(result + j, value);
            j += strlen(value);
            
            free(var_name);
            free(value);
        }
        else
            result[j++] = input[i];
        i++;
    }
    result[j] = '\0';
    
    return result;
} 