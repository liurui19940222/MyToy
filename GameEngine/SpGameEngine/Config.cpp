#include"Config.h"
#include "SpCommon\Debug.h"

void CConfig::OnInitialize()
{
	ifstream is("Config/engine.xml");
	stringstream ss;
	ss << is.rdbuf();
	is.close();
	size_t size = ss.str().size();
	char* buf = (char*)malloc(size + 1);
	ss.read(buf, size);
	buf[size] = '\0';
	xml_document<> doc;
	doc.parse<0>(buf);
	//free(buf);
	xml_node<>* root = doc.first_node();
	for (xml_node<> *it = root->first_node();
		it; it = it->next_sibling())
	{
		map<string, string> map;
		for (xml_attribute<> *attr = it->first_attribute();
			attr; attr = attr->next_attribute())
		{
			SetPair(map, string(attr->name()), string(attr->value()));
		}
		SetPair(m_configs, string(it->name()), map);
	}
}

string CConfig::GetString(const string group, const string key)
{
	auto it_configs = m_configs.find(group);
	if (it_configs == m_configs.end())
		return string();
	auto it = it_configs->second.find(key);
	if (it == it_configs->second.end())
		return string();
	return it->second;
}