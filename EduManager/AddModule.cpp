// AddModule.cpp: 实现文件
//

#include "pch.h"
#include "EduManager.h"
#include "AddModule.h"
#include "EditModule.h"
#include "afxdialogex.h"
#include <set>


// AddModule 对话框

IMPLEMENT_DYNAMIC(AddModule, CDialogEx)

AddModule::AddModule(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADDMODULE, pParent)
	, state_value(_T(""))
	, searchText_value(_T(""))
	, attendAble_value(FALSE)
	, compulsory_value(FALSE)
{

}

AddModule::~AddModule()
{
}

void AddModule::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, List);
	DDX_Text(pDX, IDC_STATE, state_value);
	DDX_Control(pDX, IDC_ADDMODULE, B_AddModule);
	//  DDX_Control(pDX, IDC_SEARCHTEXT, searchText_value);
	DDX_Text(pDX, IDC_SEARCHTEXT, searchText_value);
	DDX_Check(pDX, IDC_ATTENDABLE, attendAble_value);
	DDX_Check(pDX, IDC_COMPULSORY, compulsory_value);
	DDX_Control(pDX, IDC_INTRO, Intro);
	DDX_Control(pDX, IDC_INTRO, Intro);
}


BEGIN_MESSAGE_MAP(AddModule, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, &AddModule::OnLvnItemchangedList)
	ON_BN_CLICKED(IDC_ADDMODULE, &AddModule::OnBnClickedAddmodule)
	ON_EN_CHANGE(IDC_SEARCHTEXT, &AddModule::OnEnChangeSearchtext)
	ON_BN_CLICKED(IDC_ATTENDABLE, &AddModule::OnBnClickedAttendable)
	ON_BN_CLICKED(IDC_COMPULSORY, &AddModule::OnBnClickedCompulsory)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST, &AddModule::OnColumnclickList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &AddModule::OnDblclkList)
	ON_BN_CLICKED(IDC_INTRO, &AddModule::OnBnClickedIntro)
	ON_COMMAND(ID_SELALL, &AddModule::OnSelall)
	ON_COMMAND(ID_32791, &AddModule::On32791)
	ON_COMMAND(ID_INTRO, &AddModule::OnIntro)
	ON_COMMAND(ID_MIN, &AddModule::OnMin)
	ON_COMMAND(ID_CLOSE, &AddModule::OnClose)
END_MESSAGE_MAP()


// AddModule 消息处理程序


void AddModule::OnLvnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult)
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
		B_AddModule.EnableWindow(1);
		Intro.EnableWindow(1);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_ENABLED);
	}
	else
	{
		B_AddModule.EnableWindow(0);
		Intro.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_DISABLED);
	}

	if (totalCredit + extraCredit)state_value.Format(L"已选课程：%d   已选学分：%.1lf", totalSelect + checked.size(), totalCredit + extraCredit);
	else state_value = L"当前没有选择任何课程";

	UpdateData(FALSE);

	*pResult = 0;
}


BOOL AddModule::OnInitDialog()
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

	B_AddModule.EnableWindow(0);
	Intro.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_DISABLED);
	List.SetExtendedStyle(List.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);

	List.InsertColumn(0, L"选择", LVCFMT_CENTER, (int)(0.4 * EduManager.defaultWidth));
	List.InsertColumn(1, L"课程代码", LVCFMT_LEFT, (int)(0.8 * EduManager.defaultWidth));
	List.InsertColumn(2, L"课程名称", LVCFMT_LEFT, (int)(1.5 * EduManager.defaultWidth));
	List.InsertColumn(3, L"教师", LVCFMT_LEFT, (int)(0.7 * EduManager.defaultWidth));
	List.InsertColumn(4, L"学分", LVCFMT_LEFT, (int)(0.5 * EduManager.defaultWidth));
	List.InsertColumn(5, L"学时", LVCFMT_LEFT, (int)(0.5 * EduManager.defaultWidth));
	List.InsertColumn(6, L"必修", LVCFMT_LEFT, (int)(0.5 * EduManager.defaultWidth));
	List.InsertColumn(7, L"参与人数", LVCFMT_LEFT, (int)(0.7 * EduManager.defaultWidth));

	totalCredit = 0;
	totalSelect = 0;

	for (size_t i = 0; i < EduManager.ModuleLib.size(); i++)
	{
		if (EduManager.ModuleLib.at(i).FindStudent(target->ID) != nullptr)
		{
			totalCredit += EduManager.ModuleLib.at(i).credit;
			totalSelect++;
		}
		else
		{
			ListVector.push_back(&(EduManager.ModuleLib.at(i)));
			screened.push_back(screened.size());
		}
	}

	RefreshWindowMessage();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void AddModule::RefreshWindowMessage() {

	List.DeleteAllItems();

	for (auto p=screened.begin();p!=screened.end();p++)
	{
		CString teacherName("NULL"), credit("NULL"), hours("NULL"), compulsory("NULL"),
			attendSz("NULL");
		teacherName = EduManager.FindUser(ListVector.at(*p)->teacherID)->name;
		credit.Format(L"%.1lf", ListVector.at(*p)->credit);
		hours.Format(L"%d", ListVector.at(*p)->hours);
		if (ListVector.at(*p)->compulsory) compulsory = "是";
		else compulsory = "否";
		attendSz.Format(L"%d", ListVector.at(*p)->attend.size());


		List.InsertItem(0, L"");
		List.SetItemText(0, 1, ListVector.at(*p)->ID);
		List.SetItemText(0, 2, ListVector.at(*p)->name);
		List.SetItemText(0, 3, teacherName);
		List.SetItemText(0, 4, credit);
		List.SetItemText(0, 5, hours);
		List.SetItemText(0, 6, compulsory);
		List.SetItemText(0, 7, attendSz);

	}


	if (totalCredit)state_value.Format(L"已选课程：%d   已选学分：%.1lf", totalSelect, totalCredit);
	else state_value = L"当前没有选择任何课程";
	UpdateData(FALSE);
}

void AddModule::OnBnClickedAddmodule()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}


void AddModule::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	
	if (!B_AddModule.IsWindowEnabled())return;

	vector<CString>hasSelected;
	for (auto p = checked.begin();p != checked.end();p++)
	{
		auto q = EduManager.FindModule(List.GetItemText(*p, 1));

		if (!(q->AttendedSame(target)))
		{
			q->AddStudent(target->ID);
		}
		else
		{
			CString cs;
			cs.Format(L"%s\t《%s》",q->ID, q->name);
			hasSelected.push_back(cs);
		}
	}

	//RefreshWindowMessage();
	//checked.clear();
	B_AddModule.EnableWindow(0);
	Intro.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_DISABLED);

	if (!hasSelected.empty())
	{
		CString cs;
		cs.Format(L"已参加以下%d门课程的同名课程，请退选后再添加：\n\n", hasSelected.size());
		for (size_t i = 0; i < hasSelected.size(); i++)
		{
			cs += hasSelected.at(i);
			if (i != hasSelected.size() - 1)cs += '\n';
		}
		MessageBox(cs);
	}

	EduManager.SaveData(EduManager.basicDataPos);
	CDialogEx::OnOK();
}


void AddModule::OnEnChangeSearchtext()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	Screen();

}

void AddModule::Screen()
{
	UpdateData(TRUE);
	screened.clear();
	checked.clear();
	B_AddModule.EnableWindow(0);
	Intro.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_DISABLED);

	for (size_t i = 0; i < ListVector.size(); i++)
	{
		if (EduManager.Contain(searchText_value, ListVector.at(i)->ID) || EduManager.Contain(searchText_value, ListVector.at(i)->name) || EduManager.Contain(searchText_value, EduManager.FindUser(ListVector.at(i)->teacherID)->name) || EduManager.Contain(searchText_value, ListVector.at(i)->teacherID))
		{
			if (!attendAble_value|| !ListVector.at(i)->AttendedSame(target))
			{
				if (!compulsory_value || ListVector.at(i)->compulsory)
				{
					screened.push_back(i);
				}
			}
		}
	}

	RefreshWindowMessage();
}

void AddModule::OnBnClickedAttendable()
{
	// TODO: 在此添加控件通知处理程序代码
	Screen();
}


void AddModule::OnBnClickedCompulsory()
{
	// TODO: 在此添加控件通知处理程序代码
	Screen();
}



void AddModule::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDialogEx::OnCancel();
}


int AddModule::sort_column = 0; // 记录点击的列
bool AddModule::method = false;; // 记录比较方法


int CALLBACK AddModule::MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// 从参数中提取所需比较lc的两行数据
	int row1 = (int)lParam1;
	int row2 = (int)lParam2;
	CListCtrl* lc = (CListCtrl*)lParamSort;

	CString lp1 = lc->GetItemText(row1, sort_column);
	CString lp2 = lc->GetItemText(row2, sort_column);

	// 比较，对不同的列，不同比较，注意记录前一次排序方向，下一次要相反排序
	if (sort_column == 4 || sort_column == 5 || sort_column == 7 || sort_column == 8 || sort_column == 9)  //根据列的数据类型选择比较的类型
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

void AddModule::OnColumnclickList(NMHDR* pNMHDR, LRESULT* pResult)
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

void AddModule::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE)pNMHDR;//将传进来的消息转换成LPNMITEMACTIVAT
	auto r = temp->iItem;//获得行号
	auto c = temp->iSubItem;//获得列号
	if (r == -1)return;

	checked.clear();
	checked.push_back(r);
	B_AddModule.EnableWindow(1);
	OnOK();

	*pResult = 0;
}


void AddModule::OnBnClickedIntro()
{
	// TODO: 在此添加控件通知处理程序代码	

	if (Intro.IsWindowEnabled())OnIntro();
	
}


void AddModule::OnSelall()
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


BOOL AddModule::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (::TranslateAccelerator(GetSafeHwnd(), hAccel, pMsg))

		return true;
	return CDialogEx::PreTranslateMessage(pMsg);
}


void AddModule::On32791()
{
	// TODO: 在此添加命令处理程序代码
}


void AddModule::OnIntro()
{
	// TODO: 在此添加命令处理程序代码
	if (!Intro.IsWindowEnabled())return;

	EditModule editModule;
	for (auto p = checked.begin();p != checked.end();p++)
	{
		auto q = EduManager.FindModule(List.GetItemText(*p, 1));
		editModule.target.push_back(q);
	}
	if (editModule.DoModal() == IDOK)
	{
		RefreshWindowMessage();
		Screen();
		checked.clear();
		B_AddModule.EnableWindow(0);
		Intro.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_DISABLED);
	}

}


void AddModule::OnMin()
{
	// TODO: 在此添加命令处理程序代码
	ShowWindow(SW_SHOWMINIMIZED);
}


void AddModule::OnClose()
{
	// TODO: 在此添加命令处理程序代码
	OnCancel();
}
