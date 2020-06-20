#pragma once


// AdminForm 对话框

class AdminForm : public CDialogEx
{
	DECLARE_DYNAMIC(AdminForm)

public:
	AdminForm(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~AdminForm();

	vector<User*>ListVector;
	vector<int>checked;
	vector<int>screened;

	CEdit m_Edit;  //生成单元编辑框对象
	bool haveeditcreate;//标志编辑框已经被创建
	void createEdit(NM_LISTVIEW* pEditCtrl, CEdit* createdit, int& Item, int& SubItem, bool& havecreat);//创建单元格编辑框函数
	void distroyEdit(CListCtrl* pList, CEdit* distroyedit, int& Item, int& SubItem);//销毁单元格编辑框对象

	int e_Item;    //刚编辑的行  
	int e_SubItem; //刚编辑的列  

	CComboBox m_comBox;//生产单元格下拉列表对象
	bool haveccomboboxcreate;//标志下拉列表框已经被创建
	void createCcombobox(NM_LISTVIEW* pEditCtrl, CComboBox* createccomboboxobj, int& Item, int& SubItem, bool& havecreat);//创建单元格下拉列表框函数
	void saveCcombobox(CListCtrl* list, CComboBox* distroyccomboboxobj, int& Item, int& SubItem);//销毁单元格下拉列表框

	void Screen();

	void RefreshWindowMessage();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADMINFORM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	CListCtrl List;
	afx_msg void OnLvnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAddmodule();
	afx_msg void OnBnClickedDelmodule();
	CString state_value;
	CStatic username;
	CStatic userInfo;
	CString userInfo_value;
	CString username_value;
	CButton DelItem;
	static int sort_column; // 记录点击的列
	static bool method; // 记录比较方法
	static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	virtual void OnOK();
	afx_msg void OnColumnclickList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnCancel();
	CString unSelCom_value;
	CString searchText_value;
	afx_msg void OnEnChangeSearchtext();
	BOOL unrelease_value;
	afx_msg void OnBnClickedCompulsory();
	BOOL quitAble_value;
	afx_msg void OnBnClickedQuitable();
	afx_msg void OnBnClickedUnrelease();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedExportdata();
	CButton exportData;
	CButton Intro;
	afx_msg void OnBnClickedIntro();
	afx_msg void OnStnClickedUserinfo();
	afx_msg void OnBnClickedIntro2();
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeComboxCreateid();
	afx_msg void OnSelendokComboxCreateid();
	afx_msg void OnSelendcancelComboxCreateid();
	afx_msg void OnKillfocusEditCreateid();
	afx_msg void OnKillfocusComboxCreateid();
	afx_msg void OnBnClickedUserintro();
	afx_msg void OnBnClickedUserintro2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedIntro3();
	afx_msg void OnBnClickedExportdatabase();
	afx_msg void OnBnClickedInportdatabase();
	CButton exportDataBase;
	HACCEL hAccel;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSave();
	afx_msg void OnOpen();
	afx_msg void OnNew();
	afx_msg void OnSaveas();
	afx_msg void OnExport();
	afx_msg void OnSelall();
	afx_msg void OnAdd();
	afx_msg void OnIntro();
	afx_msg void OnDelete();
	afx_msg void OnMin();
	afx_msg void OnClose();
};
