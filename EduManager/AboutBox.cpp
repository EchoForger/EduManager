// AboutBox.cpp: 实现文件
//

#include "pch.h"
#include "EduManager.h"
#include "AboutBox.h"
#include "afxdialogex.h"


// AboutBox 对话框

IMPLEMENT_DYNAMIC(AboutBox, CDialogEx)

AboutBox::AboutBox(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ABOUTBOX, pParent)
{

}

AboutBox::~AboutBox()
{
}

void AboutBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AboutBox, CDialogEx)
	ON_COMMAND(ID_ADD, &AboutBox::OnAdd)
	ON_BN_CLICKED(IDC_BUTTON1, &AboutBox::OnBnClickedButton1)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// AboutBox 消息处理程序


void AboutBox::OnAdd()
{
	// TODO: 在此添加命令处理程序代码
}


void AboutBox::OnStnClickedState()
{
	// TODO: 在此添加控件通知处理程序代码
}


void AboutBox::OnStnClickedState2()
{
	// TODO: 在此添加控件通知处理程序代码
}


void AboutBox::OnStnClickedWebsite()
{
	// TODO: 在此添加控件通知处理程序代码
	MessageBox(L"");
}


void AboutBox::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(0, NULL, _T("www.wildptr.cn"), NULL, NULL, SW_NORMAL);
}


HBRUSH AboutBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	if (pWnd->GetDlgCtrlID() == IDC_APPNAME)
	{
		pDC->SelectObject(&m_font);
	}
	return hbr;
}


BOOL AboutBox::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_font.CreatePointFont(130, L"微软雅黑");

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
