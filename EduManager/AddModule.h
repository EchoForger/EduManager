#pragma once


// AddModule 对话框

class AddModule : public CDialogEx
{
	DECLARE_DYNAMIC(AddModule)

public:
	AddModule(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~AddModule();

	vector<Module*>ListVector;

	double totalCredit;
	int totalSelect;

	double extraCredit = 0;

	void RefreshWindowMessage();
	void Screen();

	vector<int>checked;
	vector<int>screened;

	User* target;
	HACCEL hAccel;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDMODULE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl List;
	CString state_value;
	afx_msg void OnLvnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
	CButton B_AddModule;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAddmodule();
	virtual void OnOK();
//	CEdit searchText_value;
	afx_msg void OnEnChangeSearchtext();
	CString searchText_value;
	BOOL attendAble_value;
	afx_msg void OnBnClickedAttendable();
	BOOL compulsory_value;
	afx_msg void OnBnClickedCompulsory();
	virtual void OnCancel();
	static int sort_column; // 记录点击的列
	static bool method; // 记录比较方法
	static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	afx_msg void OnColumnclickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedIntro();
	CButton Intro;
	afx_msg void OnSelall();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void On32791();
	afx_msg void OnIntro();
	afx_msg void OnMin();
	afx_msg void OnClose();
};
