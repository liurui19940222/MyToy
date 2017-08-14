#include"ColladaFile.h"
#include"Debug.h"
#include<fstream>
#include<sstream>

using namespace std;
using namespace rapidxml;

vector<xml_node<>*> CColladaFile::GetNodesByName(xml_node<>* node, const string name)
{
	vector<xml_node<>*> list;
	for (xml_node<> *it = node->first_node();
		it; it = it->next_sibling())
	{
		if (it->name() == name)
			list.push_back(it);
	}
	return list;
}

xml_node<>* CColladaFile::GetNodeByName(xml_node<>* node, const string name)
{
	for (xml_node<> *it = node->first_node();
		it; it = it->next_sibling())
	{
		if (it->name() == name)
			return it;
	}
	return NULL;
}

xml_node<>* CColladaFile::GetNodeById(xml_node<>* node, const string name)
{
	for (xml_node<> *it = node->first_node();
		it; it = it->next_sibling())
	{
		if (GetAttribute(it, name) != "")
			return it;
	}
	return NULL;
}

string CColladaFile::GetAttribute(xml_node<>* node, const string name)
{
	for (xml_attribute<> *attr = node->first_attribute();
		attr; attr = attr->next_attribute())
	{
		if (attr->name() == name)
			return attr->value();
	}
	return string("");
}

void CColladaFile::ReadJoint(xml_node<>* joint_node, byte parent_ref, int depth)
{
	string values = GetNodeByName(joint_node, "matrix")->value();
	float* m = UnpackValues<float>(values, 16);
	Joint joint;
	joint.m_localMatrix = Matrix4x4(m);
	joint.m_name = GetAttribute(joint_node, "name");
	joint.m_iParent = parent_ref;
	m_skeleton.m_joints.push_back(joint);
	byte index = m_skeleton.m_joints.size() - 1;
	string str;
	for (int i = 0; i < depth; i++)
	{
		str += "   ";
	}
	CDebug::Log("%s p:%d", (str + GetAttribute(joint_node, "name")).c_str(), parent_ref);
	for (xml_node<> *it = joint_node->first_node(); it; it = it->next_sibling())
	{
		if (GetNodeByName(it, "matrix") != NULL)
		{
			ReadJoint(it, index, depth + 1);
		}
	}
}

void CColladaFile::LoadFromFile(const char* filename)
{
	//加载xml
	ifstream is(filename);
	stringstream ss;
	ss << is.rdbuf();
	is.close();
	size_t size = sizeof(char) * ss.str().size();
	char* buffer = (char*)malloc(size + 1);
	ss.read(buffer, size);
	buffer[size] = '\0';
	xml_document<> xml_doc;
	xml_doc.parse<0>(buffer);

	xml_node<>* root = xml_doc.first_node();
	//读取几何信息
	xml_node<>* mesh = GetNodeByName(root, "library_geometries")->first_node()->first_node();

	//读取source
	map<string, ValueArray<float>> source_map;
	vector<xml_node<>*> sources = GetNodesByName(mesh, "source");
	for (auto it = sources.begin(); it != sources.end(); ++it)
	{
		xml_node<>* float_array = GetNodeByName(*it, "float_array");
		int count = GetAttribute<int>(float_array, "count");
		string values = float_array->value();
		float* v3 = UnpackValues<float>(values, count);
		source_map.insert(make_pair("#" + GetAttribute(*it, "id"), ValueArray<float>{ v3, count }));
	}

	//根据下标读取实际的值
	int m_triangleNum = 0;
	vector<xml_node<>*> triangles = GetNodesByName(mesh, "triangles");
	for (auto it = triangles.begin(); it != triangles.end(); ++it)
	{
		m_triangleNum += GetAttribute<int>(*it, "count");
	}

	int vertIndex = 0, normalIndex = 0, uvIndex = 0; //读取时已到达的索引
	m_vertexArray = (Vector3*)malloc(sizeof(Vector3) * 3 * m_triangleNum);
	m_normalArray = (Vector3*)malloc(sizeof(Vector3) * 3 * m_triangleNum);
	m_uvArray = (Vector2*)malloc(sizeof(Vector2) * 3 * m_triangleNum);

	for (auto it = triangles.begin(); it != triangles.end(); ++it)
	{
		int count = GetAttribute<int>(*it, "count");
		vector<xml_node<>*> inputs = GetNodesByName(*it, "input");
		string p = GetNodeByName(*it, "p")->value();

		bool flags[3];
		int offsets[3];
		int step = 0;
		string sourceIds[3];
		for (auto it_input = inputs.begin(); it_input != inputs.end(); ++it_input, ++step)
		{
			string semantic = GetAttribute(*it_input, "semantic");
			int offset = GetAttribute<int>(*it_input, "offset");
			string source = GetAttribute(*it_input, "source");
			int index = 0;
			if (semantic == "VERTEX")
			{
				source = GetAttribute(GetNodeByName(mesh, "vertices")->first_node(), "source");
			}
			else if (semantic == "NORMAL")
			{
				index = 1;
			}
			else if (semantic == "TEXCOORD")
			{
				index = 2;
			}
			flags[index] = true;
			offsets[index] = offset;
			sourceIds[index] = source;
		}
		int* indices = UnpackValues<int>(p, (size_t)count * 3 * step);
		for (int i = 0; i < count * 3 * step; i += step)
		{
			if (flags[0])
			{
				ValueArray<float> s = source_map[sourceIds[0]];
				m_vertexArray[vertIndex++] = ((Vector3*)source_map[sourceIds[0]].array)[indices[i] + offsets[0]];
			}
			if (flags[1])
			{
				//这里根据下标取normal索引是有问题的，所以按顺序来取
				m_normalArray[normalIndex++] = ((Vector3*)source_map[sourceIds[1]].array)[normalIndex];
			}
			if (flags[2])
			{
				m_uvArray[uvIndex++] = ((Vector2*)source_map[sourceIds[2]].array)[indices[i] + offsets[2]];
			}
		}
		free(indices);
	}
	m_vertexNum = m_triangleNum * 3;

	//上传到缓冲区
	m_buffer.MakeBuffer(m_vertexArray, NULL, m_normalArray, m_uvArray, m_vertexNum);

	//读取关节
	xml_node<>* scene_root = GetNodeByName(root, "library_visual_scenes")->first_node();
	for (xml_node<> *it = scene_root->first_node(); it; it = it->next_sibling())
	{
		if (GetNodeByName(it, "matrix") != NULL)
		{
			ReadJoint(it, 0xFF, 0);
		}
	}

	free(buffer);
}