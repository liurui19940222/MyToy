#pragma once

#include<fstream>
#include<sstream>
#include"Object.h"
#include"FileInfo.h"

using namespace std;
BEGIN_NAMESPACE_ENGINE;

class AssetUtility
{
public:
	template<typename T>
	static bool Load(T* obj, const string& path)
	{
		ifstream is(path.c_str());
		if (!is.is_open())
			return false;
		ostringstream os;
		os << is.rdbuf();

		SerilizeHelper::Deserilize(obj, os.str());
		obj->m_Reference = path;
		return true;
	}

	template<typename T>
	static bool Save(T* obj, const string& path)
	{
		obj->m_Reference = path;
		string json = SerilizeHelper::Serilize(obj);
		ofstream os(path.c_str());
		if (!os.is_open())
		{
			obj->m_Reference.clear();
			return false;
		}
		os.write(json.c_str(), json.size());
		os.close();
		return true;
	}

	inline static bool CheckReference(const Object* obj) { return FileInfo::IsExists(obj->m_Reference); }
};

END_NAMESPACE_ENGINE;

