#if 0
#include<iostream>
#include<vector>
#include"ArrayList.h"
#include"..\SpCommon\BinaryHeap.h"
#include"..\SpCommon\Math.h"
#include"..\SpCommon\Debug.h"

using namespace std;
USING_NAMESPACE_ENGINE;

void print(BinaryHeap<int>& heap)
{
	cout << endl;
	heap.Foreach([](int a) 
	{
		cout << a << endl; 
	});
}

void change(int* arr, int indexA, int indexB)
{
	int temp = arr[indexA];
	arr[indexA] = arr[indexB];
	arr[indexB] = temp;
}

void bubbleSort(int* arr, int length)
{
	int temp = 0;
	for (int i = 0; i < length - 1; ++i)
	{
		for (int j = 0; j < length - i - 1; ++j)
		{
			if (arr[j] > arr[j + 1])
			{
				change(arr, j, j + 1);
			}
		}
	}
}

void selectSort(int* arr, int length)
{
	int temp = 0;
	for (int i = 0; i < length; ++i)
	{
		temp = i;
		for (int j = i + 1; j < length; ++j)
		{
			if (arr[temp] > arr[j])
			{
				temp = j;
			}
		}
		if (temp != i)
		{
			change(arr, temp, i);
		}
	}
}

bool powerof2(int value)
{
	return (value & (value - 1)) == 0;
}

bool powerof2_(int value)
{
	return (~value + 1) == value;
}

int binarySearch(int* arr, int low, int high, int target)
{
	while (low <= high)
	{
		int mid = (high + low) >> 1;
		if (arr[mid] < target)
			low = mid + 1;
		else if (arr[mid] > target)
			high = mid - 1;
		else return mid;
	}
	return -1;
}

class A {
public:
	A() { cout << 1; }
	~A() { cout << 2; }
};

class B {
public:
	B() { cout << 3; }
	~B() { cout << 4; }
protected:
	A m_member1;
};

class C : public B {
public:
	C() { cout << 5; }
	~C() { cout << 6; }
private:
	A m_member2;
};

void D()
{
	C c;
}

int FindAllFactors(unsigned int* arrayA, int sizeA, 
	unsigned int * arrayB, int sizeB, 
	unsigned int* arrayC, int sizeC)
{
	if (!arrayA || !arrayB || !arrayC)
		return 0;
	if (!sizeA || !sizeB || !sizeC)
		return 0;
	int count = 0;
	for (int i = 0; i < sizeB; ++i)
	{
		for (int j = 0; j < sizeA; ++j)
		{
			if (arrayA[j] % arrayB[i] == 0)
			{
				if (count < sizeC)
					arrayC[count] = arrayB[i];
				count++;
				break;
			}
		}
	}
	return count;
}

#include"..\ExternInclude\rapidjson\rapidjson.h"
#include"..\ExternInclude\rapidjson\document.h"
#include"..\ExternInclude\rapidjson\writer.h"
#include"..\ExternInclude\rapidjson\stringbuffer.h"
#include"..\SpCommon\Object.h"
#include <iostream>
#include <fstream>

using namespace rapidjson;
using namespace std;

int main()
{
	//unsigned int a[] = { 7, 15, 9, 21, 48, 15 };
	//unsigned int b[] = { 2, 3, 7, 5, 16, 1, 55 };
	//unsigned int c[10] = { 0 };
	//int reuslt = FindAllFactors(a, 6, b, 7, c, 10);
	//int j = 0;
	// 1. 把 JSON 解析至 DOM。
	const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
	Document d;
	d.Parse(json);
	// 2. 利用 DOM 作出修改。
	Value& s = d["stars"];
	s.SetInt(s.GetInt() + 1);

	d.AddMember("color", 0, d.GetAllocator());
	d["color"].SetArray();
	Value& value = d["color"];
	value.PushBack(1.0, d.GetAllocator());

	d.AddMember("name", "哈哈", d.GetAllocator());

	// 3. 把 DOM 转换（stringify）成 JSON。
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	d.Accept(writer);
	// Output {"project":"rapidjson","stars":11}
	std::cout << buffer.GetString() << std::endl;

	ofstream ofs("D://2.json");
	ofs.write(buffer.GetString(), buffer.GetLength());
	ofs.close();

	Object obj(wstring(L"杰克"));
	string objjson = obj.ToJson();
	ofstream ofs2("D://obj.json");
	ofs2.write(objjson.c_str(), objjson.size());
	ofs2.close();
	system("pause");
	return 0;
}
#endif