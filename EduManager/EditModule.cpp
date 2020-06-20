// EditModule.cpp: 实现文件
//

#include "pch.h"
#include "EduManager.h"
#include "EditModule.h"
#include "AddStudent.h"
#include "EditUser.h"
#include "afxdialogex.h"
#include <set>
#include "Comment.h"
#include "ViewComment.h"
#include "TeacherForm.h"

// EditModule 对话框

IMPLEMENT_DYNAMIC(EditModule, CDialogEx)

EditModule::EditModule(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EDITMODULE, pParent)
	, moduleName_value(_T(""))
	, credit_value(_T(""))
	, hours_value(_T(""))
	, searchText_value(_T(""))
	, state_value(_T(""))
	, compulsory_value(TRUE)
	, moduleID_value(_T(""))
	, unrelease_value(FALSE)
	, pass_value(FALSE)
	, teacherID_value(_T(""))
{

}

EditModule::~EditModule()
{
}

class EditModuleEditUser :public EditUser
{
public:
	vector<ListItemClass>* parentVector;
	void OnOK()
	{
		UpdateData(TRUE);
		if (score_value != L"多种")
		{
			for (auto p = target.begin();p != target.end();p++)
			{
				for (auto q = parentVector->begin();q != parentVector->end();q++)
				{
					if (q->pUser == *p)
					{
						double score;
						if (score_value == L"无"||score_value == L"")score = -1;
						else if (score_value == L"-1")score = 0;
						else
						{
							score = _ttof(score_value);
							if (score > 100)score = 100;
							if (score < 0)score = 0;
						}
						q->score = score;
					}
				}
			}
		}
		CDialog::OnOK();
	}
	BOOL OnInitDialog()
	{
		CDialogEx::OnInitDialog();

		moduleName.SetReadOnly(1);
		userID.SetReadOnly(1);
		if (!newUser)userType.EnableWindow(0);

		userType.AddString(L"管理员");
		userType.AddString(L"教师");
		userType.AddString(L"学生");

		if (EduManager.userNow->userType == 1)
		{
			if (target.size() != 1 || target.at(0)->userType == 1)
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



		auto p = parentVector->begin();
		for (;p!= parentVector->end();p++)
		{
			if (p->pUser->ID==target.at(0)->ID)
			{
				break;
			}
		}

		name_value = p->pUser->name;
		userID_value = p->pUser->ID;
		password_value = p->pUser->password;
		classID_value = p->pUser->classID;
		major_value = p->pUser->major;
		if (p->pUser->userType == 1)userType_value = L"管理员";
		else if (p->pUser->userType == 2)userType_value = L"教师";
		else if (p->pUser->userType == 3)userType_value = L"学生";

		if (p->pUser->userType != 3)
		{
			major.SetReadOnly(1);
			classID.SetReadOnly(1);
		}

		if (moduleTarget != nullptr)
		{
			CString Score(L"NULL");
			double sc = p->score;
			if (sc != -1)Score.Format(L"%.1lf", sc);
			else Score = L"无";
			moduleName_value = moduleTarget->name;
			score_value = Score;
		}
		else score.SetReadOnly(1);

		for (size_t i = 1; i < target.size(); i++)
		{

			auto q = parentVector->begin();
			for (;q != parentVector->end();q++)
			{
				if (q->pUser->ID == target.at(i)->ID)
				{
					break;
				}
			}

			if (q->pUser->userType != 3)
			{
				major.SetReadOnly(1);
				classID.SetReadOnly(1);
			}

			CString userTypeStr(L"NULL");
			if (q->pUser->userType == 1)userTypeStr = L"管理员";
			else if (q->pUser->userType == 2)userTypeStr = L"教师";
			else if (q->pUser->userType == 3)userTypeStr = L"学生";

			name_value = q->pUser->name == name_value ? q->pUser->name : L"多种";
			major_value = q->pUser->major == major_value ? q->pUser->major : L"多种";
			userID_value = q->pUser->ID == userID_value ? q->pUser->ID : L"多种";
			classID_value = q->pUser->classID == classID_value ? q->pUser->classID : L"多种";
			password_value = q->pUser->password == password_value ? q->pUser->password : L"多种";
			userType_value = userTypeStr == userType_value ? userTypeStr : L"多种";

			if (moduleTarget != nullptr)
			{
				CString  Score(L"NULL");
				double sc = q->score;
				if (sc != -1)Score.Format(L"%.1lf", sc);
				else Score = L"无";
				score_value = score_value == Score ? Score : L"多种";
			}

			if (q->pUser->userType != 3)
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

		RefreshWindowMessage();

		return TRUE;
	}
};


void EditModule::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MODULENAME, moduleName_value);
	DDX_Text(pDX, IDC_CREDIT, credit_value);
	DDX_Text(pDX, IDC_HOURS, hours_value);
	DDX_Check(pDX, IDC_COMPULSORY, compulsory_value);
	DDX_Control(pDX, IDC_DELSTUDENT, DelItem);
	DDX_Text(pDX, IDC_SEARCHTEXT, searchText_value);
	DDX_Control(pDX, IDC_LIST, List);
	DDX_Text(pDX, IDC_STATE, state_value);
	DDX_Control(pDX, IDC_EDITMODULE, B_EditModule);
	DDX_Text(pDX, IDC_MODUELLEID, moduleID_value);
	DDX_Check(pDX, IDC_UNRELEASE2, unrelease_value);
	DDX_Check(pDX, IDC_PASS, pass_value);
	DDX_Control(pDX, IDC_EXPORTDATA, exportData);
	DDX_Control(pDX, IDC_INPORTDATA, inportData);
	DDX_Control(pDX, IDC_ADDSTUDENT, B_AddStudent);
	DDX_Control(pDX, IDC_SEARCHTEXT, searchText);
	DDX_Control(pDX, IDC_UNRELEASE2, unrelease);
	DDX_Control(pDX, IDC_PASS, pass);
	DDX_Control(pDX, IDC_MODUELLEID, moduleID);
	DDX_Control(pDX, IDC_MODUELLEID2, teacherID);
	DDX_Control(pDX, IDC_MODULENAME, moduleName);
	DDX_Control(pDX, IDC_CREDIT, credit);
	DDX_Control(pDX, IDC_HOURS, hours);
	DDX_Control(pDX, IDC_COMPULSORY, compulsory);
	DDX_Control(pDX, IDC_INTRO, Intro);
	DDX_Text(pDX, IDC_MODUELLEID2, teacherID_value);
	DDX_Control(pDX, IDC_COMMENT, comment);
	DDX_Control(pDX, IDC_HOMEPAGE, homepage);
}


BEGIN_MESSAGE_MAP(EditModule, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, &EditModule::OnLvnItemchangedListuser)
	ON_BN_CLICKED(IDC_ADDMODULE, &EditModule::OnBnClickedAdduser)
	ON_EN_CHANGE(IDC_SEARCHTEXT, &EditModule::OnEnChangeSearchtext)
	ON_BN_CLICKED(IDC_ATTENDABLE, &EditModule::OnBnClickedAttendable)
	ON_BN_CLICKED(IDC_COMPULSORY, &EditModule::OnBnClickedCompulsory)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST, &EditModule::OnColumnclickListuser)
	ON_BN_CLICKED(IDC_ADDSTUDENT, &EditModule::OnBnClickedAddstudent)
	ON_EN_CHANGE(IDC_MODULENAME, &EditModule::OnEnChangeModulename)
	ON_EN_CHANGE(IDC_CREDIT, &EditModule::OnEnChangeCredit)
	ON_EN_CHANGE(IDC_HOURS, &EditModule::OnEnChangeHours)
	ON_BN_CLICKED(IDC_DELSTUDENT, &EditModule::OnBnClickedDelstudent)
	ON_BN_CLICKED(IDC_EDITMODULE, &EditModule::OnBnClickedEditmodule)
	ON_BN_CLICKED(IDC_PASS, &EditModule::OnBnClickedPass)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &EditModule::OnDblclkList)
	ON_NOTIFY(NM_CLICK, IDC_LIST, &EditModule::OnClickList)
	ON_BN_CLICKED(IDC_UNRELEASE2, &EditModule::OnBnClickedUnrelease2)
	ON_BN_CLICKED(IDC_INPORTDATA, &EditModule::OnBnClickedInportdata)
	ON_BN_CLICKED(IDC_EXPORTDATA, &EditModule::OnBnClickedExportdata)
	ON_BN_CLICKED(IDC_INTRO, &EditModule::OnBnClickedIntro)
	ON_BN_CLICKED(IDC_COMMENT, &EditModule::OnBnClickedComment)
	ON_BN_CLICKED(IDC_HOMEPAGE, &EditModule::OnBnClickedHomepage)
	ON_COMMAND(ID_SELALL, &EditModule::OnSelall)
	ON_COMMAND(ID_MIN, &EditModule::OnMin)
	ON_COMMAND(ID_CLOSE, &EditModule::OnClose)
	ON_COMMAND(ID_INPORT, &EditModule::OnInport)
	ON_COMMAND(ID_EXPORT, &EditModule::OnExport)
	ON_COMMAND(ID_ADD, &EditModule::OnAdd)
	ON_COMMAND(ID_INTRO, &EditModule::OnIntro)
	ON_COMMAND(ID_DELETE, &EditModule::OnDelete)
END_MESSAGE_MAP()


// EditModule 消息处理程序


void EditModule::OnLvnItemchangedListuser(NMHDR* pNMHDR, LRESULT* pResult)
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
		if (EduManager.userNow->userType != 3)
		{
			DelItem.EnableWindow(1);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_DELETE, MF_BYCOMMAND | MF_ENABLED);
			Intro.EnableWindow(1);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_ENABLED);
		}
	}
	else
	{
		DelItem.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_DELETE, MF_BYCOMMAND | MF_DISABLED);
		Intro.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_DISABLED);
	}

	UpdateData(FALSE);

	*pResult = 0;
}


BOOL EditModule::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	hAccel = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ADMINACCELERATOR));
	GetMenu()->GetSubMenu(0)->DeleteMenu(ID_OPEN, MF_BYCOMMAND | MF_DISABLED);
	GetMenu()->GetSubMenu(0)->DeleteMenu(ID_SAVEAS, MF_BYCOMMAND | MF_DISABLED);

	GetMenu()->GetSubMenu(1)->ModifyMenuW(2, MF_BYPOSITION, ID_ADD, L"添加学生(N)...\tCtrl+N");
	GetMenu()->GetSubMenu(1)->ModifyMenuW(4, MF_BYPOSITION, ID_DELETE, L"删除学生(L)\tDel");

	DelItem.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_DELETE, MF_BYCOMMAND | MF_DISABLED);
	Intro.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_DISABLED);
	moduleID.SetReadOnly(1);


	if (EduManager.userNow->userType == 1)
	{
		comment.SetWindowTextW(L"查看评价");
	}
	else if (EduManager.userNow->userType == 2)
	{
		teacherID.SetReadOnly(1);
		comment.SetWindowTextW(L"查看评价");
		homepage.EnableWindow(0);
	}
	else if (EduManager.userNow->userType == 3)
	{
		B_AddStudent.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_ADD, MF_BYCOMMAND | MF_DISABLED);
		searchText.SetReadOnly(1);;
		unrelease.EnableWindow(0);
		pass.EnableWindow(0);
		teacherID.SetReadOnly(1);
		moduleName.SetReadOnly(1);
		credit.SetReadOnly(1);
		hours.SetReadOnly(1);
		compulsory.EnableWindow(0);
		exportData.EnableWindow(0);GetMenu()->GetSubMenu(0)->EnableMenuItem(ID_EXPORT, MF_BYCOMMAND | MF_DISABLED);
		inportData.EnableWindow(0);GetMenu()->GetSubMenu(0)->EnableMenuItem(ID_INPORT, MF_BYCOMMAND | MF_DISABLED);
		List.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_SELALL, MF_BYCOMMAND | MF_DISABLED);
		B_EditModule.EnableWindow(0);
		comment.SetWindowTextW(L"评价");
		homepage.EnableWindow(0);
	}

	List.SetExtendedStyle(List.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);

	List.InsertColumn(0, L"选择", LVCFMT_CENTER, (int)(0.4 * EduManager.defaultWidth));
	List.InsertColumn(1, L"账号", LVCFMT_LEFT, (int)(1.2 * EduManager.defaultWidth));
	List.InsertColumn(2, L"姓名", LVCFMT_LEFT, (int)(0.8 * EduManager.defaultWidth));
	List.InsertColumn(3, L"班级", LVCFMT_LEFT, (int)(0.8 * EduManager.defaultWidth));
	List.InsertColumn(4, L"专业", LVCFMT_LEFT, (int)(1.2 * EduManager.defaultWidth));
	List.InsertColumn(5, L"*成绩", LVCFMT_LEFT, (int)(0.5 * EduManager.defaultWidth));
	List.InsertColumn(6, L"绩点", LVCFMT_LEFT, (int)(0.5 * EduManager.defaultWidth));
	List.InsertColumn(7, L"通过", LVCFMT_LEFT, (int)(0.5 * EduManager.defaultWidth));

	BOOLChecked_compulsory = false;

	if (target.size() == 1)
	{
		for (size_t i = 0; i < target.at(0)->attend.size(); i++)
		{
			ListItemClass lis;
			//MessageBox(target.at(0)->attend.at(i).studentID);
			lis.pUser = EduManager.FindUser(target.at(0)->attend.at(i).studentID);
			lis.score = target.at(0)->FindStudent(lis.pUser->ID)->score;
			ListVector.push_back(lis);
			screened.push_back(screened.size());
		}
	}

	CString credit(L"NULL"), hours(L"NULL");
	
	if (target.at(0)->credit > 0)credit.Format(L"%.1lf", target.at(0)->credit);
	else credit = L"";
	if (target.at(0)->hours > 0)hours.Format(L"%d", target.at(0)->hours);
	else hours = L"";
	moduleName_value = target.at(0)->name;
	credit_value = credit;
	hours_value = hours;
	compulsory_value = (target.at(0)->compulsory);
	moduleID_value = target.at(0)->ID;
	teacherID_value = target.at(0)->teacherID;
	
	for (size_t i = 1; i < target.size(); i++)
	{
		CString credit(L"NULL"), hours(L"NULL");
	
		if (target.at(i)->credit > 0)credit.Format(L"%.1lf", target.at(i)->credit);
		else credit = L"";
		if (target.at(i)->hours > 0)hours.Format(L"%d", target.at(i)->hours);
		else hours = L"";
		moduleName_value = target.at(i)->name == moduleName_value ? target.at(i)->name : L"多种";
		credit_value = credit == credit_value ? credit : L"多种";
		hours_value = hours == hours_value ? hours : L"多种";
		compulsory_value = (BOOL)(target.at(i)->compulsory) == compulsory_value ? target.at(i)->compulsory : -1;
		moduleID_value = L"多种";
		teacherID_value = target.at(i)->teacherID == teacherID_value ? target.at(i)->teacherID : L"多种";
	}
	
	if(target.size()!=1)
	{
		B_AddStudent.EnableWindow(0);
		inportData.EnableWindow(0);GetMenu()->GetSubMenu(0)->EnableMenuItem(ID_INPORT, MF_BYCOMMAND | MF_DISABLED);GetMenu()->GetSubMenu(0)->EnableMenuItem(ID_INPORT, MF_BYCOMMAND | MF_DISABLED);
		exportData.EnableWindow(0);GetMenu()->GetSubMenu(0)->EnableMenuItem(ID_EXPORT, MF_BYCOMMAND | MF_DISABLED);
		unrelease.EnableWindow(0);
		pass.EnableWindow(0);
		searchText.SetReadOnly(1);
		List.EnableWindow(0);
		comment.EnableWindow(0);
		homepage.EnableWindow(0);
	}

	UpdateData(FALSE);
	if(EduManager.userNow->userType!=3)RefreshWindowMessage();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void EditModule::RefreshWindowMessage() {

	List.DeleteAllItems();
	releasedNum = 0;
	passedNum = 0;
	totalScore = 0;
	StdDev = 0;

	if (target.size() == 1)
	{
		for (auto p = screened.begin();p != screened.end();p++)
		{
			CString Score("NULL"), GPA("NULL"), pass("NULL");
			double score = ListVector.at(*p).score;
			if (score != -1)
			{
				Score.Format(L"%.1lf", score);
				double gpa = (score - 50) / 10;
				if (gpa >= 1)
				{
					GPA.Format(L"%.1lf", gpa);
					pass = L"是";
					passedNum++;
				}
				else
				{
					GPA = L"无";
					pass = L"否";
				}
				releasedNum++;
				totalScore += score;
			}
			else
			{
				Score = L"无";
				GPA = L"无";
				pass = L"无";
			}

			List.InsertItem(0, L"");
			List.SetItemText(0, 1, ListVector.at(*p).pUser->ID);
			List.SetItemText(0, 2, ListVector.at(*p).pUser->name);
			List.SetItemText(0, 3, ListVector.at(*p).pUser->classID);
			List.SetItemText(0, 4, ListVector.at(*p).pUser->major);
			List.SetItemText(0, 5, Score);
			List.SetItemText(0, 6, GPA);
			List.SetItemText(0, 7, pass);

		}
	}


	if (moduleName_value != L"" && credit_value != L"" && hours_value != L"")
	{
		if(EduManager.userNow->userType!=3)B_EditModule.EnableWindow(1);
	}
	else
	{
		B_EditModule.EnableWindow(0);
	}


	if (ListVector.size())
	{
		CString average(L"无"), passRate(L"无"),StdDevStr(L"无");
		if (releasedNum!=0)
		{
			double avr = totalScore / releasedNum;
			average.Format(L"%.1lf", avr);
			passRate.Format(L"%.1lf", 100*(double)passedNum / releasedNum);
			passRate += '%';

			for (auto p = screened.begin();p != screened.end();p++)
			{
				if (ListVector.at(*p).score != -1)
				{
					StdDev += pow((ListVector.at(*p).score - avr), 2);
				}
			}
			StdDev /= releasedNum;
			StdDev = sqrt(StdDev);
			StdDevStr.Format(L"%.1lf", StdDev);
		}

		state_value.Format(L"参与学生：%d", ListVector.size());
		state_value += L"  平均分：" + average;
		state_value += L"  通过率：" + passRate;
		state_value += L"  标准差：" + StdDevStr;
	}
	else {
		state_value = L"当前没有任何学生参与";
	}
	if (target.size() != 1)state_value.Format(L"选中的%d个课程的信息", target.size());
	if (List.GetItemCount() != 0)
	{
		if (EduManager.userNow->userType != 3)
		{
			inportData.EnableWindow(1);GetMenu()->GetSubMenu(0)->EnableMenuItem(ID_INPORT, MF_BYCOMMAND | MF_ENABLED);
			exportData.EnableWindow(1);GetMenu()->GetSubMenu(0)->EnableMenuItem(ID_EXPORT, MF_BYCOMMAND | MF_ENABLED);
		}
	}
	else
	{
		inportData.EnableWindow(0);GetMenu()->GetSubMenu(0)->EnableMenuItem(ID_INPORT, MF_BYCOMMAND | MF_DISABLED);GetMenu()->GetSubMenu(0)->EnableMenuItem(ID_INPORT, MF_BYCOMMAND | MF_DISABLED);
		exportData.EnableWindow(0);GetMenu()->GetSubMenu(0)->EnableMenuItem(ID_EXPORT, MF_BYCOMMAND | MF_DISABLED);
	}
	UpdateData(FALSE);
}

void EditModule::OnBnClickedAdduser()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}


void EditModule::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (!B_EditModule.IsWindowEnabled())return;
	if (target.size() == 1)
	{
		if (haveeditcreate == true)
		{
			distroyEdit(&List, &m_Edit, e_Item, e_SubItem);//销毁单元格编辑框对象
			haveeditcreate = false;
			return;
		}
		else
		{
			auto tempModule = *target.at(0);
			UpdateData(TRUE);
			if (!(moduleName_value != L"" && credit_value != L"" && hours_value != L""))return;

			target.at(0)->name = moduleName_value;
			target.at(0)->teacherID = teacherID_value;
			target.at(0)->credit = _ttof(credit_value);
			target.at(0)->hours = _ttoi(hours_value);
			if (target.at(0)->credit < 0.1)target.at(0)->credit = 0.1;
			if (target.at(0)->hours < 1)target.at(0)->hours = 1;
			target.at(0)->compulsory = compulsory_value;
			target.at(0)->attend.clear();
			for (size_t i = 0; i < ListVector.size(); i++)
			{
				scoreInfo at;
				at.studentID = ListVector.at(i).pUser->ID;
				at.score = ListVector.at(i).score;
				if (newModule)
				{
					at.reScore = -1;
					at.comment = L"NULL";
				}
				else
				{
					at.reScore = tempModule.FindStudent(at.studentID)->reScore;
					at.comment = tempModule.FindStudent(at.studentID)->comment;
				}
				target.at(0)->attend.push_back(at);
			}
		}
	}
	else
	{
		for (size_t i = 0; i < target.size(); i++)
		{
			if (!(moduleName_value != L"" && credit_value != L"" && hours_value != L""))return;

			UpdateData(TRUE);
			if (BOOLChecked_compulsory)target.at(i)->compulsory = compulsory_value;
			if (moduleName_value != L"多种")target.at(i)->name = moduleName_value;
			if (credit_value != L"多种")target.at(i)->credit = _ttof(credit_value);
			if (hours_value != L"多种")target.at(i)->hours = _ttoi(hours_value);
		}
	}

	EduManager.SaveData(EduManager.basicDataPos);
	CDialogEx::OnOK();
}


void EditModule::OnEnChangeSearchtext()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	Screen();

}

void EditModule::Screen()
{
	if (haveeditcreate == true)
	{
		distroyEdit(&List, &m_Edit, e_Item, e_SubItem);//销毁单元格编辑框对象
		haveeditcreate = false;
	}

	UpdateData(TRUE);
	screened.clear();
	checked.clear();

	DelItem.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_DELETE, MF_BYCOMMAND | MF_DISABLED);
	Intro.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_DISABLED);

	for (size_t i = 0; i < ListVector.size(); i++)
	{
		if (EduManager.Contain(searchText_value, ListVector.at(i).pUser->ID) || EduManager.Contain(searchText_value, ListVector.at(i).pUser->name) || EduManager.Contain(searchText_value, ListVector.at(i).pUser->classID) || EduManager.Contain(searchText_value, ListVector.at(i).pUser->major))
		{
			if (!unrelease_value|| ListVector.at(i).score==-1)
			{
				if (!pass_value || ListVector.at(i).score >= 60)
				{
					screened.push_back(i);
				}
			}
		}
	}

	RefreshWindowMessage();
}

void EditModule::OnBnClickedAttendable()
{
	// TODO: 在此添加控件通知处理程序代码
	Screen();
}


void EditModule::OnBnClickedCompulsory()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOLChecked_compulsory = true;
	UpdateData(TRUE);
}



void EditModule::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDialogEx::OnCancel();
}


int EditModule::sort_column = 0; // 记录点击的列
bool EditModule::method = false;; // 记录比较方法


int CALLBACK EditModule::MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// 从参数中提取所需比较lc的两行数据
	int row1 = (int)lParam1;
	int row2 = (int)lParam2;
	CListCtrl* lc = (CListCtrl*)lParamSort;

	CString lp1 = lc->GetItemText(row1, sort_column);
	CString lp2 = lc->GetItemText(row2, sort_column);

	// 比较，对不同的列，不同比较，注意记录前一次排序方向，下一次要相反排序
	if ( sort_column == 5 || sort_column == 6)  //根据列的数据类型选择比较的类型
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

void EditModule::OnColumnclickListuser(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	if (haveeditcreate == true)
	{
		distroyEdit(&List, &m_Edit, e_Item, e_SubItem);//销毁单元格编辑框对象
		haveeditcreate = false;
	}

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

void EditModule::OnBnClickedAddstudent()
{
	// TODO: 在此添加控件通知处理程序代码
	OnAdd();
	
}


void EditModule::OnEnChangeModulename()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (moduleName_value != L"" && credit_value != L"" && hours_value != L"")
	{
		if (EduManager.userNow->userType != 3)B_EditModule.EnableWindow(1);
	}
	else
	{
		B_EditModule.EnableWindow(0);
	}
}


void EditModule::OnEnChangeCredit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (moduleName_value != L"" && credit_value != L"" && hours_value != L"")
	{
		if (EduManager.userNow->userType != 3)B_EditModule.EnableWindow(1);
	}
	else
	{
		B_EditModule.EnableWindow(0);
	}
}


void EditModule::OnEnChangeHours()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (moduleName_value != L"" && credit_value != L"" && hours_value != L"")
	{
		if (EduManager.userNow->userType != 3)B_EditModule.EnableWindow(1);
	}
	else
	{
		B_EditModule.EnableWindow(0);
	}
}


void EditModule::OnBnClickedDelstudent()
{
	// TODO: 在此添加控件通知处理程序代码
	if(DelItem.IsWindowEnabled())OnDelete();
}


void EditModule::OnBnClickedEditmodule()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}


void EditModule::OnBnClickedPass()
{
	// TODO: 在此添加控件通知处理程序代码

	Screen();
}


void EditModule::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	NM_LISTVIEW* pEditCtrl = (NM_LISTVIEW*)pNMHDR;

	LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE)pNMHDR;//将传进来的消息转换成LPNMITEMACTIVAT
	auto r = temp->iItem;//获得行号
	auto c = temp->iSubItem;//获得列号

	if (r == -1)return;

	if (c == 5)
	{
		if (haveeditcreate == true)
		{
			if (!(e_Item == pEditCtrl->iItem && e_SubItem == pEditCtrl->iSubItem))//如果点中的单元格不是之前创建好的
			{
				distroyEdit(&List,&m_Edit, e_Item, e_SubItem);
				haveeditcreate = false;
				createEdit(pEditCtrl, &m_Edit, e_Item, e_SubItem, haveeditcreate);//创建编辑框
			}
			else//点中的单元格是之前创建好的
			{
				m_Edit.SetFocus();//设置为焦点 
			}
		}
		else
		{
			e_Item = pEditCtrl->iItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理
			e_SubItem = pEditCtrl->iSubItem;//将点中的单元格的行赋值给“刚编辑过的行”以便后期处理
			createEdit(pEditCtrl, &m_Edit, e_Item, e_SubItem, haveeditcreate);//创建编辑框
		}
	}
	else
	{
		EditModuleEditUser editUser;
		editUser.target.push_back(EduManager.FindUser(List.GetItemText(r, 1)));
		editUser.moduleTarget = target.at(0);
		editUser.parentVector = &ListVector;
		if(editUser.DoModal()==IDOK)RefreshWindowMessage();
	}


	*pResult = 0;
}

void EditModule::createEdit(NM_LISTVIEW* pEditCtrl, CEdit* createdit, int& Item, int& SubItem, bool& havecreat)
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

void EditModule::distroyEdit(CListCtrl *pList,CEdit* distroyedit, int& Item, int& SubItem)
{
	auto p = ListVector.begin();
	for (;p!=ListVector.end();p++)
	{
		if (p->pUser->ID == List.GetItemText(Item, 1))
		{
			break;
		}
	}
	CString value;
	distroyedit->GetWindowTextW(value);
	if (p!= ListVector.end())
	{
		if (value==L"无"|| value == L"")
		{
			p->score = -1;
		}
		else
		{
			p->score = _ttof(value);
			if (p->score < 0)p->score = 0;
			if (p->score > 100)p->score = 100;
		}
		if (SubItem==5)
		{
			CString Score("NULL"), GPA("NULL"), pass("NULL");
			double score = p->score;
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
			pList->SetItemText(Item, 5, Score);
			pList->SetItemText(Item, 6, GPA);
			pList->SetItemText(Item, 7, pass);
		}

		releasedNum = 0;
		passedNum = 0;
		totalScore = 0;
		StdDev = 0;

		for (auto p = screened.begin();p != screened.end();p++)
		{
			double score = ListVector.at(*p).score;
			if (score != -1)
			{
				double gpa = (score - 50) / 10;
				if (gpa >= 1)
				{
					passedNum++;
				}
				releasedNum++;
				totalScore += score;
			}
		}

		if (ListVector.size())
		{
			CString average(L"无"), passRate(L"无"), StdDevStr(L"无");
			if (releasedNum != 0)
			{
				double avr = totalScore / releasedNum;
				average.Format(L"%.1lf", avr);
				passRate.Format(L"%.1lf", 100 * (double)passedNum / releasedNum);
				passRate += '%';

				for (auto p = screened.begin();p != screened.end();p++)
				{
					if (ListVector.at(*p).score != -1)
					{
						StdDev += pow((ListVector.at(*p).score - avr), 2);
					}
				}
				StdDev /= releasedNum;
				StdDev = sqrt(StdDev);
				StdDevStr.Format(L"%.1lf", StdDev);
			}

			state_value.Format(L"参与学生：%d", ListVector.size());
			state_value += L"  平均分：" + average;
			state_value += L"  通过率：" + passRate;
			state_value += L"  标准差：" + StdDevStr;
		}
		else 
		{
			state_value = L"当前没有任何学生参与";
		}
		if (target.size() != 1)state_value.Format(L"选中的%d个课程的信息", target.size());
		UpdateData(FALSE);
	}
	distroyedit->DestroyWindow();//销毁对象，有创建就要有销毁，不然会报错
}

void EditModule::OnClickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	if (haveeditcreate == true)//如果之前创建了编辑框就销毁掉
	{
		distroyEdit(&List,&m_Edit, e_Item, e_SubItem);//销毁单元格编辑框对象
		haveeditcreate = false;
	}
	
	*pResult = 0;
}


void EditModule::OnBnClickedUnrelease2()
{
	// TODO: 在此添加控件通知处理程序代码

	Screen();
}


void EditModule::OnBnClickedInportdata()
{
	// TODO: 在此添加控件通知处理程序代码


	OnInport();
}


void EditModule::OnBnClickedExportdata()
{
	// TODO: 在此添加控件通知处理程序代码
	OnExport();
}




void EditModule::OnBnClickedIntro()
{
	// TODO: 在此添加控件通知处理程序代码
	if (Intro.IsWindowEnabled())OnIntro();
	
}


void EditModule::OnBnClickedComment()
{
	// TODO: 在此添加控件通知处理程序代码
	if (EduManager.userNow->userType==3)
	{
		Comment commmentForm;
		commmentForm.target = target.at(0)->FindStudent(EduManager.userNow->ID);
		commmentForm.DoModal();
	}
	else
	{
		ViewComment viewCommmentForm;
		viewCommmentForm.target = target.at(0);
		viewCommmentForm.DoModal();
	}
}


void EditModule::OnBnClickedHomepage()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!homepage.IsWindowEnabled())return;
	TeacherForm teacherForm;
	teacherForm.target = EduManager.FindUser(target.at(0)->teacherID);
	teacherForm.DoModal();
}


void EditModule::OnSelall()
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


BOOL EditModule::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (::TranslateAccelerator(GetSafeHwnd(), hAccel, pMsg))

		return true;
	return CDialogEx::PreTranslateMessage(pMsg);
}


void EditModule::OnMin()
{
	// TODO: 在此添加命令处理程序代码
	ShowWindow(SW_SHOWMINIMIZED);
}


void EditModule::OnClose()
{
	// TODO: 在此添加命令处理程序代码
	OnCancel();
}


void EditModule::OnInport()
{
	// TODO: 在此添加命令处理程序代码
	CString strFile = _T("");
	CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("文本文件(*.txt)|*.txt|All Files (*.*)|*.*||"), NULL);
	if (dlgFile.DoModal()) strFile = dlgFile.GetPathName();
	ifstream ifs(CT2A(strFile.GetString()));
	if (ifs.is_open())
	{
		string s;
		getline(ifs, s);
		vector<CString>unLoad;
		int loadedNum = 0;
		while (true)
		{
			getline(ifs, s);
			if (s == "") continue;
			if (s[0] == '#')break;
			istringstream iss(s);
			string ID, name;
			iss >> ID >> name;
			ListItemClass* p = nullptr;
			for (size_t i = 0; i < ListVector.size(); i++)
			{
				if (ListVector.at(i).pUser->ID == CString(ID.c_str()))
				{
					p = &(ListVector.at(i));
					break;
				}
			}
			if (p == nullptr)
			{
				CString cs;
				cs.Format(L"%s\t%s", CString(ID.c_str()), CString(name.c_str()));
				unLoad.push_back(cs);
			}
			else
			{
				iss >> name >> name >> p->score;
				loadedNum++;
			}
		}

		if (loadedNum != 0)
		{
			CString cs;
			cs.Format(L"已读取%d个学生的成绩", loadedNum);
			MessageBox(cs);
		}
		if (!unLoad.empty())
		{
			CString cs;
			cs.Format(L"以下%d个学生未找到，无法导入成绩：\n\n", unLoad.size());
			for (size_t i = 0; i < unLoad.size(); i++)
			{
				cs += unLoad.at(i);
				if (i != unLoad.size() - 1)cs += '\n';
			}
			MessageBox(cs);
		}

		
		unrelease_value = false;
		pass_value = false;
		UpdateData(FALSE);

		Screen();
	}
}


void EditModule::OnExport()
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
	CString filename = target.at(0)->name + L" (" + target.at(0)->ID + L") 学生成绩 ";
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


void EditModule::OnAdd()
{
	// TODO: 在此添加命令处理程序代码

	AddStudent addStudent;
	addStudent.parentVector = &ListVector;
	if (addStudent.DoModal() == IDOK)
	{
		
		UpdateData(FALSE);
		Screen();
	}

}


void EditModule::OnIntro()
{
	// TODO: 在此添加命令处理程序代码
	if (!Intro.IsWindowEnabled())return;
	EditModuleEditUser editUser;
	for (auto p = checked.begin();p != checked.end();p++)
	{
		auto q = ListVector.begin();
		for (;q != ListVector.end();q++)
		{
			if (q->pUser->ID == List.GetItemText(*p, 1))
			{
				break;
			}
		}
		editUser.target.push_back(q->pUser);
	}
	editUser.moduleTarget = target.at(0);
	editUser.parentVector = &ListVector;
	if (editUser.DoModal() == IDOK)
	{
		RefreshWindowMessage();
		Screen();
		checked.clear();
		DelItem.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_DELETE, MF_BYCOMMAND | MF_DISABLED);
		Intro.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_DISABLED);
	}

}


void EditModule::OnDelete()
{
	// TODO: 在此添加命令处理程序代码
	if (!DelItem.IsWindowEnabled())return;
	vector<CString>unDel;
	for (auto p = checked.begin();p != checked.end();p++)
	{
		if (List.GetItemText(*p, 5) == L"无")
		{
			bool find = false;
			auto q = ListVector.begin();
			for (; q != ListVector.end(); q++)
			{
				if ((*q).pUser->ID == List.GetItemText(*p, 1))
				{
					find = true;
					break;
				}
			}
			if (find)
			{
				ListVector.erase(q);
			}
		}
		else
		{
			CString cs;
			cs.Format(L"%s\t%s", List.GetItemText(*p, 1), List.GetItemText(*p, 2));
			unDel.push_back(cs);
		}
	}

	

	Screen();
	checked.clear();
	DelItem.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_DELETE, MF_BYCOMMAND | MF_DISABLED);
	Intro.EnableWindow(0);GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_INTRO, MF_BYCOMMAND | MF_DISABLED);

	if (!unDel.empty())
	{
		CString cs;
		cs.Format(L"以下%d个学生的成绩已经公布，请将其成绩删除后再删除学生：\n\n", unDel.size());
		for (size_t i = 0; i < unDel.size(); i++)
		{
			cs += unDel.at(i);
			if (i != unDel.size() - 1)cs += '\n';
		}
		MessageBox(cs);
	}

}
