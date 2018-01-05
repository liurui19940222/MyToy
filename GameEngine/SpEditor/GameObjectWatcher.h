#ifndef _GAMEOBJECT_WATCHER_H_
#define _GAMEOBJECT_WATCHER_H_

#include"Watcher.h"

class CGameObjectWatcher : public CWatcher
{
	int m_instanceId;
	CGUIManager* m_gui;
	CGUICheckBox* m_ckActive;
	CGUITextEdit* m_inName;
	CGUITextEdit* m_inPosX;
	CGUITextEdit* m_inPosY;
	CGUITextEdit* m_inPosZ;
	CGUITextEdit* m_inRotX;
	CGUITextEdit* m_inRotY;
	CGUITextEdit* m_inRotZ;
	CGUITextEdit* m_inScaX;
	CGUITextEdit* m_inScaY;
	CGUITextEdit* m_inScaZ;

	CGUIText* CreateAndPutTextToLayout(CGUIManager& gui, CGUIHorizontalLayout* layout, const wstring& str, int colIndex, EAlignment alignment = EAlignment::LEFT_MIDDLE);
	CGUITextEdit* CreateAndPutEditToLayout(CGUIManager& gui, CGUIHorizontalLayout* layout, int colIndex);
public:
	CGameObjectWatcher(CWatcherWindow* window);
	virtual ~CGameObjectWatcher() override;
	virtual void Show(void* obj) override;
};

#endif