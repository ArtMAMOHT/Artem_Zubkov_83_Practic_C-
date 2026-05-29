#pragma once

#include <cstdint>
#include <iosfwd>
#include <string>
#include <string_view>


class Int128 {
public:
    Int128();
    Int128(std::int64_t value);
    Int128(std::string_view text);

    explicit operator std::int64_t() const;
    explicit operator double() const;

    std::string str() const;

    Int128 operator-() const;

    Int128& operator+=(const Int128& rhs);
    Int128& operator-=(const Int128& rhs);
    Int128& operator*=(const Int128& rhs);
    Int128& operator/=(const Int128& rhs);

    friend bool operator==(const Int128& a, const Int128& b);
    friend bool operator!=(const Int128& a, const Int128& b);
    friend std::ostream& operator<<(std::ostream& os, const Int128& v);

private:
    std::uint64_t low_;
    std::uint64_t high_;

    Int128(std::uint64_t high, std::uint64_t low);

    bool isNegative() const { return (high_ >> 63) != 0; }
    void negateInPlace();

    static int cmpAbs(const Int128& a, const Int128& b);

    static void divModUnsigned(Int128 num, Int128 den,
                               Int128& quot, Int128& rem);
};

Int128 operator+(Int128 a, const Int128& b);
Int128 operator-(Int128 a, const Int128& b);
Int128 operator*(Int128 a, const Int128& b);
Int128 operator/(Int128 a, const Int128& b);
