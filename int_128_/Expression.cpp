#include "Expression.h"

#include <ostream>
#include <utility>


std::ostream& operator<<(std::ostream& os, const Expression& e) {
    e.print(os);
    return os;
}


Const::Const(Int128 value) : value_(std::move(value)) {}

Int128 Const::eval(const VarMap& /*vars*/) const {
    return value_;
}

Expression* Const::clone() const {
    return new Const(*this);
}

void Const::print(std::ostream& os) const {
    os << value_;
}


Variable::Variable(std::string name) : name_(std::move(name)) {}

Int128 Variable::eval(const VarMap& vars) const {
    return vars.at(name_);
}

Expression* Variable::clone() const {
    return new Variable(*this);
}

void Variable::print(std::ostream& os) const {
    os << name_;
}


Negate::Negate(const Expression& arg) : arg_(arg.clone()) {}

Negate::Negate(const Negate& other) : arg_(other.arg_->clone()) {}

Negate& Negate::operator=(const Negate& other) {
    if (this != &other) {
        arg_.reset(other.arg_->clone());
    }
    return *this;
}

Int128 Negate::eval(const VarMap& vars) const {
    return -arg_->eval(vars);
}

Expression* Negate::clone() const {
    return new Negate(*this);
}

void Negate::print(std::ostream& os) const {
    os << "(-" << *arg_ << ")";
}


BinaryOp::BinaryOp(const Expression& lhs, const Expression& rhs)
    : lhs_(lhs.clone()), rhs_(rhs.clone()) {}

BinaryOp::BinaryOp(const BinaryOp& other)
    : lhs_(other.lhs_->clone()), rhs_(other.rhs_->clone()) {}

BinaryOp& BinaryOp::operator=(const BinaryOp& other) {
    if (this != &other) {
        lhs_.reset(other.lhs_->clone());
        rhs_.reset(other.rhs_->clone());
    }
    return *this;
}

void BinaryOp::print(std::ostream& os) const {
    os << '(' << *lhs_ << ' ' << symbol() << ' ' << *rhs_ << ')';
}


Int128 Add::eval(const VarMap& vars) const {
    return lhs_->eval(vars) + rhs_->eval(vars);
}
Expression* Add::clone() const { return new Add(*this); }

Int128 Subtract::eval(const VarMap& vars) const {
    return lhs_->eval(vars) - rhs_->eval(vars);
}
Expression* Subtract::clone() const { return new Subtract(*this); }

Int128 Multiply::eval(const VarMap& vars) const {
    return lhs_->eval(vars) * rhs_->eval(vars);
}
Expression* Multiply::clone() const { return new Multiply(*this); }

Int128 Divide::eval(const VarMap& vars) const {
    return lhs_->eval(vars) / rhs_->eval(vars);
}
Expression* Divide::clone() const { return new Divide(*this); }


Add      operator+(const Expression& a, const Expression& b) { return Add(a, b); }
Subtract operator-(const Expression& a, const Expression& b) { return Subtract(a, b); }
Multiply operator*(const Expression& a, const Expression& b) { return Multiply(a, b); }
Divide   operator/(const Expression& a, const Expression& b) { return Divide(a, b); }
Negate   operator-(const Expression& a)                       { return Negate(a); }
