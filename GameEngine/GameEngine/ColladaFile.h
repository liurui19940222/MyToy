#ifndef _COLLADA_FILE_H_
#define _COLLADA_FILE_H_

#include<vector>
#include"Converter.h"
#include"include\rapidxml\rapidxml.hpp"
#include"ModelFile.h"
#include"BoneAnimation.h"

using namespace std;
using namespace rapidxml;

template<typename T>
struct ValueArray
{
	T* array;
	int size;
};

class CColladaFile : public CModelFile
{
private:
	Matrix4x4 m_bindShapeMat;

	vector<xml_node<>*> GetNodesByName(xml_node<>* node, const string name);
	xml_node<>* GetNodeByName(xml_node<>* node, const string name);
	xml_node<>* GetNodeById(xml_node<>* node, const string name);
	string GetAttribute(xml_node<>* node, const string name);
	vector<string> ReadSource(xml_node<>* node, const string id);
	map<string, vector<string>> ReadSources(xml_node<>* node);
	void ReadJoint(xml_node<>* joint_node, byte parent_ref, int depth);
	vector<string> UnpackValues(string& str, size_t count);

	template<typename T>
	T GetAttribute(xml_node<>* node, const string name)
	{
		return CConverter::ToValue<T>(GetAttribute(node, name));
	}

	template<typename TValue>
	TValue* UnpackValues(string& str, size_t count)
	{
		int start_pos = 0, array_index = 0, str_size = 0, total_size = str.size();
		bool begin = false;
		TValue* valueArray = (TValue*)malloc(sizeof(TValue) * count);
		memset(valueArray, 0, sizeof(TValue) * count);
		for (size_t i = 0; i <= total_size; ++i)
		{
			if (str[i] != ' ' && str[i] != '\r' && str[i] != '\n' && i < total_size)
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
					valueArray[array_index++] = CConverter::ToValue<TValue>(str.substr(start_pos, str_size));
					str_size = 0;
				}
			}
		}
		return valueArray;
	}

	template<typename T>
	T* ReadSource(xml_node<>* node, const string id, int* count)
	{
		xml_node<>* source_node = GetNodeById(node, id);
		xml_node<>* accessor_node = GetNodeByName(source_node, "technique_common")->first_node();
		*count = GetAttribute<int>(accessor_node, "count");
		string source_id = GetAttribute(accessor_node, "source");
		source_id = source_id.substr(1, source_id.size());
		xml_node<>* array_node = GetNodeById(source_node, source_id);
		*count = GetAttribute<int>(array_node, "count");
		string value = array_node->value();
		return UnpackValues<T>(value, *count);
	}

public:
	Skeleton m_skeleton;
	JointWeight* m_jointWeights;

	virtual void LoadFromFile(const char* filename) override;
};

#endif