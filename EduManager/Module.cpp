#include "pch.h"
#include "Module.h"
#include "EduManager.h"

scoreInfo* Module::FindStudent(CString studentID)
{
	scoreInfo* res = nullptr;
	for (size_t i = 0; i < attend.size(); i++)
	{
		if (attend.at(i).studentID == studentID)
		{
			res = &(attend.at(i));
			break;
		}
	}
	return res;
}

bool Module::AddStudent(CString studentID, double score)
{
	if (FindStudent(studentID) == nullptr)
	{
		scoreInfo s;
		s.studentID = studentID;
		s.score = score;
		s.reScore = -1;
		s.comment = L"NULL";
		attend.push_back(s);
		return true;
	}
	else return false;
}

bool Module::SetScore(CString studentID, double score)
{
	auto target = FindStudent(studentID);
	if (target != nullptr)
	{
		target->score = score;
		return true;
	}
	else return false;
}

bool Module::DelStudent(CString studentID)
{
	bool find = false; 
	auto p = attend.begin();
	for (; p!= attend.end(); p++)
	{
		if (p->studentID == studentID)
		{
			find = true;
			break;
		}
	}
	if (find)
	{
		attend.erase(p);
	}
	return find;
}

bool Module::AttendedSame(User* user)
{
	bool attended = false;
	for (size_t i = 0; i < EduManager.ModuleLib.size(); i++)
	{
		if (EduManager.ModuleLib.at(i).name == name && EduManager.ModuleLib.at(i).FindStudent(user->ID) != nullptr)
		{
			attended = true;
			break;
		}
	}
	return attended;
}

bool Module::Release()
{
	bool res =false;
	if (attend.size())
	{
		for (size_t i = 0; i < attend.size(); i++)
		{
			if (attend.at(i).score != -1)
			{
				res = true;
				break;
			}
		}
	}
	return res;
}