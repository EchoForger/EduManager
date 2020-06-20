#pragma once
#include "EditModule.h"

// AddStudent 对话框


class AddStudent : public CDialogEx
{
	DECLARE_DYNAMIC(AddStudent)

public:
	AddStudent(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~AddStudent();
	
	vector<User*>ListVector;

	double totalCredit;
	int totalSelect;

	double extraCredit = 0;

	void RefreshWindowMessage();
	void Screen();

	vector<int>checked;
	vector<int>screened;


	vector<ListItemClass>* parentVector;

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDSTUDENT };
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
	CButton B_AddStudent;
	CString state_value;
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedIntro();
	CButton Intro;
	afx_msg void OnSelall();
	HACCEL hAccel;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	afx_msg void OnMin();
	afx_msg void OnIntro();
};
