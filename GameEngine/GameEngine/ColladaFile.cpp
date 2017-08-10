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

void CColladaFile::LoadFromFile(const char* filename)
{
	int* fff = UnpackValues<int>(string("89 2970 79 90 2971 80 80 2972 81 80 2973 81 81 2974 82 89 2975 79 80 2976 81 79 2977 84 82 2978 83 82 2979 83 81 2980 82 80 2981 81 84 2982 87 83 2983 85 88 2984 86 88 2985 86 87 2986 88 84 2987 87 85 2988 91 84 2989 87 87 2990 88 87 2991 88 86 2992 92 85 2993 91 91 2994 89 92 2995 90 85 2996 91 85 2997 91 86 2998 92 91 2999 89 83 3000 85 82 3001 83 79 3002 84 79 3003 84 88 3004 86 83 3005 85 255 3006 260 265 3007 261 264 3008 262 264 3009 262 256 3010 263 255 3011 260 256 3012 263 257 3013 264 254 3014 265 254 3015 265 255 3016 260 256 3017 263 262 3018 269 263 3019 267 258 3020 266 258 3021 266 259 3022 268 262 3023 269 261 3024 273 262 3025 269 259 3026 268 259 3027 268 260 3028 270 261 3029 273 260 3030 270 267 3031 271 266 3032 272 266 3033 272 261 3034 273 260 3035 270 263 3036 267 254 3037 265 257 3038 264 257 3039 264 258 3040 266 263 3041 267"), 24 * 8);
	for (int i = 0; i < 24 * 8; ++i)
	{
		CDebug::Log("%d", fff[i]);
	}
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
	map<string, SValueArray<float>> source_map;
	vector<xml_node<>*> sources = GetNodesByName(mesh, "source");
	for (auto it = sources.begin(); it != sources.end(); ++it)
	{
		xml_node<>* float_array = GetNodeByName(*it, "float_array");
		int count = GetAttribute<int>(float_array, "count");
		string values = float_array->value();
		float* v3 = UnpackValues<float>(values, count);
		source_map.insert(make_pair(GetAttribute(float_array, "id"), SValueArray<float>{ v3, count }));
	}

	vector<xml_node<>*> triangles = GetNodesByName(mesh, "triangles");
	for (auto it = triangles.begin(); it != triangles.end(); ++it)
	{
		int count = GetAttribute<int>(*it, "count");
	}
}