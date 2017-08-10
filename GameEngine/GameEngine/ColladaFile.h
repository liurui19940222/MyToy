#ifndef _COLLADA_FILE_H_
#define _COLLADA_FILE_H_

#include<vector>
#include"Converter.h"
#include"include\rapidxml\rapidxml.hpp"
#include"ModelFile.h"

using namespace std;
using namespace rapidxml;

template<typename T>
struct SValueArray
{
	T* array;
	int size;
};

class CColladaFile : public CModelFile
{

	vector<xml_node<>*> GetNodesByName(xml_node<>* node, const string name);
	xml_node<>* GetNodeByName(xml_node<>* node, const string name);
	xml_node<>* GetNodeById(xml_node<>* node, const string name);
	string GetAttribute(xml_node<>* node, const string name);

	template<typename T>
	T GetAttribute(xml_node<>* node, const string name)
	{
		return CConverter::ToValue<T>(GetAttribute(node, name));
	}

	template<typename TValue>
	TValue* UnpackValues(string& str, size_t count)
	{
		int start_pos = 0;
		TValue* valueArray = (TValue*)malloc(sizeof(TValue) * count);
		int array_index = 0;
		int str_size = 0;
		bool begin = false;
		for (size_t i = 0; i < str.size(); ++i)
		{
			if (str[i] != ' ' && str[i] != '\r' && str[i] != '\n')
			{
				if (!begin)
				{
					start_pos = i;
					begin = true;
				}
				str_size++;
			}
			else
			{
				if (begin)
				{
					begin = false;
					TValue value = CConverter::ToValue<TValue>(str.substr(start_pos, str_size));
					valueArray[array_index++] = value;
					str_size = 0;
				}
			}
		}
		return valueArray;
	}

public:
	virtual void LoadFromFile(const char* filename) override;
};

#endif