#pragma once


// EditUser 对话框

class EditUser : public CDialogEx
{
	DECLARE_DYNAMIC(EditUser)

public:
	EditUser(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~EditUser();

	void RefreshWindowMessage();
	vector<User*> target;
	Module* moduleTarget;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDITUSER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit name;
	CEdit major;
	CEdit userID;
	CEdit classID;
	CEdit password;
	CEdit moduleName;
	CEdit score;
	CString state_value;
	CString name_value;
	CString major_value;
	CString userID_value;
	CString classID_value;
	CString password_value;
	CString moduleName_value;
	CString score_value;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedEditmodule();
	virtual void OnOK();
	afx_msg void OnEnChangeName();
	afx_msg void OnEnChangeMajor();
	afx_msg void OnEnChangeUserid();
	afx_msg void OnEnChangeClassid();
	afx_msg void OnEnChangePassword();
	afx_msg void OnEnChangeScore();
	CComboBox userType;
	CButton B_EditUser;
	CString userType_value;
	bool newUser;
	afx_msg void OnBnClickedEdituser();
	afx_msg void OnCbnSelchangeUsertype();
	afx_msg void OnBnClickedIntro();
	CButton homepage;
	afx_msg void OnBnClickedHomepage();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMin();
	afx_msg void OnClose();
};
