#pragma once


// StudentForm 对话框

class StudentForm : public CDialogEx
{
	DECLARE_DYNAMIC(StudentForm)

public:
	StudentForm(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~StudentForm();

	User* target;

	vector<Module*>ListVector;
	vector<int>checked;
	vector<int>screened;
	HACCEL hAccel;

	double totalCredit;
	double avaCredit;
	double totalGPA;

	void Screen();

	void RefreshWindowMessage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STUDENTFORM };
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
	BOOL compulsory_value;
	afx_msg void OnBnClickedCompulsory();
	BOOL quitAble_value;
	afx_msg void OnBnClickedQuitable();
	afx_msg void OnBnClickedExportdata();
	CButton exportData;
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	CButton Intro;
	afx_msg void OnBnClickedIntro();
	afx_msg void OnBnClickedIntro3();
	afx_msg void OnSelall();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnAdd();
	afx_msg void OnIntro();
	afx_msg void OnDelete();
	afx_msg void OnMin();
	afx_msg void OnClose();
	afx_msg void OnExport();
	afx_msg void OnStnClickedUnselcom();
};
