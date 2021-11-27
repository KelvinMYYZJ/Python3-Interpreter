#ifndef int2048_h
#define int2048_h
#include <complex>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

namespace sjtu {
typedef long long storage_type;
static const storage_type base_len = 9;
static const storage_type base = 1000000000; // base = pow(10 , base_len)
template <class T>
extern inline bool Is_Negative(const T& a)
{
    return a < T(0);
}
template <class T>
extern inline T Max(const T a, const T b)
{
    return a < b ? b : a;
}
template <class T>
extern inline T Min(const T a, const T b)
{
    return a > b ? b : a;
}
template <class T>
extern inline T Mod_Num(const T a, const T b)
{
    return (a % b + b) % b;
}
template <class T>
extern inline T Devide_Num(const T a, const T b)
{
    return (a - Mod_Num(a, b)) / b;
}
class int2048 {
private:
    std::vector<storage_type>
        val; // save the value of the number. One storage_type saves base_len
        // numbers. The front of the vector saves the lowest number.
    bool negative;
    void Remove_Leading_Zero(); // remove the leading zero which is unessential
    int2048 Negative_Of() const;
    int2048 Turn_Negative();
    int2048 Abs() const; // return the absolute value
    int2048 Turn_Abs(); // make this a postive number
    void Resize(storage_type);
    // void Calc_Carry();
    inline storage_type Size() const; // return the size of val
    void Bit_Move(storage_type); // multiply with pow(base,x)
        // todo
public:
    // 构造函数
    int2048();
    int2048(storage_type);
    int2048(const std::string&);
    int2048(const int2048&);
    int2048(const int2048&&);
    bool Is_Zero() const;
    friend int2048 Sum_Abs(
        const int2048&, const int2048&); // calc the sum of the two abs numeber
    friend int2048 Diff_Abs(
        const int2048&,
        const int2048&); // calc the difference of the two abs numeber and the
        // first must be bigger than the second.
    void read(const std::string& s);
    void print();
	operator double();
    int2048& add(const int2048&);
    friend int2048 add(const int2048&, const int2048&);
    int2048& minus(const int2048&);
    friend int2048 minus(const int2048&, const int2048&);
    int2048& operator=(const int2048&);
    int2048& operator+=(int2048);
    friend int2048 operator+(const int2048&, const int2048&);
    int2048& operator-=(int2048);
    friend int2048 operator-(const int2048&, const int2048&);
    friend int2048 operator-(const int2048&);
    int2048& operator*=(const int2048&);
    friend int2048 operator*(const storage_type&, const int2048&);
    friend int2048 operator*(const int2048&, const storage_type&);
    friend int2048 operator*(const int2048&, const int2048&);
    int2048& operator/=(const int2048&);
    friend int2048 operator/(const int2048&, const int2048&);
    friend int2048 operator%(const int2048&, const int2048&);
    friend std::istream& operator>>(std::istream&, int2048&);
    friend std::ostream& operator<<(std::ostream&, const int2048&);
    friend bool operator==(const int2048&, const int2048&);
    friend bool operator!=(const int2048&, const int2048&);
    friend bool operator<(const int2048&, const int2048&);
    friend bool operator>(const int2048&, const int2048&);
    friend bool operator<=(const int2048&, const int2048&);
    friend bool operator>=(const int2048&, const int2048&);
};

    // inline bool operator==(const int2048&, const int2048&);
    // inline bool operator!=(const int2048&, const int2048&);
    // inline bool operator<(const int2048&, const int2048&);
    // inline bool operator>(const int2048&, const int2048&);
    // inline bool operator<=(const int2048&, const int2048&);
    // inline bool operator>=(const int2048&, const int2048&);
};
#endif