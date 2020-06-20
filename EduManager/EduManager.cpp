
// EduManager.cpp: 定义应用程序的类行为。
//

#include "pch.h"
#include "framework.h"
#include "EduManager.h"
#include "Login.h"
#include <algorithm>
#include <cmath>
#include <set>
#include "AboutBox.h"
#include<AfxGlobals.h>



using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*the seconds of round year = 3600*24*366 */
#define SECONDOFROUNDYEAR 31622400

/*the seconds of general year = 3600*24*365 */
#define SECONDOFYEAR 31536000

/*the seconds of general year = 3600*24 */
#define MAXSECONDOFDAY 86400


// CEduManagerApp

BEGIN_MESSAGE_MAP(CEduManagerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
	ON_COMMAND(ID_QUIT, &CEduManagerApp::OnQuit)
	ON_COMMAND(ID_OPEN, &CEduManagerApp::OnOpen)
	ON_COMMAND(ID_ABOUT, &CEduManagerApp::OnAbout)
END_MESSAGE_MAP()


// CEduManagerApp 构造

CEduManagerApp::CEduManagerApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CEduManagerApp 对象

CEduManagerApp EduManager;


// CEduManagerApp 初始化

BOOL CEduManagerApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
		//设置正常字体
	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);
	afxGlobalData.fontRegular.DeleteObject();
	lf.lfHeight = -12;
	lstrcpy(lf.lfFaceName, _T("微软雅黑"));
	afxGlobalData.fontRegular.CreateFontIndirect(&lf);

	//设置加粗的字体 
	afxGlobalData.fontBold.GetLogFont(&lf);
	afxGlobalData.fontBold.DeleteObject();
	lf.lfHeight = -12;
	lstrcpy(lf.lfFaceName, _T("微软雅黑"));
	afxGlobalData.fontBold.CreateFontIndirect(&lf);

	afxGlobalData.fontTooltip.GetLogFont(&lf);
	afxGlobalData.fontTooltip.DeleteObject();
	lf.lfHeight = -12;
	lstrcpy(lf.lfFaceName, _T("微软雅黑"));
	afxGlobalData.fontTooltip.CreateFontIndirect(&lf);


	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	Login dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

bool comp1(const User& a, const User& b)
{
	return a.lastLogin > b.lastLogin;
}

void CEduManagerApp::ReadData(string dataPos)
{
	ifstream ifs(dataPos);
	if (ifs)
	{
		string s;
		getline(ifs, s);
		while (true)
		{
			getline(ifs, s); 
			if (s == "") continue;
			if (s[0] == '#')break;
			User u;
			istringstream iss(s);
			string ID, name, password;
			iss >> ID >> name >> password >> u.remUsn >> u.remPsw >> u.lastLogin >> u.userType;
			u.ID = ID.c_str();
			u.name = name.c_str();
			u.password = password.c_str();
			if (FindUser(u.ID) != nullptr)continue;
			if (u.userType == 3)
			{
				string classID, major;
				iss >> classID >> major;
				u.classID = classID.c_str();
				u.major = major.c_str();
			}
			else
			{
				u.classID = "NULL";
				u.major = "NULL";
			}
			UserLib.push_back(u);
		}
		sort(UserLib.begin(), UserLib.end(), comp1);
		getline(ifs, s);
		getline(ifs, s);
		while (true)
		{
			getline(ifs, s);
			if (s == "") continue;
			if (s[0] == '#') break;
			Module m;
			istringstream iss(s);
			string ID, name, teacherID;
			iss >> ID >> name >> teacherID >> m.credit >> m.hours >> m.compulsory;
			m.ID = ID.c_str();
			m.name = name.c_str();
			m.teacherID = teacherID.c_str();
			if (FindModule(m.ID) != nullptr)continue;
			if (FindUser(m.teacherID) == nullptr || FindUser(m.teacherID)->userType != 2)continue;
			while (true)
			{
				scoreInfo at;
				string studentID;
				string comment;
				iss >> studentID >> at.score>>at.reScore>>comment;
				if (studentID == "") break;
				at.studentID=studentID.c_str();
				at.comment=comment.c_str();
				if (m.FindStudent(at.studentID) != nullptr)
				{
					continue;
				}
				m.attend.push_back(at);
			}
			ModuleLib.push_back(m);
		}
		ifs.close();
	}
}

bool comp2(const User& a, const User& b)
{
	if (a.userType == b.userType)
	{
		return a.lastLogin > b.lastLogin;
	}
	return a.userType > b.userType;
}

void CEduManagerApp::SaveData(string dataPos)
{
	ofstream ofs(dataPos);
	if (ofs)
	{
		auto tempUserLib = UserLib;
		sort(tempUserLib.begin(), tempUserLib.end(), comp2);
		ofs << "#账号\t姓名\t密码\t记住账号\t记住密码\t上次登录\t账户类型\t班级（可选）\t专业（可选）\n";
		for (size_t i = 0; i < tempUserLib.size(); i++)
		{
			User u = tempUserLib.at(i);
			ofs << eraseBlank(u.ID) << '\t' << eraseBlank(u.name) << '\t' << eraseBlank(u.password) << '\t';
			ofs << u.remUsn << '\t' << u.remPsw << '\t' << u.lastLogin <<'\t'<< u.userType;
			if (u.userType != 3)
			{ 
				ofs << '\n';
				continue;
			}
			ofs << '\t' << eraseBlank(u.classID) << '\t' << eraseBlank(u.major) << '\n';
		}
		ofs << "#END\n\n";
		ofs << "#课程代码\t课程名称\t教师账号\t学分\t学时\t必修\t（学生账号，分数，反馈分数，反馈评语）...\n";
		for (size_t i = 0; i < ModuleLib.size(); i++)
		{
			Module m = ModuleLib.at(i);
			ofs << eraseBlank(m.ID) << '\t' << eraseBlank(m.name) << '\t' << eraseBlank(m.teacherID) << '\t';
			ofs << m.credit << '\t' << m.hours << '\t' << m.compulsory;
			for (size_t i = 0; i < m.attend.size(); i++)
			{
				ofs << '\t' << eraseBlank(m.attend.at(i).studentID) << '\t' << m.attend.at(i).score << '\t' << m.attend.at(i).reScore << '\t' << eraseBlank(m.attend.at(i).comment);
			}
			ofs << '\n';
		}
		ofs << "#END";
		ofs.close();
	}
}

bool IsRound(int year)
{
	if ((year % 100) && (year % 4 == 0)) return 1;
	if ((year % 100 == 0) && (year % 400 == 0)) return 1;
	return 0;
}

unsigned int CEduManagerApp::GetSecondsFrom1970()
{
	SYSTEMTIME st;
	unsigned int tTemp = 0;
	unsigned int tSecond = 0;
	int month_s[2][12] = { {31,28,31,30,31,30,31,31,30,31,30,31},
	{31,29,31,30,31,30,31,31,30,31,30,31} };
	int nDays = 0;
	int nCount = 0;
	int i;
	int j;
	GetLocalTime(&st);
	tSecond = st.wHour * 3600 + st.wMinute * 60 + st.wSecond;
	for (i = 1970; i < st.wYear; ++i)
	{
		if (IsRound(i))
			++nCount;
	}
	tTemp += (st.wYear - 1970 - nCount) * SECONDOFYEAR + nCount * SECONDOFROUNDYEAR;
	if (st.wMonth > 1)
	{
		if (IsRound(st.wYear))
		{
			for (j = 0; j < st.wMonth - 1; ++j) tTemp += month_s[1][j] * MAXSECONDOFDAY;
			tTemp += (st.wDay - 1) * MAXSECONDOFDAY + tSecond;
		}
		else
		{
			for (j = 0; j < st.wMonth - 1; ++j) tTemp += month_s[0][j] * MAXSECONDOFDAY;
			tTemp += (st.wDay - 1) * MAXSECONDOFDAY + tSecond;
		}
	}
	else tTemp += (st.wDay - 1) * MAXSECONDOFDAY + tSecond;
	return tTemp;
}

bool  CEduManagerApp::Contain(CString a, CString b)	//a是否包含于b
{
	if (a.GetLength() > b.GetLength())return false;
	for (size_t i = 0; i <= (size_t)(b.GetLength() - a.GetLength()); i++)
	{
		if (b[i] == a[0] || abs(b[i] - a[0]) == abs('A' - 'a'))
		{
			bool match = true;
			for (size_t j = 1; j < (size_t)a.GetLength(); j++)
			{
				if (b[i + j] != a[j] && abs(b[i + j] - a[j]) != abs('A' - 'a'))
				{
					match = false;
					break;
				}
			}
			if (match)
			{
				return true;
			}
		}
	}
	return false;
}

CString CEduManagerApp::RandomID(int n)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	CString res;
	res.Format(L"%d", st.wYear);
	for (size_t i = 0; i < (size_t)n-4; i++)
	{
		res += to_string(rand() % 10).c_str();
	}
	return res;
}

User* CEduManagerApp::FindUser(CString userID)
{
	User* res = nullptr;
	for (size_t i = 0; i < UserLib.size(); i++)
	{
		if (userID == UserLib.at(i).ID)
		{
			res = &(UserLib.at(i));
		}
	}
	return res;
}

Module* CEduManagerApp::FindModule(CString moduleID)
{
	Module* res = nullptr;
	for (size_t i = 0; i < ModuleLib.size(); i++)
	{
		if (moduleID == ModuleLib.at(i).ID)
		{
			res = &(ModuleLib.at(i));
		}
	}
	return res;
}

void CEduManagerApp::DelUser(CString userID)
{
	for (auto p = UserLib.begin();p != UserLib.end();p++)
	{
		if (p->ID == userID)
		{
			UserLib.erase(p);
			return;
		}
	}
}

void CEduManagerApp::DelModule(CString moduleID)
{
	for (auto p = ModuleLib.begin();p != ModuleLib.end();p++)
	{
		if (p->ID == moduleID)
		{
			ModuleLib.erase(p);
			return;
		}
	}
}

inline void DataBaseRoll(string & name,string & classID,string & major){//随机生成函数
	//单姓
	const vector<string> NA1 = { "赵", "钱", "孙", "李", "周", "吴", "郑", "王", "冯", "陈", "褚", "卫", "蒋", "沈", "韩", "杨", "朱", "秦", "尤", "许", "何", "吕", "施", "张", "孔", "曹", "严", "华", "金", "魏", "陶", "姜", "戚", "谢", "邹", "喻", "柏", "水", "窦", "章", "云", "苏", "潘", "葛", "奚", "范", "彭", "郎", "鲁", "韦", "昌", "马", "苗", "凤", "花", "方", "俞", "任", "袁", "柳", "酆", "鲍", "史", "唐", "费", "廉", "岑", "薛", "雷", "贺", "倪", "汤", "滕", "殷", "罗", "毕", "郝", "邬", "安", "常", "乐", "于", "时", "傅", "皮", "卞", "齐", "康", "伍", "余", "元", "卜", "顾", "孟", "平", "黄", "和", "穆", "萧", "尹", "姚", "邵", "湛", "汪", "祁", "毛", "禹", "狄", "米", "贝", "明", "臧", "计", "伏", "成", "戴", "谈", "宋", "茅", "庞", "熊", "纪", "舒", "屈", "项", "祝", "董", "梁", "杜", "阮", "蓝", "闵", "席", "季", "麻", "强", "贾", "路", "娄", "危", "江", "童", "颜", "郭", "梅", "盛", "林", "刁", "钟", "徐", "邱", "骆", "高", "夏", "蔡", "田", "樊", "胡", "凌", "霍", "虞", "万", "支", "柯", "昝", "管", "卢", "莫", "经", "房", "裘", "缪", "干", "解", "应", "宗", "丁", "宣", "贲", "邓", "郁", "单", "杭", "洪", "包", "诸", "左", "石", "崔", "吉", "钮"};
	//复姓
	const vector<string> NA2 = { "万俟", "司马", "上官", "欧阳", "夏侯", "诸葛", "闻人", "东方", "赫连", "皇甫", "尉迟", "公羊", "澹台", "公冶", "宗政", "濮阳", "淳于", "单于", "太叔", "申屠", "公孙", "仲孙", "轩辕", "令狐", "钟离", "宇文", "长孙", "慕容", "鲜于", "闾丘", "司徒", "司空", "亓官", "司寇", "子车", "颛孙", "端木", "巫马", "公西", "漆雕", "乐正", "壤驷", "公良", "拓跋", "夹谷", "宰父", "谷梁", "百里", "东郭", "南门", "呼延", "羊舌", "微生", "梁丘", "左丘", "东门", "西门", "南宫", "第五" };
	//男性常用名
	const vector<string> ME1m = { "伟", "刚", "勇", "毅", "俊", "峰", "强", "军", "平", "保", "东", "文", "辉", "力", "明", "永", "健", "世", "广", "志", "义", "兴", "良", "海", "山", "仁", "波", "宁", "贵", "福", "生", "龙", "元", "全", "国", "胜", "学", "祥", "才", "发", "武", "新", "利", "清", "飞", "彬", "富", "顺", "信", "子", "杰", "涛", "昌", "成", "康", "星", "光", "天", "达", "安", "岩", "中", "茂", "进", "林", "有", "坚", "和", "彪", "博", "诚", "先", "敬", "震", "振", "壮", "会", "思", "群", "豪", "心", "邦", "承", "乐", "绍", "功", "松", "善", "厚", "庆", "磊", "民", "友", "裕", "河", "哲", "江", "超", "浩", "亮", "政", "谦", "亨", "奇", "固", "之", "轮", "翰", "朗", "伯", "宏", "言", "若", "鸣", "朋", "斌", "梁", "栋", "维", "启", "克", "伦", "翔", "旭", "鹏", "泽", "晨", "辰", "士", "以", "建", "家", "致", "树", "炎", "德", "行", "时", "泰", "盛" };
	//女性常用名
	const vector<string> ME1f = { "秀", "娟", "英", "华", "慧", "巧", "美", "娜", "静", "淑", "惠", "珠", "翠", "雅", "芝", "玉", "萍", "红", "娥", "玲", "芬", "芳", "燕", "彩", "春", "菊", "兰", "凤", "洁", "梅", "琳", "素", "云", "莲", "真", "环", "雪", "荣", "爱", "妹", "霞", "香", "月", "莺", "媛", "艳", "瑞", "凡", "佳", "嘉", "琼", "勤", "珍", "贞", "莉", "桂", "娣", "叶", "璧", "璐", "娅", "琦", "晶", "妍", "茜", "秋", "珊", "莎", "锦", "黛", "青", "倩", "婷", "姣", "婉", "娴", "瑾", "颖", "露", "瑶", "怡", "婵", "雁", "蓓", "纨", "仪", "荷", "丹", "蓉", "眉", "君", "琴", "蕊", "薇", "菁", "梦", "岚", "苑", "筠", "柔", "竹", "霭", "凝", "晓", "欢", "霄", "枫", "芸", "菲", "寒", "欣", "滢", "伊", "亚", "宜", "可", "姬", "舒", "影", "荔", "枝", "思", "丽", "秀", "飘", "育", "馥", "琦", "晶", "妍", "茜", "秋", "珊", "莎", "锦", "黛", "青", "倩", "婷", "宁","蓓", "纨", "苑", "婕", "馨", "瑗", "琰", "韵", "融", "园", "艺", "咏", "卿", "聪", "澜", "纯", "毓", "悦", "昭", "冰", "爽", "琬", "茗", "羽", "希" };
	//班级 
	const vector<string> Class = {"16G01C","16G02C","16G03C","17G01C","17G02C","17G03C","17G04C","18G01C","18G02C","18G03C" ,"19G01C","19G02C","19G03C" ,"19G04C","20G01C","20G02C","20G03C","20G04C","19C19D" };	//专业 
	const vector<string> Major = {"软件工程","动画设计","网络工程","通信工程","网络安全","电气工程","电子信息科学","物联网工程","工商管理","计算机科学与技术" ,"计算机科学与技术" ,"理论物理","大数据"};
	bool sex = rand() % 2;
	name = (rand() % 20 != 0 ? NA1[rand() % NA1.size()] : NA2[rand() % NA2.size()]);//单姓Or复姓选择
	name += (sex  ? ME1m[rand()% ME1m.size()] : ME1f[rand()% ME1f.size()]);//取名第一字
	if (rand() % 5 != 0) name += (sex ? ME1m[rand() % ME1m.size()] : ME1f[rand() % ME1f.size()]);//取名第二字
	classID = Class[(rand()) % Class.size()];//选择班级
	major = Major[(rand()) % Major.size()];//选择专业
}

User CEduManagerApp::RandomUser(int userType)
{
	User u;
	while (true)
	{
		CString id = RandomID(11);
		if (FindUser(id) == nullptr)
		{
			u.ID = id;
			u.password = L"Psw" + u.ID;
			break;
		}
	}

	string name, classID, major;
	DataBaseRoll(name, classID, major);

	u.name = name.c_str();
	u.remUsn = u.remPsw = 1;
	u.lastLogin = GetSecondsFrom1970();
	u.userType = userType;
	if (u.userType == 3)
	{
		u.classID = classID.c_str();
		u.major = major.c_str();
	}
	else
	{
		u.classID = "NULL";
		u.major = "NULL";
	}
	return u;
}

Module CEduManagerApp::RandomModule() 
{
	Module m;
	//课程名称
	const vector<string> moduleName = { "马克思主义基本原理", "大学英语", "计算科学导论", "计算机高级语言程序设计", "概率论与数理统计", "离散数学", "高等数学", "数据结构", "计算机组成原理", "操作系统原理", "计算机网络", "数据库系统概论", "面向对象程序设计", "汇编语言程序设计", "计算方法","编译原理" ,"计算机体系结构" ,"C#程序设计" ,"Linux系统应用与开发" ,"Java程序设计" ,"并行计算"};
	//学分
	const vector<double> credit = { 1.0,1.5,2.0,2.5,3.0,4.0,5.0 };
	//学时
	const vector<int> hours = { 20,22,25,28,30,32,35,38,40,42 };
	//课程名称
	const vector<string> moduleComment = {"真的非常喜欢这门课，老师非常有耐心，讲解的很仔细。下次还要选这个老师的课。","感谢老师，给分是真的高呀哈哈哈！","是我太菜了吗呜呜呜，上课的内容我基本跟不上，课下得花很长的时间自学呜呜呜~希望老师讲课的速度可以放慢一点。","感谢老师！！","五星好评，老师讲解真的很有耐心呀！！","下次还想选这个老师的课。","差点挂科。。。谢谢老师的仁慈呜呜呜我下学期一定好好学习。","感谢老师的孜孜教诲！！","老师的提纲真的做的非常好呀，每节课都会给我们归纳知识点，给老师点个赞！","果然还是我太菜了，好难呀呜呜呜~~~~","下学期还选老师的课！","谢谢老师一个学习的教诲真的有学到很多有用的知识。","这种基础课程真的非常重要呀，老师讲的也非常好！！","噗，感觉老师有点过于严肃。。我都不敢问老师问题。。怕被骂。","谢谢老师！","下次有机会还想上老师的课。","为什么这么难呀！！！根本听不懂。。","谢谢老师~","我哭了，挂科了。","希望老师讲课速度可以快一点。。有点催眠。","老师的作业未免有点太多了。。根本写不完呀！！！","希望老师作业可以多一点，感觉非常学有余力。","老师再见！！！","希望还能遇到这么好的老师。","很好","挺满意的。","平时分占比有点高呀。。","老师真的太有耐心呀！！！好多我不会的东西老师真的超有耐心的讲解！！谢谢老师没有嫌我笨哈哈哈！！","五星好评！","不是很喜欢老师的教学方式","感觉老师上课的时候有点无聊，让我没法听下去。","老师的课堂真的太精彩了，每周都很期待老师的课！！"};
	m.ID = RandomID(8);
	m.name = moduleName[rand() % moduleName.size()].c_str();
	vector<User>teacher;
	for (auto p = UserLib.begin(); p != UserLib.end(); p++)
	{
		if (p->userType == 2)
		{
			teacher.push_back(*p);
		}
	}
	m.teacherID = teacher[rand() % teacher.size()].ID;
	m.credit = credit[rand() % credit.size()];
	m.hours = hours[rand() % hours.size()];
	m.compulsory = rand() % 3 == 0 ? 1 : 0;

	vector<User>student;
	for (auto p = UserLib.begin(); p != UserLib.end(); p++)
	{
		if (p->userType == 3)
		{
			student.push_back(*p);
		}
	}
		
	unsigned int attendSz = 100 + rand() % 40;
	if (attendSz > student.size())attendSz = student.size();

	while (attendSz)
	{
		User stu = student.at(rand() % student.size());
		bool attended = false;
		for (auto p = EduManager.ModuleLib.begin();p != EduManager.ModuleLib.end();p++)
		{
			if (p->name == m.name && p->FindStudent(stu.ID) != nullptr)
			{
				attended = true;
				break;
			}
		}
		if (attended)
		{
			continue;
		}
		else
		{
			scoreInfo at;
			at.studentID = stu.ID;
			at.score = rand() % 3 == 0 ? -1 : 58 + (rand() % 42);
			at.reScore = 50 + rand() % 50;
			at.comment = moduleComment[rand() % moduleComment.size()].c_str();
			m.attend.push_back(at);
			attendSz--;
		}
	}

	return m;
}

void CEduManagerApp::AddRandomStuff(unsigned int adminNum, unsigned int teacherNum, unsigned int studentNum, unsigned int moduleNum)
{
	for (size_t i = 0; i < adminNum; i++) UserLib.push_back(RandomUser(1));
	for (size_t i = 0; i < teacherNum; i++) UserLib.push_back(RandomUser(2));
	for (size_t i = 0; i < studentNum; i++) UserLib.push_back(RandomUser(3));
	for (size_t i = 0; i < moduleNum; i++) ModuleLib.push_back(RandomModule());
}

string CEduManagerApp::CSTS(CString cstr)
{
	LPCWSTR wstr(cstr);

	int size = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);

	char* p = new char[size + 1];

	WideCharToMultiByte(CP_ACP, 0, wstr, -1, p, size, NULL, NULL);

	return p;
}

LPCWSTR CEduManagerApp::StringToLPCWSTR(string str)
{
	int size = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t* buffer = new wchar_t[size + 1];

	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, size);

	return buffer;
}

string CEduManagerApp::LPCWSTRToString(LPCWSTR wstr)
{
	int size = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);

	char* p = new char[size + 1];

	WideCharToMultiByte(CP_ACP, 0, wstr, -1, p, size, NULL, NULL);

	return p;
}

void CEduManagerApp::OnQuit()
{
	// TODO: 在此添加命令处理程序代码
	PostQuitMessage(0);
}


void CEduManagerApp::OnOpen()
{
	// TODO: 在此添加命令处理程序代码
	
}


void CEduManagerApp::OnAbout()
{
	// TODO: 在此添加命令处理程序代码
	AboutBox aboutBox;
	aboutBox.DoModal();
}

string CEduManagerApp::eraseBlank(CString cs)
{
	for (size_t i = 0; i < (size_t)cs.GetLength(); i++)
	{
		if (cs[i] == ' ' || cs[i] == '\t')
		{
			cs = cs.Left(i) + cs.Right(cs.GetLength() - i - 1);
		}
	}
	string s = CT2A(cs.GetString());
	return s;
}