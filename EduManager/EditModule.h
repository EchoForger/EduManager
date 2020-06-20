#pragma once


// EditModule 对话框

class ListItemClass
{
public:
	User* pUser;
	double score;
};

class EditModule : public CDialogEx
{
	DECLARE_DYNAMIC(EditModule)

public:
	EditModule(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~EditModule();

	vector<ListItemClass>ListVector;

	int releasedNum;
	int passedNum;
	double totalScore;

	void RefreshWindowMessage();
	void Screen();

	vector<int>checked;
	vector<int>screened;

	CEdit m_Edit;  //生成单元编辑框对象
	bool haveeditcreate;//标志编辑框已经被创建
	void createEdit(NM_LISTVIEW* pEditCtrl, CEdit* createdit, int& Item, int& SubItem, bool& havecreat);//创建单元格编辑框函数
	void distroyEdit(CListCtrl* pList, CEdit* distroyedit, int& Item, int& SubItem);//销毁单元格编辑框对象

	int e_Item;    //刚编辑的行  
	int e_SubItem; //刚编辑的列  
	HACCEL hAccel;

	vector<Module*> target;

	bool BOOLChecked_compulsory;
	bool newModule;

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDITMODULE};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchangedListuser(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAdduser();
	virtual void OnOK();
	afx_msg void OnEnChangeSearchtext();
	afx_msg void OnBnClickedAttendable();
	afx_msg void OnBnClickedCompulsory();
	virtual void OnCancel();
	static int sort_column; // 记录点击的列
	static bool method; // 记录比较方法
	static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	afx_msg void OnColumnclickListuser(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedAddstudent();
	afx_msg void OnEnChangeModulename();
	afx_msg void OnEnChangeCredit();
	afx_msg void OnEnChangeHours();
	afx_msg void OnBnClickedNewmodule();
	CString moduleName_value;
	CString credit_value;
	CString hours_value;
	BOOL compulsory_value;
	CButton DelItem;
	CString searchText_value;
	CListCtrl List;
	CString state_value;
	CButton B_EditModule;
	afx_msg void OnBnClickedDelstudent();
	afx_msg void OnBnClickedEditmodule();
	CString moduleID_value;
	BOOL unrelease_value;
	BOOL pass_value;
	afx_msg void OnBnClickedPass();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedUnrelease2();
	afx_msg void OnBnClickedInportdata();
	afx_msg void OnBnClickedExportdata();
	CButton exportData;
	CButton inportData;
	afx_msg void OnBnClickedIntro();
	CButton B_AddStudent;
	CEdit searchText;
	CButton unrelease;
	CButton pass;
	CEdit moduleID;
	CEdit teacherID;
	CEdit moduleName;
	CEdit credit;
	CEdit hours;
	CButton compulsory;
	CButton Intro;
	CString teacherID_value;
	CButton comment;
	afx_msg void OnBnClickedComment();
	CButton homepage;
	afx_msg void OnBnClickedHomepage();
	afx_msg void OnSelall();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMin();
	afx_msg void OnClose();
	afx_msg void OnInport();
	afx_msg void OnExport();
	afx_msg void OnAdd();
	afx_msg void OnIntro();
	afx_msg void OnDelete();
	double StdDev;
};
