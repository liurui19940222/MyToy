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

	//��ȡ�����Ǽ�
	void ReadSkeleton(xml_node<>* node, Model* model);

	//��ȡ�ؽ���Ϣ,����û����Ƥ��Ϣ�Ľڵ�
	void ReadJoint(xml_node<>* joint_node, byte parent_ref, int depth, Model* model);

	//��ȡ��Ƥ
	void ReadSkin(xml_node<>* root, vector<Vector4>& weights, vector<BVector4>& indices, Model* model);

	//������ʱ�䣬�����й����Ĳ�������
	void AddSample(map<float, AnimationSample>& p_samples, float time, byte jointIndex, const JointPose& pose);

	//��ȡMesh
	void ReadMesh(xml_node<>* root, vector<Vector4>& weights, vector<BVector4>& indices, Model* model);

	//��ȡ����
	void ReadAnimation(xml_node<>* root, Model* model);

#pragma endregion

#pragma region read_xml

	//����xml�ļ�
	void LoadXmlDocument(const char* filename);

	//�õ�node��������Ϊname�����нڵ�
	vector<xml_node<>*> GetNodesByName(xml_node<>* node, const string name);

	//�õ�node��������Ϊname�ĵ�һ���ڵ�
	xml_node<>* GetNodeByName(xml_node<>* node, const string name);

	//�õ�node����IdΪ(name)�Ľڵ�
	xml_node<>* GetNodeById(xml_node<>* node, const string name);

	//�õ�node�ڵ������Ϊ(name)��ֵ
	string GetAttribute(xml_node<>* node, const string name);

	//��ȡidΪ(id)��Source�ڵ��ֵ
	vector<string> ReadSource(xml_node<>* node, const string id);

	//��ȡnode�ڵ�������source�ڵ��ֵ
	map<string, vector<string>> ReadSources(xml_node<>* node);

	//���ո񡢻��С��س��ָ����ַ�����ת����vector<string>
	vector<string> UnpackValues(string& str, size_t count);

	//ȥ���ַ����ĵ�һ��#
	inline string RemoveAtFirst(string& str)
	{
		if (str.size() < 1 || str[0] != '#')
			return str;
		return str.substr(1, str.size());
	}

	//�õ�node��name���Ե�ֵ
	template<typename T>
	T GetAttribute(xml_node<>* node, const string name)
	{
		return CConverter::ToValue<T>(GetAttribute(node, name));
	}

	//��8 9 5 6 33���ַָ�����ת��Ϊ����
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

	//��ȡSource�ڵ�������������
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