#include "Expression.h"
#include "Int128.h"

#include <iostream>
#include <memory>
#include <string>

int main() {
    std::cout << "=== Int128 ===\n";

    const Int128 a(123456789);
    const Int128 b("987654321987654321");
    const Int128 c("-1000000000000000000000000000000");  

    std::cout << "a = " << a << '\n';
    std::cout << "b = " << b << '\n';
    std::cout << "c = " << c << '\n';

    std::cout << "a + b = " << (a + b) << '\n';
    std::cout << "b * b = " << (b * b) << '\n';
    std::cout << "b * b / b = " << (b * b / b) << '\n';
    std::cout << "-c = " << (-c) << '\n';
    std::cout << "c - c = " << (c - c) << '\n';

    const Int128 x(static_cast<std::int64_t>(42));
    std::cout << "int64_t(x) = " << static_cast<std::int64_t>(x) << '\n';
    std::cout << "double(b)  = " << static_cast<double>(b)  << '\n';

    std::cout << std::boolalpha;
    std::cout << "a == Int128(123456789) -> "
              << (a == Int128(123456789)) << '\n';
    std::cout << "a != b -> " << (a != b) << '\n';

    Int128 minVal("-170141183460469231731687303715884105728");
    std::cout << "INT128_MIN     = " << minVal << '\n';
    std::cout << "-INT128_MIN    = " << (-minVal) << '\n';  

    std::cout << "\n=== Expression ===\n";

    const Add expr1(Multiply(Const(Int128(2)), Variable("x")),
                    Const(Int128(1)));
    const Add expr2 = Const(Int128(2)) * Variable("x") + Const(Int128(1));

    const Expression::VarMap vars = {
        {"x", Int128(100)},
        {"y", Int128(42)},
    };

    std::cout << "expr1 = " << expr1 << "\n";
    std::cout << "expr2 = " << expr2 << "\n";
    std::cout << "expr1.eval({x=100}) = " << expr1.eval(vars) << '\n';
    std::cout << "expr2.eval({x=100}) = " << expr2.eval(vars) << '\n';

    const Divide expr3 =
        (Variable("x") * Variable("x") - Variable("y"))
      / (-Variable("x") + Variable("y"));

    std::cout << "expr3 = " << expr3 << '\n';
    std::cout << "expr3.eval(...) = " << expr3.eval(vars) << '\n';

    std::unique_ptr<Expression> copy(expr3.clone());
    std::cout << "copy  = " << *copy << '\n';
    std::cout << "copy.eval(...) = " << copy->eval(vars) << '\n';

    return 0;
}
