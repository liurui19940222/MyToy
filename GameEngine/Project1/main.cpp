#include<iostream>

using namespace std;

class Vector2
{
public:
	int x;
	int y;

	Vector2() : x(0), y(0) { cout << "�޲ι��캯��" << endl; }
	Vector2(int x, int y) : x(x), y(y) { cout << "�вι��캯��" << endl; }

	Vector2(const Vector2& v) 
	{
		x = v.x;
		y = v.y;
		cout << "���ÿ������캯��" << endl;
	}

	Vector2(Vector2&& v) noexcept = default;

	void operator=(const Vector2& v)
	{
		x = v.x;
		y = v.y;
		cout << "������ֵ����" << endl;
	}

	void operator=(Vector2&& v) noexcept
	{
		x = v.x;
		y = v.y;
		v.x = 999;
		v.y = 888;
		cout << "�ƶ���ֵ����" << endl;
	}
};

Vector2 get()
{
	return Vector2(5 , 6);
}

int main()
{
	Vector2 a(3, 3);
	cout << "a:" << &a << endl;
	Vector2 b = std::move(a);
	cout << "a:" << &a << endl;
	cout << "b:" << &b << endl;
	system("pause");
	return 0;
}