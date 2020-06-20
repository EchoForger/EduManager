#pragma once


// AboutBox 对话框

class AboutBox : public CDialogEx
{
	DECLARE_DYNAMIC(AboutBox)

public:
	AboutBox(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~AboutBox();
	CFont m_font;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnAdd();
	afx_msg void OnStnClickedState();
	afx_msg void OnStnClickedState2();
	afx_msg void OnStnClickedWebsite();
	afx_msg void OnBnClickedButton1();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
};
