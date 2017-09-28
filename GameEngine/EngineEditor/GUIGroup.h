#ifndef _GUI_GROUP_H_
#define _GUI_GROUP_H_

#include<vector>
#include"GUIElement.h"
#include"GUIText.h"
#include"GUITexture.h"
#include"GUIMesh.h"

using namespace std;

#define ARROW_SIZE 10
#define DEPTH_STEP 15

class CGUIGroup : public CGUIElement
{
private:
	vector<CGUIElement*> m_members;
	bool m_hasIcon;
	bool m_opened;
	bool m_selected;
	CGUIText* m_guiText;
	CGUITexture* m_guiTexture;
	CGUIMesh* m_guiMesh;
	Vector3 m_arrowPos;
	SRect2D m_arrowRect;

	void UpdateVisible();
	int GetBottomMostRowIndex(CGUIGroup* group);

public:
	CGUIGroup();
	virtual void OnStart() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;
	virtual void OnLayoutChanged(const SRect2D& rect) override;
	virtual void OnVisibleChanged(bool visible) override;
	virtual void OnStateChanged() override;
	virtual bool Overlay(Vector2 pos) override;

	bool IsOpened();
	bool IsSelected();

	void AddMember(CGUIElement* element);
	void RemoveMember(CGUIElement* element);
	void RemoveInParent();
	CGUIGroup* SetOpened(bool opened);
	CGUIGroup* SetSelected(bool selected);
	CGUIGroup* SetIcon(CTexture2D* texture);
	CGUIGroup* SetText(const wstring& text);

	CGUIElement* operator[](int index);
	CGUIElement* GetMember(int index);
	int GetBottomMostRowIndex();
};

#endif