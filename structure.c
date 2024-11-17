#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef enum e_token_type
{
    TOKEN_WORD,     // Commands and arguments
    TOKEN_PIPE,     // |
    TOKEN_REDIR_IN, // <
    TOKEN_REDIR_OUT,// >
    TOKEN_APPEND,   // >>
    TOKEN_HEREDOC,  // <<
    TOKEN_SPACE     // Space or tabs
} t_token_type;

typedef struct s_token
{
    char            *value;
    t_token_type    type;
    struct s_token  *next;
} t_token;

typedef struct s_arg
{
    char            *value;
    struct s_arg    *next;
} t_arg;

typedef struct s_command
{
    char            *cmd_name;   // The command name (e.g., "echo")
    t_arg           *args;       // A linked list of arguments
    char            *infile;     // Input redirection (optional)
    char            *outfile;    // Output redirection (optional)
    int             append_mode; // Whether output redirection is in append mode (1) or not (0)
    struct s_command *next;      // Points to the next command in the list
} t_command;

// Token list management functions:
t_token *create_token(char *value, t_token_type type)
{
    t_token *new_token = malloc(sizeof(t_token));
    if (!new_token) return NULL;
    new_token->value = strdup(value);
    new_token->type = type;
    new_token->next = NULL;
    return new_token;
}

void add_token(t_token **list, t_token *new_token)
{
    if (!*list)
    {
        *list = new_token;
        return;
    }
    t_token *current = *list;
    while (current->next) current = current->next;
    current->next = new_token;
}

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

// Command list management functions:
t_command *create_command(void)
{
    t_command *cmd = malloc(sizeof(t_command));
    if (!cmd) return NULL;

    cmd->cmd_name = NULL;
    cmd->args = NULL;
    cmd->infile = NULL;
    cmd->outfile = NULL;
    cmd->append_mode = 0;
    cmd->next = NULL;
    return cmd;
}

void add_argument(t_command *cmd, char *arg_value)
{
    t_arg *new_arg = malloc(sizeof(t_arg));
    if (!new_arg) return;

    new_arg->value = strdup(arg_value);
    new_arg->next = NULL;

    if (!cmd->args)
    {
        cmd->args = new_arg;
    }
    else
    {
        t_arg *current = cmd->args;
        while (current->next) current = current->next;
        current->next = new_arg;
    }
}

void add_command(t_command **list, t_command *new_cmd)
{
    if (!*list)
    {
        *list = new_cmd;
        return;
    }
    t_command *current = *list;
    while (current->next) current = current->next;
    current->next = new_cmd;
}

// Tokenizer function:
t_token *tokenize_input(char *input)
{
    t_token *token_list = NULL;
    int i = 0;
    char buffer[1024];
    int j = 0;

    while (input[i])
    {
        // Handle spaces
        if (input[i] == ' ' || input[i] == '\t')
        {
            if (j > 0)
            {
                buffer[j] = '\0';
                add_token(&token_list, create_token(buffer, TOKEN_WORD));
                j = 0;
            }
            i++;
            continue;
        }
        // Handle pipes
        else if (input[i] == '|')
        {
            if (j > 0)
            {
                buffer[j] = '\0';
                add_token(&token_list, create_token(buffer, TOKEN_WORD));
                j = 0;
            }
            add_token(&token_list, create_token("|", TOKEN_PIPE));
            i++;
            continue;
        }
        // Handle redirections
        else if (input[i] == '>' || input[i] == '<')
        {
            if (j > 0)
            {
                buffer[j] = '\0';
                add_token(&token_list, create_token(buffer, TOKEN_WORD));
                j = 0;
            }
            if (input[i] == '>' && input[i + 1] == '>')
            {
                add_token(&token_list, create_token(">>", TOKEN_APPEND));
                i += 2;
            }
            else if (input[i] == '<' && input[i + 1] == '<')
            {
                add_token(&token_list, create_token("<<", TOKEN_HEREDOC));
                i += 2;
            }
            else if (input[i] == '>')
            {
                add_token(&token_list, create_token(">", TOKEN_REDIR_OUT));
                i++;
            }
            else
            {
                add_token(&token_list, create_token("<", TOKEN_REDIR_IN));
                i++;
            }
            continue;
        }
        buffer[j++] = input[i++];
    }
    if (j > 0)
    {
        buffer[j] = '\0';
        add_token(&token_list, create_token(buffer, TOKEN_WORD));
    }
    return token_list;
}

// Convert tokens to commands:
t_command *tokenize_input_to_commands(char *input)
{
    t_token *tokens = tokenize_input(input);
    t_command *command_list = NULL;
    t_command *current_command = NULL;
    t_token *token = tokens;

    while (token)
    {
        if (token->type == TOKEN_WORD)
        {
            // If it's the first token of the command, create a new command
            if (!current_command)
            {
                current_command = create_command();
                current_command->cmd_name = strdup(token->value); // Set the command name
            }
            else
            {
                // Add the token as an argument to the current command
                add_argument(current_command, token->value);
            }
        }
        else if (token->type == TOKEN_PIPE || token->type == TOKEN_REDIR_IN ||
                 token->type == TOKEN_REDIR_OUT || token->type == TOKEN_APPEND || token->type == TOKEN_HEREDOC)
        {
            // Handle pipes or redirections (optional)
        }

        token = token->next;
    }

    // Add the last command to the list if it exists
    if (current_command)
        add_command(&command_list, current_command);

    free_tokens(tokens); // Free the token list

    return command_list;
}

// Print command list and arguments for testing
void print_commands(t_command *cmd_list)
{
    t_command *cmd = cmd_list;
    while (cmd)
    {
        printf("Command: %s\n", cmd->cmd_name);

        t_arg *arg = cmd->args;
        while (arg)
        {
            printf("  Arg: %s\n", arg->value);
            arg = arg->next;
        }

        cmd = cmd->next;
    }
}

int main(void)
{
    char *input;
    t_command *commands;

    while (1)
    {
        input = readline("minishell> ");
        if (!input)
            break; // Exit on EOF (Ctrl+D)

        if (input[0] != '\0') // Don't process empty input
        {
            add_history(input); // Add to history
            commands = tokenize_input_to_commands(input); // Tokenize input into commands and arguments

            // Print the parsed commands and arguments
            print_commands(commands);

            // Free the command list and its arguments
            t_command *cmd = commands;
            while (cmd)
            {
                t_command *temp_cmd = cmd;
                cmd = cmd->next;

                // Free arguments
                t_arg *arg = temp_cmd->args;
                while (arg)
                {
                    t_arg *temp_arg = arg;
                    arg = arg->next;
                    free(temp_arg->value);
                    free(temp_arg);
                }

                free(temp_cmd->cmd_name);
                free(temp_cmd);
            }
        }

        free(input); // Free the input string
    }

    return 0;
}
