#include<iostream>
#include"ArrayList.h"

using namespace std;
using namespace my_collection;

void Print(ArrayList<int>& list)
{
	list.Foreach([](int& i) { cout << i << endl; return false; });
	cout << "Capacity:" << list.capacity() << " Size:" << list.size() << endl;
}

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
	int a = 5, b = 6;
	_asm {
		mov eax, dword ptr ds:[a]
		mov ebx, dword ptr ds:[b]
		mov dword ptr ds:[b], eax
		mov dword ptr ds:[a], ebx
	}
	//int result = Div(5, 6);
	//result = Div(5, 7);
	//result = Div(5, 8);
	//result = Div(5, 2);
	//ArrayList<int> list;
	//list.PushBack(10);
	//list.PushBack(5);
	//list.Insert(1, 3);
	//list[1] = 4;
	//Print(list);
	//list.PushBack(99);
	//list.PushBack(88);
	//list.Erase(88);
	//list.PushBack(77);
	//list.PushBack(66);
	//Print(list);
	//cout << list[2] << endl;
	system("pause");
	return 0;
}