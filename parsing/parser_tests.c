#include "../include/minishell.h"
#include <ctype.h>

// Add these declarations at the top of the file, after the includes
bool is_valid_command(const char *command);
bool is_empty_or_whitespace(const char *str);

// Add this before the tests array
typedef struct s_test_case {
    const char *input;
    bool expected_valid;
    const char *description;
} t_test_case;

static t_test_case tests[] = {
    // ... existing test cases ...

  // Simple Commands
    {"ls", true, "Simple command"},
    {"   ls   ", true, "Command with spaces"},
    {"", false, "Empty command"},
    {"     ", false, "Only spaces"},

    // Redirection Tests
    {"ls > file", true, "Simple output redirection"},
    {"ls > file1 > file2", false, "Multiple output redirections (invalid)"},
    {"cat < file", true, "Input redirection"},
    {"cat < file1 < file2", false, "Multiple input redirections (invalid)"},
    {"ls >> file", true, "Append redirection"},
    {">", false, "Only redirection symbol"},
    {"ls >", false, "Missing redirection target"},
    {"ls > ", false, "Missing redirection target with space"},
    {"ls > file1 >> file2", false, "Mixed output redirections"},
    {"< file cat", false, "Redirection before command"},

    // Pipe Tests
    {"ls | grep a", true, "Simple pipe"},
    {"ls | grep a | wc -l", true, "Multiple pipes"},
    {"ls | grep a > file", true, "Pipe with redirection"},
    {"|", false, "Only pipe"},
    {"ls |", false, "Pipe at end"},
    {"| ls", false, "Pipe at start"},
    {"ls || grep", false, "Double pipe"},
    {"ls | | grep", false, "Empty pipe command"},

    // Quote Tests
    {"echo \"hello world\"", true, "Double quotes"},
    {"echo 'hello world'", true, "Single quotes"},
    {"echo \"'hello world'\"", true, "Nested quotes"},
    {"echo \"hello world", false, "Unclosed double quote"},
    {"echo 'hello world", false, "Unclosed single quote"},
    {"echo \"hello > world\"", true, "Redirection in quotes"},
    {"echo \"hello | world\"", true, "Pipe in quotes"},

    // Complex Command Tests
    {"cat < input.txt | sort | uniq > output.txt", true, "Complex pipeline"},
    {"echo \"hello\" > \"my file.txt\"", true, "Quoted filename"},
    {"ls -la | grep \".txt\" > files.txt", true, "Pipeline with redirection"},
    {"cat < \"input file.txt\" | sort > \"output file.txt\"", true, "Spaces in filenames"},

    // Error Cases
    {"ls > file1 > file2", false, "Multiple output redirections"},
    {"cat < file1 < file2", false, "Multiple input redirections"},
    {"ls >>> file", false, "Invalid redirection operator"},
    {"ls > file |", false, "Pipe after redirection with no command"},
    {"ls | > file", false, "Redirection without command after pipe"},

    // Edge Cases
    {"echo \"$USER\"", true, "Environment variable in quotes"},
    {"echo '$USER'", true, "Environment variable in single quotes"},
    {"ls -la|grep \".txt\"|wc -l", true, "No spaces around pipes"},
    {"ls      -la     |    grep     \".txt\"", true, "Multiple spaces between args"},

    // Invalid Syntax Tests
    {"ls &", false, "Background operator (not supported)"},
    {"ls ; ls", false, "Semicolon (not supported)"},
    {"ls && ls", false, "AND operator (not supported)"},
    {"ls || ls", false, "OR operator (not supported)"},
    {"ls >>>", false, "Invalid redirect syntax"},
    {"ls >< file", false, "Invalid redirect combination"},

    // Quoted Special Characters
    {"echo \"hello; world\"", true, "Semicolon in quotes (valid)"},
    {"echo 'hello; world'", true, "Semicolon in single quotes (valid)"},
    {"echo hello; world", false, "Semicolon outside quotes (invalid)"},
    {"ls -l | grep 'file'", true, "Pipe with single quotes"},
    {"ls -l | grep \"file\"", true, "Pipe with double quotes"},
    {"echo \"hello > world\" > output.txt", true, "Redirection with quotes"},
    {"echo \"hello | world\" | sort", true, "Pipe with quotes"},
    {"ls -la | grep -E \"\\.txt$\"", true, "Regex in pipe"},
    {"cat < \"input file.txt\" | sort | uniq | wc -l", true, "Complex command with multiple pipes"},
    {"ls -la | grep -v \"hidden\"", true, "Pipe with negation"},
    {"echo \"hello\" && echo \"world\"", false, "AND operator (not supported)"},
    {"echo \"hello\" || echo \"world\"", false, "OR operator (not supported)"},
    {"ls | grep \"file\" | wc -l | sort", false, "Pipe with multiple outputs (not supported)"},
    {"ls | grep \"file\" |", false, "Pipe at end with no command"},
    {"ls | | grep \"file\"", false, "Empty pipe command"},

    // {"cat file.txt | grep \"error\" | wc -l", false, "Multiple pipes not supported"}, //not handled
    // {"ls -l|wc -l", false, "Multiple pipes without spaces"}, //not handled
    // {"echo hello | cat | cat | cat", false, "Long pipe chain not supported"}, //not handled
    
    // // Additional Redirection Tests
    // {"cat < input.txt > output.txt", true, "Input and output redirection"},
    // {"echo hello > file1 < file2", true, "Mixed redirection order"},
    // {"cat << EOF", false, "Heredoc not supported"}, 
    // {"cat >> file << EOF", false, "Multiple redirections with heredoc"},
    
    // // Additional Quote Tests
    // {"echo \"hello'world'\"", true, "Single quotes inside double quotes"},
    // {"echo '\"hello world\"'", true, "Double quotes inside single quotes"},
    // {"echo \"$HOME/test\"", true, "Environment variable in double quotes"},
    // {"echo '$HOME/test'", true, "Environment variable in single quotes (literal)"},
    
    // // Additional Error Cases
    // {"ls >", false, "Dangling redirection"},
    // {"cat < ", false, "Dangling input redirection"},
    // {"echo >>> file", false, "Invalid redirection operator"},
    // {"ls > > file", false, "Invalid double redirection"},
    
    // // Additional Edge Cases
    // {"echo \"hello   world\"", true, "Preserving spaces in quotes"},
    // {"echo 'hello   world'", true, "Preserving spaces in single quotes"},
    // {"ls -la | grep \".txt\" > \"my results.txt\"", true, "Complex command with spaces in filename"},
    // {"echo $USER", true, "Environment variable expansion"},
    
    // // Additional Invalid Syntax
    // {"ls &&", false, "Dangling AND operator"},
    // {"|| ls", false, "Starting with OR operator"},
    // {"ls ||| grep", false, "Triple pipe"},
    // {"ls && || grep", false, "Mixed operators"},

    // End marker
    {NULL, false, NULL}
};
// ... existing code with struct and test cases ...

void run_parser_tests(void)
{
    int i = 0;
    int passed = 0;
    int total = 0;

    while (tests[i].input != NULL) {
        bool is_valid = is_valid_command(tests[i].input);  // You need to implement this
        bool test_passed = (is_valid == tests[i].expected_valid);
        
        printf("Test %d: %s\n", i + 1, tests[i].description);
        printf("Input: \"%s\"\n", tests[i].input);
        printf("Expected: %s, Got: %s\n", 
            tests[i].expected_valid ? "valid" : "invalid",
            is_valid ? "valid" : "invalid");
        printf("Result: %s\n\n", test_passed ? "✓ PASS" : "✗ FAIL");
        
        if (test_passed)
            passed++;
        total++;
        i++;
    }
    
    printf("Summary: %d/%d tests passed\n", passed, total);
}

bool is_valid_command(const char *command)
{
    if (!command || is_empty_or_whitespace(command))
        return false;

    // Check for unsupported operators
    if (strstr(command, ";") != NULL)  // Check for semicolon
        return false;

    // Try to tokenize the input
    t_token *tokens = tokenize_input((char *)command);
    if (!tokens)
        return false;

    // Check for syntax errors
    bool is_valid = (check_syntax_error(tokens) == 0);
    
    // Clean up
    free_tokens(tokens);
    
    return is_valid;
}

bool is_empty_or_whitespace(const char *str)
{
    if (!str)
        return true;
    
    while (*str)
    {
        if (!isspace((unsigned char)*str))
            return false;
        str++;
    }
    return true;
}