
// Login.h: 头文件
//

#pragma once


// Login 对话框
class Login : public CDialogEx
{
// 构造
public:
	Login(CWnd* pParent = nullptr);	// 标准构造函数


	void UpdateRecent();

	vector<User*>recentUser;

	bool m_bBackSpace;
	bool sel;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGIN };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString password_value;
	BOOL remPsw_value;
//	CEdit username_value;
	afx_msg void OnEnChangePassword();
	afx_msg void OnBnClickedLogin();
	CString username_value;
	afx_msg void OnEditchangeUsername();
	afx_msg void OnCbnSelchangeUsername();
	CComboBox username;
//	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnBnClickedRempsw();
//	afx_msg void OnDropdownShowpsw(NMHDR* pNMHDR, LRESULT* pResult);
//	afx_msg void OnHotitemchangeShowpsw(NMHDR* pNMHDR, LRESULT* pResult);
	CEdit password;
//	afx_msg void OnBnClickedShowpsw();
	CButton B_Login;
	virtual void OnOK();
	afx_msg void OnBnClickedRandommodule();
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
