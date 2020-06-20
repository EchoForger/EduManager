#pragma once
#include "afxstr.h"
class User
{
public:
	CString ID;
	CString name;
	CString password;
	bool remUsn;
	bool remPsw;
	unsigned int lastLogin;
	int userType;

	//以下内容是非必须的
	CString classID;
	CString major;
};