// EditUser.cpp: 实现文件
//

#include "pch.h"
#include "EduManager.h"
#include "EditUser.h"
#include "afxdialogex.h"
#include "StudentForm.h"
#include "TeacherForm.h"


// EditUser 对话框

IMPLEMENT_DYNAMIC(EditUser, CDialogEx)

EditUser::EditUser(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EDITUSER, pParent)
	, state_value(_T(""))
	, name_value(_T(""))
	, major_value(_T(""))
	, userID_value(_T(""))
	, classID_value(_T(""))
	, password_value(_T(""))
	, moduleName_value(_T(""))
	, score_value(_T(""))
	, userType_value(_T(""))
{

}

EditUser::~EditUser()
{
}

void EditUser::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NAME, name);
	DDX_Control(pDX, IDC_MAJOR, major);
	DDX_Control(pDX, IDC_USERID, userID);
	DDX_Control(pDX, IDC_CLASSID, classID);
	DDX_Control(pDX, IDC_PASSWORD, password);
	DDX_Control(pDX, IDC_MODULENAME, moduleName);
	DDX_Control(pDX, IDC_SCORE, score);
	DDX_Text(pDX, IDC_STATE, state_value);
	DDX_Text(pDX, IDC_NAME, name_value);
	DDX_Text(pDX, IDC_MAJOR, major_value);
	DDX_Text(pDX, IDC_USERID, userID_value);
	DDX_Text(pDX, IDC_CLASSID, classID_value);
	DDX_Text(pDX, IDC_PASSWORD, password_value);
	DDX_Text(pDX, IDC_MODULENAME, moduleName_value);
	DDX_Text(pDX, IDC_SCORE, score_value);
	DDX_Control(pDX, IDC_USERTYPE, userType);
	DDX_Control(pDX, IDC_EDITUSER, B_EditUser);
	DDX_CBString(pDX, IDC_USERTYPE, userType_value);
	DDX_Control(pDX, IDC_HOMEPAGE, homepage);
}


BEGIN_MESSAGE_MAP(EditUser, CDialogEx)
	ON_BN_CLICKED(IDC_EDITMODULE, &EditUser::OnBnClickedEditmodule)
	ON_EN_CHANGE(IDC_NAME, &EditUser::OnEnChangeName)
	ON_EN_CHANGE(IDC_MAJOR, &EditUser::OnEnChangeMajor)
	ON_EN_CHANGE(IDC_USERID, &EditUser::OnEnChangeUserid)
	ON_EN_CHANGE(IDC_CLASSID, &EditUser::OnEnChangeClassid)
	ON_EN_CHANGE(IDC_PASSWORD, &EditUser::OnEnChangePassword)
	ON_EN_CHANGE(IDC_SCORE, &EditUser::OnEnChangeScore)
	ON_BN_CLICKED(IDC_EDITUSER, &EditUser::OnBnClickedEdituser)
	ON_CBN_SELCHANGE(IDC_USERTYPE, &EditUser::OnCbnSelchangeUsertype)
	ON_BN_CLICKED(IDC_INTRO, &EditUser::OnBnClickedIntro)
	ON_BN_CLICKED(IDC_HOMEPAGE, &EditUser::OnBnClickedHomepage)
	ON_COMMAND(ID_MIN, &EditUser::OnMin)
	ON_COMMAND(ID_CLOSE, &EditUser::OnClose)
END_MESSAGE_MAP()


// EditUser 消息处理程序

void EditUser::RefreshWindowMessage()
{
	UpdateData(TRUE);

		if (name.IsWindowEnabled() || userID.IsWindowEnabled() || classID.IsWindowEnabled() || major.IsWindowEnabled() || password.IsWindowEnabled() || score.IsWindowEnabled())
		{
			if (userID_value != L"" && name_value != L"" && password_value != L"" && major_value != L"" && classID_value != L"")
			{
				B_EditUser.EnableWindow(1);
			}
			else
			{
				B_EditUser.EnableWindow(0);
			}
		}
		else
		{
			B_EditUser.EnableWindow(0);
		}
	
}

BOOL EditUser::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	UpdateData(TRUE);

	moduleName.SetReadOnly(1);
	userID.SetReadOnly(1);
	if (!newUser)userType.EnableWindow(0);

	userType.AddString(L"管理员");
	userType.AddString(L"教师");
	userType.AddString(L"学生");

	if (EduManager.userNow->userType == 1)
	{
		if (target.size()!=1||target.at(0)->userType==1||newUser)
		{
			homepage.EnableWindow(0);
		}
	}
	else if (EduManager.userNow->userType == 2)
	{
		name.SetReadOnly(1);
		major.SetReadOnly(1);
		classID.SetReadOnly(1);
		password.SetReadOnly(1);
		homepage.EnableWindow(0);
	}
	else if (EduManager.userNow->userType == 3)
	{
		name.SetReadOnly(1);
		major.SetReadOnly(1);
		classID.SetReadOnly(1);
		score.SetReadOnly(1);
		password.SetReadOnly(1);
		homepage.EnableWindow(0);
	}

	

	if (target.at(0)->ID == EduManager.userNow->ID)
	{
		password.SetReadOnly(1);
	}


	

	name_value = target.at(0)->name;
	userID_value = target.at(0)->ID;
	password_value = target.at(0)->password;
	classID_value = target.at(0)->classID;
	major_value = target.at(0)->major;
	if (target.at(0)->userType == 1)userType_value = L"管理员";
	else if (target.at(0)->userType == 2)userType_value = L"教师";
	else if (target.at(0)->userType == 3)userType_value = L"学生";

	if (target.at(0)->userType != 3)
	{
		major.SetReadOnly(1);
		classID.SetReadOnly(1);
	}

	if (moduleTarget != nullptr)
	{
		CString  Score(L"NULL");
		double sc = moduleTarget->FindStudent(target.at(0)->ID)->score;
		if (sc != -1)Score.Format(L"%.1lf", sc);
		else Score = L"无";
		moduleName_value = moduleTarget->name;
		score_value = Score;
	}
	else score.SetReadOnly(1);

	for (size_t i = 1; i < target.size(); i++)
	{
		if (target.at(i)->userType != 3)
		{
			major.SetReadOnly(1);
			classID.SetReadOnly(1);
		}
		homepage.EnableWindow(0);

		CString userTypeStr(L"NULL");
		if (target.at(i)->userType == 1)userTypeStr = L"管理员";
		else if (target.at(i)->userType == 2)userTypeStr = L"教师";
		else if (target.at(i)->userType == 3)userTypeStr = L"学生";

		name_value = target.at(i)->name == name_value ? target.at(i)->name : L"多种";
		major_value = target.at(i)->major == major_value ? target.at(i)->major : L"多种";
		userID_value = target.at(i)->ID == userID_value ? target.at(i)->ID : L"多种";
		classID_value = target.at(i)->classID == classID_value ? target.at(i)->classID : L"多种";
		password_value = target.at(i)->password == password_value ? target.at(i)->password : L"多种";
		userType_value = userTypeStr == userType_value ? userTypeStr : L"多种";

		if (moduleTarget != nullptr)
		{
			CString  Score(L"NULL");
			double sc = moduleTarget->FindStudent(target.at(i)->ID)->score;
			if (sc != -1)Score.Format(L"%.1lf", sc);
			else Score = L"无";
			score_value = score_value == Score ? Score : L"多种";
		}

		if (target.at(i)->userType != 3)
		{
			major.SetReadOnly(1);
			userID.SetReadOnly(1);
		}
	}

	if (classID_value == L"NULL")classID_value = L"无";
	if (major_value == L"NULL")major_value = L"无";

	if (target.size() != 1)state_value.Format(L"选中的%d个用户的信息", target.size());
	else state_value.Format(L"“%s”用户的信息", name_value);

	UpdateData(FALSE);

	if (userID_value == EduManager.superAdmin.ID)
	{
		name.SetReadOnly(1);
		password.SetReadOnly(1);
		B_EditUser.EnableWindow(0);
	}

	RefreshWindowMessage();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void EditUser::OnBnClickedEditmodule()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}


void EditUser::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	if (!B_EditUser.IsWindowEnabled())return;

	UpdateData(TRUE);
	for (size_t i = 0; i < target.size(); i++)
	{
		if (name_value != L"多种")target.at(i)->name = name_value;
		if (userID_value != L"多种")target.at(i)->ID = userID_value;
		if (major_value != L"多种" && major_value != L"无")target.at(i)->major = major_value;
		if (classID_value != L"多种" && classID_value != L"无")target.at(i)->classID = classID_value;
		if (target.at(i)->major == L"")target.at(i)->major = L"NULL";
		if (target.at(i)->classID == L"")target.at(i)->classID = L"NULL";
		if (password_value != L"多种")target.at(i)->password = password_value;
		if (moduleTarget != nullptr && score_value != L"多种")
		{
			auto q = moduleTarget->FindStudent(userID_value);
			double score;
			if (score_value == L"无" || score_value == L"")score = -1;
			else if (score_value == L"-1")score = 0;
			else
			{
				score = _ttof(score_value);
				if (score > 100)score = 100;
				if (score < 0)score = 0;
			}
			q->score = score;
		}
		if (userType_value == L"管理员")target.at(i)->userType = 1;
		else if (userType_value == L"教师")target.at(i)->userType = 2;
		else if (userType_value == L"学生")target.at(i)->userType = 3;
	}
	EduManager.SaveData(EduManager.basicDataPos);

	CDialogEx::OnOK();
}


void EditUser::OnEnChangeName()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (target.size() != 1)state_value.Format(L"选中的%d个用户的信息", target.size());
	else state_value.Format(L"“%s”用户的信息", name_value);

	UpdateData(FALSE);
}


void EditUser::OnEnChangeMajor()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	RefreshWindowMessage();
}


void EditUser::OnEnChangeUserid()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	RefreshWindowMessage();
}


void EditUser::OnEnChangeClassid()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	RefreshWindowMessage();
}


void EditUser::OnEnChangePassword()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	RefreshWindowMessage();
}


void EditUser::OnEnChangeScore()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	RefreshWindowMessage();
}


void EditUser::OnBnClickedEdituser()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}


void EditUser::OnCbnSelchangeUsertype()
{
	// TODO: 在此添加控件通知处理程序代码
	userType.GetLBText(userType.GetCurSel(), userType_value);
	if (userType_value==L"管理员"||userType_value == L"教师")
	{
		classID_value = L"无";
		major_value = L"无";
		classID.SetReadOnly(1);
		major.SetReadOnly(1);
	}
	else
	{
		classID_value = L"";
		major_value = L"";
		classID.SetReadOnly(0);
		major.SetReadOnly(0);
	}
	UpdateData(FALSE);
}


void EditUser::OnBnClickedIntro()
{
	// TODO: 在此添加控件通知处理程序代码

}


void EditUser::OnBnClickedHomepage()
{
	// TODO: 在此添加控件通知处理程序代码
	auto p = target.at(0);
	if (p->userType == 1)
	{
		EditUser editUser;
		editUser.target.push_back(p);
		editUser.DoModal();
	}
	else if ((p->userType == 2))
	{
		TeacherForm teacherForm;
		teacherForm.target = p;
		teacherForm.DoModal();
	}
	else if (p->userType == 3)
	{
		StudentForm studentForm;
		studentForm.target = p;
		studentForm.DoModal();
	}
}


BOOL EditUser::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	return CDialogEx::PreTranslateMessage(pMsg);
}


void EditUser::OnMin()
{
	// TODO: 在此添加命令处理程序代码
	ShowWindow(SW_SHOWMINIMIZED);
}


void EditUser::OnClose()
{
	// TODO: 在此添加命令处理程序代码
	OnCancel();
}
