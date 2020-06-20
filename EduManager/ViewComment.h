#pragma once


// ViewComment 对话框


class ViewComment : public CDialogEx
{
	DECLARE_DYNAMIC(ViewComment)

public:
	ViewComment(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ViewComment();

	vector<scoreInfo*>ListVector;

	double totalCredit;
	int totalSelect;

	double extraCredit = 0;
	double totalReScore;

	HACCEL hAccel;
	void RefreshWindowMessage();
	void Screen();

	vector<int>checked;
	vector<int>screened;

	Module* target;

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIEWCOMMENT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedAddmodule();
	virtual void OnOK();
	afx_msg void OnEnChangeSearchtext();
	afx_msg void OnBnClickedAttendable();
	afx_msg void OnBnClickedCompulsory();
	virtual void OnCancel();
	static int sort_column; // 记录点击的列
	static bool method; // 记录比较方法
	static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	afx_msg void OnColumnclickList(NMHDR* pNMHDR, LRESULT* pResult);
	CListCtrl List;
	CString searchText_value;
	CButton B_ViewComment;
	CString state_value;
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedIntro();
	CButton Intro;
	afx_msg void OnSelall();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedExportdata();
	afx_msg void OnExport();
	afx_msg void OnMin();
	afx_msg void OnClose();
	afx_msg void OnIntro();
	CButton exportData;
};
