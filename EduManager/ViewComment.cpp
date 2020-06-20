// ViewComment.cpp: 实现文件
//

#include "pch.h"
#include "EduManager.h"
#include "ViewComment.h"
#include "afxdialogex.h"
#include <set>
#include "EditUser.h"
#include "Comment.h"


// ViewComment 对话框

IMPLEMENT_DYNAMIC(ViewComment, CDialogEx)


ViewComment::ViewComment(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VIEWCOMMENT, pParent)
	, searchText_value(_T(""))
	, state_value(_T(""))
{

}


ViewComment::~ViewComment()
{
}


void ViewComment::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SEARCHTEXT, searchText_value);
	DDX_Control(pDX, IDC_ADDSTUDENT, B_ViewComment);
	DDX_Control(pDX, IDC_LIST, List);
	DDX_Text(pDX, IDC_STATE, state_value);
	DDX_Control(pDX, IDC_INTRO, Intro);
	DDX_Control(pDX, IDC_EXPORTDATA, exportData);
}


BEGIN_MESSAGE_MAP(ViewComment, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, &ViewComment::OnLvnItemchangedList)
	ON_BN_CLICKED(IDC_ADDSTUDENT, &ViewComment::OnBnClickedAddmodule)
	ON_EN_CHANGE(IDC_SEARCHTEXT, &ViewComment::OnEnChangeSearchtext)
	ON_BN_CLICKED(IDC_ATTENDABLE, &ViewComment::OnBnClickedAttendable)
	ON_BN_CLICKED(IDC_COMPULSORY, &ViewComment::OnBnClickedCompulsory)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST, &ViewComment::OnColumnclickList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &ViewComment::OnDblclkList)
	ON_BN_CLICKED(IDC_INTRO, &ViewComment::OnBnClickedIntro)
	ON_COMMAND(ID_SELALL, &ViewComment::OnSelall)
	ON_BN_CLICKED(IDC_EXPORTDATA, &ViewComment::OnBnClickedExportdata)
	ON_COMMAND(ID_EXPORT, &ViewComment::OnExport)
	ON_COMMAND(ID_MIN, &ViewComment::OnMin)
	ON_COMMAND(ID_CLOSE, &ViewComment::OnClose)
	ON_COMMAND(ID_INTRO, &ViewComment::OnIntro)
END_MESSAGE_MAP()


// ViewComment 消息处理程序


void ViewComment::OnLvnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	checked.clear();
	for (size_t i = 0; i < (size_t)(List.GetItemCount()); i++)
	{
		if (List.GetCheck(i))
		{
			checked.push_back(i);
		}
	}
	if (checked.size() == 1)
	{
		Intro.EnableWindow(1);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_ENABLED);
	}
	else
	{
		Intro.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_DISABLED);
	}

	*pResult = 0;
}


BOOL ViewComment::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	hAccel = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ADMINACCELERATOR));
	GetMenu()->GetSubMenu(0)->DeleteMenu(ID_OPEN, MF_BYCOMMAND | MF_DISABLED);
	GetMenu()->GetSubMenu(0)->DeleteMenu(ID_SAVEAS, MF_BYCOMMAND | MF_DISABLED);
	GetMenu()->GetSubMenu(0)->DeleteMenu(ID_INPORT, MF_BYCOMMAND | MF_DISABLED);
	GetMenu()->GetSubMenu(1)->DeleteMenu(ID_ADD, MF_BYCOMMAND | MF_DISABLED);
	GetMenu()->GetSubMenu(1)->DeleteMenu(ID_DELETE, MF_BYCOMMAND | MF_DISABLED);

	Intro.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_DISABLED);
	List.SetExtendedStyle(List.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);

	List.InsertColumn(0, L"选择", LVCFMT_CENTER, (int)(0.4 * EduManager.defaultWidth));
	List.InsertColumn(1, L"评分", LVCFMT_LEFT, (int)(0.4 * EduManager.defaultWidth));
	List.InsertColumn(2, L"评价", LVCFMT_LEFT, (int)(3.4 * EduManager.defaultWidth));


	for (size_t i = 0; i < target->attend.size(); i++)
	{
		if (target->attend.at(i).reScore != -1)
		{
			ListVector.push_back(&(target->attend.at(i)));
			screened.push_back(screened.size());
		}
	}
	RefreshWindowMessage();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void ViewComment::RefreshWindowMessage() {

	List.DeleteAllItems();

	totalReScore = 0;

	for (auto p = screened.begin();p != screened.end();p++)
	{
		CString reScore(L"NULL");

		reScore.Format(L"%.1lf", ListVector.at(*p)->reScore);

		List.InsertItem(0, L"");
		List.SetItemText(0, 1, reScore);
		List.SetItemText(0, 2, ListVector.at(*p)->comment);

		totalReScore += ListVector.at(*p)->reScore;
	}

	if (ListVector.size())
	{
		state_value.Format(L"已收到评价：%d    综合评分：%.1lf", ListVector.size(), totalReScore / ListVector.size());
		exportData.EnableWindow(1);GetMenu()->GetSubMenu(0)->EnableMenuItem(ID_EXPORT, MF_BYCOMMAND | MF_ENABLED);
	}
	else
	{
		state_value = L"当前没有任何学生评价";
		exportData.EnableWindow(0);GetMenu()->GetSubMenu(0)->EnableMenuItem(ID_EXPORT, MF_BYCOMMAND | MF_DISABLED);
	}

	UpdateData(FALSE);
}


void ViewComment::OnBnClickedAddmodule()
{
	// TODO: 在此添加控件通知处理程序代码

	OnOK();
}


void ViewComment::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnOK();
}


void ViewComment::OnEnChangeSearchtext()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	Screen();

}


void ViewComment::Screen()
{
	UpdateData(TRUE);
	screened.clear();
	checked.clear();
	Intro.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_DISABLED);

	for (size_t i = 0; i < ListVector.size(); i++)
	{
		if (EduManager.Contain(searchText_value, ListVector.at(i)->comment))
		{
			screened.push_back(i);
		}
	}

	RefreshWindowMessage();
}


void ViewComment::OnBnClickedAttendable()
{
	// TODO: 在此添加控件通知处理程序代码
	Screen();
}


void ViewComment::OnBnClickedCompulsory()
{
	// TODO: 在此添加控件通知处理程序代码
	Screen();
}



void ViewComment::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDialogEx::OnCancel();
}


int ViewComment::sort_column = 0; // 记录点击的列


bool ViewComment::method = false;; // 记录比较方法


int CALLBACK ViewComment::MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// 从参数中提取所需比较lc的两行数据
	int row1 = (int)lParam1;
	int row2 = (int)lParam2;
	CListCtrl* lc = (CListCtrl*)lParamSort;

	CString lp1 = lc->GetItemText(row1, sort_column);
	CString lp2 = lc->GetItemText(row2, sort_column);

	// 比较，对不同的列，不同比较，注意记录前一次排序方向，下一次要相反排序
	if (false)  //根据列的数据类型选择比较的类型
	{
		// int型比较
		long ilp1, ilp2;
		if (lp1 != L"无")ilp1 = _ttoi(lp1);
		else ilp1 = -1;

		if (lp2 != L"无")ilp2 = _ttoi(lp2);
		else ilp2 = -1;

		if (method) return  ilp1 - ilp2;
		else return ilp2 - ilp1;
	}
	else
	{
		// 文字型比较
		if (method) return lp1.CompareNoCase(lp2);
		else return lp2.CompareNoCase(lp1);
	}
	return 0;
}


void ViewComment::OnColumnclickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	sort_column = pNMLV->iSubItem;//点击的列
	int count = List.GetItemCount();

	if (sort_column == 0)
	{
		OnSelall();
		return;
	}

	for (int i = 0;i < count;i++)
	{
		List.SetItemData(i, i); // 每行的比较关键字，此处为列序号（点击的列号），可以设置为其他比较函数的第一二个参数
	}
	List.SortItems(MyCompareProc, (DWORD_PTR)&List);//排序第二个参数是比较函数的第三个参数

	//取反排序模式后，才能进行下次排序
	method = !method;

	*pResult = 0;
}


void ViewComment::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE)pNMHDR;//将传进来的消息转换成LPNMITEMACTIVAT
	auto r = temp->iItem;//获得行号
	auto c = temp->iSubItem;//获得列号

	if (r == -1)return;

	auto p = ListVector.begin();
	for (;p != ListVector.end();p++)
	{
		CString cs;
		cs.Format(L"%.1lf", (*p)->reScore);
		if ((*p)->comment == List.GetItemText(r, 2) && cs == List.GetItemText(r, 1))
		{
			break;
		}
	}
	if (p != ListVector.end())
	{
		Comment comment;
		comment.target = target->FindStudent((*p)->studentID);
		comment.DoModal();
	}
	else
	{
		MessageBox(L"ERROR");
	}


	*pResult = 0;
}


void ViewComment::OnBnClickedIntro()
{
	// TODO: 在此添加控件通知处理程序代码

	if (Intro.IsWindowEnabled())OnIntro();
}

void ViewComment::OnSelall()
{
	// TODO: 在此添加命令处理程序代码
	
	hAccel = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ADMINACCELERATOR));
	GetMenu()->GetSubMenu(0)->EnableMenuItem(ID_OPEN, MF_BYCOMMAND | MF_DISABLED);
	GetMenu()->GetSubMenu(0)->EnableMenuItem(ID_SAVEAS, MF_BYCOMMAND | MF_DISABLED);
	GetMenu()->GetSubMenu(0)->EnableMenuItem(ID_INPORT, MF_BYCOMMAND | MF_DISABLED);
	bool checkAll = true;
	for (size_t i = 0; i < (size_t)List.GetItemCount(); i++)
	{
		if (!List.GetCheck(i))
		{
			checkAll = false;
			break;
		}
	}
	if (!checkAll)
		for (size_t i = 0; i < (size_t)List.GetItemCount(); i++)
			List.SetCheck(i, 1);
	else
		for (size_t i = 0; i < (size_t)List.GetItemCount(); i++)
			List.SetCheck(i, 0);
	UpdateData(FALSE);
}


BOOL ViewComment::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (::TranslateAccelerator(GetSafeHwnd(), hAccel, pMsg))

		return true;
	return CDialogEx::PreTranslateMessage(pMsg);
}


void ViewComment::OnBnClickedExportdata()
{
	// TODO: 在此添加控件通知处理程序代码
	OnExport();
}


void ViewComment::OnExport()
{
	// TODO: 在此添加命令处理程序代码
	if (List.GetItemCount() <= 0)
	{
		MessageBox(L"当前列表中没有数据");
		return;
	}
	CString str; //获取系统时间 　　
	CTime tm; tm = CTime::GetCurrentTime();
	str = tm.Format("%Y-%m-%d %H-%M-%S");
	CString filename = EduManager.FindUser(target->teacherID)->name + L"老师 (" + target->teacherID + L") 在《"+target->name+L"》课程收到的评价 ";
	if (screened.size() != ListVector.size())filename += L"(部分) ";
	filename += str;
	CString szFilter = L"文本文件(*.txt)";
	CFileDialog dlg(FALSE, _T("txt"), filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	if (dlg.DoModal() != IDOK)return;
	CString strFileName;
	strFileName = dlg.GetPathName();
	DWORD dwRe = GetFileAttributes(strFileName);
	if (dwRe != (DWORD)-1)DeleteFile(strFileName);
	ofstream outfile(strFileName);
	int nRow = List.GetItemCount();
	int nCol = List.GetHeaderCtrl()->GetItemCount();
	TCHAR     lpBuffer[256];
	//LPCTSTR   lpszmyString;
	HDITEM   hdi;
	hdi.mask = HDI_TEXT;
	hdi.pszText = lpBuffer;
	hdi.cchTextMax = 256;
	outfile << "#";
	for (int i = 1; i < nCol; i++)
	{
		List.GetHeaderCtrl()->GetItem(i, &hdi);
		string s = EduManager.CSTS(hdi.pszText).c_str();
		if (s[0] == '*')s = s.substr(1, s.length() - 1);
		outfile << s;
		if (i != nCol - 1)outfile << '\t';
	}
	outfile << endl;
	for (size_t i = 0; i < (size_t)nRow; i++)
	{
		for (size_t j = 1; j < (size_t)nCol; j++)
		{
			outfile << CT2A(List.GetItemText(i, j).GetString());
			if (j != nCol - 1)outfile << '\t';
		}
		outfile << endl;
	}
	outfile << "#END";
	outfile.close();
}


void ViewComment::OnMin()
{
	// TODO: 在此添加命令处理程序代码
	ShowWindow(SW_SHOWMINIMIZED);
}


void ViewComment::OnClose()
{
	// TODO: 在此添加命令处理程序代码
	OnCancel();
}


void ViewComment::OnIntro()
{
	// TODO: 在此添加命令处理程序代码
	if (!Intro.IsWindowEnabled())return;
	auto p = ListVector.begin();
	for (;p != ListVector.end();p++)
	{
		CString cs;
		cs.Format(L"%.1lf", (*p)->reScore);
		if ((*p)->comment == List.GetItemText(checked.at(0), 2) && cs == List.GetItemText(checked.at(0), 1))
		{
			break;
		}
	}
	if (p != ListVector.end())
	{
		Comment comment;
		comment.target = target->FindStudent((*p)->studentID);
		comment.DoModal();
	}
	else
	{
		MessageBox(L"ERROR");
	}
}
