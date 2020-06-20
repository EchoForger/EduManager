// TeacherForm.cpp: 实现文件
//

#include "pch.h"
#include "EduManager.h"
#include "TeacherForm.h"
#include "afxdialogex.h"
#include "AddModule.h"
#include "EditModule.h"
#include <algorithm>
#include <set>
#include "EditUser.h"



// TeacherForm 对话框

IMPLEMENT_DYNAMIC(TeacherForm, CDialogEx)

TeacherForm::TeacherForm(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TEACHERFORM, pParent)
	, state_value(_T(""))
	, userInfo_value(_T(""))
	, username_value(_T(""))
	, unSelCom_value(_T(""))
	, searchText_value(_T(""))
	, unrelease_value(FALSE)
	, quitAble_value(FALSE)
{

}

TeacherForm::~TeacherForm()
{
}

void TeacherForm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, List);
	//  DDX_Control(pDX, IDC_STATE, state);
	//  DDX_Text(pDX, IDC_STATE, state);
	//  DDX_Control(pDX, IDC_STATE, state_value);
	//  DDX_Text(pDX, IDC_STATE, state);
	DDX_Text(pDX, IDC_STATE, state_value);
	DDX_Control(pDX, IDC_USERNAME, username);
	DDX_Control(pDX, IDC_USERINFO, userInfo);
	DDX_Text(pDX, IDC_USERINFO, userInfo_value);
	DDX_Text(pDX, IDC_USERNAME, username_value);
	DDX_Control(pDX, IDC_DELMODULE, DelItem);
	DDX_Text(pDX, IDC_SEARCHTEXT, searchText_value);
	DDX_Check(pDX, IDC_UNRELEASE, unrelease_value);
	DDX_Control(pDX, IDC_EXPORTDATA, exportData);
	DDX_Control(pDX, IDC_INTRO, Intro);
}


BEGIN_MESSAGE_MAP(TeacherForm, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, &TeacherForm::OnLvnItemchangedList)
	ON_BN_CLICKED(IDC_ADDMODULE, &TeacherForm::OnBnClickedAddmodule)
	ON_BN_CLICKED(IDC_DELMODULE, &TeacherForm::OnBnClickedDelmodule)
	ON_WM_RBUTTONDBLCLK()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST, &TeacherForm::OnColumnclickList)
	ON_EN_CHANGE(IDC_SEARCHTEXT, &TeacherForm::OnEnChangeSearchtext)
	ON_BN_CLICKED(IDC_UNRELEASE, &TeacherForm::OnBnClickedCompulsory)
	ON_BN_CLICKED(IDC_QUITABLE, &TeacherForm::OnBnClickedQuitable)
	ON_BN_CLICKED(IDC_UNRELEASE, &TeacherForm::OnBnClickedUnrelease)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &TeacherForm::OnDblclkList)
	ON_BN_CLICKED(IDC_EXPORTDATA, &TeacherForm::OnBnClickedExportdata)
	ON_BN_CLICKED(IDC_INTRO, &TeacherForm::OnBnClickedIntro)
	ON_STN_CLICKED(IDC_USERINFO, &TeacherForm::OnStnClickedUserinfo)
	ON_BN_CLICKED(IDC_INTRO2, &TeacherForm::OnBnClickedIntro2)
	ON_NOTIFY(NM_CLICK, IDC_LIST, &TeacherForm::OnClickList)
	ON_CBN_SELCHANGE(IDC_COMBOX_CREATEID, &TeacherForm::OnSelchangeComboxCreateid)
	ON_CBN_SELENDOK(IDC_COMBOX_CREATEID, &TeacherForm::OnSelendokComboxCreateid)
	ON_CBN_SELENDCANCEL(IDC_COMBOX_CREATEID, &TeacherForm::OnSelendcancelComboxCreateid)
	ON_EN_KILLFOCUS(IDC_EDIT_CREATEID, &TeacherForm::OnKillfocusEditCreateid)
	ON_CBN_KILLFOCUS(IDC_COMBOX_CREATEID, &TeacherForm::OnKillfocusComboxCreateid)
	ON_BN_CLICKED(IDC_USERINTRO, &TeacherForm::OnBnClickedUserintro)
	ON_BN_CLICKED(IDC_INTRO3, &TeacherForm::OnBnClickedIntro3)
	ON_COMMAND(ID_EXPORT, &TeacherForm::OnExport)
	ON_COMMAND(ID_SELALL, &TeacherForm::OnSelall)
	ON_COMMAND(ID_ADD, &TeacherForm::OnAdd)
	ON_COMMAND(ID_INTRO, &TeacherForm::OnIntro)
	ON_COMMAND(ID_DELETE, &TeacherForm::OnDelete)
	ON_COMMAND(ID_MIN, &TeacherForm::OnMin)
	ON_COMMAND(ID_CLOSE, &TeacherForm::OnClose)
END_MESSAGE_MAP()


// TeacherForm 消息处理程序


void TeacherForm::OnLvnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult)
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
	if (!checked.empty())
	{
		DelItem.EnableWindow(1);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_DELETE, MF_BYCOMMAND | MF_ENABLED);
		Intro.EnableWindow(1);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_ENABLED);
	}
	else
	{
		DelItem.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_DELETE, MF_BYCOMMAND | MF_DISABLED);
		Intro.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_DISABLED);
	}


	*pResult = 0;
}


BOOL TeacherForm::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	hAccel = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ADMINACCELERATOR));
	GetMenu()->GetSubMenu(0)->DeleteMenu(ID_OPEN, MF_BYCOMMAND | MF_DISABLED);
	GetMenu()->GetSubMenu(0)->DeleteMenu(ID_SAVEAS, MF_BYCOMMAND | MF_DISABLED);
	GetMenu()->GetSubMenu(0)->DeleteMenu(ID_INPORT, MF_BYCOMMAND | MF_DISABLED);

	GetMenu()->GetSubMenu(1)->ModifyMenuW(2, MF_BYPOSITION, ID_ADD, L"新建课程(N)...\tCtrl+N");
	GetMenu()->GetSubMenu(1)->ModifyMenuW(4, MF_BYPOSITION, ID_DELETE, L"删除课程(L)\tDel");

	DelItem.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_DELETE, MF_BYCOMMAND | MF_DISABLED);
	Intro.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_DISABLED);

	ModifyStyleEx(0, WS_EX_APPWINDOW);

	List.SetExtendedStyle(List.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);

	List.InsertColumn(0, L"选择", LVCFMT_CENTER, (int)(0.4 * EduManager.defaultWidth));
	List.InsertColumn(1, L"课程代码", LVCFMT_LEFT, (int)(0.8 * EduManager.defaultWidth));
	List.InsertColumn(2, L"课程名称", LVCFMT_LEFT, (int)(1.5 * EduManager.defaultWidth));
	List.InsertColumn(3, L"教师", LVCFMT_LEFT, (int)(0.7 * EduManager.defaultWidth));
	List.InsertColumn(4, L"*学分", LVCFMT_LEFT, (int)(0.5 * EduManager.defaultWidth));
	List.InsertColumn(5, L"*学时", LVCFMT_LEFT, (int)(0.5 * EduManager.defaultWidth));
	List.InsertColumn(6, L"*必修", LVCFMT_LEFT, (int)(0.5 * EduManager.defaultWidth));
	List.InsertColumn(7, L"参与人数", LVCFMT_LEFT, (int)(0.7 * EduManager.defaultWidth));
	List.InsertColumn(8, L"公布", LVCFMT_LEFT, (int)(0.5 * EduManager.defaultWidth));

	for (size_t i = 0; i < EduManager.ModuleLib.size(); i++)
	{
		if (EduManager.ModuleLib.at(i).teacherID == target->ID)
		{
			ListVector.push_back(&(EduManager.ModuleLib.at(i)));
			screened.push_back(screened.size());
		}
	}
	RefreshWindowMessage();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void TeacherForm::OnBnClickedAddmodule()
{

	// TODO: 在此添加控件通知处理程序代码
	OnAdd();
}


void TeacherForm::OnBnClickedDelmodule()
{
	// TODO: 在此添加控件通知处理程序代码
	if(DelItem.IsWindowEnabled())OnDelete();
}

void TeacherForm::RefreshWindowMessage()
{
	List.DeleteAllItems();

	ListVector.clear();
	for (size_t i = 0; i < EduManager.ModuleLib.size(); i++)
	{
		if (EduManager.ModuleLib.at(i).teacherID == target->ID)
		{
			ListVector.push_back(&(EduManager.ModuleLib.at(i)));
		}
	}


	for (size_t i = 0; i < ListVector.size(); i++)
	{
		CString teacherName("NULL"), credit("NULL"), hours("NULL"), compulsory("NULL"),
			attendSz("NULL"), Score("NULL"), GPA("NULL"), pass("NULL"),release("NULL");
		teacherName = EduManager.FindUser(ListVector.at(i)->teacherID)->name;
		credit.Format(L"%.1lf", ListVector.at(i)->credit);
		hours.Format(L"%d", ListVector.at(i)->hours);
		if (ListVector.at(i)->compulsory) compulsory = "是";
		else compulsory = "否";
		attendSz.Format(L"%d", ListVector.at(i)->attend.size());
		if (ListVector.at(i)->Release()) release = L"是";
		else  release = L"否";

		if (find(screened.begin(), screened.end(), i) < screened.end())
		{
			List.InsertItem(0, L"");
			List.SetItemText(0, 1, ListVector.at(i)->ID);
			List.SetItemText(0, 2, ListVector.at(i)->name);
			List.SetItemText(0, 3, teacherName);
			List.SetItemText(0, 4, credit);
			List.SetItemText(0, 5, hours);
			List.SetItemText(0, 6, compulsory);
			List.SetItemText(0, 7, attendSz);
			List.SetItemText(0, 8, release);
		}

	}



	userInfo_value = target->ID  + L"   教师";
	username_value = target->name;

	if (ListVector.size())
	{
		state_value.Format(L"已任教课程：%d", ListVector.size());
	}
	else state_value = L"当前没有任教任何课程";

	
	if (List.GetItemCount() != 0)
	{
		exportData.EnableWindow(1);GetMenu()->GetSubMenu(0)->EnableMenuItem(ID_EXPORT, MF_BYCOMMAND | MF_ENABLED);
	}
	else
	{
		exportData.EnableWindow(0);GetMenu()->GetSubMenu(0)->EnableMenuItem(ID_EXPORT, MF_BYCOMMAND | MF_DISABLED);
	}

	UpdateData(FALSE);
}

void TeacherForm::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (haveeditcreate == true)//如果之前创建了编辑框就销毁掉
	{
		distroyEdit(&List, &m_Edit, e_Item, e_SubItem);//销毁单元格编辑框对象
		haveeditcreate = false;
	}
	if (haveccomboboxcreate == true)//如果之前创建了下拉列表框就销毁掉
	{
		saveCcombobox(&List, &m_comBox, e_Item, e_SubItem);
		haveccomboboxcreate = false;
	}
	//CDialogEx::OnOK();
}

void TeacherForm::OnColumnclickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	sort_column = pNMLV->iSubItem;//点击的列
	int count = List.GetItemCount();

	if (sort_column==0)
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

void TeacherForm::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnCancel();
}

void TeacherForm::Screen()
{

	UpdateData(TRUE);
	screened.clear();

	checked.clear();

	DelItem.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_DELETE, MF_BYCOMMAND | MF_DISABLED);
	Intro.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_DISABLED);

	for (size_t i = 0; i < ListVector.size(); i++)
	{
		if (EduManager.Contain(searchText_value, ListVector.at(i)->ID) || EduManager.Contain(searchText_value, ListVector.at(i)->name) || EduManager.Contain(searchText_value, EduManager.FindUser(ListVector.at(i)->teacherID)->name) || EduManager.Contain(searchText_value, ListVector.at(i)->teacherID))
		{
			if (!unrelease_value || !ListVector.at(i)->Release())
			{
				screened.push_back(i);
			}
		}
	}

	RefreshWindowMessage();
}

void TeacherForm::OnEnChangeSearchtext()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	Screen();
}


void TeacherForm::OnBnClickedCompulsory()
{
	// TODO: 在此添加控件通知处理程序代码
	Screen();
}


void TeacherForm::OnBnClickedQuitable()
{
	// TODO: 在此添加控件通知处理程序代码
	Screen();
}


int TeacherForm::sort_column = 0; // 记录点击的列
bool TeacherForm::method = false;; // 记录比较方法


int CALLBACK TeacherForm::MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// 从参数中提取所需比较lc的两行数据
	int row1 = (int)lParam1;
	int row2 = (int)lParam2;
	CListCtrl* lc = (CListCtrl*)lParamSort;

	CString lp1 = lc->GetItemText(row1, sort_column);
	CString lp2 = lc->GetItemText(row2, sort_column);

	// 比较，对不同的列，不同比较，注意记录前一次排序方向，下一次要相反排序
	if (sort_column == 4 || sort_column == 5 || sort_column == 7)  //根据列的数据类型选择比较的类型
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

void TeacherForm::OnBnClickedUnrelease()
{
	// TODO: 在此添加控件通知处理程序代码
	Screen();
}


void TeacherForm::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	NM_LISTVIEW* pEditCtrl = (NM_LISTVIEW*)pNMHDR;

	LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE)pNMHDR;//将传进来的消息转换成LPNMITEMACTIVAT
	auto r = temp->iItem;//获得行号
	auto c = temp->iSubItem;//获得列号

	if (r == -1)return;

	if (c == 4 || c == 5)
	{
			e_Item = pEditCtrl->iItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理
			e_SubItem = pEditCtrl->iSubItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理
			createEdit(pEditCtrl, &m_Edit, e_Item, e_SubItem, haveeditcreate);//创建编辑框
	}
	else if (c == 6)
	{
		if (haveccomboboxcreate == true)
		{
			if (!(e_Item == pEditCtrl->iItem && e_SubItem == pEditCtrl->iSubItem))//如果点中的单元格不是之前创建好的
			{
				saveCcombobox(&List, &m_comBox, e_Item, e_SubItem);
				haveccomboboxcreate = false;
				createCcombobox(pEditCtrl, &m_comBox, e_Item, e_SubItem, haveccomboboxcreate);//创建编辑框
				m_comBox.AddString(L"是");
				m_comBox.AddString(L"否");
				m_comBox.ShowDropDown();//自动下拉
			}
			else//点中的单元格是之前创建好的
			{
				m_comBox.SetFocus();//设置为焦点 
			}
		}
		else
		{
			e_Item = pEditCtrl->iItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理
			e_SubItem = pEditCtrl->iSubItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理
			createCcombobox(pEditCtrl, &m_comBox, e_Item, e_SubItem, haveccomboboxcreate);//创建编辑框
			m_comBox.AddString(L"是");
			m_comBox.AddString(L"否");
			m_comBox.ShowDropDown();//自动下拉
		}
	}
	else
	{
		EditModule editModule;
		editModule.target.push_back(EduManager.FindModule(List.GetItemText(r, 1)));
		if (editModule.DoModal() == IDOK)RefreshWindowMessage();
	}


	*pResult = 0;
}


void TeacherForm::OnBnClickedExportdata()
{
	// TODO: 在此添加控件通知处理程序代码
	OnExport();
}


void TeacherForm::OnBnClickedIntro()
{
	// TODO: 在此添加控件通知处理程序代码

	if (Intro.IsWindowEnabled())OnIntro();

	
}


void TeacherForm::OnStnClickedUserinfo()
{
	// TODO: 在此添加控件通知处理程序代码
}


void TeacherForm::OnBnClickedIntro2()
{
	// TODO: 在此添加控件通知处理程序代码
}


void TeacherForm::createEdit(NM_LISTVIEW* pEditCtrl, CEdit* createdit, int& Item, int& SubItem, bool& havecreat)
{
	Item = pEditCtrl->iItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理
	SubItem = pEditCtrl->iSubItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理
	createdit->Create(ES_AUTOHSCROLL | WS_CHILD | ES_LEFT | ES_WANTRETURN,
		CRect(0, 0, 0, 0), this, IDC_EDIT_CREATEID);//创建编辑框对象,IDC_EDIT_CREATEID为控件ID号1002，在resource.h中定义
	havecreat = true;
	createdit->SetFont(this->GetFont(), FALSE);//设置字体,不设置这里的话上面的字会很突兀的感觉
	createdit->SetParent(&List);//将list control设置为父窗口,生成的Edit才能正确定位,这个也很重要
	CRect  EditRect;
	List.GetSubItemRect(e_Item, e_SubItem, LVIR_LABEL, EditRect);//获取单元格的空间位置信息
	EditRect.SetRect(EditRect.left + 1, EditRect.top + 1, EditRect.left + List.GetColumnWidth(e_SubItem) - 1, EditRect.bottom - 1);//+1和-1可以让编辑框不至于挡住列表框中的网格线
	CString strItem = List.GetItemText(e_Item, e_SubItem);//获得相应单元格字符
	createdit->SetWindowText(strItem);//将单元格字符显示在编辑框上
	createdit->MoveWindow(&EditRect);//将编辑框位置放在相应单元格上
	createdit->ShowWindow(SW_SHOW);//显示编辑框在单元格上面
	createdit->SetFocus();//设置为焦点 
	createdit->SetSel(-1);//设置光标在文本框文字的最后
}

void TeacherForm::distroyEdit(CListCtrl* pList, CEdit* distroyedit, int& Item, int& SubItem)
{
	auto p = EduManager.FindModule(List.GetItemText(Item, 1));
	CString value;
	distroyedit->GetWindowTextW(value);
	if (p != nullptr)
	{
		if (SubItem == 4)//学分
		{
			p->credit = _ttof(value);
			if (p->credit < 0.1)p->credit = 0.1;
			value.Format(L"%.1lf", p->credit);
		}
		else if (SubItem == 5)
		{
			p->hours = _ttoi(value);
			if (p->hours < 1)p->hours = 1;
			value.Format(L"%d", p->hours);
		}

		pList->SetItemText(Item, SubItem, value);
	}

	distroyedit->DestroyWindow();//销毁对象，有创建就要有销毁，不然会报错
}

void TeacherForm::createCcombobox(NM_LISTVIEW* pEditCtrl, CComboBox* createccomboboxobj, int& Item, int& SubItem, bool& havecreat)
{
	Item = pEditCtrl->iItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理
	SubItem = pEditCtrl->iSubItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理
	havecreat = true;
	createccomboboxobj->Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | CBS_OEMCONVERT, CRect(0, 0, 0, 0), this, IDC_COMBOX_CREATEID);
	createccomboboxobj->SetFont(this->GetFont(), FALSE);//设置字体,不设置这里的话上面的字会很突兀的感觉
	createccomboboxobj->SetParent(&List);//将list control设置为父窗口,生成的Ccombobox才能正确定位,这个也很重要
	CRect  EditRect;
	List.GetSubItemRect(e_Item, e_SubItem, LVIR_LABEL, EditRect);//获取单元格的空间位置信息
	EditRect.SetRect(EditRect.left + 1, EditRect.top + 1, EditRect.left + List.GetColumnWidth(e_SubItem) - 1, EditRect.bottom - 1);//+1和-1可以让编辑框不至于挡住列表框中的网格线
	CString strItem = List.GetItemText(e_Item, e_SubItem);//获得相应单元格字符
	createccomboboxobj->SetWindowText(strItem);//将单元格字符显示在编辑框上
	createccomboboxobj->MoveWindow(&EditRect);//将编辑框位置放在相应单元格上
	createccomboboxobj->ShowWindow(SW_SHOW);//显示编辑框在单元格上面
}

void TeacherForm::saveCcombobox(CListCtrl* pList, CComboBox* distroyccomboboxobj, int& Item, int& SubItem)
{
	auto p = EduManager.FindModule(List.GetItemText(Item, 1));
	CString value;
	distroyccomboboxobj->GetLBText(distroyccomboboxobj->GetCurSel(), value);
	if (p != nullptr)
	{
		if (SubItem == 6)//学分
		{
			p->compulsory = value == L"是";
			if (!(p->compulsory))value = L"否";
		}
		pList->SetItemText(Item, SubItem, value);
	}
}

void TeacherForm::OnClickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码


	*pResult = 0;
}


void TeacherForm::OnSelchangeComboxCreateid()
{
	// TODO: 在此添加控件通知处理程序代码


}


void TeacherForm::OnSelendokComboxCreateid()
{
	// TODO: 在此添加控件通知处理程序代码	
	if (haveccomboboxcreate == true)//如果之前创建了下拉列表框就销毁掉
	{
		saveCcombobox(&List, &m_comBox, e_Item, e_SubItem);
		m_comBox.DestroyWindow();//销毁对象，有创建就要有销毁，不然会报错
		haveccomboboxcreate = false;
	}
}


void TeacherForm::OnSelendcancelComboxCreateid()
{
	// TODO: 在此添加控件通知处理程序代码
	if (haveccomboboxcreate == true)//如果之前创建了下拉列表框就销毁掉
	{
		m_comBox.DestroyWindow();//销毁对象，有创建就要有销毁，不然会报错
		haveccomboboxcreate = false;
	}
}


void TeacherForm::OnKillfocusEditCreateid()
{
	// TODO: 在此添加控件通知处理程序代码
	if (haveeditcreate == true)//如果之前创建了编辑框就销毁掉
	{
		distroyEdit(&List, &m_Edit, e_Item, e_SubItem);//销毁单元格编辑框对象
		haveeditcreate = false;
	}
}


void TeacherForm::OnKillfocusComboxCreateid()
{
	// TODO: 在此添加控件通知处理程序代码
}


void TeacherForm::OnBnClickedUserintro()
{
	// TODO: 在此添加控件通知处理程序代码

}


void TeacherForm::OnBnClickedUserintro2()
{
	// TODO: 在此添加控件通知处理程序代码
	MessageBox(L"修改密码");

}


void TeacherForm::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码


}


void TeacherForm::OnBnClickedIntro3()
{
	// TODO: 在此添加控件通知处理程序代码
	EditUser editUser;
	editUser.target.push_back(target);
	if (editUser.DoModal() == IDOK)EduManager.SaveData(EduManager.basicDataPos);
}


void TeacherForm::OnExport()
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
	CString filename = EduManager.FindUser(target->ID)->name + L"老师 (" + target->ID + L") 任教的课程 ";
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


void TeacherForm::OnSelall()
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


BOOL TeacherForm::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (::TranslateAccelerator(GetSafeHwnd(), hAccel, pMsg))

		return true;
	return CDialogEx::PreTranslateMessage(pMsg);
}


void TeacherForm::OnAdd()
{
	// TODO: 在此添加命令处理程序代码
	Module m;
	EduManager.ModuleLib.push_back(m);
	auto p = EduManager.ModuleLib.end() - 1;

	EditModule editModule;
	editModule.target.push_back(&(*p));
	p->ID = EduManager.RandomID(8);
	p->teacherID = target->ID;

	if (editModule.DoModal() == IDOK)
	{
		

		UpdateData(FALSE);

		RefreshWindowMessage();
		Screen();
	}
	else EduManager.ModuleLib.erase(p);

	EduManager.SaveData(EduManager.basicDataPos);
}


void TeacherForm::OnIntro()
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
		DelItem.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_DELETE, MF_BYCOMMAND | MF_DISABLED);
		Intro.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_DISABLED);
		EduManager.SaveData(EduManager.basicDataPos);
	}


}


void TeacherForm::OnDelete()
{
	// TODO: 在此添加命令处理程序代码
	if (!DelItem.IsWindowEnabled())return;
	vector<CString>unDel;
	for (auto p = checked.begin();p != checked.end();p++)
	{
		auto q = EduManager.FindModule(List.GetItemText(*p, 1));
		if (List.GetItemText(*p, 8) == L"否")
		{
			EduManager.DelModule(q->ID);
		}
		else
		{
			CString cs;
			cs.Format(L"%s\t《%s》", q->ID, q->name);
			unDel.push_back(cs);
		}
	}

	RefreshWindowMessage();
	Screen();
	checked.clear();
	DelItem.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_DELETE, MF_BYCOMMAND | MF_DISABLED);
	Intro.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_DISABLED);

	if (!unDel.empty())
	{
		CString cs;
		cs.Format(L"以下%d门课程的成绩已经公布，不可以删除：\n\n", unDel.size());
		for (size_t i = 0; i < unDel.size(); i++)
		{
			cs += unDel.at(i);
			if (i != unDel.size() - 1)cs += '\n';
		}
		MessageBox(cs);
	}


	EduManager.SaveData(EduManager.basicDataPos);
}


void TeacherForm::OnMin()
{
	// TODO: 在此添加命令处理程序代码
	ShowWindow(SW_SHOWMINIMIZED);
}


void TeacherForm::OnClose()
{
	// TODO: 在此添加命令处理程序代码
	OnCancel();
}
