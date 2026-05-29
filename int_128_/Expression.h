#pragma once

#include "Int128.h"

#include <iosfwd>
#include <memory>
#include <string>
#include <unordered_map>

class Expression {
public:
    using VarMap = std::unordered_map<std::string, Int128>;

    virtual ~Expression() = default;

    virtual Int128 eval(const VarMap& vars) const = 0;
    virtual Expression* clone() const = 0;
    virtual void print(std::ostream& os) const = 0;
};

std::ostream& operator<<(std::ostream& os, const Expression& e);


class Const : public Expression {
public:
    explicit Const(Int128 value);

    Int128 eval(const VarMap& vars) const override;
    Expression* clone() const override;
    void print(std::ostream& os) const override;

private:
    Int128 value_;
};

class Variable : public Expression {
public:
    explicit Variable(std::string name);

    Int128 eval(const VarMap& vars) const override;
    Expression* clone() const override;
    void print(std::ostream& os) const override;

private:
    std::string name_;
};

class Negate : public Expression {
public:
    explicit Negate(const Expression& arg);

    Negate(const Negate& other);
    Negate(Negate&&) noexcept = default;
    Negate& operator=(const Negate& other);
    Negate& operator=(Negate&&) noexcept = default;

    Int128 eval(const VarMap& vars) const override;
    Expression* clone() const override;
    void print(std::ostream& os) const override;

private:
    std::unique_ptr<Expression> arg_;
};


class BinaryOp : public Expression {
public:
    BinaryOp(const Expression& lhs, const Expression& rhs);

    BinaryOp(const BinaryOp& other);
    BinaryOp(BinaryOp&&) noexcept = default;
    BinaryOp& operator=(const BinaryOp& other);
    BinaryOp& operator=(BinaryOp&&) noexcept = default;

    void print(std::ostream& os) const override;

protected:
    std::unique_ptr<Expression> lhs_;
    std::unique_ptr<Expression> rhs_;

    // Символ операции для вывода ('+', '-', '*' или '/').
    virtual char symbol() const = 0;
};

class Add : public BinaryOp {
public:
    using BinaryOp::BinaryOp;

    Int128 eval(const VarMap& vars) const override;
    Expression* clone() const override;

protected:
    char symbol() const override { return '+'; }
};

class Subtract : public BinaryOp {
public:
    using BinaryOp::BinaryOp;

    Int128 eval(const VarMap& vars) const override;
    Expression* clone() const override;

protected:
    char symbol() const override { return '-'; }
};

class Multiply : public BinaryOp {
public:
    using BinaryOp::BinaryOp;

    Int128 eval(const VarMap& vars) const override;
    Expression* clone() const override;

protected:
    char symbol() const override { return '*'; }
};

class Divide : public BinaryOp {
public:
    using BinaryOp::BinaryOp;

    Int128 eval(const VarMap& vars) const override;
    Expression* clone() const override;

protected:
    char symbol() const override { return '/'; }
};

Add      operator+(const Expression& a, const Expression& b);
Subtract operator-(const Expression& a, const Expression& b);
Multiply operator*(const Expression& a, const Expression& b);
Divide   operator/(const Expression& a, const Expression& b);
Negate   operator-(const Expression& a);
