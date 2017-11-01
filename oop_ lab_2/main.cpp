#include <iostream>
#include "BigInt.h"

using namespace std;

int main()
{
	BigInt a("123456789"), b("987654321");


	cout << "a = " << a << endl;
	cout << "b = " << b << endl;
	cout << "mul = " << a*b << endl << endl;
	
	BigInt x("21108"), y("736");

	cout << "x = " << x << endl;
	cout << "y = " << y << endl;
	cout << "div = " << x / y << endl << endl;

	system("pause");
	return 0;
}
