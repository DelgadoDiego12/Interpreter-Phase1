#include <cstdlib>
#include <iostream>
#include <string>

#include "Parser.hpp"

void Parser::die(const std::string &where,
                 const std::string &message,
                 const Token &token) const {
    std::cerr << where << ": " << message << " at line " << token.lineNumber()
              << ", column " << token.columnNumber() << ". Got: ";
    token.print(std::cerr);
    std::cerr << "\n\nTokens identified up to this point:\n";
    tokenizer.printProcessedTokens(std::cerr);
    std::exit(EXIT_FAILURE);
}

Statements *Parser::program() {
    // <program> -> <statements> EOF
    Statements *parsedStatements = statements();
    Token eof = tokenizer.getToken();
    if (!eof.isEof()) {
        delete parsedStatements;
        die("Parser::program", "expected EOF", eof);
    }
    return parsedStatements;
}

Statements *Parser::statements() {
    // <statements> -> <statement> NEWLINE { <statement> NEWLINE }
    auto *parsedStatements = new Statements();
    parsedStatements->addStatement(statement());

    Token newline = tokenizer.getToken();
    if (!newline.isNewline()) {
        delete parsedStatements;
        die("Parser::statements", "expected NEWLINE after statement", newline);
    }

    Token next = tokenizer.getToken();
    while (next.isIdentifier() || next.isKeyword()) {
        tokenizer.ungetToken();
        parsedStatements->addStatement(statement());

        newline = tokenizer.getToken();
        if (!newline.isNewline()) {
            delete parsedStatements;
            die("Parser::statements", "expected NEWLINE after statement", newline);
        }

        next = tokenizer.getToken();
    }

    tokenizer.ungetToken();
    return parsedStatements;
}

Statement *Parser::statement() {
    // <statement> -> <for-statement>
    //             | <assignment-statement>
    //             | <print-statement>
    Token token = tokenizer.getToken();

    if (token.isIdentifier()) {
        tokenizer.ungetToken();
        return assignmentStatement();
    }
    if (token.isForKeyword()) {
        tokenizer.ungetToken();
        return forStatement();
    }
    if (token.isPrintKeyword()) {
        tokenizer.ungetToken();
        return printStatement();
    }

    die("Parser::statement", "expected a statement", token);
}

AssignmentStatement *Parser::assignmentStatement() {
    // <assignment-statement> -> <id> = <rel-expr>
    // The caller consumes the context-dependent terminator: NEWLINE in a
    // statement list or ';' in a future for-loop header.
    Token variable = tokenizer.getToken();
    if (!variable.isIdentifier())
        die("Parser::assignmentStatement", "expected an identifier", variable);

    Token assignmentOperator = tokenizer.getToken();
    if (!assignmentOperator.isAssignmentOperator())
        die("Parser::assignmentStatement", "expected '='", assignmentOperator);

    return new AssignmentStatement(variable.identifier(), relExpr());
}

PrintStatement *Parser::printStatement() {
    // <print-statement> -> print <rel-expr>
    Token printToken = tokenizer.getToken();
    if (!printToken.isPrintKeyword()) {
        die ("Parser::printStatement", "expected 'print'", printToken);
    }
    return new PrintStatement(relExpr());
}

ForStatement *Parser::forStatement() {
    // <for-statement> -> for ( <assignment-statement> ; <real-expr> ; <assignment-statement> ) { NEWLINE <statements> }
    Token forToken = tokenizer.getToken();
    if (!forToken.isForKeyword()) {
        die ("Parser::forStatement", "expected 'for'", forToken);
    }
    Token openParenToken = tokenizer.getToken();
    if (!openParenToken.isOpenParen()){
        die ("Parser::forStatement", "expected ' ( '", openParenToken);
    }

    AssignmentStatement *initialization = assignmentStatement();

    // <assignment-statement> the initialization statement
    Token firstSemicolonToken = tokenizer.getToken();
    if (!firstSemicolonToken.isSemicolon()) {
        die ("Parser::forStatement", "expected ' ; '", firstSemicolonToken);
    }

    ExprNode *condition = relExpr();

    //<real-expr> the relational condition that continues while it evaluates to nonzero value, reevaluate for every iteration
    //under a loop while condition != 0?
    Token secondSemicolonToken = tokenizer.getToken();
    if (!secondSemicolonToken.isSemicolon()) {
        die ("Parser::forStatement", "expected ' ; '", secondSemicolonToken);

    }

    AssignmentStatement *assignment = assignmentStatement();

    //<assignment-statement> update after every iteration
    Token closeParenToken = tokenizer.getToken();
    if (!closeParenToken.isCloseParen()) {
        die ("Parser::forStatement", "expected ' ) '", closeParenToken);
    }

    Token openCurBraceToken = tokenizer.getToken();
    if (!openCurBraceToken.isOpenCurlyBrace()){
        die ("Parser::forStatement", "expected ' { '", openCurBraceToken);
    }

    Token newlineToken = tokenizer.getToken();
    if (!newlineToken.isNewline()) {
        die("Parser::forStatement", "expected NEWLINE after '{'", newlineToken);
    }

    Statements *loopBody = statements();

    //NEWLINE <statements>
    Token closeCurBraceToken = tokenizer.getToken();
    if (!closeCurBraceToken.isCloseCurlyBrace()) {
        die ("Parser::forStatement", "expected ' } '", closeCurBraceToken);
    }
    //return new ForStatement(something in here);
    return new ForStatement(
        initialization,
        condition,
        assignment,
        loopBody);
}

ExprNode *Parser::relExpr() {
    // <rel-expr> -> <rel-term> [ <equality-op> <rel-term> ]
    // The optional equality operation is left for students to implement.
    ExprNode *left = relTerm();
    Token token = tokenizer.getToken();

    if (token.isEqualToOperator() || token.isNotEqualToOperator()) {
        ExprNode *right = relTerm();
        left = new BinaryExprNode(token, left, right);
    }  else {
        tokenizer.ungetToken();

    }
    return left;
}

ExprNode *Parser::relTerm() {
    // <rel-term> -> <rel-primary> [ <ordering-op> <rel-primary> ]
    // The optional ordering operation is left for students to implement.
    ExprNode *left = relPrimary();
    Token token = tokenizer.getToken();

    if (token.isGreaterThanOperator() ||
        token.isLessThanOperator() ||
        token.isGreaterOrEqualToOperator()||
        token.isLessOrEqualToOperator()) {

        ExprNode *right = relPrimary();
        left = new BinaryExprNode(token, left, right);
        } else {
            tokenizer.ungetToken();
        }
    return left;
}

ExprNode *Parser::relPrimary() {
    // <rel-primary> -> <arith-expr>

    return arithExpr();
}

ExprNode *Parser::arithExpr() {
    // <arith-expr> -> <arith-term> { <add-op> <arith-term> }
    ExprNode *left = arithTerm();
    Token token = tokenizer.getToken();

    while (token.isAdditionOperator() || token.isSubtractionOperator()) {
        ExprNode *right = arithTerm();
        left = new BinaryExprNode(token, left, right);
        token = tokenizer.getToken();
    }

    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::arithTerm() {
    // <arith-term> -> <arith-primary> { <mult-op> <arith-primary> }
    ExprNode *left = arithPrimary();
    Token token = tokenizer.getToken();

    while (token.isMultiplicationOperator() ||
           token.isDivisionOperator() ||
           token.isModuloOperator()) {
        ExprNode *right = arithPrimary();
        left = new BinaryExprNode(token, left, right);
        token = tokenizer.getToken();
    }

    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::arithPrimary() {
    // <arith-primary> -> [ <sign> ] <arith-atom>
    Token token = tokenizer.getToken();
    if (token.isAdditionOperator() || token.isSubtractionOperator())
        return new UnaryExprNode(token, arithAtom());

    tokenizer.ungetToken();
    return arithAtom();
}

ExprNode *Parser::arithAtom() {
    // <arith-atom> -> <id> | <integer> | '(' <rel-expr> ')'
    Token token = tokenizer.getToken();

    if (token.isInteger())
        return new IntegerLiteral(token);
    if (token.isIdentifier())
        return new Variable(token);
    if (token.isOpenParen()) {
        ExprNode *expression = relExpr();
        Token closeParen = tokenizer.getToken();
        if (!closeParen.isCloseParen())
            die("Parser::arithAtom", "expected ')'", closeParen);
        return expression;
    }

    die("Parser::arithAtom", "expected an identifier, integer, or '('", token);
}
