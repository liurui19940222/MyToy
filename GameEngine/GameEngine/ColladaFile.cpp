#include"ColladaFile.h"
#include<fstream>
#include<sstream>

using namespace std;
using namespace rapidxml;

void CColladaFile::LoadXmlDocument(const char* filename)
{
	ifstream is(filename);
	stringstream ss;
	ss << is.rdbuf();
	is.close();
	size_t size = sizeof(char) * ss.str().size();
	m_xmlData = (char*)malloc(size + 1);
	ss.read(m_xmlData, size);
	m_xmlData[size] = '\0';
	m_xmlDoc.parse<0>(m_xmlData);
}

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
		if (GetAttribute(it, "id") == name)
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

vector<string> CColladaFile::UnpackValues(string& str, size_t count)
{
	int start_pos = 0, array_index = 0, str_size = 0, total_size = str.size();
	bool begin = false;
	vector<string> list;
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
				list.push_back(str.substr(start_pos, str_size));
				str_size = 0;
			}
		}
	}
	return list;
}

vector<string> CColladaFile::ReadSource(xml_node<>* node, const string id)
{
	map<string, vector<string>> dic;
	xml_node<>* source_node = GetNodeById(node, id);
	xml_node<>* accessor_node = GetNodeByName(source_node, "technique_common")->first_node();
	int count = GetAttribute<int>(accessor_node, "count");
	string source_id = GetAttribute(accessor_node, "source");
	source_id = source_id.substr(1, source_id.size());
	xml_node<>* array_node = GetNodeById(source_node, source_id);
	count = GetAttribute<int>(array_node, "count");
	string value = array_node->value();
	return  UnpackValues(value, count);
}

map<string, vector<string>> CColladaFile::ReadSources(xml_node<>* node)
{
	map<string, vector<string>> dic;
	vector<xml_node<>*> nodes = GetNodesByName(node, "source");
	for (xml_node<>* source_node : nodes)
	{
		xml_node<>* accessor_node = GetNodeByName(source_node, "technique_common")->first_node();
		int count = GetAttribute<int>(accessor_node, "count");
		string source_id = GetAttribute(accessor_node, "source");
		source_id = source_id.substr(1, source_id.size());
		xml_node<>* array_node = GetNodeById(source_node, source_id);
		count = GetAttribute<int>(array_node, "count");
		string value = array_node->value();
		dic.insert(make_pair("#" + GetAttribute(array_node, "id"), UnpackValues(value, count)));
	}
	return dic;
}

void CColladaFile::ReadSkeleton(xml_node<>* node)
{
	xml_node<>* scene_root = GetNodeByName(node, "library_visual_scenes")->first_node();
	for (xml_node<> *it = scene_root->first_node(); it; it = it->next_sibling())
	{
		if (GetNodeByName(it, "matrix") != NULL)
		{
			ReadJoint(it, 0xFF, 0);
		}
	}
	m_skeletonPose.m_aGlobalPose = (Matrix4x4*)malloc(sizeof(Matrix4x4) * m_skeleton.GetSize());
}

void CColladaFile::ReadJoint(xml_node<>* joint_node, byte parent_ref, int depth)
{
	string values = GetNodeByName(joint_node, "matrix")->value();
	float* m = UnpackValues<float>(values, 16);
	Joint joint;
	joint.m_localMatrix = Matrix4x4(m);
	joint.m_name = GetAttribute(joint_node, "name");
	joint.m_iParent = parent_ref;
	m_skeleton.AddJoint(joint);
	byte index = m_skeleton.GetSize() - 1;
	//--------------------打印关节树----------------------
	//string str;
	//for (int i = 0; i < depth; i++)
	//{
	//	str += "   ";
	//}
	//CDebug::Log("%s p:%d", (str + GetAttribute(joint_node, "name")).c_str(), parent_ref);

	for (xml_node<> *it = joint_node->first_node(); it; it = it->next_sibling())
	{
		if (GetNodeByName(it, "matrix") != NULL)
		{
			ReadJoint(it, index, depth + 1);
		}
	}
}

JointWeight* CColladaFile::ReadSkin(xml_node<>* root)
{
	//读取蒙皮
	xml_node<>* skin_node = GetNodeByName(root, "library_controllers")->first_node()->first_node();
	string mat_value = GetNodeByName(skin_node, "bind_shape_matrix")->value();
	float* p_mat = UnpackValues<float>(mat_value, 16);
	m_bindShapeMat = p_mat;
	free(p_mat);
	xml_node<>* joint_node = GetNodeByName(skin_node, "joints");
	vector<xml_node<>*> joint_inputs = GetNodesByName(joint_node, "input");
	vector<string> joint_source;
	vector<float> weight_source;
	vector<Matrix4x4> matrix_source;
	for (xml_node<>* input : joint_inputs)
	{
		string semantic = GetAttribute(input, "semantic");
		string source_id = GetAttribute(input, "source");
		source_id = source_id.substr(1, source_id.size());
		int count = 0;
		if (semantic == "JOINT")
		{
			joint_source = ReadSource(skin_node, source_id);
		}
		else if (semantic == "INV_BIND_MATRIX")
		{
			float* f = ReadSource<float>(skin_node, source_id, &count);
			for (int i = 0; i < count; i += 16)
			{
				matrix_source.push_back(Matrix4x4(
					f[i + 0], f[i + 1], f[i + 2], f[i + 3],
					f[i + 4], f[i + 5], f[i + 6], f[i + 7],
					f[i + 8], f[i + 9], f[i + 10], f[i + 11],
					f[i + 12], f[i + 13], f[i + 14], f[i + 15]
				));
			}
			free(f);
		}
	}

	xml_node<>* vertex_weights_node = GetNodeByName(skin_node, "vertex_weights");
	int weights_count = GetAttribute<int>(vertex_weights_node, "count");
	vector<xml_node<>*> weights_inputs = GetNodesByName(vertex_weights_node, "input");
	int weights_offsets[2];
	for (xml_node<>* weights_input : weights_inputs)
	{
		string semantic = GetAttribute(weights_input, "semantic");
		int offset = GetAttribute<int>(weights_input, "offset");
		if (semantic == "JOINT")
		{
			weights_offsets[0] = offset;
		}
		else if (semantic == "WEIGHT")
		{
			string source_id = GetAttribute(weights_input, "source");
			source_id = source_id.substr(1, source_id.size());
			weights_offsets[1] = offset;
			int count = 0;
			float* f = ReadSource<float>(skin_node, source_id, &count);
			for (int i = 0; i < count; ++i)
			{
				weight_source.push_back(f[i]);
			}
			free(f);
		}
	}
	string vcount_str = GetNodeByName(vertex_weights_node, "vcount")->value();
	string v_str = GetNodeByName(vertex_weights_node, "v")->value();
	int* vcount = UnpackValues<int>(vcount_str, weights_count);
	int v_length = 0;
	for (int i = 0; i < weights_count; i++)
		v_length += vcount[i] * 2;
	int* v_array = UnpackValues<int>(v_str, v_length);
	JointWeight* p_jointWeights = (JointWeight*)malloc(sizeof(JointWeight) * weights_count);

	for (int i = 0, v = 0; i < weights_count; i++)
	{
		int num = vcount[i];
		p_jointWeights[i].m_count = num;
		p_jointWeights[i].m_jointIndices = (byte*)malloc(num);
		p_jointWeights[i].m_weights = (float*)malloc(num * sizeof(float));
		for (int j = 0; j < num; j++)
		{
			p_jointWeights[i].m_jointIndices[j] = m_skeleton.GetJointIndex(joint_source[v_array[weights_offsets[0] + v + j * 2]]);
			p_jointWeights[i].m_weights[j] = weight_source[v_array[weights_offsets[1] + v + j * 2]];
		}
		v += num * 2;
	}

	for (int i = 0; i < joint_source.size(); i++)
	{
		Joint& joint = *m_skeleton.GetJoint(joint_source[i]);
		joint.m_invBindPose = matrix_source[i];
	}
	return p_jointWeights;
}

void CColladaFile::CalculateGlobalMatrix()
{
	for (Joint& joint : m_skeleton.GetJoints())
	{
		Matrix4x4 matj = Matrix4x4::Identity();
		Joint* p = &joint;
		do {
			matj = p->m_localMatrix * matj;
			if (p->m_iParent == 0xFF)
				break;
			p = m_skeleton.GetJoint(p->m_iParent);
		} while (true);
		joint.m_globalMatrix = matj;
	}
}

void CColladaFile::CalculateSkinningMatrix()
{
	Matrix4x4 mat;
	vector<Joint>& joints = m_skeleton.GetJoints();
	for (int i = 0; i < joints.size(); ++i)
	{
		mat.MakeZero();
		mat += joints[i].m_globalMatrix * joints[i].m_invBindPose * m_bindShapeMat;
		m_skeletonPose.m_aGlobalPose[i] = mat;
	}
}

void CColladaFile::ReadMesh(xml_node<>* root, JointWeight* p_jointWeights)
{
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
	m_vertexNum = m_triangleNum * 3;

	int vertIndex = 0, normalIndex = 0, uvIndex = 0; //读取时已到达的索引
	m_vertexArray = (Vector3*)malloc(sizeof(Vector3) * m_vertexNum);
	m_normalArray = (Vector3*)malloc(sizeof(Vector3) * m_vertexNum);
	m_uvArray = (Vector2*)malloc(sizeof(Vector2) * m_vertexNum);
	m_jointWeights = (JointWeight*)malloc(sizeof(JointWeight) * m_vertexNum);

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
				source = GetAttribute(GetNodeByName(mesh, "vertices")->first_node(), "source");
			else if (semantic == "NORMAL")
				index = 1;
			else if (semantic == "TEXCOORD")
				index = 2;
			flags[index] = true;
			offsets[index] = offset;
			sourceIds[index] = source;
		}
		int* indices = UnpackValues<int>(p, (size_t)count * 3 * step);
		for (int i = 0; i < count * 3 * step; i += step)
		{
			int vi = indices[i] + offsets[0];
			m_jointWeights[vertIndex] = p_jointWeights[vi];

			if (flags[0])
				m_vertexArray[vertIndex++] = m_skeletonPose.m_aGlobalPose[ m_jointWeights[vertIndex].m_jointIndices[0]] * Vector4(((Vector3*)source_map[sourceIds[0]].array)[vi]);
			if (flags[1])
				//这里根据下标取normal索引是有问题的，所以按顺序来取
				m_normalArray[normalIndex++] = ((Vector3*)source_map[sourceIds[1]].array)[normalIndex];
			if (flags[2])
				m_uvArray[uvIndex++] = ((Vector2*)source_map[sourceIds[2]].array)[indices[i] + offsets[2]];
		}
		free(indices);
	}
	//for (int i = 0; i < m_vertexNum; i++)
	//{
	//	CDebug:::Log();
	//}
	free(p_jointWeights);
}

void CColladaFile::LoadFromFile(const char* filename)
{
	LoadXmlDocument(filename);
	xml_node<>* root = m_xmlDoc.first_node();
	ReadSkeleton(root);
	JointWeight* p_jointWeights = ReadSkin(root);
	CalculateGlobalMatrix();
	CalculateSkinningMatrix();
	ReadMesh(root, p_jointWeights);

	//上传到缓冲区
	m_buffer.MakeBuffer(m_vertexArray, NULL, m_normalArray, m_uvArray, m_vertexNum);
}