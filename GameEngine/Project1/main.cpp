#include<iostream>
#include<vector>
#include"ArrayList.h"

using namespace std;
using namespace my_collection;

int Add(int a, int b)
{
	int ra = a ^ b;
	int rb = a & b;
	return rb ? Add(ra, rb << 1) : ra;
}

int Sub(int a, int b)
{
	return Add(a, -b);
}

int Mul(int a, int b)
{
	int result = 0;
	for (int i = 0; i < b; i = Add(i, 1))
	{
		result = Add(result, a);
	}
	return result;
}

int Div(int a, int b)
{
	if (b == 0)
		assert(0);
	int result = 0;
	while(a > b)
	{
		a = Sub(a, b);
		result = Add(result, 1);
	}
	return result;
}

class AAA
{
public:
	struct B{
		int a;
		int b;
		int c;
	};
private:
	int size;
};

int main()
{
	cout << sizeof(AAA) << endl;
	system("pause");
	return 0;
}