#include "Int128.h"

#include <algorithm>
#include <ostream>

namespace {

std::uint64_t mul64Full(std::uint64_t a, std::uint64_t b,
                        std::uint64_t& outHigh) {
    const std::uint64_t aLo = static_cast<std::uint32_t>(a);
    const std::uint64_t aHi = a >> 32;
    const std::uint64_t bLo = static_cast<std::uint32_t>(b);
    const std::uint64_t bHi = b >> 32;

    const std::uint64_t ll = aLo * bLo;
    const std::uint64_t lh = aLo * bHi;
    const std::uint64_t hl = aHi * bLo;
    const std::uint64_t hh = aHi * bHi;

    const std::uint64_t mid = (ll >> 32)
                            + static_cast<std::uint32_t>(lh)
                            + static_cast<std::uint32_t>(hl);

    outHigh = hh + (lh >> 32) + (hl >> 32) + (mid >> 32);
    return (mid << 32) | static_cast<std::uint32_t>(ll);
}

constexpr double kTwoTo64 = 18446744073709551616.0;  // 2^64

} 

Int128::Int128() : low_(0), high_(0) {}

Int128::Int128(std::uint64_t high, std::uint64_t low)
    : low_(low), high_(high) {}

Int128::Int128(std::int64_t value) {
    low_ = static_cast<std::uint64_t>(value);
    high_ = (value < 0) ? ~std::uint64_t{0} : std::uint64_t{0};
}

Int128::Int128(std::string_view text) : low_(0), high_(0) {
    if (text.empty()) {
        return;
    }
    std::size_t i = 0;
    bool neg = false;
    if (text[0] == '-') {
        neg = true;
        i = 1;
    } else if (text[0] == '+') {
        i = 1;
    }

    const Int128 ten(10);
    for (; i < text.size(); ++i) {
        const char c = text[i];
        if (c < '0' || c > '9') {
            break;  // по условию обработка ошибок не требуется
        }
        *this *= ten;
        *this += Int128(static_cast<std::int64_t>(c - '0'));
    }
    if (neg) {
        negateInPlace();
    }
}


Int128::operator std::int64_t() const {

    return static_cast<std::int64_t>(low_);
}

Int128::operator double() const {
    if (isNegative()) {
        Int128 abs = *this;
        abs.negateInPlace();
        return -static_cast<double>(abs);
    }
    return static_cast<double>(high_) * kTwoTo64
         + static_cast<double>(low_);
}

std::string Int128::str() const {
    if (high_ == 0 && low_ == 0) {
        return "0";
    }

    Int128 v = *this;
    const bool neg = isNegative();
    if (neg) {
        
        v.negateInPlace();
    }

    std::string out;
    const Int128 ten(10);
    while (!(v.high_ == 0 && v.low_ == 0)) {
        Int128 q;
        Int128 r;
        divModUnsigned(v, ten, q, r);
        out.push_back(static_cast<char>('0' + r.low_));
        v = q;
    }
    if (neg) {
        out.push_back('-');
    }
    std::reverse(out.begin(), out.end());
    return out;
}


Int128 Int128::operator-() const {
    Int128 r = *this;
    r.negateInPlace();
    return r;
}

void Int128::negateInPlace() {
    low_ = ~low_;
    high_ = ~high_;
    ++low_;
    if (low_ == 0) {
        ++high_;
    }
}


Int128& Int128::operator+=(const Int128& rhs) {
    const std::uint64_t oldLow = low_;
    low_ += rhs.low_;
    high_ += rhs.high_;
    if (low_ < oldLow) {
        ++high_; 
    }
    return *this;
}

Int128& Int128::operator-=(const Int128& rhs) {
    const std::uint64_t oldLow = low_;
    low_ -= rhs.low_;
    high_ -= rhs.high_;
    if (low_ > oldLow) {
        --high_;  
    }
    return *this;
}

Int128& Int128::operator*=(const Int128& rhs) {
    std::uint64_t resHi = 0;
    const std::uint64_t resLo = mul64Full(low_, rhs.low_, resHi);

    resHi += low_  * rhs.high_;   
    resHi += high_ * rhs.low_;

    low_  = resLo;
    high_ = resHi;
    return *this;
}

Int128& Int128::operator/=(const Int128& rhs) {
    const bool negResult = isNegative() ^ rhs.isNegative();

    Int128 a = *this;
    Int128 b = rhs;
    if (a.isNegative()) a.negateInPlace();
    if (b.isNegative()) b.negateInPlace();

    Int128 q;
    Int128 r;
    divModUnsigned(a, b, q, r);
    if (negResult) {
        q.negateInPlace();
    }
    *this = q;
    return *this;
}


bool operator==(const Int128& a, const Int128& b) {
    return a.low_ == b.low_ && a.high_ == b.high_;
}

bool operator!=(const Int128& a, const Int128& b) {
    return !(a == b);
}


std::ostream& operator<<(std::ostream& os, const Int128& v) {
    return os << v.str();
}

Int128 operator+(Int128 a, const Int128& b) { a += b; return a; }
Int128 operator-(Int128 a, const Int128& b) { a -= b; return a; }
Int128 operator*(Int128 a, const Int128& b) { a *= b; return a; }
Int128 operator/(Int128 a, const Int128& b) { a /= b; return a; }

int Int128::cmpAbs(const Int128& a, const Int128& b) {
    if (a.high_ != b.high_) {
        return a.high_ < b.high_ ? -1 : 1;
    }
    if (a.low_ != b.low_) {
        return a.low_ < b.low_ ? -1 : 1;
    }
    return 0;
}

void Int128::divModUnsigned(Int128 num, Int128 den,
                            Int128& quot, Int128& rem) {
    quot = Int128();
    rem  = Int128();

    
    for (int i = 127; i >= 0; --i) {
        rem.high_ = (rem.high_ << 1) | (rem.low_ >> 63);
        rem.low_ <<= 1;

        std::uint64_t bit;
        if (i >= 64) {
            bit = (num.high_ >> (i - 64)) & 1ULL;
        } else {
            bit = (num.low_ >> i) & 1ULL;
        }
        rem.low_ |= bit;

        if (cmpAbs(rem, den) >= 0) {
            rem -= den;
            if (i >= 64) {
                quot.high_ |= (1ULL << (i - 64));
            } else {
                quot.low_  |= (1ULL << i);
            }
        }
    }
}
