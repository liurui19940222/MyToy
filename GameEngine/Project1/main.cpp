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

int main()
{
	vector<int> list;
	vector<int> list2;
	list.push_back(1);
	list.push_back(2);
	list.push_back(3);
	list2.push_back(5);
	list2.push_back(6);
	list2.push_back(7);
	list.insert(list.begin() + list.size(), list2.begin(), list2.end());
	for each (int var in list)
	{
		cout << var << endl;
	}
	cout << list.size() << " " << list.capacity() << endl;
	system("pause");
	return 0;
}