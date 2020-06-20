
// EduManager.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"		// 主符号
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "User.h"
#include "Module.h"
#include "Login.h"


// CEduManagerApp:
// 有关此类的实现，请参阅 EduManager.cpp
//

class CEduManagerApp : public CWinApp
{
public:
	User superAdmin;

	CEduManagerApp();

	const string basicDataPos = "C:/ProgramData/EchoForger/EduManager/CurrentData.emgdata";

	void ReadData(string dataPos);
	void SaveData(string dataPos);

	User RandomUser(int userType);
	Module RandomModule();
	void AddRandomStuff(unsigned int adminNum, unsigned int teacherNum, unsigned int studentNum, unsigned int moduleNum);

	User* FindUser(CString userID);
	Module* FindModule(CString moduleID);

	void DelUser(CString userID);
	void DelModule(CString moduleID);

	CString RandomID(int n);

	unsigned int defaultWidth=162;

	unsigned int GetSecondsFrom1970();
	bool Contain(CString a, CString b);

	vector<User>UserLib;
	vector<Module>ModuleLib;

	string eraseBlank(CString cs);

	User* userNow;

public:
	string CSTS(CString cstr);//将CString转化成string类型
	LPCWSTR StringToLPCWSTR(string str);//将多字节字符转换为宽字节字符
	string LPCWSTRToString(LPCWSTR wstr);//将宽字节字符转换为多字节字符
	//LPCWSTR是一个指向unicode编码的常量字符串的32位指针，所指向字符串是wchar型。LP表示是一个long指针,C表示是const常量
	//LPCSTR是一个指向以NULL（‘\0’）结尾的常量字符串的指针，所指向字符串是char型。

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
	afx_msg void OnNew();
	afx_msg void OnQuit();
	afx_msg void OnOpen();
	afx_msg void OnAbout();
};

extern CEduManagerApp EduManager;
