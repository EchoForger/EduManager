// AddStudent.cpp: 实现文件
//

#include "pch.h"
#include "EduManager.h"
#include "AddStudent.h"
#include "afxdialogex.h"
#include <set>
#include "EditUser.h"
#include "EditModule.h"


// AddStudent 对话框

IMPLEMENT_DYNAMIC(AddStudent, CDialogEx)


AddStudent::AddStudent(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADDSTUDENT, pParent)
	, searchText_value(_T(""))
	, state_value(_T(""))
{

}


AddStudent::~AddStudent()
{
}


void AddStudent::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SEARCHTEXT, searchText_value);
	DDX_Control(pDX, IDC_ADDSTUDENT, B_AddStudent);
	DDX_Control(pDX, IDC_LIST, List);
	DDX_Text(pDX, IDC_STATE, state_value);
	DDX_Control(pDX, IDC_INTRO, Intro);
}


BEGIN_MESSAGE_MAP(AddStudent, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, &AddStudent::OnLvnItemchangedList)
	ON_BN_CLICKED(IDC_ADDSTUDENT, &AddStudent::OnBnClickedAddmodule)
	ON_EN_CHANGE(IDC_SEARCHTEXT, &AddStudent::OnEnChangeSearchtext)
	ON_BN_CLICKED(IDC_ATTENDABLE, &AddStudent::OnBnClickedAttendable)
	ON_BN_CLICKED(IDC_COMPULSORY, &AddStudent::OnBnClickedCompulsory)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST, &AddStudent::OnColumnclickList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &AddStudent::OnDblclkList)
	ON_BN_CLICKED(IDC_INTRO, &AddStudent::OnBnClickedIntro)
	ON_COMMAND(ID_SELALL, &AddStudent::OnSelall)
	ON_COMMAND(ID_CLOSE, &AddStudent::OnClose)
	ON_COMMAND(ID_MIN, &AddStudent::OnMin)
	ON_COMMAND(ID_INTRO, &AddStudent::OnIntro)
END_MESSAGE_MAP()


// AddStudent 消息处理程序


void AddStudent::OnLvnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	checked.clear();
	extraCredit = 0;

	for (size_t i = 0; i < (size_t)(List.GetItemCount()); i++)
	{
		if (List.GetCheck(i))
		{
			checked.push_back(i);
			extraCredit += _ttof(List.GetItemText(i, 4));
		}
	}
	if (!checked.empty())
	{
		B_AddStudent.EnableWindow(1);
		Intro.EnableWindow(1);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_ENABLED);
	}
	else
	{
		B_AddStudent.EnableWindow(0);
		Intro.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_DISABLED);
	}

	if (checked.size())state_value.Format(L"已选学生：%d", checked.size());
	else state_value = L"当前没有选择任何学生";

	UpdateData(FALSE);

	*pResult = 0;
}


BOOL AddStudent::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	hAccel = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ADMINACCELERATOR));
	GetMenu()->GetSubMenu(0)->DeleteMenu(ID_OPEN, MF_BYCOMMAND | MF_DISABLED);
	GetMenu()->GetSubMenu(0)->DeleteMenu(ID_SAVEAS, MF_BYCOMMAND | MF_DISABLED);
	GetMenu()->GetSubMenu(0)->DeleteMenu(ID_INPORT, MF_BYCOMMAND | MF_DISABLED);
	GetMenu()->GetSubMenu(0)->DeleteMenu(ID_EXPORT, MF_BYCOMMAND | MF_DISABLED);
	GetMenu()->GetSubMenu(1)->DeleteMenu(ID_ADD, MF_BYCOMMAND | MF_DISABLED);
	GetMenu()->GetSubMenu(1)->DeleteMenu(ID_DELETE, MF_BYCOMMAND | MF_DISABLED);

	B_AddStudent.EnableWindow(0);
	List.SetExtendedStyle(List.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);

	List.InsertColumn(0, L"选择", LVCFMT_CENTER, (int)(0.4 * EduManager.defaultWidth));
	List.InsertColumn(1, L"账号", LVCFMT_LEFT, (int)(1.2 * EduManager.defaultWidth));
	List.InsertColumn(2, L"姓名", LVCFMT_LEFT, (int)(0.8 * EduManager.defaultWidth));
	List.InsertColumn(3, L"班级", LVCFMT_LEFT, (int)(0.8 * EduManager.defaultWidth));
	List.InsertColumn(4, L"专业", LVCFMT_LEFT, (int)(1.2 * EduManager.defaultWidth));

	totalCredit = 0;
	totalSelect = 0;

	for (size_t i = 0; i < EduManager.UserLib.size(); i++)
	{
		if (EduManager.UserLib.at(i).userType == 3)
		{
			bool added = false;
			for (size_t j = 0; j < parentVector->size(); j++)
			{
				if (parentVector->at(j).pUser->ID == EduManager.UserLib.at(i).ID)
				{
					added = true;
					break;
				}
			}
			if (!added)
			{
				ListVector.push_back(&(EduManager.UserLib.at(i)));
				screened.push_back(screened.size());
			}
		}
	}

	RefreshWindowMessage();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void AddStudent::RefreshWindowMessage() {

	List.DeleteAllItems();

	for (auto p = screened.begin();p != screened.end();p++)
	{
		List.InsertItem(0, L"");
		List.SetItemText(0, 1, ListVector.at(*p)->ID);
		List.SetItemText(0, 2, ListVector.at(*p)->name);
		List.SetItemText(0, 3, ListVector.at(*p)->classID);
		List.SetItemText(0, 4, ListVector.at(*p)->major);

	}


	if (checked.size())
	{
		state_value.Format(L"已选择学生：%d", checked.size());
	}
	else state_value = L"当前没有选择任何学生";

	UpdateData(FALSE);
}


void AddStudent::OnBnClickedAddmodule()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!B_AddStudent.IsWindowEnabled())return;
	for (auto p = checked.begin();p != checked.end();p++)
	{
		auto q = EduManager.FindUser(List.GetItemText(*p, 1));
		ListItemClass lis;
		lis.pUser = q;
		lis.score = -1;
		parentVector->push_back(lis);
	}
	CDialog::OnOK();
}


void AddStudent::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	MessageBox(L"ERROR");

	CDialogEx::OnOK();
}


void AddStudent::OnEnChangeSearchtext()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	Screen();

}


void AddStudent::Screen()
{
	UpdateData(TRUE);
	screened.clear();
	checked.clear();
	B_AddStudent.EnableWindow(0);
	Intro.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_DISABLED);

	for (size_t i = 0; i < ListVector.size(); i++)
	{
		if (EduManager.Contain(searchText_value, ListVector.at(i)->ID) || EduManager.Contain(searchText_value, ListVector.at(i)->name) || EduManager.Contain(searchText_value, ListVector.at(i)->classID) || EduManager.Contain(searchText_value, ListVector.at(i)->major))
		{
			screened.push_back(i);
		}
	}

	RefreshWindowMessage();
}


void AddStudent::OnBnClickedAttendable()
{
	// TODO: 在此添加控件通知处理程序代码
	Screen();
}


void AddStudent::OnBnClickedCompulsory()
{
	// TODO: 在此添加控件通知处理程序代码
	Screen();
}



void AddStudent::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDialogEx::OnCancel();
}


int AddStudent::sort_column = 0; // 记录点击的列


bool AddStudent::method = false;; // 记录比较方法


int CALLBACK AddStudent::MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
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


void AddStudent::OnColumnclickList(NMHDR* pNMHDR, LRESULT* pResult)
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


void AddStudent::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE)pNMHDR;//将传进来的消息转换成LPNMITEMACTIVAT
	auto r = temp->iItem;//获得行号
	auto c = temp->iSubItem;//获得列号
	if (r == -1)return;
	checked.clear();
	checked.push_back(r);
	B_AddStudent.EnableWindow(1);
	OnOK();

	*pResult = 0;
}


void AddStudent::OnBnClickedIntro()
{
	// TODO: 在此添加控件通知处理程序代码
	if (Intro.IsWindowEnabled())OnIntro();



	
}


void AddStudent::OnSelall()
{
	// TODO: 在此添加命令处理程序代码
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


BOOL AddStudent::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (::TranslateAccelerator(GetSafeHwnd(), hAccel, pMsg))

		return true;
	return CDialogEx::PreTranslateMessage(pMsg);
}


void AddStudent::OnClose()
{
	// TODO: 在此添加命令处理程序代码
	OnCancel();
}


void AddStudent::OnMin()
{
	// TODO: 在此添加命令处理程序代码

	ShowWindow(SW_SHOWMINIMIZED);
}


void AddStudent::OnIntro()
{
	// TODO: 在此添加命令处理程序代码
	if (!Intro.IsWindowEnabled())return;

	EditUser editUser;
	for (auto p = checked.begin();p != checked.end();p++)
	{
		auto q = EduManager.FindUser(List.GetItemText(*p, 1));
		editUser.target.push_back(&(*q));
	}
	if (editUser.DoModal() == IDOK)
	{
		RefreshWindowMessage();
		Screen();
		checked.clear();
		B_AddStudent.EnableWindow(0);
		Intro.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_DISABLED);
	}
}
