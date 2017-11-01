#include<iostream>
#include<cstdlib>
#include"BigInt.h"


BigInt::BigInt() :sign(0) { val.push_front(0); }

BigInt::BigInt(const string &v)//till first non-digit or till end; first zeros ignored; if empty, then 0
{
	int length = v.size(), i = 0; char tmp;

	if (length>0)
	{
		tmp = v[i];
		if (tmp == '-') { sign = 1; ++i; }
		else { if (tmp == '+') ++i; sign = 0; }

		for (; i<length && (tmp = v[i]) && isdigit(tmp); ++i)
		{
			if (!(tmp == '0' && val.empty())) val.push_back(tmp - '0');
		}
	}

	if (val.empty()) { val.push_back(0); sign = 0; }
}

BigInt::BigInt(int n)
{
	if (n<0) { sign = true; n = -n; }
	else sign = false;
	do
	{
		val.push_front(n % 10); n /= 10;
	} while (n>0);
}

void BigInt::print(std::ostream& os) const
{
	int length = val.size();
	if (sign) os << '-';
	for (int i = 0; i<length; ++i) os << val[i];
}

std::ostream& operator<<(std::ostream& os, const BigInt& n)
{
	n.print(os);
	return os;
}

BigInt::operator bool() const
{
	return (val.front());
}

bool BigInt::operator==(const BigInt &other) const
{
	if (val[0] == 0 && other.val[0] == 0) return true;
	if ((sign != other.sign) || (val.size() != other.val.size())) return false;
	int s = val.size();
	for (int i = 0; i<s; ++i) if (val[i] != other.val[i]) return false;
	return true;
}

bool BigInt::operator!=(const BigInt &other) const
{
	return !((*this) == other);
}

bool BigInt::operator>(const BigInt &other) const
{
	if (val[0] == 0)
	{
		if (other.val[0] == 0) return false;
		if (other.sign) return true;
		return false;
	}
	if (other.val[0] == 0) return !sign;
	if (sign != other.sign) return !sign;
	if (val.size()>other.val.size()) return !sign;
	if (val.size()<other.val.size()) return sign;
	bool res = false, stop = false;
	for (int i = 0; i<val.size() && !stop; ++i)
	{
		if (val[i]>other.val[i]) { res = true; stop = true; }
		if (val[i]<other.val[i]) { res = false; stop = true; }
	}
	if (stop) { if (!sign) return res; else return !res; }
	return false;
}

BigInt BigInt::operator-() const
{
	BigInt res = *this;
	res.sign = (!sign);
	return res;
}

BigInt BigInt::operator+(const BigInt &other) const
{
	if (!sign && other.sign) return (*this) - (-other);
	else if (sign && !other.sign) return other - (-(*this)); //if different signs, do minus
	BigInt res; res.val.clear();
	const BigInt *big, *small;
	int add = 0, curr = 0, maxsize, minsize, i;
	if (val.size()>other.val.size()) { maxsize = val.size(); minsize = other.val.size(); small = &other; big = this; }
	else { maxsize = other.val.size(); minsize = val.size(); small = this; big = &other; }
	for (i = 1; i <= minsize; ++i) //do plus
	{
		curr = add + (*small).val[minsize - i] + (*big).val[maxsize - i];
		add = curr / 10;
		res.val.push_front(curr % 10);
	}
	while (i <= maxsize)
	{
		curr = add + (*big).val[maxsize - i];
		add = curr / 10;
		res.val.push_front(curr % 10);
		++i;
	}
	if (add != 0) res.val.push_front(add);
	res.sign = sign; //sign = signs of arguments
	return res;
}


BigInt BigInt::operator-(const BigInt &other) const
{
	if (sign != other.sign) return (*this) + (-other); //if different signs, do plus
	BigInt res;
	const BigInt *small, *big;
	res.val.clear();

	if (other>(*this))
	{
		res.sign = true; if (!sign) { big = &other; small = this; }
		else { big = this; small = &other; }
	}
	else { res.sign = false; if (!sign) { big = this; small = &other; } else { big = &other; small = this; } }

	int add = 0, curr = 0, maxsize = (*big).val.size(), minsize = (*small).val.size(), i;
	for (i = 1; i <= minsize; ++i) //do minus
	{
		curr = add + (*big).val[maxsize - i] - (*small).val[minsize - i];
		add = 0;
		if (curr<0) { curr += 10; --add; }
		res.val.push_front(curr);
	}
	while (i <= maxsize)
	{
		curr = add + (*big).val[maxsize - i];
		add = 0;
		if (curr<0) { curr += 10; --add; }
		res.val.push_front(curr);
		++i;
	}
	while (res.val.size()>1 && res.val[0] == 0) res.val.pop_front();
	return res;
}

BigInt BigInt::operator*(const BigInt &other) const
{
	BigInt res; res.val.clear();
	int size1 = val.size(), size2 = other.val.size(), resSize = size1 + size2;
	int add = 0, curr = 0;
	for (int i = 1; i<resSize; ++i)
	{
		curr = 0;
		for (int j = 1; j<size2 + 1; ++j)
			if (size1 >= i - j + 1 && i - j + 1>0) curr += val[size1 - i + j - 1] * other.val[size2 - j];
		curr += add;
		add = curr / 10;
		res.val.push_front(curr % 10);
	}
	if (add != 0) { res.val.push_front(add); }
	if (sign != other.sign) res.sign = true;
	return res;
	
}


BigInt BigInt::operator/(const BigInt &other) const //do not divide by 0
{
	if (!val[0]) return BigInt(0);
	BigInt a = (*this), b = other, tmp, res; res.val.clear(); int next = 0, i = 0;
	b.sign = false;
	while (b>tmp && !a.val.empty())
	{
		if (!tmp.val[0]) tmp.val.clear(); tmp.val.push_back(a.val.front()); a.val.pop_front();
	}
	do
	{
		while (b>tmp && !a.val.empty())
		{
			if (!tmp.val[0]) tmp.val.clear();
			tmp.val.push_back(a.val.front());
			a.val.pop_front();
			if (i) res.val.push_back(0);
			++i;
		}
		while (!(b>tmp)) { tmp = tmp - b; ++next; }

		res.val.push_back(next); next = 0; i = 0;
	} while (!a.val.empty());
	if (sign != other.sign) res.sign = true;
	return res;
}

BigInt BigInt::operator%(const BigInt &other) const //only for [0 or positive]/positive
{
	if (!val[0]) return BigInt(0);
	BigInt a = (*this), tmp;
	do
	{
		while (other>tmp && !a.val.empty())
		{
			if (!tmp.val[0]) tmp.val.clear();
			tmp.val.push_back(a.val.front());
			a.val.pop_front();
		}
		while (!(other>tmp)) tmp = tmp - other;
	} while (!a.val.empty());
	return tmp;
}
