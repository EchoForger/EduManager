// StudentForm.cpp: 实现文件
//

#include "pch.h"
#include "EduManager.h"
#include "StudentForm.h"
#include "afxdialogex.h"
#include "AddModule.h"
#include "EditModule.h"
#include "EditUser.h"
#include <algorithm>
#include <set>



// StudentForm 对话框

IMPLEMENT_DYNAMIC(StudentForm, CDialogEx)

StudentForm::StudentForm(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_STUDENTFORM, pParent)
	, state_value(_T(""))
	, userInfo_value(_T(""))
	, username_value(_T(""))
	, unSelCom_value(_T(""))
	, searchText_value(_T(""))
	, compulsory_value(FALSE)
	, quitAble_value(FALSE)
{

}

StudentForm::~StudentForm()
{
}

void StudentForm::DoDataExchange(CDataExchange* pDX)
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
	DDX_Text(pDX, IDC_UNSELCOM, unSelCom_value);
	DDX_Text(pDX, IDC_SEARCHTEXT, searchText_value);
	DDX_Check(pDX, IDC_COMPULSORY, compulsory_value);
	DDX_Check(pDX, IDC_QUITABLE, quitAble_value);
	DDX_Control(pDX, IDC_EXPORTDATA, exportData);
	DDX_Control(pDX, IDC_INTRO, Intro);
}


BEGIN_MESSAGE_MAP(StudentForm, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, &StudentForm::OnLvnItemchangedList)
	ON_BN_CLICKED(IDC_ADDMODULE, &StudentForm::OnBnClickedAddmodule)
	ON_BN_CLICKED(IDC_DELMODULE, &StudentForm::OnBnClickedDelmodule)
	ON_WM_RBUTTONDBLCLK()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST, &StudentForm::OnColumnclickList)
	ON_EN_CHANGE(IDC_SEARCHTEXT, &StudentForm::OnEnChangeSearchtext)
	ON_BN_CLICKED(IDC_COMPULSORY, &StudentForm::OnBnClickedCompulsory)
	ON_BN_CLICKED(IDC_QUITABLE, &StudentForm::OnBnClickedQuitable)
	ON_BN_CLICKED(IDC_EXPORTDATA, &StudentForm::OnBnClickedExportdata)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &StudentForm::OnDblclkList)
	ON_BN_CLICKED(IDC_INTRO, &StudentForm::OnBnClickedIntro)
	ON_BN_CLICKED(IDC_INTRO3, &StudentForm::OnBnClickedIntro3)
	ON_COMMAND(ID_SELALL, &StudentForm::OnSelall)
	ON_COMMAND(ID_ADD, &StudentForm::OnAdd)
	ON_COMMAND(ID_INTRO, &StudentForm::OnIntro)
	ON_COMMAND(ID_DELETE, &StudentForm::OnDelete)
	ON_COMMAND(ID_MIN, &StudentForm::OnMin)
	ON_COMMAND(ID_CLOSE, &StudentForm::OnClose)
	ON_COMMAND(ID_EXPORT, &StudentForm::OnExport)
	ON_STN_CLICKED(IDC_UNSELCOM, &StudentForm::OnStnClickedUnselcom)
END_MESSAGE_MAP()


// StudentForm 消息处理程序


void StudentForm::OnLvnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult)
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


BOOL StudentForm::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	hAccel = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ADMINACCELERATOR));
	GetMenu()->GetSubMenu(0)->DeleteMenu(ID_OPEN, MF_BYCOMMAND | MF_DISABLED);
	GetMenu()->GetSubMenu(0)->DeleteMenu(ID_SAVEAS, MF_BYCOMMAND | MF_DISABLED);
	GetMenu()->GetSubMenu(0)->DeleteMenu(ID_INPORT, MF_BYCOMMAND | MF_DISABLED);

	GetMenu()->GetSubMenu(1)->ModifyMenuW(2, MF_BYPOSITION, ID_ADD, L"添加课程(N)...\tCtrl+N");
	GetMenu()->GetSubMenu(1)->ModifyMenuW(4, MF_BYPOSITION, ID_DELETE, L"删除课程(L)\tDel");

	DelItem.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_DELETE, MF_BYCOMMAND | MF_DISABLED);
	Intro.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_DISABLED);

	ModifyStyleEx(0, WS_EX_APPWINDOW);

	List.SetExtendedStyle(List.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);

	List.InsertColumn(0, L"选择", LVCFMT_CENTER, (int)(0.4 * EduManager.defaultWidth));
	List.InsertColumn(1, L"课程代码", LVCFMT_LEFT, (int)(0.8 * EduManager.defaultWidth));
	List.InsertColumn(2, L"课程名称", LVCFMT_LEFT, (int)(1.5 * EduManager.defaultWidth));
	List.InsertColumn(3, L"教师", LVCFMT_LEFT, (int)(0.7 * EduManager.defaultWidth));
	List.InsertColumn(4, L"学分", LVCFMT_LEFT, (int)(0.5 * EduManager.defaultWidth));
	List.InsertColumn(5, L"学时", LVCFMT_LEFT, (int)(0.5 * EduManager.defaultWidth));
	List.InsertColumn(6, L"必修", LVCFMT_LEFT, (int)(0.5 * EduManager.defaultWidth));
	List.InsertColumn(7, L"参与人数", LVCFMT_LEFT, (int)(0.7 * EduManager.defaultWidth));
	List.InsertColumn(8, L"成绩", LVCFMT_LEFT, (int)(0.5 * EduManager.defaultWidth));
	List.InsertColumn(9, L"绩点", LVCFMT_LEFT, (int)(0.5 * EduManager.defaultWidth));
	List.InsertColumn(10, L"通过", LVCFMT_LEFT, (int)(0.5 * EduManager.defaultWidth));
	List.InsertColumn(11, L"评价", LVCFMT_LEFT, (int)(0.5 * EduManager.defaultWidth));

	for (size_t i = 0; i < EduManager.ModuleLib.size(); i++)
	{
		if (EduManager.ModuleLib.at(i).FindStudent(target->ID) != nullptr)
		{
			ListVector.push_back(&(EduManager.ModuleLib.at(i)));
			screened.push_back(screened.size());
		}
	}
	RefreshWindowMessage();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void StudentForm::OnBnClickedAddmodule()
{
	
	// TODO: 在此添加控件通知处理程序代码
	OnAdd();
	
}


void StudentForm::OnBnClickedDelmodule()
{
	// TODO: 在此添加控件通知处理程序代码
	if(DelItem.IsWindowEnabled())OnDelete();
}

void StudentForm::RefreshWindowMessage() 
{
	List.DeleteAllItems();

	totalCredit = 0;
	avaCredit = 0;
	totalGPA = 0;

	ListVector.clear();
	for (size_t i = 0; i < EduManager.ModuleLib.size(); i++)
	{
		if (EduManager.ModuleLib.at(i).FindStudent(target->ID) != nullptr)
		{
			ListVector.push_back(&(EduManager.ModuleLib.at(i)));
		}
	}


	for (size_t i = 0; i < ListVector.size(); i++)
	{
		CString teacherName("NULL"), credit("NULL"), hours("NULL"), compulsory("NULL"),
			attendSz("NULL"), Score("NULL"), GPA("NULL"), pass("NULL"),comment(L"NULL");

		teacherName = EduManager.FindUser(ListVector.at(i)->teacherID)->name;
		credit.Format(L"%.1lf", ListVector.at(i)->credit);
		hours.Format(L"%d", ListVector.at(i)->hours);
		if (ListVector.at(i)->compulsory) compulsory = "是";
		else compulsory = "否";
		attendSz.Format(L"%d", ListVector.at(i)->attend.size());
		double score = ListVector.at(i)->FindStudent(target->ID)->score;
		if (score != -1)
		{
			Score.Format(L"%.1lf", score);
			double gpa = (score - 50) / 10;
			if (gpa >= 1)
			{
				GPA.Format(L"%.1lf", gpa);
				pass = L"是";
			}
			else
			{
				GPA = L"无";
				pass = L"否";
			}
		}
		else
		{
			Score = L"无";
			GPA = L"无";
			pass = L"无";
		}
		double reScore = ListVector.at(i)->FindStudent(target->ID)->reScore;
		if (reScore != -1) comment.Format(L"%.1lf", reScore);
		else comment = "否";

		if (find(screened.begin(),screened.end(), i)< screened.end())
		{
			List.InsertItem(0, L"");
			List.SetItemText(0, 1, ListVector.at(i)->ID);
			List.SetItemText(0, 2, ListVector.at(i)->name);
			List.SetItemText(0, 3, teacherName);
			List.SetItemText(0, 4, credit);
			List.SetItemText(0, 5, hours);
			List.SetItemText(0, 6, compulsory);
			List.SetItemText(0, 7, attendSz);
			List.SetItemText(0, 8, Score);
			List.SetItemText(0, 9, GPA);
			List.SetItemText(0, 10, pass);
			List.SetItemText(0, 11, comment);
		}


		totalCredit += ListVector.at(i)->credit;
		if (score != -1)
		{
			avaCredit += ListVector.at(i)->credit;
			totalGPA += score * ListVector.at(i)->credit;
		}




	}

	CString totalGPAstr;

	if (avaCredit)totalGPA /= avaCredit;
	totalGPA = (totalGPA - 60) * 0.1 + 1;
	if (totalGPA >= 0)totalGPAstr.Format(L"%.3lf", totalGPA);
	else totalGPAstr = L"无";

	userInfo_value = target->ID  + L"   学生";
	username_value = target->name + "   " + target->major + "   " + target->classID;

	if (totalCredit) 
	{
		state_value.Format(L"已选课程：%d    已选学分：%.1lf    综合学分绩点：", ListVector.size(), totalCredit);
		state_value += totalGPAstr;
	}
	else state_value = L"当前没有选择任何课程";

	set<CString>unSelectedCompulsory;
	for (size_t i = 0; i < EduManager.ModuleLib.size(); i++)
	{
		if (!EduManager.ModuleLib.at(i).AttendedSame(target) && EduManager.ModuleLib.at(i).compulsory)
		{
			CString cs;
			cs.Format(L"《%s》", EduManager.ModuleLib.at(i).name);
			unSelectedCompulsory.insert(cs);
		}

	}
	if (unSelectedCompulsory.size() != 0)unSelCom_value.Format(L"有%d门必修课程尚未参加",unSelectedCompulsory.size());
	else unSelCom_value = L"";
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

void StudentForm::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}

void StudentForm::OnColumnclickList(NMHDR* pNMHDR, LRESULT* pResult)
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

void StudentForm::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnCancel();
}

void StudentForm::Screen()
{
	UpdateData(TRUE);

	checked.clear();
	screened.clear();

	DelItem.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_DELETE, MF_BYCOMMAND | MF_DISABLED);
	Intro.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_DISABLED);

	for (size_t i = 0; i < ListVector.size(); i++)
	{
		if (EduManager.Contain(searchText_value, ListVector.at(i)->ID) || EduManager.Contain(searchText_value, ListVector.at(i)->name) || EduManager.Contain(searchText_value, EduManager.FindUser(ListVector.at(i)->teacherID)->name) || EduManager.Contain(searchText_value, ListVector.at(i)->teacherID))
		{
			if (!compulsory_value || !ListVector.at(i)->compulsory)
			{
				if (!quitAble_value || ListVector.at(i)->FindStudent(target->ID)->score==-1)
				{
					screened.push_back(i);
				}
			}
		}
	}

	RefreshWindowMessage();
}

void StudentForm::OnEnChangeSearchtext()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	Screen();
}


void StudentForm::OnBnClickedCompulsory()
{
	// TODO: 在此添加控件通知处理程序代码
	Screen();
}


void StudentForm::OnBnClickedQuitable()
{
	// TODO: 在此添加控件通知处理程序代码
	Screen();
}


int StudentForm::sort_column = 0; // 记录点击的列
bool StudentForm::method = false;; // 记录比较方法


int CALLBACK StudentForm::MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// 从参数中提取所需比较lc的两行数据
	int row1 = (int)lParam1;
	int row2 = (int)lParam2;
	CListCtrl * lc = (CListCtrl*)lParamSort;
	
	CString lp1 = lc->GetItemText(row1, sort_column);
	CString lp2 = lc->GetItemText(row2, sort_column);
	
	// 比较，对不同的列，不同比较，注意记录前一次排序方向，下一次要相反排序
	if ( sort_column == 4 || sort_column == 5 || sort_column == 7 || sort_column == 8 || sort_column == 9)  //根据列的数据类型选择比较的类型
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

void StudentForm::OnBnClickedExportdata()
{
	// TODO: 在此添加控件通知处理程序代码
	OnExport();
}


void StudentForm::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE)pNMHDR;//将传进来的消息转换成LPNMITEMACTIVAT
	auto r = temp->iItem;//获得行号
	auto c = temp->iSubItem;//获得列号

	if (r == -1)return;

	EditModule editModule;
	editModule.target.push_back(EduManager.FindModule(List.GetItemText(r, 1)));
	if (editModule.DoModal() == IDOK)RefreshWindowMessage();

	RefreshWindowMessage();

	*pResult = 0;
}


void StudentForm::OnBnClickedIntro()
{
	// TODO: 在此添加控件通知处理程序代码
	if (Intro.IsWindowEnabled())OnIntro();

	
}


void StudentForm::OnBnClickedIntro3()
{
	// TODO: 在此添加控件通知处理程序代码
	EditUser editUser;
	editUser.target.push_back(target);
	if (editUser.DoModal() == IDOK)EduManager.SaveData(EduManager.basicDataPos);

	userInfo_value = EduManager.userNow->ID + L"   学生";
	username_value = EduManager.userNow->name;
	UpdateData(FALSE);
}


void StudentForm::OnSelall()
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


BOOL StudentForm::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (::TranslateAccelerator(GetSafeHwnd(), hAccel, pMsg))

		return true;
	return CDialogEx::PreTranslateMessage(pMsg);
}


void StudentForm::OnAdd()
{
	// TODO: 在此添加命令处理程序代码
	AddModule addModule;
	addModule.target = target;
	if (addModule.DoModal() == IDOK)
	{
		
		compulsory_value = false;
		quitAble_value = false;

		UpdateData(FALSE);

		RefreshWindowMessage();
		Screen();
	}
	EduManager.SaveData(EduManager.basicDataPos);
}


void StudentForm::OnIntro()
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
	}
}


void StudentForm::OnDelete()
{
	// TODO: 在此添加命令处理程序代码
	if (!DelItem.IsWindowEnabled())return;
	vector<CString>unDel;
	for (auto p = checked.begin();p != checked.end();p++)
	{
		auto q = EduManager.FindModule(List.GetItemText(*p, 1));
		if (q->FindStudent(target->ID)->score == -1)
		{
			q->DelStudent(target->ID);
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


void StudentForm::OnMin()
{
	// TODO: 在此添加命令处理程序代码
	ShowWindow(SW_SHOWMINIMIZED);
}


void StudentForm::OnClose()
{
	// TODO: 在此添加命令处理程序代码
	OnCancel();
}


void StudentForm::OnExport()
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
	CString filename = EduManager.FindUser(target->ID)->name + L"同学 (" + target->ID + L") 参与的课程 ";
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


void StudentForm::OnStnClickedUnselcom()
{
	// TODO: Add your control notification handler code here
}
