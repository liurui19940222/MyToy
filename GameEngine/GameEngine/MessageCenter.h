#ifndef _MESSAGE_CENTER_H_
#define _MESSAGE_CENTER_H_

#include<string>
#include<vector>
#include<map>
#include"ApiDefine.h"

using namespace std;

struct ENGINE_API SMessage
{
	string m_msgType;
	void* m_sender;
	void* m_body;
};

class ENGINE_API IReceiver
{
public:
	virtual void OnReceiveMsg(SMessage& message) = 0;
};

class ENGINE_API CMessageCenter
{
	static map<string, vector<IReceiver*>> m_map;
public:
	static void Register(const string& msg_type, IReceiver* receiver);
	static void Unregister(const string& msg_type, IReceiver* receiver);
	static void Send(SMessage& message);
};

#endif