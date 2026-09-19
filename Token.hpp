#ifndef ALEXICALANALYZER_TOKEN_HPP
#define ALEXICALANALYZER_TOKEN_HPP

#include <cstddef>
#include <iosfwd>
#include <string>
#include <utility>

enum class Keyword {
    none,
    forKeyword,
    printKeyword
};

class Token {
public:
    [[nodiscard]] bool isEof() const { return _eof; }
    [[nodiscard]] bool isNewline() const { return _newline; }
    void markAsEof() { _eof = true; }
    void markAsNewline() { _newline = true; }

    void setLocation(std::size_t line, std::size_t column) {
        _lineNumber = line;
        _columnNumber = column;
    }
    [[nodiscard]] std::size_t lineNumber() const { return _lineNumber; }
    [[nodiscard]] std::size_t columnNumber() const { return _columnNumber; }

    void setSymbol(std::string symbol) { _symbol = symbol; }
    [[nodiscard]] std::string symbol() const { return _symbol; }

    [[nodiscard]] bool isOpenParen() const { return _symbol == "("; }
    [[nodiscard]] bool isCloseParen() const { return _symbol == ")"; } 
    [[nodiscard]] bool isSemicolon() const { return _symbol == ";"; }
    [[nodiscard]] bool isAssignmentOperator() const { return _symbol == "="; }
    [[nodiscard]] bool isMultiplicationOperator() const { return _symbol == "*"; }
    [[nodiscard]] bool isAdditionOperator() const { return _symbol == "+"; }
    [[nodiscard]] bool isSubtractionOperator() const { return _symbol == "-"; }
    [[nodiscard]] bool isModuloOperator() const { return _symbol == "%"; }
    [[nodiscard]] bool isDivisionOperator() const { return _symbol == "/"; }

    //my additions
    [[nodiscard]] bool isLessThanOperator() const { return _symbol == "<"; }
    [[nodiscard]] bool isGreaterThanOperator() const { return _symbol == ">"; }
    [[nodiscard]] bool isOpenCurlyBrace() const { return _symbol == "{"; }
    [[nodiscard]] bool isCloseCurlyBrace() const { return _symbol == "}"; }
    [[nodiscard]] bool isEqualToOperator() const { return _symbol == "=="; }
    [[nodiscard]] bool isNotEqualToOperator() const { return _symbol == "!="; }
    [[nodiscard]] bool isLessOrEqualToOperator() const { return _symbol == "<="; }
    [[nodiscard]] bool isGreaterOrEqualToOperator() const { return _symbol == ">="; }


    void setIdentifier(std::string identifier) { _identifier = std::move(identifier); }
    [[nodiscard]] bool isIdentifier() const { return !_identifier.empty(); }
    [[nodiscard]] const std::string &identifier() const { return _identifier; }

    void setKeyword(Keyword keyword) { _keyword = keyword; }
    [[nodiscard]] bool isKeyword() const { return _keyword != Keyword::none; }
    [[nodiscard]] bool isForKeyword() const { return _keyword == Keyword::forKeyword; }
    [[nodiscard]] bool isPrintKeyword() const { return _keyword == Keyword::printKeyword; }
    [[nodiscard]] Keyword keyword() const { return _keyword; }

    void setIntegerValue(int value) {
        _integerValue = value;
        _isInteger = true;
    }
    [[nodiscard]] bool isInteger() const { return _isInteger; }
    [[nodiscard]] int integerValue() const { return _integerValue; }

    void print(std::ostream &output) const;

private:
    std::string _identifier{};
    Keyword _keyword{Keyword::none};
    bool _eof{false};
    bool _newline{false};
    bool _isInteger{false};
    std::string _symbol{'\0'};
    int _integerValue{0};
    std::size_t _lineNumber{0};
    std::size_t _columnNumber{0};
};

#endif // ALEXICALANALYZER_TOKEN_HPP
