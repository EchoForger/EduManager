
// EduManagerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "EduManager.h"
#include "Login.h"
#include "afxdialogex.h"
#include "StudentForm.h"
#include "TeacherForm.h"
#include <algorithm>
#include "AdminForm.h"
#include "locale.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框





bool comp(User* a, User* b) 
{
	return a->lastLogin > b->lastLogin;
}

void Login::UpdateRecent()
{
	username.ResetContent();

	for (size_t i = 0; i < EduManager.UserLib.size(); i++)
	{
		if (EduManager.UserLib.at(i).remUsn)
		{
			recentUser.push_back(&(EduManager.UserLib.at(i)));
		}
	}
	if (recentUser.empty())
	{
		return;
	}
	sort(recentUser.begin(), recentUser.end(),comp);
	for (size_t i = 0; i < recentUser.size(); i++)
	{
		username.AddString(recentUser.at(i)->ID);
	}
	username_value = recentUser.at(0)->ID;
	if (recentUser.at(0)->remPsw)
	{
		password_value = recentUser.at(0)->password;
		remPsw_value = true;
		B_Login.EnableWindow(1);
	}
	else
	{
		password_value = L"";
		remPsw_value = false;
		B_Login.EnableWindow(0);
	}
	UpdateData(FALSE);
	if (username.GetCount()!=0)
	{
		username.AddString(L"清空最近登录记录");
	}
}

// Login 对话框



Login::Login(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOGIN, pParent)
	, password_value(_T(""))
	, remPsw_value(FALSE)
	, username_value(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Login::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PASSWORD, password_value);
	DDX_Check(pDX, IDC_REMPSW, remPsw_value);
	//  DDX_Control(pDX, IDC_USERNAME, username_value);
	DDX_Text(pDX, IDC_USERNAME, username_value);
	DDX_Control(pDX, IDC_USERNAME, username);
	DDX_Control(pDX, IDC_PASSWORD, password);
	DDX_Control(pDX, IDC_LOGIN, B_Login);
}

BEGIN_MESSAGE_MAP(Login, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_PASSWORD, &Login::OnEnChangePassword)
	ON_BN_CLICKED(IDC_LOGIN, &Login::OnBnClickedLogin)
	ON_CBN_EDITCHANGE(IDC_USERNAME, &Login::OnEditchangeUsername)
	ON_CBN_SELCHANGE(IDC_USERNAME, &Login::OnCbnSelchangeUsername)
//	ON_WM_GETMINMAXINFO()
ON_BN_CLICKED(IDC_REMPSW, &Login::OnBnClickedRempsw)
//ON_NOTIFY(BCN_DROPDOWN, IDC_SHOWPSW, &Login::OnDropdownShowpsw)
//ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_SHOWPSW, &Login::OnHotitemchangeShowpsw)
//ON_BN_CLICKED(IDC_SHOWPSW, &Login::OnBnClickedShowpsw)
END_MESSAGE_MAP()


// Login 消息处理程序

BOOL Login::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	EduManager.superAdmin.ID = L"superadmin";
	EduManager.superAdmin.password = L"superadmin";
	EduManager.superAdmin.name = L"超级管理员";
	EduManager.superAdmin.userType = 1;
	EduManager.superAdmin.lastLogin = 0;
	EduManager.superAdmin.classID = L"NULL";
	EduManager.superAdmin.major = L"NULL";

	B_Login.EnableWindow(0);

	EduManager.ReadData(EduManager.basicDataPos);

	//下面的代码用于随机生成
	
	//EduManager.AddRandomStuff(1, 20, 500, 50);EduManager.SaveData(EduManager.basicDataPos);

	UpdateRecent();

	//下面的代码用于自动登录superadmin

	//username_value = EduManager.superAdmin.ID;
	//password_value = EduManager.superAdmin.password;
	//B_Login.EnableWindow(1);
	//UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Login::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{

	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Login::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Login::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Login::OnEnChangePassword()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (username_value == "" || password_value == "") B_Login.EnableWindow(0);
	else B_Login.EnableWindow(1);
}


void Login::OnBnClickedLogin()
{
	// TODO: 在此添加控件通知处理程序代码

	OnOK();

}

bool contain(CString a,CString b) 
{
	if (a.GetLength()>b.GetLength()||a.GetLength()==0)return false;
	for (size_t i = 0; i < (size_t)a.GetLength(); i++)
	{
		if (a[i] != b[i])return false;
	}
	return true;
}

void Login::OnEditchangeUsername()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if (username_value.GetLength() >= 11)
	{
		username_value = username_value.Left(11);
		UpdateData(FALSE);
		username.SetEditSel(11, 11);
		sel = false;
	}

	if (m_bBackSpace)
	{
		if (sel)
		{
			username_value = username_value.Left(username_value.GetLength() - 1);
			UpdateData(FALSE);
			username.SetEditSel(username_value.GetLength(), username_value.GetLength());
		}
	}

	UpdateData(TRUE);

	sel = false;
	for (size_t i = 0; i < recentUser.size(); i++)
	{
		if (contain(username_value,recentUser.at(i)->ID))
		{
			auto start = username_value.GetLength();
			username_value = recentUser.at(i)->ID;
			UpdateData(FALSE);
			username.SetEditSel(start, username_value.GetLength());
			sel = true;
			break;
		}
	}

	UpdateData(TRUE);
	auto p = EduManager.FindUser(username_value);
	if (p != nullptr && p->remPsw)
	{
		password_value = p->password;
		remPsw_value = true;
	}
	else
	{
		password_value = L"";
		remPsw_value = false;
	}

	if (username_value != L"" && password_value != L"")
	{
		B_Login.EnableWindow(1);
	}
	else
		B_Login.EnableWindow(0);

	UpdateData(FALSE);

	return;
}


void Login::OnCbnSelchangeUsername()
{
	// TODO: 在此添加控件通知处理程序代码

	if (username.GetCurSel()==username.GetCount()-1)	//清空最近登录记录
	{
		username.ResetContent();
		for (size_t i = 0; i < EduManager.UserLib.size(); i++)
		{
			EduManager.UserLib.at(i).remUsn = EduManager.UserLib.at(i).remPsw = false;
		}
		password_value = "";
		remPsw_value = false;
		B_Login.EnableWindow(0);
		UpdateData(FALSE);
		recentUser.clear();
		return;
	}

	username.GetLBText(username.GetCurSel(), username_value);

	for (size_t i = 0; i < EduManager.UserLib.size(); i++)
	{
		if (EduManager.UserLib.at(i).ID == username_value)
		{
			if (EduManager.UserLib.at(i).remPsw)
			{
				password_value = EduManager.UserLib.at(i).password;
				remPsw_value = true;
				UpdateData(FALSE);
				if (username_value == "" || password_value == "") B_Login.EnableWindow(0);
				else B_Login.EnableWindow(1);
				return;
			}
			break;
		}
	}
	password_value = "";
	remPsw_value = false;

	if (username_value == "" || password_value == "") B_Login.EnableWindow(0);
	else B_Login.EnableWindow(1);
	UpdateData(FALSE);


}

BOOL Login::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialogEx::PreCreateWindow(cs);
	
}

void Login::OnBnClickedRempsw()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
}

void Login::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	if (!B_Login.IsWindowEnabled())return;
	UpdateData(TRUE);
	

	if (username_value == EduManager.superAdmin.ID && password_value == EduManager.superAdmin.password)
	{
		EduManager.userNow = &(EduManager.superAdmin);
	}
	for (size_t i = 0; i < EduManager.UserLib.size(); i++)
	{
		
		if (EduManager.UserLib.at(i).ID == username_value && EduManager.UserLib.at(i).password == password_value&& EduManager.userNow==nullptr)
		{
			EduManager.userNow = &(EduManager.UserLib.at(i));
			break;
		}
	}

	if (EduManager.userNow != nullptr)
	{
		EduManager.userNow->remUsn = true;
		if (remPsw_value)
		{
			EduManager.userNow->remPsw = true;
		}
		else
		{
			EduManager.userNow->remPsw = false;
		}
		EduManager.userNow->lastLogin = EduManager.GetSecondsFrom1970();
		UpdateRecent();
		username_value = "";
		password_value = "";
		remPsw_value = false;
		UpdateData(FALSE);

		OnCancel();

		if ((EduManager.userNow->userType) == 1)
		{
			AdminForm adminForm;
			adminForm.DoModal();
		}
		else if ((EduManager.userNow->userType) == 2)
		{
			TeacherForm teacherForm;
			teacherForm.target = EduManager.userNow;
			teacherForm.DoModal();
		}
		else if ((EduManager.userNow->userType) == 3)
		{
			StudentForm studentForm;
			studentForm.target = EduManager.userNow;
			studentForm.DoModal();
		}

		EduManager.SaveData(EduManager.basicDataPos);

		UpdateRecent();
		EduManager.userNow = nullptr;
	}
	else
	{
		MessageBox(L"账号或密码错误");
		B_Login.EnableWindow(0);
		password_value = "";
		remPsw_value = false;
		UpdateData(FALSE);
	}

	//CDialogEx::OnOK();
}


void Login::OnBnClickedRandommodule()
{
	// TODO: 在此添加控件通知处理程序代码
	bool hasTeacher = false;
	for (auto p = EduManager.UserLib.begin(); p != EduManager.UserLib.end(); p++)
	{
		if (p->userType == 2)
		{
			hasTeacher = true;
			break;
		}
	}
	if (hasTeacher)
	{
		Module m = EduManager.RandomModule();
		EduManager.ModuleLib.push_back(m);
		UpdateRecent();
	}
	else
	{
		MessageBox(L"请先添加至少一个教师");
	}
}

void Login::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	EduManager.SaveData(EduManager.basicDataPos);
	CDialogEx::OnCancel();
}


BOOL Login::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_BACK:
			m_bBackSpace = TRUE;
			break;
		default:
			m_bBackSpace = FALSE;
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
