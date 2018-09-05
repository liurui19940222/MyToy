#include "RTTI.h"

void RTTI::RegisterCreatetor(const char* clsName, ClassCreateHandler handler)
{
	string name(clsName), fullClassName, briefClassName;
	fullClassName = name.substr(6, name.length() - 6);
	int index = -1;
	if ((index = fullClassName.find("::")) >= 0)
		briefClassName = fullClassName.substr(index + 2, fullClassName.length() - index - 2);

	auto func = [&](const string& s, ClassCreateHandler h) {
		if (getMap().find(s) != getMap().end())
			getMap()[s] = h;
		else
			getMap().insert(make_pair(s, h));
	};

	func(fullClassName, handler);
	if (briefClassName.length() > 0 && briefClassName != fullClassName)
		func(briefClassName, handler);
}


vector<Property> Metadata::GetAllProperties() const
{
	list<const Metadata*> metas;
	const Metadata* meta = this;
	while (meta)
	{
		metas.push_front(meta);
		meta = meta->GetParentMetaData();
	}
	vector<Property> props;
	std::for_each(metas.begin(), metas.end(), [&](const Metadata* m) {
		props.insert(props.end(), m->GetProperties()->begin(), m->GetProperties()->end());
	});
	return std::move(props);
}