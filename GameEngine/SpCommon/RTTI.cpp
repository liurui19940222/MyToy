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