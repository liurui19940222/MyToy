#ifndef _COLLADA_FILE_H_
#define _COLLADA_FILE_H_

#include<vector>
#include"ModelLoader.h"
#include"SpCommon\Converter.h"
#include"ExternInclude\rapidxml\rapidxml.hpp"
#include"SpRendering\SkeletonAnimation.h"
#include"SpCommon\Debug.h"

using namespace std;
using namespace rapidxml;

BEGIN_NAMESPACE_ENGINE

template<typename T>
struct ValueArray
{
	T* array;
	int size;
};

class ColladaLoader : public ModelLoader
{
	xml_document<> m_xmlDoc;
	char* m_xmlData;

#pragma region read_mesh

	//读取整个骨架
	void ReadSkeleton(xml_node<>* node, Model* model);

	//读取关节信息,包括没有蒙皮信息的节点
	void ReadJoint(xml_node<>* joint_node, byte parent_ref, int depth, Model* model);

	//读取蒙皮
	void ReadSkin(xml_node<>* root, vector<Vector4>& weights, vector<BVector4>& indices, Model* model);

	//按采样时间，把所有骨骼的采样分类
	void AddSample(map<float, AnimationSample>& p_samples, float time, byte jointIndex, const JointPose& pose);

	//读取Mesh
	void ReadMesh(xml_node<>* root, vector<Vector4>& weights, vector<BVector4>& indices, Model* model);

	//读取动画
	void ReadAnimation(xml_node<>* root, Model* model);

#pragma endregion

#pragma region read_xml

	//加载xml文件
	void LoadXmlDocument(const char* filename);

	//得到node下面名字为name的所有节点
	vector<xml_node<>*> GetNodesByName(xml_node<>* node, const string name);

	//得到node下面名字为name的第一个节点
	xml_node<>* GetNodeByName(xml_node<>* node, const string name);

	//得到node下面Id为(name)的节点
	xml_node<>* GetNodeById(xml_node<>* node, const string name);

	//得到node节点的属性为(name)的值
	string GetAttribute(xml_node<>* node, const string name);

	//读取id为(id)的Source节点的值
	vector<string> ReadSource(xml_node<>* node, const string id);

	//读取node节点下所有source节点的值
	map<string, vector<string>> ReadSources(xml_node<>* node);

	//将空格、换行、回车分隔的字符串，转换成vector<string>
	vector<string> UnpackValues(string& str, size_t count);

	//去掉字符串的第一个#
	inline string RemoveAtFirst(string& str)
	{
		if (str.size() < 1 || str[0] != '#')
			return str;
		return str.substr(1, str.size());
	}

	//得到node的name属性的值
	template<typename T>
	T GetAttribute(xml_node<>* node, const string name)
	{
		return CConverter::ToValue<T>(GetAttribute(node, name));
	}

	//将8 9 5 6 33这种分隔数据转换为数组
	template<typename TValue>
	TValue* UnpackValues(string& str, size_t count)
	{
		int start_pos = 0, array_index = 0, str_size = 0, total_size = str.size();
		bool begin = false;
		TValue* valueArray = (TValue*)malloc(sizeof(TValue) * count);
		memset(valueArray, 0, sizeof(TValue) * count);
		for (int i = 0; i <= total_size; ++i)
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

	//读取Source节点下面所有数据
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

#pragma endregion

public:
	virtual ModelPtr LoadFromFile(const char* filename) override;
	virtual void ReleaseSource() override;
};

END_NAMESPACE_ENGINE

#endif