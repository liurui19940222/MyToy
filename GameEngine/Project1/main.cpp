#include<iostream>

using namespace std;

class AAA
{
public:
	AAA() { cout << "AAA构造" << endl; }
};

class BBB
{
public:
	BBB(AAA) { cout << "BBB构造" << endl; }
};

void main()
{
	BBB bbb(AAA());
	system("pause");
}