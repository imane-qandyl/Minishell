#include "../include/minishell.h"

static void test_result(const char *test_name, int success)
{
    printf("%-40s: %s\n", test_name, success ? "\033[32m[OK]\033[0m" : "\033[31m[FAIL]\033[0m");
}

static int test_tokenizer(void)
{
    int success = 1;
    char *test_cases[] = {
        "echo hello world",
        "ls -la | grep test",
        "echo \"Hello World\" > output.txt",
        "cat < input.txt | grep pattern > output.txt",
        "echo $USER",
        "echo 'single quoted $USER'",
        "echo \"double quoted $USER\"",
        NULL
    };

    for (int i = 0; test_cases[i]; i++)
    {
        t_token *tokens = tokenize_input(test_cases[i]);
        if (!tokens)
        {
            printf("Failed to tokenize: %s\n", test_cases[i]);
            success = 0;
            continue;
        }
        printf("\nTokens for: %s\n", test_cases[i]);
        t_token *current = tokens;
        while (current)
        {
            printf("Token: {value: '%s', type: %d}\n", current->value, current->type);
            current = current->next;
        }
        free_tokens(tokens);
    }
    return success;
}

static int test_syntax_errors(void)
{
    struct {
        char *input;
        int should_fail;
    } test_cases[] = {
        {"echo hello", 0},          // Valid
        {"|echo hello", 1},         // Invalid: pipe at start
        {"echo hello||", 1},        // Invalid: double pipe
        {"echo > ", 1},             // Invalid: missing redirect target
        {"echo hello > file.txt", 0}, // Valid
        {"echo hello >", 1},        // Invalid: missing redirect target
        {NULL, 0}
    };

    int success = 1;
    for (int i = 0; test_cases[i].input; i++)
    {
        t_token *tokens = tokenize_input(test_cases[i].input);
        int result = check_syntax_error(tokens);
        if ((result != 0) != test_cases[i].should_fail)
        {
            printf("Syntax error test failed for: %s\n", test_cases[i].input);
            printf("Expected %d, got %d\n", test_cases[i].should_fail, result != 0);
            success = 0;
        }
        free_tokens(tokens);
    }
    return success;
}

static int test_env_expansion(void)
{
    int exit_status = 0;
    struct {
        char *input;
        char *expected_contains;
    } test_cases[] = {
        {"$USER", getenv("USER") ? getenv("USER") : ""},
        {"$PATH", getenv("PATH") ? getenv("PATH") : ""},
        {"$NONEXISTENT", ""},
        {"$?", "0"},
        {NULL, NULL}
    };

    int success = 1;
    for (int i = 0; test_cases[i].input; i++)
    {
        char *result = expand_env_vars(test_cases[i].input, &exit_status);
        
        if (result == NULL || test_cases[i].expected_contains == NULL) {
            printf("Environment expansion failed for: %s\n", test_cases[i].input);
            success = 0;
            free(result);
            continue;
        }

        if (!strstr(result, test_cases[i].expected_contains))
        {
            printf("Environment expansion failed for: %s\n", test_cases[i].input);
            success = 0;
        }
        free(result);
    }
    return success;
}

static int test_quote_handling(void)
{
    struct {
        char *input;
        char expected_result[256];
    } test_cases[] = {
        {"'hello world'", "hello world"},
        {"\"$USER\"", ""},
        {"'$USER'", "$USER"},
        {NULL, ""}
    };

    int success = 1;
    for (int i = 0; test_cases[i].input; i++)
    {
        char buffer[256] = {0};
        int j = 0;
        int pos = 0;
        
        if (handle_quotes(test_cases[i].input, &pos, buffer, &j) != 0)
        {
            printf("Quote handling failed for: %s\n", test_cases[i].input);
            success = 0;
            continue;
        }
        
        if (strcmp(buffer, test_cases[i].expected_result) != 0)
        {
            printf("Quote handling mismatch for: %s\n", test_cases[i].input);
            printf("Expected: %s\n", test_cases[i].expected_result);
            printf("Got: %s\n", buffer);
            success = 0;
        }
    }
    return success;
}

void run_parser_tests(void)
{
    printf("\n=== Running Parser Tests ===\n\n");
    
    test_result("Tokenizer Test", test_tokenizer());
    test_result("Syntax Error Test", test_syntax_errors());
    test_result("Environment Expansion Test", test_env_expansion());
    test_result("Quote Handling Test", test_quote_handling());
    
    printf("\n=== Tests Complete ===\n\n");
}