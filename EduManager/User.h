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

	//���������ǷǱ����
	CString classID;
	CString major;
};