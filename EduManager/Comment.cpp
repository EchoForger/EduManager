// Comment.cpp: 实现文件
//

#include "pch.h"
#include "EduManager.h"
#include "Comment.h"
#include "afxdialogex.h"


// Comment 对话框

IMPLEMENT_DYNAMIC(Comment, CDialogEx)

Comment::Comment(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COMMENT, pParent)
	, moduleName_value(_T(""))
	, teacherName_value(_T(""))
	, rescore_value(_T(""))
	, commennt_value(_T(""))
	, state_value(_T(""))
{

}

Comment::~Comment()
{
}

void Comment::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MODULENAME, moduleName_value);
	DDX_Text(pDX, IDC_TEACHERNAME, teacherName_value);
	DDX_Text(pDX, IDC_RESCORE, rescore_value);
	DDX_Text(pDX, IDC_COMMENT, commennt_value);
	DDX_Text(pDX, IDC_STATE, state_value);
	DDX_Control(pDX, IDC_SUBMIT, B_Submit);
	DDX_Control(pDX, IDC_RESCORE, rescore);
	DDX_Control(pDX, IDC_COMMENT, comment);
}


BEGIN_MESSAGE_MAP(Comment, CDialogEx)
	ON_EN_CHANGE(IDC_RESCORE, &Comment::OnEnChangeRescore)
	ON_EN_CHANGE(IDC_COMMENT, &Comment::OnEnChangeComment)
	ON_BN_CLICKED(IDC_SUBMIT, &Comment::OnBnClickedSubmit)
	ON_COMMAND(ID_SELALL, &Comment::OnSelall)
END_MESSAGE_MAP()


// Comment 消息处理程序


BOOL Comment::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化


	for (size_t i = 0; i < EduManager.ModuleLib.size(); i++)
	{
		bool finded = false;
		for (size_t j = 0; j < EduManager.ModuleLib.at(i).attend.size(); j++)
		{
			if (&EduManager.ModuleLib.at(i).attend.at(j)==target)
			{
				moduleName_value = EduManager.ModuleLib.at(i).name;
				teacherName_value = EduManager.FindUser(EduManager.ModuleLib.at(i).teacherID)->name;
				finded = true;
				break;
			}
		}
		if (finded)break;
	}

	
	if (target->reScore==-1)
	{
		state_value = L"请给" + teacherName_value + L"老师的《" + moduleName_value + L"》课程评价";
		B_Submit.EnableWindow(0);
	}
	else
	{
		rescore_value.Format(L"%.1lf", target->reScore);
		commennt_value = target->comment;
		state_value = L"评价已提交";
	}

	if (EduManager.userNow->userType != 3)
	{
		B_Submit.EnableWindow(0);
		comment.SetReadOnly(1);
		rescore.SetReadOnly(1);
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void Comment::OnEnChangeRescore()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (rescore_value != L"" && commennt_value != L"")
	{
		B_Submit.EnableWindow(1);
	}
	else
	{
		B_Submit.EnableWindow(0);
	}
}


void Comment::OnEnChangeComment()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (rescore_value != L"" && commennt_value != L"")
	{
		B_Submit.EnableWindow(1);
	}
	else
	{
		B_Submit.EnableWindow(0);
	}
}


void Comment::OnBnClickedSubmit()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}


void Comment::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	if (!B_Submit.IsWindowEnabled())return;

	target->reScore = _ttof(rescore_value);
	if (target->reScore < 0)target->reScore = 0;
	else if (target->reScore > 100)target->reScore = 100;
	target->comment = commennt_value;

	EduManager.SaveData(EduManager.basicDataPos);

	CDialogEx::OnOK();
}


void Comment::OnSelall()
{
	// TODO: 在此添加命令处理程序代码
}
