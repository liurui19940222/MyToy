#include<iostream>

using namespace std;

class AAA
{
public:
	AAA() { cout << "AAA����" << endl; }
};

class BBB
{
public:
	BBB(AAA) { cout << "BBB����" << endl; }
};

void main()
{
	BBB bbb(AAA());
	system("pause");
}