#include <iostream>
#include <string>
#include <sstream>
#include <cctype>

enum class TokenType {
    INTEGER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    END_OF_INPUT
};

struct Token {
    TokenType type;
    int value;

    Token(TokenType t, int v) : type(t), value(v) {}
};

class Lexer {
public:
    Lexer(const std::string& input) : input_(input), position_(0) {}

    Token getNextToken() {
        while (position_ < input_.size() && std::isspace(input_[position_])) {
            ++position_;
        }

        if (position_ >= input_.size()) {
            return {TokenType::END_OF_INPUT, 0};
        }

        char currentChar = input_[position_];
        if (std::isdigit(currentChar)) {
            return {TokenType::INTEGER, parseInt()};
        } else {
            ++position_;
            switch (currentChar) {
                case '+': return {TokenType::PLUS, 0};
                case '-': return {TokenType::MINUS, 0};
                case '*': return {TokenType::MULTIPLY, 0};
                case '/': return {TokenType::DIVIDE, 0};
                default: throw std::runtime_error("Invalid token: " + std::string(1, currentChar));
            }
        }
    }

private:
    int parseInt() {
        int result = 0;
        while (position_ < input_.size() && std::isdigit(input_[position_])) {
            result = result * 10 + (input_[position_] - '0');
            ++position_;
        }
        return result;
    }

    std::string input_;
    size_t position_;
};

class Parser {
public:
    Parser(Lexer& lexer) : lexer_(lexer) {}

    int parseExpression() {
        int left = parseTerm();
        Token token;
        while ((token = lexer_.getNextToken()).type != TokenType::END_OF_INPUT) {
            switch (token.type) {
                case TokenType::PLUS:
                    left += parseTerm();
                    break;
                case TokenType::MINUS:
                    left -= parseTerm();
                    break;
                default:
                    lexer_.putBackToken(token);  // Assuming there's a way to put back a token
                    return left;
            }
        }
        return left;
    }

    int parseTerm() {
        int left = parseFactor();
        Token token;
        while ((token = lexer_.getNextToken()).type != TokenType::END_OF_INPUT) {
            switch (token.type) {
                case TokenType::MULTIPLY:
                    left *= parseFactor();
                    break;
                case TokenType::DIVIDE:
                    int divisor = parseFactor();
                    if (divisor == 0) throw std::runtime_error("Division by zero");
                    left /= divisor;
                    break;
                default:
                    lexer_.putBackToken(token);
                    return left;
            }
        }
        return left;
    }

    int parseFactor() {
        Token token = lexer_.getNextToken();
        if (token.type == TokenType::INTEGER) {
            return token.value;
        } else {
            throw std::runtime_error("Expected integer but got another token");
        }
    }

private:
    Lexer& lexer_;
};

int main() {
    std::string input;
    std::cout << "Enter an arithmetic expression: ";
    std::getline(std::cin, input);

    Lexer lexer(input);
    Parser parser(lexer);

    try {
        int result = parser.parseExpression();
        std::cout << "Result: " << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
