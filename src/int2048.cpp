#ifndef int2048_cpp
#define int2048_cpp
#include "int2048.h"
#include <complex>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
// 请不要使用 using namespace std;
using namespace sjtu;

sjtu::int2048::int2048()
{
    val.clear();
    negative = 0;
    // return;
}
sjtu::int2048::int2048(storage_type x)
{
    if (x < 0) {
        x = -x;
        negative = true;
    } else
        negative = false;
    val.clear();
    while (x) {
        val.push_back(x % base);
        x /= base;
    }
    // return;
}
sjtu::int2048::int2048(const std::string& s) { this->read(s); }
sjtu::int2048::int2048(const int2048& other) { *this = other; }
sjtu::int2048::int2048(const int2048&& other)
{
    *this = other;
    return;
}
void sjtu::int2048::read(const std::string& s)
{
    storage_type string_len = s.length();
    storage_type number_begin = 0; // record the postion of the first non-zero number.
    if (s[0] == '-') {
        number_begin++;
        negative = 1;
    } else
        negative = 0;
    val.clear();
    while (number_begin < string_len && s[number_begin] == '0')
        number_begin++;
    for (storage_type i = string_len - 1; i >= number_begin;
         i -= base_len) // i points to the postion of the last number of the
    // base_len number(s) in the string.
    {
        storage_type temp_num = 0;
        // for (storage_type j = i; j >= number_begin && j > i - base_len; j--) //
        // confirm that s[j] exists
        for (storage_type j = Max(i - base_len + 1, number_begin); j <= i;
             j++) // confirm that s[j] exists
            temp_num = temp_num * 10 + s[j] - '0';
        val.push_back(temp_num);
    }
    return;
}
void sjtu::int2048::print()
{
    std::cout << *this;
    return;
}
sjtu::int2048::operator double()
{
    double ans = 0;
    for (auto temp_riter = val.rbegin(); temp_riter != val.rend(); temp_riter++)
        ans = ans * base + *temp_riter;
	return ans;
}
void sjtu::int2048::Remove_Leading_Zero()
{
    while (!Is_Zero() && !(*(val.rbegin())))
        val.pop_back();
    return;
}
std::istream& operator>>(std::istream& is, int2048& x)
{
    std::string temp_string; // record the number;
    is >> temp_string;
    x = temp_string;
    return is;
}

std::ostream& sjtu::operator<<(std::ostream& os, const int2048& x)
{
    if (x.Is_Zero()) {
        os << 0;
        return os;
    }
    if (x.negative) {
        os << "-";
    }
    auto riter_temp = x.val.rbegin(); // use reverse_iterator to output the
        // number from end to begin.
    os << *riter_temp; // the highest numbers don't need leading zero(s).
    riter_temp++;
    for (; riter_temp != x.val.rend(); riter_temp++) {
        char temp_string[base_len + 1]; // record the context to output.
        storage_type tmp = *riter_temp;
        for (storage_type i = base_len - 1; i >= 0; i--) {
            temp_string[i] = (tmp % 10) + '0';
            tmp /= 10;
        }
        temp_string[base_len] = '\0';
        os << temp_string;
    }
    return os;
}
int2048& sjtu::int2048::operator=(const int2048& other)
{
    val = other.val;
    negative = other.negative;
    return *this;
}
int2048 sjtu::Sum_Abs(const int2048& a, const int2048& b)
{
    int2048 ans = a;
    storage_type size_ans = ans.Size();
    storage_type size_b = b.Size();
    ans.Resize(size_b);
    auto iter_ans = ans.val.begin();
    auto iter_b = b.val.begin();
    storage_type temp_carry = 0;
    for (; iter_b != b.val.end(); iter_b++, iter_ans++) {
        *iter_ans = *iter_ans + *iter_b + temp_carry;
        temp_carry = *iter_ans / base;
        *iter_ans %= base;
    }
    if (temp_carry) {
        if (iter_ans == ans.val.end())
            ans.val.push_back(temp_carry);
        else {
            for (; temp_carry && iter_ans != ans.val.end(); iter_ans++) {
                *iter_ans = *iter_ans + temp_carry;
                temp_carry = *iter_ans / base;
                *iter_ans %= base;
            }
            if (temp_carry)
                ans.val.push_back(temp_carry);
        }
    }
    return ans;
}
int2048 sjtu::Diff_Abs(const int2048& a, const int2048& b)
{
    int2048 ans = a;
    ans.negative = 0;
    auto iter_ans = ans.val.begin();
    auto iter_b = b.val.begin();
    storage_type temp_carry = 0;
    for (; iter_b != b.val.end(); iter_ans++, iter_b++) {
        *iter_ans += temp_carry - *iter_b;
        temp_carry = Devide_Num(*iter_ans, base);
        *iter_ans = Mod_Num(*iter_ans, base);
    }
    for (; temp_carry; iter_ans++) {
        *iter_ans += temp_carry;
        temp_carry = Devide_Num(*iter_ans, base);
        *iter_ans = Mod_Num(*iter_ans, base);
    }
    ans.Remove_Leading_Zero();
    return ans;
}

int2048 sjtu::operator+(const int2048& a, const int2048& b)
{
    if (a.Is_Zero())
        return b;
    if (b.Is_Zero())
        return a;
    if (a.negative ^ b.negative) {
        if (a.Negative_Of() == b) // a + b = 0
            return 0;
        int2048 ans;
        ans = Diff_Abs(Max(a.Abs(), b.Abs()), Min(a.Abs(), b.Abs()));
        ans.negative ^= a.negative ^ (a.Abs() < b.Abs());
		ans.Remove_Leading_Zero();
        return ans;
    } else {
        int2048 ans;
        ans = Sum_Abs(a, b);
        ans.negative = a.negative;
		ans.Remove_Leading_Zero();
        return ans;
    }
}
int2048 sjtu::int2048::Negative_Of() const
{
    int2048 ans(*this);
    if (!ans.Is_Zero())
        ans.negative ^= 1;
    return ans;
}
int2048 sjtu::int2048::Turn_Negative()
{
    if (!Is_Zero())
        negative ^= 1;
    return *this;
}
int2048 sjtu::int2048::Abs() const
{
    if (negative)
        return Negative_Of();
    return *this;
}
int2048 sjtu::int2048::Turn_Abs()
{
    negative = 0;
    return *this;
}
void sjtu::int2048::Resize(storage_type x)
{
    storage_type tmp_size = Size();
    while (x > tmp_size) {
        x--;
        val.push_back(0);
    }
    return;
}
bool sjtu::int2048::Is_Zero() const { return val.empty(); }
inline storage_type sjtu::int2048::Size() const { return val.size(); }
void sjtu::int2048::Bit_Move(storage_type x)
{
    val.insert(val.begin(), x, 0);
	Remove_Leading_Zero();
    return;
}
int2048& sjtu::int2048::operator+=(int2048 other)
{
    *this = *this + other;
    return *this;
}
int2048& sjtu::int2048::add(const int2048& other) { return *this += other; }
int2048 sjtu::add(const int2048& a, const int2048& b) { return a + b; }
int2048 sjtu::operator-(const int2048& a, const int2048& b)
{
    return a + b.Negative_Of();
}
int2048 sjtu::operator-(const int2048& obj)
{
	return obj.Negative_Of();
}
int2048& sjtu::int2048::operator-=(int2048 other)
{
    *this = *this - other;
    return *this;
}
int2048& sjtu::int2048::minus(const int2048& other)
{
    *this -= other;
    return *this;
}
// 输出两个大整数之差
int2048 sjtu::minus(const int2048& a, const int2048& b) { return a - b; }
bool sjtu::operator==(const int2048& a, const int2048& b)
{
    if (a.Is_Zero() && b.Is_Zero())
        return true; // a = b = 0
    if (a.Is_Zero() || b.Is_Zero())
        return false;
    if (a.negative ^ b.negative)
        return false;
    if (a.Size() != b.Size())
        return false;
    for (auto iter_a = a.val.rbegin(), iter_b = b.val.rbegin();
         iter_a != a.val.rend(); iter_a++, iter_b++)
        if (*iter_a != *iter_b)
            return false;
    return true;
}
bool sjtu::operator!=(const int2048& a, const int2048& b) { return !(a == b); }
bool sjtu::operator<(const int2048& a, const int2048& b)
{
    if (a.Is_Zero() && b.Is_Zero())
        return false; // a = b = 0
    if (a.Is_Zero())
        return !(b.negative);
    if (b.Is_Zero())
        return a.negative;
    if (a.negative ^ b.negative)
        return a.negative;
    bool flag = a.negative; // records whether both a and b are negative.
    // the final answer will be (flag ^ (Abs(a) < Abs(b))) (if a != b)
    if (a.Size() != b.Size())
        return (flag ^ (a.Size() < b.Size()));
    for (auto iter_a = a.val.rbegin(), iter_b = b.val.rbegin();
         iter_a != a.val.rend(); iter_a++, iter_b++)
        if (*iter_a != *iter_b)
            return (flag ^ (*iter_a < *iter_b));
    return false; // this is executed onlt when a=b.
}
bool sjtu::operator>(const int2048& a, const int2048& b)
{
    return !(a < b || a == b);
}
bool sjtu::operator<=(const int2048& a, const int2048& b)
{
    return (a < b || a == b);
}
bool sjtu::operator>=(const int2048& a, const int2048& b)
{
    return (a > b || a == b);
}
int2048 sjtu::operator*(const int2048& a, const int2048& b)
{
    int2048 ans;
    ans.negative = a.negative ^ b.negative;
    storage_type size_a = a.Size();
    storage_type size_b = b.Size();
    ans.Resize(size_a + size_b - 1);
    auto iter_a = a.val.begin();
    auto iter_ans = ans.val.begin(); // point to ans.val[index_a]
    for (; iter_a != a.val.end(); iter_a++, iter_ans++) {
        if (!(*iter_a))
            continue; // when the number on iter_a is zero we dont need to calculate.
        auto iter_ans_modify = iter_ans;
        for (auto iter_b = b.val.begin(); iter_b != b.val.end();
             iter_b++, iter_ans_modify++)
            if (*iter_b)
                *iter_ans_modify += (*iter_a) * (*iter_b);
        storage_type temp_carry = 0;
        for (auto temp_iter_ans = ans.val.begin(); temp_iter_ans != ans.val.end();
             temp_iter_ans++) {
            *temp_iter_ans += temp_carry;
            temp_carry = *temp_iter_ans / base;
            *temp_iter_ans %= base;
        }
        if (temp_carry)
            ans.val.push_back(temp_carry);
    }
	ans.Remove_Leading_Zero();
    return ans;
}
int2048 sjtu::operator*(const storage_type& b, const int2048& a) { return a * b; }
int2048 sjtu::operator*(const int2048& a, const storage_type& b)
{
    if (!b)
        return int2048(0);
    int2048 ans = a;
    if (b == 1)
        return ans;
    if (b == -1) {
        ans.Turn_Negative();
        return ans;
    }
    if (abs(b) > base)
        return int2048(b) * a;
    ans.negative = a.negative ^ Is_Negative(b);
    storage_type size_a = a.Size();
    ans.Resize(size_a);
    auto iter_a = a.val.begin();
    auto iter_ans = ans.val.begin(); // point to ans.val[index_a]
    for (; iter_a != a.val.end(); iter_a++, iter_ans++)
        *iter_ans *= b;
    storage_type temp_carry = 0;
    for (auto temp_iter_ans = ans.val.begin(); temp_iter_ans != ans.val.end();
         temp_iter_ans++) {
        *temp_iter_ans += temp_carry;
        temp_carry = *temp_iter_ans / base;
        *temp_iter_ans %= base;
    }
    if (temp_carry)
        ans.val.push_back(temp_carry);
	ans.Remove_Leading_Zero();
    return ans;
}
int2048& sjtu::int2048::operator*=(const int2048& other)
{
    *this = *this * other;
    return *this;
}
int2048 sjtu::operator/(const int2048& a, const int2048& b)
{
    int2048 temp_a = a.Abs(); // record the remaining dividend
    int2048 temp_b = b.Abs(); // record the remaining dividend
    int2048 ans;
    ans.negative = a.negative ^ b.negative;
    ans.Resize(a.Size() - b.Size() + 1);
    storage_type temp_b_first = *(b.val.rbegin());
    storage_type temp_pos = temp_a.Size() - b.Size();
    auto riter_ans = ans.val.rbegin();
    bool flag = false;
    while (temp_a >= temp_b) {
        // int2048 temp_a_modify = temp_a; // NOT STANDARD where the highest may
        // contain actually two number while the actual number is still the same.
        storage_type temp_a_first = *(temp_a.val.rbegin());
        temp_pos = temp_a.Size() - b.Size();
        if (*(temp_a.val.rbegin()) < temp_b_first || flag) {
            temp_a_first = temp_a_first * base + *(++temp_a.val.rbegin());
            riter_ans++;
            temp_pos--;
        }
        storage_type temp_l = temp_a_first / (temp_b_first + 1); // *iter_ans >= temp_l
        storage_type temp_r = temp_a_first / temp_b_first + 1; // *iter_ans < temp_r
        int2048 temp_product;
        while (temp_l < temp_r - 1) {
            storage_type temp_mid = (temp_l + temp_r) >> 1; // equal to (temp_l + temp_r) / 2
            temp_product = (temp_mid * temp_b);
            temp_product.Bit_Move(temp_pos);
            if (temp_product <= temp_a)
                temp_l = temp_mid;
            else
                temp_r = temp_mid;
        }
        if (temp_l) {
            temp_product = (temp_l * temp_b);
            temp_product.Bit_Move(temp_pos);
            temp_a -= temp_product;
            ans.val[temp_pos] += temp_l;
            flag = false;
        } else
            flag = true;
    }
    if (ans.negative && !temp_a.Is_Zero())
        ans -= 1;
    ans.Remove_Leading_Zero();
    return ans;
}

int2048 sjtu::operator%(const int2048& a, const int2048& b) // a%b=a-(a//b)*b
{
	// int2048 x = a/b;
	return a - (a / b) * b;
}

int2048& sjtu::int2048::operator/=(const int2048& x)
{
    *this = *this / x;
    return *this;
}
#endif