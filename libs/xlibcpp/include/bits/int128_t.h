#ifndef INT128_T_H
#define INT128_T_H

#include <stdint.h>

namespace std
{
class int128_t_base;
class int128_t;
class uint128_t;

extern const int128_t  INT128_MIN;
extern const int128_t  INT128_MAX;

extern const uint128_t UINT128_MIN;
extern const uint128_t UINT128_MAX;

class int128_t_base
{
public:
    // Constructors / destructors
    int128_t_base();
    int128_t_base(uint32_t nPart0, uint32_t nPart1, uint32_t nPart2, uint32_t nPart3);
    int128_t_base(uint64_t nPart0, uint64_t nPart1);
    int128_t_base(uint8_t value);
    int128_t_base(uint16_t value);
    int128_t_base(uint32_t value);
    int128_t_base(uint64_t value);
    int128_t_base(const int128_t_base& value);

    // To do: Add an int128_t_base(unsigned int) constructor just for Metrowerks,
    // as Metrowerks defines uint32_t to be unsigned long and not unsigned int.

    // Assignment operator
    int128_t_base& operator=(const int128_t_base& value);

    // Math operators
    static void operatorPlus (const int128_t_base& value1, const int128_t_base& value2, int128_t_base& result);
    static void operatorMinus(const int128_t_base& value1, const int128_t_base& value2, int128_t_base& result);
    static void operatorMul  (const int128_t_base& value1, const int128_t_base& value2, int128_t_base& result);

    // Shift operators
    static void operatorShiftRight(const int128_t_base& value, int nShift, int128_t_base& result);
    static void operatorShiftLeft (const int128_t_base& value, int nShift, int128_t_base& result);

    // Unary arithmetic/logic operators
    bool operator!() const;

    // Logical operators
    static void operatorXOR(const int128_t_base& value1, const int128_t_base& value2, int128_t_base& result);
    static void operatorOR (const int128_t_base& value1, const int128_t_base& value2, int128_t_base& result);
    static void operatorAND(const int128_t_base& value1, const int128_t_base& value2, int128_t_base& result);

    // Operators to convert back to basic types
    // We do not provide casting operators (e.g. operator float()) because doing so would
    // cause the compiler to complian about multiple conversion choices while doing freeform
    // math. That's standard C++ behaviour and the conventional solution is to not provide
    // implicit casting operators but to provide functions such as AsFloat() to allow the
    // user to do explicit casts.
    bool     AsBool()   const;
    uint8_t  AsUint8()  const;
    uint16_t AsUint16() const;
    uint32_t AsUint32() const;
    uint64_t AsUint64() const;

    // Misc. Functions
    // The index values below start with zero, and zero means the lowest part.
    // For example, calling SetPartUint32(3, 0x00000000) zeros the high 32 bits of the int128.
    int      GetBit(int nIndex) const;
    void     SetBit(int nIndex, int value);
    uint8_t  GetPartUint8 (int nIndex) const;
    uint16_t GetPartUint16(int nIndex) const;
    uint32_t GetPartUint32(int nIndex) const;
    uint64_t GetPartUint64(int nIndex) const;
    void     SetPartUint8 (int nIndex, uint8_t  value);
    void     SetPartUint16(int nIndex, uint16_t value);
    void     SetPartUint32(int nIndex, uint32_t value);
    void     SetPartUint64(int nIndex, uint64_t value);

    bool     IsZero() const;
    void     SetZero();
    void     TwosComplement();
    void     InverseTwosComplement();

    enum LeadingZeroes
    {
        kLZDefault,     // Do the default for the base. By default there are leading zeroes only with base 16.
        kLZEnable,
        kLZDisable
    };

    enum Prefix
    {
        kPrefixDefault, // Do the default for the base. By default there is a prefix only with base 16.
        kPrefixEnable,
        kPrefixDisable
    };

protected:
    void DoubleToUint128(double value);

protected:
    #if TARGET_X86_64
        #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
            uint64_t mPart1;  // Most significant byte.
            uint64_t mPart0;  // Least significant byte.
        #else
            uint64_t mPart0;  // Most significant byte.
            uint64_t mPart1;  // Least significant byte.
        #endif
    #else
        #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
            uint32_t mPart3;  // Most significant byte.
            uint32_t mPart2;
            uint32_t mPart1;
            uint32_t mPart0;  // Least significant byte.
        #else
            uint32_t mPart0;  // Most significant byte.
            uint32_t mPart1;
            uint32_t mPart2;
            uint32_t mPart3;  // Least significant byte.
        #endif
    #endif
};

class int128_t: public int128_t_base
{
public:
    int128_t();
    int128_t(uint32_t nPart0, uint32_t nPart1, uint32_t nPart2, uint32_t nPart3);
    int128_t(uint64_t nPart0, uint64_t nPart1);

    int128_t(int8_t value);
    int128_t(uint8_t value);

    int128_t(int16_t value);
    int128_t(uint16_t value);

    int128_t(int32_t value);
    int128_t(uint32_t value);

    int128_t(int64_t value);
    int128_t(uint64_t value);

    int128_t(const int128_t& value);

    int128_t(const float value);
    int128_t(const double value);

    int128_t(const char* pValue, int nBase = 10);
    int128_t(const wchar_t* pValue, int nBase = 10);

    // Assignment operator
    int128_t& operator=(const int128_t_base& value);

    // Unary arithmetic/logic operators
    int128_t  operator-() const;
    int128_t& operator++();
    int128_t& operator--();
    int128_t  operator++(int);
    int128_t  operator--(int);
    int128_t  operator~() const;
    int128_t  operator+() const;

    // Math operators
    friend int128_t operator+(const int128_t& value1, const int128_t& value2);
    friend int128_t operator-(const int128_t& value1, const int128_t& value2);
    friend int128_t operator*(const int128_t& value1, const int128_t& value2);
    friend int128_t operator/(const int128_t& value1, const int128_t& value2);
    friend int128_t operator%(const int128_t& value1, const int128_t& value2);

    int128_t& operator+=(const int128_t& value);
    int128_t& operator-=(const int128_t& value);
    int128_t& operator*=(const int128_t& value);
    int128_t& operator/=(const int128_t& value);
    int128_t& operator%=(const int128_t& value);

    // Shift operators
    int128_t  operator>> (int nShift) const;
    int128_t  operator<< (int nShift) const;
    int128_t& operator>>=(int nShift);
    int128_t& operator<<=(int nShift);

    // Logical operators
    friend int128_t operator^(const int128_t& value1, const int128_t& value2);
    friend int128_t operator|(const int128_t& value1, const int128_t& value2);
    friend int128_t operator&(const int128_t& value1, const int128_t& value2);

    int128_t& operator^= (const int128_t& value);
    int128_t& operator|= (const int128_t& value);
    int128_t& operator&= (const int128_t& value);

    // Equality operators
    friend int  compare   (const int128_t& value1, const int128_t& value2);
    friend bool operator==(const int128_t& value1, const int128_t& value2);
    friend bool operator!=(const int128_t& value1, const int128_t& value2);
    friend bool operator> (const int128_t& value1, const int128_t& value2);
    friend bool operator>=(const int128_t& value1, const int128_t& value2);
    friend bool operator< (const int128_t& value1, const int128_t& value2);
    friend bool operator<=(const int128_t& value1, const int128_t& value2);

    // Operators to convert back to basic types
    int8_t  AsInt8()   const;
    int16_t AsInt16()  const;
    int32_t AsInt32()  const;
    int64_t AsInt64()  const;
    float   AsFloat()  const;
    double  AsDouble() const;

    // Misc. Functions
    void    Negate();
    bool    IsNegative() const;
    bool    IsPositive() const;
    void    Modulus(const int128_t& divisor, int128_t& quotient, int128_t& remainder) const;

    // String conversion functions
    int128_t StrToInt128(const char* pValue, char** ppEnd, int base);
    int128_t StrToInt128(const wchar_t* pValue, wchar_t** ppEnd, int base);
    void     Int128ToStr(char* pValue, char** ppEnd, int nBase, LeadingZeroes lz, Prefix prefix) const;
    void     Int128ToStr(wchar_t* pValue, wchar_t** ppEnd, int nBase, LeadingZeroes lz, Prefix prefix) const;

}; /* class int128_t */

class uint128_t: public int128_t_base
{
public:
    uint128_t();
    uint128_t(uint32_t nPart0, uint32_t nPart1, uint32_t nPart2, uint32_t nPart3);
    uint128_t(uint64_t nPart0, uint64_t nPart1);

    uint128_t(int8_t value);
    uint128_t(uint8_t value);

    uint128_t(int16_t value);
    uint128_t(uint16_t value);

    uint128_t(int32_t value);
    uint128_t(uint32_t value);

    uint128_t(int64_t value);
    uint128_t(uint64_t value);

    uint128_t(const int128_t& value);
    uint128_t(const uint128_t& value);

    uint128_t(const float value);
    uint128_t(const double value);

    uint128_t(const char* pValue, int nBase = 10);
    uint128_t(const wchar_t* pValue, int nBase = 10);

    // Assignment operator
    uint128_t& operator=(const int128_t_base& value);

    // Unary arithmetic/logic operators
    uint128_t  operator-() const;
    uint128_t& operator++();
    uint128_t& operator--();
    uint128_t  operator++(int);
    uint128_t  operator--(int);
    uint128_t  operator~() const;
    uint128_t  operator+() const;

    // Math operators
    friend uint128_t operator+(const uint128_t& value1, const uint128_t& value2);
    friend uint128_t operator-(const uint128_t& value1, const uint128_t& value2);
    friend uint128_t operator*(const uint128_t& value1, const uint128_t& value2);
    friend uint128_t operator/(const uint128_t& value1, const uint128_t& value2);
    friend uint128_t operator%(const uint128_t& value1, const uint128_t& value2);

    uint128_t& operator+=(const uint128_t& value);
    uint128_t& operator-=(const uint128_t& value);
    uint128_t& operator*=(const uint128_t& value);
    uint128_t& operator/=(const uint128_t& value);
    uint128_t& operator%=(const uint128_t& value);

    // Shift operators
    uint128_t  operator>> (int nShift) const;
    uint128_t  operator<< (int nShift) const;
    uint128_t& operator>>=(int nShift);
    uint128_t& operator<<=(int nShift);

    // Logical operators
    friend uint128_t operator^(const uint128_t& value1, const uint128_t& value2);
    friend uint128_t operator|(const uint128_t& value1, const uint128_t& value2);
    friend uint128_t operator&(const uint128_t& value1, const uint128_t& value2);
    uint128_t& operator^= (const uint128_t& value);
    uint128_t& operator|= (const uint128_t& value);
    uint128_t& operator&= (const uint128_t& value);

    // Equality operators
    friend int  compare   (const uint128_t& value1, const uint128_t& value2);
    friend bool operator==(const uint128_t& value1, const uint128_t& value2);
    friend bool operator!=(const uint128_t& value1, const uint128_t& value2);
    friend bool operator> (const uint128_t& value1, const uint128_t& value2);
    friend bool operator>=(const uint128_t& value1, const uint128_t& value2);
    friend bool operator< (const uint128_t& value1, const uint128_t& value2);
    friend bool operator<=(const uint128_t& value1, const uint128_t& value2);

    // Operators to convert back to basic types
    int8_t  AsInt8()   const;
    int16_t AsInt16()  const;
    int32_t AsInt32()  const;
    int64_t AsInt64()  const;
    float   AsFloat()  const;
    double  AsDouble() const;

    // Misc. Functions
    void    Negate();
    bool    IsNegative() const;
    bool    IsPositive() const;
    void    Modulus(const uint128_t& divisor, uint128_t& quotient, uint128_t& remainder) const;

    // String conversion functions
    uint128_t StrToInt128(const char* pValue, char** ppEnd, int base);
    uint128_t StrToInt128(const wchar_t* pValue, wchar_t** ppEnd, int base);
    void      Int128ToStr(char* pValue, char** ppEnd, int nBase, LeadingZeroes lz, Prefix prefix) const;
    void      Int128ToStr(wchar_t* pValue, wchar_t** ppEnd, int nBase, LeadingZeroes lz, Prefix prefix) const;

}; /* class uint128_t */
} /* namespace std */

#endif /* INT128_T_H */
