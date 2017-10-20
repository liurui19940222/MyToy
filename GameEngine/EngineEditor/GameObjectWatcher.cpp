#include"GameObjectWatcher.h"
#include"WatcherWindow.h"
#include<GameEngine\Engine.h>
#include<GameEngine\Converter.h>
#include<GameEngine\MessageDef.h>

CGameObjectWatcher::CGameObjectWatcher(CWatcherWindow* window) : CWatcher(window)
{
	CGUIManager& gui = window->GetGUIManager();
	m_gui = &gui;
	//创建第一行的文字框
	m_inName = gui.Create<CGUITextEdit>();
	m_inName->SetEditValueMode(EEditValueMode::Word);
	CGUITexture* texture = gui.Create<CGUITexture>();
	texture->SetTexture(window->m_icons[EIconTypeOnWatcher::GameObject]);
	m_ckActive = gui.Create<CGUICheckBox>();
	m_ckActive->SetIcon(window->m_icons[EIconTypeOnWatcher::Gou]);
	CGUIHorizontalLayout* layout = gui.Create<CGUIHorizontalLayout>();
	layout->SetWeights(vector<float>{ 0.08f, 0.75f, 0.01f, 0.1f, 0.07f }, true);
	gui.PutIntoGrid(0, 0, layout, false);
	layout->AddMember(texture, 0);
	layout->AddMember(m_inName, 1);
	layout->AddMember(m_ckActive, 3);

	//创建Position, Rotation, LocalScale
	CGUIGroup* transformGroup = gui.Create<CGUIGroup>();
	gui.PutIntoGrid(-1, 0, transformGroup, false);
	transformGroup->SetText(L"Transform")->SetCanSelect(false);
	transformGroup->SetIcon(window->m_icons[EIconTypeOnWatcher::Transform]);
	transformGroup->SetOpened(true);
	CGUIHorizontalLayout* posLayout = gui.Create<CGUIHorizontalLayout>();
	CGUIHorizontalLayout* rotLayout = gui.Create<CGUIHorizontalLayout>();
	CGUIHorizontalLayout* scaLayout = gui.Create<CGUIHorizontalLayout>();
	vector<float> layoutWeights{ 0.02f, 0.25f, 0.05f, 0.2f, 0.05f, 0.2f, 0.05f, 0.2f, 0.07f };
	posLayout->SetWeights(layoutWeights, true);
	rotLayout->SetWeights(layoutWeights, true);
	scaLayout->SetWeights(layoutWeights, true);
	transformGroup->AddMember(scaLayout);
	transformGroup->AddMember(rotLayout);
	transformGroup->AddMember(posLayout);
	CreateAndPutTextToLayout(gui, posLayout, L"Position", 1);
	CreateAndPutTextToLayout(gui, rotLayout, L"Rotation", 1);
	CreateAndPutTextToLayout(gui, scaLayout, L"Scale", 1);
	CreateAndPutTextToLayout(gui, posLayout, L"X", 2, EAlignment::CENTER_MIDDLE);
	CreateAndPutTextToLayout(gui, rotLayout, L"X", 2, EAlignment::CENTER_MIDDLE);
	CreateAndPutTextToLayout(gui, scaLayout, L"X", 2, EAlignment::CENTER_MIDDLE);
	CreateAndPutTextToLayout(gui, posLayout, L"Y", 4, EAlignment::CENTER_MIDDLE);
	CreateAndPutTextToLayout(gui, rotLayout, L"Y", 4, EAlignment::CENTER_MIDDLE);
	CreateAndPutTextToLayout(gui, scaLayout, L"Y", 4, EAlignment::CENTER_MIDDLE);
	CreateAndPutTextToLayout(gui, posLayout, L"Z", 6, EAlignment::CENTER_MIDDLE);
	CreateAndPutTextToLayout(gui, rotLayout, L"Z", 6, EAlignment::CENTER_MIDDLE);
	CreateAndPutTextToLayout(gui, scaLayout, L"Z", 6, EAlignment::CENTER_MIDDLE);
	m_inPosX = CreateAndPutEditToLayout(gui, posLayout, 3);
	m_inRotX = CreateAndPutEditToLayout(gui, rotLayout, 3);
	m_inScaX = CreateAndPutEditToLayout(gui, scaLayout, 3);
	m_inPosY = CreateAndPutEditToLayout(gui, posLayout, 5);
	m_inRotY = CreateAndPutEditToLayout(gui, rotLayout, 5);
	m_inScaY = CreateAndPutEditToLayout(gui, scaLayout, 5);
	m_inPosZ = CreateAndPutEditToLayout(gui, posLayout, 7);
	m_inRotZ = CreateAndPutEditToLayout(gui, rotLayout, 7);
	m_inScaZ = CreateAndPutEditToLayout(gui, scaLayout, 7);
}

CGameObjectWatcher::~CGameObjectWatcher()
{
	m_gui->DestroyAll();
}

CGUIText* CGameObjectWatcher::CreateAndPutTextToLayout(CGUIManager& gui, CGUIHorizontalLayout* layout, const wstring& str, int colIndex, EAlignment alignment)
{
	CGUIText* text = gui.Create<CGUIText>();
	layout->AddMember(text->SetText(str)->SetAlignment(alignment), colIndex);
	return text;
}

CGUITextEdit* CGameObjectWatcher::CreateAndPutEditToLayout(CGUIManager& gui, CGUIHorizontalLayout* layout, int colIndex)
{
	CGUITextEdit* edit = gui.Create<CGUITextEdit>();
	layout->AddMember(edit->SetEditValueMode(EEditValueMode::Number), colIndex);
	return edit;
}

void CGameObjectWatcher::Show(void* obj)
{
	m_instanceId = (int)obj;
	CGameObject* go = _Maker->FindGameObject(m_instanceId);
	if (go == NULL) return;

	m_inName->SetValue(go->GetName());
	m_inPosX->SetValueWithNumber(go->GetLocalPosition().x);
	m_inPosY->SetValueWithNumber(go->GetLocalPosition().y);
	m_inPosZ->SetValueWithNumber(go->GetLocalPosition().z);
	m_inRotX->SetValueWithNumber(go->GetLocalEulerAngles().x);
	m_inRotY->SetValueWithNumber(go->GetLocalEulerAngles().y);
	m_inRotZ->SetValueWithNumber(go->GetLocalEulerAngles().z);
	m_inScaX->SetValueWithNumber(go->GetLocalScale().x);
	m_inScaY->SetValueWithNumber(go->GetLocalScale().y);
	m_inScaZ->SetValueWithNumber(go->GetLocalScale().z);
	m_ckActive->SetSelected(go->IsActive());

	m_inName->SetOnSubmitListener([this](wstring value) {
		CGameObject* go = _Maker->FindGameObject(m_instanceId);
		if (go == NULL) return;
		go->SetName(value);
		CMessageCenter::Send(SMessage{ MSG_ON_GMAEOBJECT_NAME_CHANGED, this, go });
	});

	m_ckActive->SetOnValueChangedListener([this](bool isSelected) {
		CGameObject* go = _Maker->FindGameObject(m_instanceId);
		if (go == NULL) return;
		go->SetActive(isSelected);
		CMessageCenter::Send(SMessage{ MSG_ON_GAMEOBJECT_ACTIVE, this, go });
	});

	m_inPosX->SetOnSubmitListener([this](wstring value) {
		CGameObject* go = _Maker->FindGameObject(m_instanceId);
		if (go == NULL) return;
		float v = CConverter::ToValue<float>(value);
		Vector3 pos = go->GetLocalPosition();
		pos.x = v;
		go->SetLocalPosition(pos);
	});

	m_inPosY->SetOnSubmitListener([this](wstring value) {
		CGameObject* go = _Maker->FindGameObject(m_instanceId);
		if (go == NULL) return;
		float v = CConverter::ToValue<float>(value);
		Vector3 pos = go->GetLocalPosition();
		pos.y = v;
		go->SetLocalPosition(pos);
	});

	m_inPosZ->SetOnSubmitListener([this](wstring value) {
		CGameObject* go = _Maker->FindGameObject(m_instanceId);
		if (go == NULL) return;
		float v = CConverter::ToValue<float>(value);
		Vector3 pos = go->GetLocalPosition();
		pos.z = v;
		go->SetLocalPosition(pos);
	});

	m_inRotX->SetOnSubmitListener([this](wstring value) {
		CGameObject* go = _Maker->FindGameObject(m_instanceId);
		if (go == NULL) return;
		float v = CConverter::ToValue<float>(value);
		Vector3 euler = go->GetLocalEulerAngles();
		euler.x = v;
		go->SetLocalEulerAngles(euler);
	});

	m_inRotY->SetOnSubmitListener([this](wstring value) {
		CGameObject* go = _Maker->FindGameObject(m_instanceId);
		if (go == NULL) return;
		float v = CConverter::ToValue<float>(value);
		Vector3 euler = go->GetLocalEulerAngles();
		euler.y = v;
		go->SetLocalEulerAngles(euler);
	});

	m_inRotZ->SetOnSubmitListener([this](wstring value) {
		CGameObject* go = _Maker->FindGameObject(m_instanceId);
		if (go == NULL) return;
		float v = CConverter::ToValue<float>(value);
		Vector3 euler = go->GetLocalEulerAngles();
		euler.z = v;
		go->SetLocalEulerAngles(euler);
	});

	m_inScaX->SetOnSubmitListener([this](wstring value) {
		CGameObject* go = _Maker->FindGameObject(m_instanceId);
		if (go == NULL) return;
		float v = CConverter::ToValue<float>(value);
		Vector3 scale = go->GetLocalScale();
		scale.x = v;
		go->SetLocalScale(scale);
	});

	m_inScaY->SetOnSubmitListener([this](wstring value) {
		CGameObject* go = _Maker->FindGameObject(m_instanceId);
		if (go == NULL) return;
		float v = CConverter::ToValue<float>(value);
		Vector3 scale = go->GetLocalScale();
		scale.y = v;
		go->SetLocalScale(scale);
	});

	m_inScaZ->SetOnSubmitListener([this](wstring value) {
		CGameObject* go = _Maker->FindGameObject(m_instanceId);
		if (go == NULL) return;
		float v = CConverter::ToValue<float>(value);
		Vector3 scale = go->GetLocalScale();
		scale.z = v;
		go->SetLocalScale(scale);
	});
}