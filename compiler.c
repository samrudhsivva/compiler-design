#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum {
    NUMBER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    LPAREN,
    RPAREN,
    END_OF_INPUT
} TokenType;

typedef struct {
    TokenType type;
    int value;
} Token;

char *input = "3 + 4 * (5 - 2)";
char *currentChar;

Token getNextToken() {
    // Initialize currentChar on first call
    if (currentChar == NULL) {
        currentChar = input;
    }

    // Skip any whitespace
    while (isspace(*currentChar)) {
        currentChar++;
    }

    // Prepare the token to return
    Token token;

    // Check the current character and decide what to do
    if (isdigit(*currentChar)) {
        token.type = NUMBER;
        token.value = 0;
        
        // Parse the full number
        while (isdigit(*currentChar)) {
            token.value = token.value * 10 + (*currentChar - '0');
            currentChar++;
        }
    } else {
        switch (*currentChar) {
            case '+':
                token.type = PLUS;
                currentChar++;
                break;
            case '-':
                token.type = MINUS;
                currentChar++;
                break;
            case '*':
                token.type = MULTIPLY;
                currentChar++;
                break;
            case '/':
                token.type = DIVIDE;
                currentChar++;
                break;
            case '(':
                token.type = LPAREN;
                currentChar++;
                break;
            case ')':
                token.type = RPAREN;
                currentChar++;
                break;
            case '\0': // End of input
                token.type = END_OF_INPUT;
                break;
            default: // Handle unexpected characters
                printf("Error: Unexpected character '%c'\n", *currentChar);
                exit(1);
        }
    }

    return token;
}

int main() {
    // Example usage
    Token token;
    do {
        token = getNextToken();
        printf("Token type: %d, Value: %d\n", token.type, token.value);
    } while (token.type != END_OF_INPUT);

    return 0;
}
