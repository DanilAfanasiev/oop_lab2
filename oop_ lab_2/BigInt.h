#pragma once

#include<deque>
#include<string>
using namespace std;


class BigInt
{
public:
	BigInt();
	BigInt(int);
	BigInt(const string&);

	void print(std::ostream&) const;

	/*operator bool() const;*/
	bool operator==(const BigInt &other) const;
	bool operator!=(const BigInt &other) const;
	bool operator>(const BigInt&) const;
	BigInt operator-() const;
	BigInt operator+(const BigInt&) const;
	BigInt operator-(const BigInt&) const;
	BigInt operator*(const BigInt&) const;
	BigInt operator/(const BigInt&) const; //dont divide by 0
	BigInt operator%(const BigInt&) const; //only for 0 or positive

	BigInt karatsuba(const BigInt&) const;
private:
	bool sign; //true if <0
	deque<int> val;

	BigInt digits;
	int n;
	// this double = digits * 10^n

};
std::ostream& operator<<(std::ostream&, const BigInt&);