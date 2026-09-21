#ifndef EXPRINTER_STATEMENTS_HPP
#define EXPRINTER_STATEMENTS_HPP

#include <string>
#include <vector>

#include "Expr.hpp"
#include "SymbolTable.hpp"

class Statement {
public:
    virtual ~Statement() = default;

    virtual void print() const = 0;
    virtual void evaluate(SymbolTable &symbolTable) const = 0;
};

class Statements {
public:
    ~Statements();

    void addStatement(Statement *statement);
    void evaluate(SymbolTable &symbolTable) const;
    void print() const;

private:
    std::vector<Statement *> statements;
};

class AssignmentStatement final : public Statement {
public:
    AssignmentStatement(std::string variableName,
                        ExprNode *expression);
    ~AssignmentStatement() override;

    void evaluate(SymbolTable &symbolTable) const override;
    void print() const override;
    void otherPrint() const; //same as print but with no newline

private:
    std::string variableName;
    ExprNode *expression;
};

class PrintStatement final : public Statement {
public:
    PrintStatement(ExprNode *expression);
    ~PrintStatement() override;

    void evaluate(SymbolTable &symbolTable) const override;
    void print() const override;

private:
    ExprNode *expression;
};

class ForStatement final : public Statement {
public:
    ForStatement(AssignmentStatement *initialization, ExprNode *condition, AssignmentStatement *assignment, Statements *loopBody);
    ~ForStatement() override;
    void evaluate(SymbolTable &symbolTable) const override;
    void print() const override;
private:
    AssignmentStatement *initialization;
    ExprNode *condition;
    AssignmentStatement *assignment;
    Statements *loopBody;

};

#endif // EXPRINTER_STATEMENTS_HPP
