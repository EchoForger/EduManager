#pragma once


// Comment 对话框

class Comment : public CDialogEx
{
	DECLARE_DYNAMIC(Comment)

public:
	Comment(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Comment();

	scoreInfo* target;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMMENT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString moduleName_value;
	CString teacherName_value;
	CString rescore_value;
	CString commennt_value;
	virtual BOOL OnInitDialog();
	CString state_value;
	CButton B_Submit;
	afx_msg void OnEnChangeRescore();
	afx_msg void OnEnChangeComment();
	afx_msg void OnBnClickedSubmit();
	virtual void OnOK();
	CEdit rescore;
	CEdit comment;
	afx_msg void OnSelall();
};
