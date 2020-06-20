#pragma once
#include "afxstr.h"
#include "User.h"
#include <vector>

using namespace std;

class scoreInfo
{
public:
	CString studentID;
	double score;
	double reScore;
	CString comment;
};

class Module
{
public:
	CString ID;
	CString name;
	CString teacherID;
	double credit;
	unsigned int hours;
	bool compulsory;
	vector<scoreInfo> attend;

	scoreInfo* FindStudent(CString studentID);
	bool AddStudent(CString studentID, double score = -1);
	bool SetScore(CString studentID, double score);
	bool DelStudent(CString studentID);
	bool Release();

	bool AttendedSame(User* user);

	
};