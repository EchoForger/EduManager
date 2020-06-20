// AdminForm.cpp: 实现文件
//

#include "pch.h"
#include "EduManager.h"
#include "AdminForm.h"
#include "afxdialogex.h"
#include "AddModule.h"
#include "EditModule.h"
#include <algorithm>
#include <set>
#include "EditUser.h"
#include "TeacherForm.h"
#include "StudentForm.h"



// AdminForm 对话框

IMPLEMENT_DYNAMIC(AdminForm, CDialogEx)

AdminForm::AdminForm(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADMINFORM, pParent)
	, state_value(_T(""))
	, userInfo_value(_T(""))
	, username_value(_T(""))
	, unSelCom_value(_T(""))
	, searchText_value(_T(""))
{

}

AdminForm::~AdminForm()
{
}

void AdminForm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
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
	DDX_Control(pDX, IDC_LIST, List);
	DDX_Control(pDX, IDC_INTRO, Intro);
	DDX_Control(pDX, IDC_EXPORTDATA, exportData);
	DDX_Control(pDX, IDC_EXPORTDATABASE, exportDataBase);
}


BEGIN_MESSAGE_MAP(AdminForm, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, &AdminForm::OnLvnItemchangedList)
	ON_BN_CLICKED(IDC_ADDMODULE, &AdminForm::OnBnClickedAddmodule)
	ON_BN_CLICKED(IDC_DELMODULE, &AdminForm::OnBnClickedDelmodule)
	ON_WM_RBUTTONDBLCLK()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST, &AdminForm::OnColumnclickList)
	ON_EN_CHANGE(IDC_SEARCHTEXT, &AdminForm::OnEnChangeSearchtext)
	ON_BN_CLICKED(IDC_UNRELEASE, &AdminForm::OnBnClickedCompulsory)
	ON_BN_CLICKED(IDC_QUITABLE, &AdminForm::OnBnClickedQuitable)
	ON_BN_CLICKED(IDC_UNRELEASE, &AdminForm::OnBnClickedUnrelease)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &AdminForm::OnDblclkList)
	ON_BN_CLICKED(IDC_EXPORTDATA, &AdminForm::OnBnClickedExportdata)
	ON_BN_CLICKED(IDC_INTRO, &AdminForm::OnBnClickedIntro)
	ON_STN_CLICKED(IDC_USERINFO, &AdminForm::OnStnClickedUserinfo)
	ON_BN_CLICKED(IDC_INTRO2, &AdminForm::OnBnClickedIntro2)
	ON_NOTIFY(NM_CLICK, IDC_LIST, &AdminForm::OnClickList)
	ON_CBN_SELCHANGE(IDC_COMBOX_CREATEID, &AdminForm::OnSelchangeComboxCreateid)
	ON_CBN_SELENDOK(IDC_COMBOX_CREATEID, &AdminForm::OnSelendokComboxCreateid)
	ON_CBN_SELENDCANCEL(IDC_COMBOX_CREATEID, &AdminForm::OnSelendcancelComboxCreateid)
	ON_EN_KILLFOCUS(IDC_EDIT_CREATEID, &AdminForm::OnKillfocusEditCreateid)
	ON_CBN_KILLFOCUS(IDC_COMBOX_CREATEID, &AdminForm::OnKillfocusComboxCreateid)
	ON_BN_CLICKED(IDC_USERINTRO, &AdminForm::OnBnClickedUserintro)
	ON_BN_CLICKED(IDC_INTRO3, &AdminForm::OnBnClickedIntro3)
	ON_BN_CLICKED(IDC_EXPORTDATABASE, &AdminForm::OnBnClickedExportdatabase)
	ON_BN_CLICKED(IDC_INPORTDATABASE, &AdminForm::OnBnClickedInportdatabase)
	ON_COMMAND(ID_OPEN, &AdminForm::OnOpen)
	ON_COMMAND(ID_SAVEAS, &AdminForm::OnSaveas)
	ON_COMMAND(ID_EXPORT, &AdminForm::OnExport)
	ON_COMMAND(ID_SELALL, &AdminForm::OnSelall)
	ON_COMMAND(ID_ADD, &AdminForm::OnAdd)
	ON_COMMAND(ID_INTRO, &AdminForm::OnIntro)
	ON_COMMAND(ID_DELETE, &AdminForm::OnDelete)
	ON_COMMAND(ID_MIN, &AdminForm::OnMin)
	ON_COMMAND(ID_CLOSE, &AdminForm::OnClose)
END_MESSAGE_MAP()


// AdminForm 消息处理程序


void AdminForm::OnLvnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult)
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


BOOL AdminForm::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	hAccel = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ADMINACCELERATOR));
	
	GetMenu()->GetSubMenu(0)->DeleteMenu(ID_INPORT, MF_BYCOMMAND | MF_DISABLED);

	GetMenu()->GetSubMenu(1)->ModifyMenuW(2, MF_BYPOSITION, ID_ADD, L"新建用户(N)...\tCtrl+N");
	GetMenu()->GetSubMenu(1)->ModifyMenuW(4, MF_BYPOSITION, ID_DELETE, L"删除用户(L)\tDel");

	DelItem.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_DELETE, MF_BYCOMMAND | MF_DISABLED);
	Intro.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_DISABLED);

	ModifyStyleEx(0, WS_EX_APPWINDOW);

	List.SetExtendedStyle(List.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);

	List.InsertColumn(0, L"选择", LVCFMT_CENTER, (int)(0.4 * EduManager.defaultWidth));
	List.InsertColumn(1, L"账号", LVCFMT_LEFT, (int)(1.2 * EduManager.defaultWidth));
	List.InsertColumn(2, L"姓名", LVCFMT_LEFT, (int)(0.8 * EduManager.defaultWidth));
	List.InsertColumn(3, L"类型", LVCFMT_LEFT, (int)(0.8 * EduManager.defaultWidth));
	List.InsertColumn(4, L"*班级", LVCFMT_LEFT, (int)(0.8 * EduManager.defaultWidth));
	List.InsertColumn(5, L"*专业", LVCFMT_LEFT, (int)(1.2 * EduManager.defaultWidth));

	for (size_t i = 0; i < EduManager.UserLib.size(); i++)
	{
		ListVector.push_back(&(EduManager.UserLib.at(i)));
		screened.push_back(screened.size());
	}
	RefreshWindowMessage();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void AdminForm::OnBnClickedAddmodule()
{

	// TODO: 在此添加控件通知处理程序代码
	OnAdd();
}


void AdminForm::OnBnClickedDelmodule()
{
	// TODO: 在此添加控件通知处理程序代码
	if(DelItem.IsWindowEnabled())OnDelete();
}

void AdminForm::RefreshWindowMessage()
{
	if (EduManager.UserLib.size()==0)
		exportDataBase.EnableWindow(0);
	else
		exportDataBase.EnableWindow(1);


	List.DeleteAllItems();

	ListVector.clear();
	for (size_t i = 0; i < EduManager.UserLib.size(); i++)
	{
		ListVector.push_back(&(EduManager.UserLib.at(i)));
	}


	for (size_t i = 0; i < ListVector.size(); i++)
	{
		CString userType("NULL"), classID("NULL"), major("NULL");
		if (ListVector.at(i)->userType == 1)userType = L"管理员";
		else if (ListVector.at(i)->userType == 2)userType = L"教师";
		else if (ListVector.at(i)->userType == 3)userType = L"学生";
		if (ListVector.at(i)->classID != L"NULL")classID = ListVector.at(i)->classID;
		else classID = L"无";
		if (ListVector.at(i)->major != L"NULL")major = ListVector.at(i)->major;
		else major = L"无";

		if (find(screened.begin(), screened.end(), i) < screened.end())
		{
			List.InsertItem(0, L"");
			List.SetItemText(0, 1, ListVector.at(i)->ID);
			List.SetItemText(0, 2, ListVector.at(i)->name);
			List.SetItemText(0, 3, userType);
			List.SetItemText(0, 4, classID);
			List.SetItemText(0, 5, major);
		}

	}
	
	userInfo_value = EduManager.userNow->ID + L"   管理员";
	username_value = EduManager.userNow->name;

	if (ListVector.size())
	{
		state_value.Format(L"用户数量：%d", ListVector.size());
	}
	else state_value = L"当前没有任何用户";

	
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

void AdminForm::OnOK()
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

void AdminForm::OnColumnclickList(NMHDR* pNMHDR, LRESULT* pResult)
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

void AdminForm::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnCancel();
}

void AdminForm::Screen()
{

	UpdateData(TRUE);
	screened.clear();

	checked.clear();

	DelItem.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_DELETE, MF_BYCOMMAND | MF_DISABLED);
	Intro.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_DISABLED);

	for (size_t i = 0; i < ListVector.size(); i++)
	{
		bool pushed = false;
		if (EduManager.Contain(searchText_value, ListVector.at(i)->ID) || EduManager.Contain(searchText_value, ListVector.at(i)->name) || EduManager.Contain(searchText_value, ListVector.at(i)->classID) || EduManager.Contain(searchText_value, ListVector.at(i)->major))
		{
			screened.push_back(i);
			pushed = true;
		}
		if (pushed)continue;
		for (size_t j = 0; j < EduManager.ModuleLib.size(); j++)
		{
			if (EduManager.ModuleLib.at(j).teacherID == ListVector.at(i)->ID)
			{
				if (EduManager.Contain(searchText_value, EduManager.ModuleLib.at(j).ID) || EduManager.Contain(searchText_value, EduManager.ModuleLib.at(j).name))
				{
					screened.push_back(i);
					pushed = true;
					break;
				}
			}
		}
		if (pushed)continue;
		if ((ListVector.at(i)->userType==1&& EduManager.Contain(searchText_value, L"管理员"))|| (ListVector.at(i)->userType == 2 && EduManager.Contain(searchText_value, L"教师"))|| (ListVector.at(i)->userType == 3 && EduManager.Contain(searchText_value, L"学生")))
		{
			screened.push_back(i);
			pushed = true;
		}
		if (pushed)continue;
	}

	RefreshWindowMessage();
}

void AdminForm::OnEnChangeSearchtext()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	Screen();
}


void AdminForm::OnBnClickedCompulsory()
{
	// TODO: 在此添加控件通知处理程序代码
	Screen();
}


void AdminForm::OnBnClickedQuitable()
{
	// TODO: 在此添加控件通知处理程序代码
	Screen();
}


int AdminForm::sort_column = 0; // 记录点击的列
bool AdminForm::method = false;; // 记录比较方法


int CALLBACK AdminForm::MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
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

void AdminForm::OnBnClickedUnrelease()
{
	// TODO: 在此添加控件通知处理程序代码
	Screen();
}


void AdminForm::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
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
		auto p = EduManager.FindUser(List.GetItemText(r, 1));
		if (p->userType == 3)
		{
			e_Item = pEditCtrl->iItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理
			e_SubItem = pEditCtrl->iSubItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理
			createEdit(pEditCtrl, &m_Edit, e_Item, e_SubItem, haveeditcreate);//创建编辑框
		}
		else if (p->userType == 1)
		{
			EditUser editUser;
			editUser.target.push_back(p);
			if (editUser.DoModal() == IDOK)
			{
				RefreshWindowMessage();
				Screen();
			}
		}
		else if (p->userType == 2)
		{
			TeacherForm teacherForm;
			teacherForm.target = p;
			teacherForm.DoModal();
		}
	}
	else
	{
		auto p = EduManager.FindUser(List.GetItemText(r, 1));
		if (p->userType == 1)
		{
			EditUser editUser;
			editUser.target.push_back(p);
			if (editUser.DoModal() == IDOK)
			{
				RefreshWindowMessage();
				Screen();
			}
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


	*pResult = 0;
}


void AdminForm::OnBnClickedExportdata()
{
	// TODO: 在此添加控件通知处理程序代码
	OnExport();
}


void AdminForm::OnBnClickedIntro()
{
	// TODO: 在此添加控件通知处理程序代码

	if (Intro.IsWindowEnabled())OnIntro();
}


void AdminForm::OnStnClickedUserinfo()
{
	// TODO: 在此添加控件通知处理程序代码
}


void AdminForm::OnBnClickedIntro2()
{
	// TODO: 在此添加控件通知处理程序代码
}


void AdminForm::createEdit(NM_LISTVIEW* pEditCtrl, CEdit* createdit, int& Item, int& SubItem, bool& havecreat)
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

void AdminForm::distroyEdit(CListCtrl* pList, CEdit* distroyedit, int& Item, int& SubItem)
{
	auto p = EduManager.FindUser(List.GetItemText(Item, 1));
	CString value;
	distroyedit->GetWindowTextW(value);
	if (p != nullptr)
	{
		if (SubItem == 4)//班级
		{
			p->classID = value;
		}
		else if (SubItem == 5)//专业
		{
			p->major = value;
		}

		pList->SetItemText(Item, SubItem, value);
	}

	distroyedit->DestroyWindow();//销毁对象，有创建就要有销毁，不然会报错
}

void AdminForm::createCcombobox(NM_LISTVIEW* pEditCtrl, CComboBox* createccomboboxobj, int& Item, int& SubItem, bool& havecreat)
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

void AdminForm::saveCcombobox(CListCtrl* pList, CComboBox* distroyccomboboxobj, int& Item, int& SubItem)
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

void AdminForm::OnClickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码


	*pResult = 0;
}


void AdminForm::OnSelchangeComboxCreateid()
{
	// TODO: 在此添加控件通知处理程序代码


}


void AdminForm::OnSelendokComboxCreateid()
{
	// TODO: 在此添加控件通知处理程序代码	
	if (haveccomboboxcreate == true)//如果之前创建了下拉列表框就销毁掉
	{
		saveCcombobox(&List, &m_comBox, e_Item, e_SubItem);
		m_comBox.DestroyWindow();//销毁对象，有创建就要有销毁，不然会报错
		haveccomboboxcreate = false;
	}
}


void AdminForm::OnSelendcancelComboxCreateid()
{
	// TODO: 在此添加控件通知处理程序代码
	if (haveccomboboxcreate == true)//如果之前创建了下拉列表框就销毁掉
	{
		m_comBox.DestroyWindow();//销毁对象，有创建就要有销毁，不然会报错
		haveccomboboxcreate = false;
	}
}


void AdminForm::OnKillfocusEditCreateid()
{
	// TODO: 在此添加控件通知处理程序代码
	if (haveeditcreate == true)//如果之前创建了编辑框就销毁掉
	{
		distroyEdit(&List, &m_Edit, e_Item, e_SubItem);//销毁单元格编辑框对象
		haveeditcreate = false;
	}
}


void AdminForm::OnKillfocusComboxCreateid()
{
	// TODO: 在此添加控件通知处理程序代码
}


void AdminForm::OnBnClickedUserintro()
{
	// TODO: 在此添加控件通知处理程序代码

}


void AdminForm::OnBnClickedUserintro2()
{
	// TODO: 在此添加控件通知处理程序代码

}


void AdminForm::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码


}


void AdminForm::OnBnClickedIntro3()
{
	// TODO: 在此添加控件通知处理程序代码
	EditUser editUser;
	editUser.target.push_back(EduManager.userNow);
	if (editUser.DoModal() == IDOK)EduManager.SaveData(EduManager.basicDataPos);

	userInfo_value = EduManager.userNow->ID + L"   管理员";
	username_value = EduManager.userNow->name;
	UpdateData(FALSE);
}


void AdminForm::OnBnClickedExportdatabase()
{
	// TODO: 在此添加控件通知处理程序代码
	OnSaveas();
}


void AdminForm::OnBnClickedInportdatabase()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOpen();
}


BOOL AdminForm::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (::TranslateAccelerator(GetSafeHwnd(), hAccel, pMsg))

		return true;
	return CDialogEx::PreTranslateMessage(pMsg);
}

void AdminForm::OnOpen()
{
	// TODO: 在此添加命令处理程序代码
	CString strFile = _T("");
	CFileDialog	dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("EduManager 数据库(*.emgdata)|*.emgdata|All Files (*.*)|*.*||"), NULL);
	dlgFile.DoModal();
	strFile = dlgFile.GetPathName();
	if (strFile != L"")
	{
		if (IDOK == MessageBox(L"读取新的数据库将覆盖当前数据库，是否继续？", L"读取数据库", IDOK))
		{
			EduManager.UserLib.clear();
			EduManager.ModuleLib.clear();
			string s = CT2A(strFile);
			EduManager.ReadData(s);
			ListVector.clear();
			screened.clear();
			for (size_t i = 0; i < EduManager.UserLib.size(); i++)
			{
				ListVector.push_back(&(EduManager.UserLib.at(i)));
				screened.push_back(screened.size());
			}
			CString cs;
			cs.Format(L"已读取%d个用户信息和%d个课程信息", EduManager.UserLib.size(), EduManager.ModuleLib.size());
			MessageBox(cs);
			RefreshWindowMessage();

			EduManager.SaveData(EduManager.basicDataPos);
		}
	}
}


void AdminForm::OnSaveas()
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
	CString filename = L"保存的数据库 ";
	if (screened.size() != ListVector.size())filename += L"(部分) ";
	filename += str;
	CString szFilter = L"EduManager 数据库(*.emgdata)";
	CFileDialog dlg(FALSE, _T("emgdata"), filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	if (dlg.DoModal() != IDOK)return;
	CString strFileName;
	strFileName = dlg.GetPathName();
	DWORD dwRe = GetFileAttributes(strFileName);
	if (dwRe != (DWORD)-1)DeleteFile(strFileName);
	string s = CT2A(strFileName.GetString());
	EduManager.SaveData(s);
}


void AdminForm::OnExport()
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
	CString filename = L"所有用户 ";
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
	TCHAR lpBuffer[256];
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


void AdminForm::OnSelall()
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


void AdminForm::OnAdd()
{
	// TODO: 在此添加命令处理程序代码

	User m;
	CString userNowID = EduManager.userNow->ID;
	EduManager.UserLib.push_back(m);
	if (EduManager.userNow->ID != EduManager.superAdmin.ID)EduManager.userNow = EduManager.FindUser(userNowID);
	auto p = EduManager.UserLib.end() - 1;

	EditUser editUser;
	editUser.target.push_back(&(*p));
	editUser.newUser = true;
	p->ID = EduManager.RandomID(11);
	p->userType = 3;
	p->remPsw = 0;
	p->remUsn = 0;
	p->lastLogin = 0;
	if (editUser.DoModal() == IDOK)
	{
		UpdateData(FALSE);
		RefreshWindowMessage();
		Screen();
	}
	else EduManager.UserLib.erase(p);

	EduManager.SaveData(EduManager.basicDataPos);
}


void AdminForm::OnIntro()
{
	// TODO: 在此添加命令处理程序代码
	if (!Intro.IsWindowEnabled())return;
	EditUser editUser;
	for (auto p = checked.begin();p != checked.end();p++)
	{
		auto q = EduManager.FindUser(List.GetItemText(*p, 1));
		editUser.target.push_back(q);
	}
	if (editUser.DoModal() == IDOK)
	{
		RefreshWindowMessage();
		Screen();
		checked.clear();
		DelItem.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_DELETE, MF_BYCOMMAND | MF_DISABLED);
		Intro.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_DISABLED);
		EduManager.SaveData(EduManager.basicDataPos);
	}
}


void AdminForm::OnDelete()
{
	// TODO: 在此添加命令处理程序代码
	if (!DelItem.IsWindowEnabled())return;
	vector<CString>unDel;
	for (auto p = checked.begin();p != checked.end();p++)
	{
		auto q = EduManager.FindUser(List.GetItemText(*p, 1));
		if (&(*q) != EduManager.userNow)
		{
			EduManager.DelUser(q->ID);
		}
		else
		{
			CString cs;
			cs.Format(L"%s\t%s", q->ID, q->name);
			unDel.push_back(cs);
		}
	}

	if (!unDel.empty())
	{
		CString cs;
		cs.Format(L"以下%d个用户不可以删除：\n\n", unDel.size());
		for (size_t i = 0; i < unDel.size(); i++)
		{
			cs += unDel.at(i);
			if (i != unDel.size() - 1)cs += '\n';
		}
		MessageBox(cs);
	}
	if (unDel.size() != checked.size())
	{
		RefreshWindowMessage();
		Screen();
		EduManager.SaveData(EduManager.basicDataPos);
	}

	checked.clear();
	DelItem.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_DELETE, MF_BYCOMMAND | MF_DISABLED);
	Intro.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_DISABLED);

}


void AdminForm::OnMin()
{
	// TODO: 在此添加命令处理程序代码
	ShowWindow(SW_SHOWMINIMIZED);
}


void AdminForm::OnClose()
{
	// TODO: 在此添加命令处理程序代码
	OnCancel();
}
