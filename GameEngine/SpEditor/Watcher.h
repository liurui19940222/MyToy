#ifndef _WATCHER_H_
#define _WATCHER_H_

#include"GUIManager.h"
#include"GUIText.h"
#include"GUITexture.h"
#include"GUIElement.h"
#include"GUIGroup.h"
#include"GUITextEdit.h"
#include"GUIHorizontalLayout.h"
#include"GUICheckBox.h"
#include<SpGameEngine\RawRenderer.h>

class CWatcherWindow;

class CWatcher
{
public:
	CWatcher(CWatcherWindow* window);
	virtual ~CWatcher();
	virtual void Show(void* obj) = 0;
};

#endif