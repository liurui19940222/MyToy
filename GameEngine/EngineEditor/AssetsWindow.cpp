#include"AssetsWindow.h"
#include<GameEngine\Config.h>
#include<GameEngine\DirectoryInfo.h>

map<EIconTypeOnAssets, CTexture*> CAssetsWindow::m_icons;

CAssetsWindow::CAssetsWindow(wchar_t* class_name, HINSTANCE instance, HWND parent, int width, int height, DWORD dword) : CChannel()
{
	m_caption = L"Assets";
	Create(class_name, instance, parent, width, height, dword);
}

EWindowType CAssetsWindow::GetType()
{
	return EWindowType::Assets;
}

DWORD CAssetsWindow::GetStyle()
{
	return WS_CHILD;
}

void CAssetsWindow::OnCreate()
{
	CChannel::OnCreate();
	m_gui.SetGridRowHeight(20);
	m_gui.SetGridRowCount(20);
	m_tree.SetManager(&m_gui);

	m_icons.insert(make_pair(EIconTypeOnAssets::Other, CTexture2D::Create("textures/gui/file_other_ico.png")->SetFilterMode(ETexFilterMode::Point)));
	m_icons.insert(make_pair(EIconTypeOnAssets::Folder, CTexture2D::Create("textures/gui/ico_folder.png")->SetFilterMode(ETexFilterMode::Point)));
	m_icons.insert(make_pair(EIconTypeOnAssets::Texture, CTexture2D::Create("textures/gui/file_other_ico.png")->SetFilterMode(ETexFilterMode::Point)));

	RefreshAssetsList();
}

void CAssetsWindow::ForeachAssets(const wstring& path, CGUITreeNode<wstring>* node)
{
	if (CFileSystemInfo::IsDirectory(path))
	{
		CDirectoryInfo info(path);
		CGUITreeNode<wstring>* curNode = NULL;
		wstring dirname = info.GetFullPath();
		if (info.GetFullName() != L"Assets")
		{
			if (node)
			{
				curNode = node->AddNode(info.GetFullName(), info.GetFullName());
			}
			else
			{
				curNode = m_tree.AddNode(info.GetFullName(), info.GetFullName());
			}
			curNode->group()->SetIcon(m_icons[EIconTypeOnAssets::Folder]);
		}
		
		for (CDirectoryInfo_PTR dir : info.GetDirectorys())
		{
			ForeachAssets(dir.get()->GetFullPath(), curNode);
		}
		for (CFileInfo_PTR file : info.GetFiles())
		{
			CGUITreeNode<wstring>* fileNode = curNode->AddNode(file.get()->GetName(), file.get()->GetFullPath());
			wstring extension = file.get()->GetExtension();
			if (extension == L"png" || extension == L"jpg" || extension == L"bmp" || extension == L"tga")
			{
				fileNode->group()->SetIcon(CTexture2D::Create(CConverter::WStringToString(file.get()->GetFullPath()).c_str()), true);
			}
			else fileNode->group()->SetIcon(m_icons[EIconTypeOnAssets::Other]);
		}
	}
}

void CAssetsWindow::RefreshAssetsList()
{
	m_tree.Clear();
	wstring assetsPath = CConverter::StringToWString(_Config->GetString("project", "path")) + L"//Assets";
	ForeachAssets(assetsPath, NULL);
}

void CAssetsWindow::OnDraw()
{
	CChannel::OnDraw();
}

void CAssetsWindow::OnClose()
{
	CChannel::OnClose();
}