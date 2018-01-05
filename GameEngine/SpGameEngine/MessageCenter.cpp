#include "MessageCenter.h"

map<string, vector<IReceiver*>> CMessageCenter::m_map;

void CMessageCenter::Register(const string& msg_type, IReceiver* receiver)
{
	auto it = m_map.find(msg_type);
	if (it == m_map.end())
		m_map.insert(make_pair(msg_type, vector<IReceiver*>{ receiver }));
	else
		it->second.push_back(receiver);
}

void CMessageCenter::Unregister(const string& msg_type, IReceiver* receiver)
{
	auto it = m_map.find(msg_type);
	if (it != m_map.end())
	{
		vector<IReceiver*>& list = m_map[msg_type];
		for (int i = 0; i < list.size(); ++i)
		{
			if (list[i] == receiver)
			{
				list.erase(list.begin() + i);
				return;
			}
		}
	}
}

void CMessageCenter::Send(SMessage& message)
{
	auto it = m_map.find(message.m_msgType);
	if (it != m_map.end())
	{
		vector<IReceiver*>& list = m_map[message.m_msgType];
		for (IReceiver* receiver : list)
		{
			if (receiver) receiver->OnReceiveMsg(message);
		}
	}
}