#include <iostream>

#include "Token.hpp"

void Token::print(std::ostream &output) const {
    if (isNewline())
        output << "NEWLINE";
    else if (isEof())
        output << "EOF";
    else if (isForKeyword())
        output << "for";
    else if (isPrintKeyword())
        output << "print";
    else if (isOpenParen())
        output << '(';
    else if (isCloseParen())
        output << ')';
    else if (isAssignmentOperator())
        output << " = ";
    else if (isSemicolon())
        output << ';';
    else if (isMultiplicationOperator())
        output << " * ";
    else if (isAdditionOperator())
        output << " + ";
    else if (isSubtractionOperator())
        output << " - ";
    else if (isModuloOperator())
        output << " % ";
    else if (isDivisionOperator())
        output << " / ";
    else if (isIdentifier())
        output << identifier();
    else if (isInteger())
        output << integerValue();

    //my additions
    else if (isLessThanOperator())
        output << " < ";
    else if (isGreaterThanOperator())
        output << " > ";
    else if (isOpenCurlyBrace())
        output << " {";
    else if (isCloseCurlyBrace())
        output << "} ";
    else if (isEqualToOperator())
        output << " == ";
    else if (isNotEqualToOperator())
        output << " != ";
    else if (isLessOrEqualToOperator())
        output << " <= ";
    else if (isGreaterOrEqualToOperator())
        output << " >= ";
    else
        output << "uninitialized token";
}
/*

    [[nodiscard]] bool isLessThanOperator() const { return _symbol == "<"; }
    [[nodiscard]] bool isGreaterThanOperator() const { return _symbol == ">"; }
    [[nodiscard]] bool isOpenCurlyBrace() const { return _symbol == "{"; }
    [[nodiscard]] bool isCloseCurlyBrace() const { return _symbol == "}"; }
    [[nodiscard]] bool isEqualToOperator() const { return _symbol == "=="; }
    [[nodiscard]] bool isNotEqualToOperator() const { return _symbol == "!="; }
    [[nodiscard]] bool isLessOrEqualToOperator() const { return _symbol == "<="; }
    [[nodiscard]] bool isGreaterOrEqualToOperator() const { return _symbol == ">="; }
*/