// DlgDB_Connect.cpp : 实现文件
//

#include "stdafx.h"
#include "DataManager.h"
#include "DlgDB_Connect.h"
#include "afxdialogex.h"
#include <list>
#include <fstream>
#include "osgDB/ReadFile"
#include "osgDB/FileNameUtils"
#include "osgDB/FileUtils"
#include "EncodeString.h"
#include "updateShpFile.h"

std::string IP_SERVER;
std::string USER_NAME;
std::string USER_PASSWORD;
std::string DATABASE_NAME = "lxc";
std::string TABLE_1;
UINT PORT_NUM = 3306;

const int recordNum = 100;
const int saveRecordNum = 5;
using namespace std;


bool gConnect;
sqlite3* sql3con;

bool isOK = false;

CCriticalSection gCriticalSection;//临界区锁
std::string latestUpdateTime;//最近跟新的记录时间，用于筛选记录

//data
DATACONTAINER vlxcclck,vlxcclrk,vdoor6,vpolices,vmanHole,vsmokeSensor,vdoorSensor,veleMeter;

//创建本地sqlite3数据库
sqlite3 * OpenLocalDatabase(char* dataBaseFileName)
{
	sqlite3 *pDB = NULL;
	int result = sqlite3_open(dataBaseFileName, &pDB);
	return pDB;
}
//创建本地sqlite3数据库表
void create_Table(sqlite3 *pDB, char* createTableSql)
{
	int result = sqlite3_exec(pDB, createTableSql, 0, 0, NULL);
}
//删除记录
void ClearAllRecord(sqlite3 *pDB, char* deleteSql)
{
	sqlite3_exec(pDB, "BEGIN TRANSACTION;", NULL, NULL, NULL);
	sqlite3_exec(pDB, deleteSql, NULL, NULL, NULL);
	sqlite3_exec(pDB, "COMMIT TRANSACTION;", NULL, NULL, NULL);
}
//插入记录
void InsertRecord(sqlite3 *pDB, char* insertTableRecord)
{
	int result = sqlite3_exec(pDB, insertTableRecord, 0, 0, NULL);
}
void StartBegin(sqlite3 *pDB)
{
	sqlite3_exec(pDB, "begin;", 0, 0, 0);
}
void Commit(sqlite3 *pDB)
{
	sqlite3_exec(pDB, "commit;", 0, 0, 0);
}
//关闭数据库
void CloseDatabase(sqlite3 *pDB)
{
	sqlite3_close(pDB);
}
//写入数据库
void save_data(sqlite3 *sql3con)
{
	char querys[99];
	sql3con = OpenLocalDatabase("DRV.db");
	ClearAllRecord(sql3con, "delete from lxcclrk");
	StartBegin(sql3con);
	strcpy(querys, "insert into lxcclrk(info)values('%s')");
	std::list<std::string> ls;
	COMFUNC::getLastNRecord(vlxcclrk.LData,ls,saveRecordNum);
	for (list<string>::iterator ite = ls.begin();ite != ls.end();++ite)
	{
		char* InsertSql = sqlite3_mprintf(querys, (*ite).c_str());
		InsertRecord(sql3con, InsertSql);
	}
	ClearAllRecord(sql3con, "delete from lxcclck");
	StartBegin(sql3con);
	strcpy(querys, "insert into lxcclck(info)values('%s')");
	COMFUNC::getLastNRecord(vlxcclck.LData,ls,saveRecordNum);
	for (list<string>::iterator ite = ls.begin();ite != ls.end();++ite)
	{
		char* InsertSql = sqlite3_mprintf(querys, (*ite).c_str());
		InsertRecord(sql3con, InsertSql);
	}

	ClearAllRecord(sql3con, "delete from door6");
	StartBegin(sql3con);
	strcpy(querys, "insert into door6(info)values('%s')");
	COMFUNC::getLastNRecord(vdoor6.LData,ls,saveRecordNum);
	for (list<string>::iterator ite = ls.begin();ite != ls.end();++ite)
	{
		char* InsertSql = sqlite3_mprintf(querys, (*ite).c_str());
		InsertRecord(sql3con, InsertSql);
	}

	ClearAllRecord(sql3con, "delete from manHole");
	StartBegin(sql3con);
	strcpy(querys, "insert into manHole(info)values('%s')");
	COMFUNC::getLastNRecord(vmanHole.LData,ls,saveRecordNum);
	for (list<string>::iterator ite = ls.begin();ite != ls.end();++ite)
	{
		char* InsertSql = sqlite3_mprintf(querys, (*ite).c_str());
		InsertRecord(sql3con, InsertSql);
	}
	
	ClearAllRecord(sql3con, "delete from smokeSensor");
	StartBegin(sql3con);
	strcpy(querys, "insert into smokeSensor(info)values('%s')");
	COMFUNC::getLastNRecord(vsmokeSensor.LData,ls,saveRecordNum);
	for (list<string>::iterator ite = ls.begin();ite != ls.end();++ite)
	{
		char* InsertSql = sqlite3_mprintf(querys, (*ite).c_str());
		InsertRecord(sql3con, InsertSql);
	}

	ClearAllRecord(sql3con, "delete from doorSensor");
	StartBegin(sql3con);
	strcpy(querys, "insert into doorSensor(info)values('%s')");
	COMFUNC::getLastNRecord(vdoorSensor.LData,ls,saveRecordNum);
	for (list<string>::iterator ite = ls.begin();ite != ls.end();++ite)
	{
		char* InsertSql = sqlite3_mprintf(querys, (*ite).c_str());
		InsertRecord(sql3con, InsertSql);
	}

	ClearAllRecord(sql3con, "delete from eleMeter");
	StartBegin(sql3con);
	strcpy(querys, "insert into eleMeter(info)values('%s')");
	COMFUNC::getLastNRecord(veleMeter.LData,ls,saveRecordNum);
	for (list<string>::iterator ite = ls.begin();ite != ls.end();++ite)
	{
		char* InsertSql = sqlite3_mprintf(querys, (*ite).c_str());
		InsertRecord(sql3con, InsertSql);
	}
	Commit(sql3con);
	CloseDatabase(sql3con);
}
// CDlgDB_Connect 对话框

IMPLEMENT_DYNAMIC(CDlgDB_Connect, CDialogEx)

CDlgDB_Connect::CDlgDB_Connect(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgDB_Connect::IDD, pParent)
{
	sql3con = OpenLocalDatabase("DRV.db");
	create_Table(sql3con, "create Table lxcclrk(info);");
	ClearAllRecord(sql3con, "delete from lxcclrk");

	create_Table(sql3con, "create Table lxcclck(info);");
	ClearAllRecord(sql3con, "delete from lxcclck");
	
	create_Table(sql3con, "create Table door6(info);");
	ClearAllRecord(sql3con, "delete from door6");

	create_Table(sql3con, "create Table manHole(info);");
	ClearAllRecord(sql3con, "delete from manHole");

	create_Table(sql3con, "create Table smokeSensor(info);");
	ClearAllRecord(sql3con, "delete from smokeSensor");

	create_Table(sql3con, "create Table doorSensor(info);");
	ClearAllRecord(sql3con, "delete from doorSensor");

	create_Table(sql3con, "create Table eleMeter(info);");
	ClearAllRecord(sql3con, "delete from eleMeter");
	CloseDatabase(sql3con);
}

CDlgDB_Connect::~CDlgDB_Connect()
{
	if (bConnect)
	{
		mysql_close(&myCont);//断开连接
	}	
}

void CDlgDB_Connect::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_wndList);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	DDX_Control(pDX, IDC_COMBO1, m_combox);
	DDX_Control(pDX, IDC_STATIC_INFO, m_staticInfo);
}


BEGIN_MESSAGE_MAP(CDlgDB_Connect, CDialogEx)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDlgDB_Connect::OnNMDblclkList1)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CDlgDB_Connect::OnEnKillfocusEdit1)
	ON_BN_CLICKED(IDC_BUTTON_NEW, &CDlgDB_Connect::OnBnClickedButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CDlgDB_Connect::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CDlgDB_Connect::OnBnClickedButtonConnect)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CDlgDB_Connect::OnNMClickList1)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, &CDlgDB_Connect::OnBnClickedButtonDisconnect)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CDlgDB_Connect::OnBnClickedButtonApply)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CDlgDB_Connect::OnBnClickedButtonExit)
END_MESSAGE_MAP()


// CDlgDB_Connect 消息处理程序
void CDlgDB_Connect::setNetState(bool b)
{
	bConnect = b;
	gConnect = b;
}

BOOL CDlgDB_Connect::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetWindowPos(&CWnd::wndTopMost,0,0,200,200,SWP_NOSIZE | SWP_NOMOVE);
	RECT r ;
	m_wndList.GetWindowRect(&r);
	int w = r.right - r.left;

	m_wndList.InsertColumn (0, _T("IP"), LVCFMT_CENTER, w/10*2.5);
	m_wndList.InsertColumn (1, _T("端口号"), LVCFMT_CENTER, w/10*1.5);
	m_wndList.InsertColumn (2, _T("用户名"), LVCFMT_CENTER, w/5);
	m_wndList.InsertColumn (3, _T("密码"), LVCFMT_CENTER, w/5);
	m_wndList.InsertColumn (4, _T("状态"), LVCFMT_CENTER, w/5);
	
	//检测账号记录文件
	if (osgDB::fileExists("Data\\DRVdata\\server.dat"))
	{
		char s1[99],s2[99],s3[99],s4[99];
		std::fstream file ;
		string ss;
		file.open("Data\\DRVdata\\server.dat",ios_base::in);
		if (file)
		{
			while(!file.eof())
			{
				file>>s1>>s2>>s3>>s4;
				if (strcmp(s1,"") == 0)
				{
					continue;
				}
				ss = string(s4);
				ENCODESTRING::EncodeString(ss,"visiontek",false);
				int m = m_wndList.GetItemCount();
				m_wndList.InsertItem(m, _T("1"));
				m_wndList.SetItemText (m, 0, CString(s1));
				m_wndList.SetItemText (m, 1, CString(s2));
				m_wndList.SetItemText (m, 2, CString(s3));
				string pw(ss.size(),'*');
				if (ss == "<NULL>")
				{
					pw = "<NULL>";
				}
				m_wndList.SetItemText (m, 3, CString(pw.c_str()));
				m_wndList.SetItemText (m, 4, _T("未连接"));

				vPassword.push_back(CString(ss.c_str()));
			}
		}
	}
	
	m_wndList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,
		LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES );
	m_edit.ShowWindow(SW_HIDE);
	m_combox.SetCurSel(1);
	m_staticInfo.SetWindowTextW(_T("未连接任何服务器"));
	setNetState(false);//设置网络未连接
	m_rowAbled = -1;

	////////////////////以下代码可读取更新时间////////////
	/*std::ifstream file;
	file.open("Data\\DRVdata\\sqlTime.dat");
	if (file)
	{ 
		getline(file, latestUpdateTime);
		file.close();
	}
	else*/
		latestUpdateTime = "2000-01-01 00:00:00";

	SetTimer(1,10000,NULL);
	SetTimer(2,1800000,NULL);
	SetTimer(3,5000,NULL);
	CWinThread* sqlThread =  AfxBeginThread(StartThreadForStatistics, 
		this,
		THREAD_PRIORITY_NORMAL,
		0,
		0,
		NULL);  
	sqlThread->ResumeThread();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

UINT CDlgDB_Connect::getData()
{
	getDataFromDB();
	return 0;
}

void CDlgDB_Connect::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	FILE* outfile = fopen("temp.dat","w");
	if (bConnect)
	{
		fprintf(outfile,"%d",1);//将网络状态写入文件  传递给其他模块调用
		if (!isOK)//判断数据库读取是否正常
		{
			mysql_close(&myCont);
			setNetState(false);
			m_wndList.SetItemText(m_rowAbled,4,_T("访问异常"));
			m_staticInfo.SetWindowTextW(_T("已关闭当前连接"));
			m_rowAbled = -1;
			return;
		}
	}
	else
		fprintf(outfile,"%d",0);
	fclose(outfile);

	if (nIDEvent == 1)//定时从数据库读数据
	{
		CWinThread* sqlThread =  AfxBeginThread(StartThreadForSQL, 
			this,
			THREAD_PRIORITY_NORMAL,
			0,
			0,
			NULL);  
		sqlThread->ResumeThread();
	}
	else if (nIDEvent == 2 || nIDEvent == 3)
	{
		CWinThread* sqlThread =  AfxBeginThread(StartThreadForStatistics, 
			this,
			THREAD_PRIORITY_NORMAL,
			0,
			0,
			NULL);  
		sqlThread->ResumeThread();
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CDlgDB_Connect::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
}


int CDlgDB_Connect::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}


void CDlgDB_Connect::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;
	CRect rc;
	if(pNMListView->iItem!=-1)
	{
		m_row=pNMListView->iItem;//m_row为被选中行的行序号（int类型成员变量）
		m_column=pNMListView->iSubItem;//m_column为被选中行的列序号（int类型成员变量）
		if (m_column == 4)
		{
			return;
		}
		m_wndList.GetSubItemRect(pNMListView->iItem, pNMListView->iSubItem,LVIR_LABEL,rc);//取得子项的矩形
		rc.left+=14;
		rc.top+=14;
		rc.right+=14;
		rc.bottom+=14;

		m_edit.SetWindowText(m_wndList.GetItemText(pNMListView->iItem, pNMListView->iSubItem));//将子项的内容显示到编辑框中
		m_edit.ShowWindow(SW_SHOW);//显示编辑框
		m_edit.MoveWindow(&rc);//将编辑框移动到子项上面，覆盖在子项上
		m_edit.SetFocus();//使编辑框取得焦点
		m_edit.CreateSolidCaret(1,rc.Height()-5);//创建一个光标
		m_edit.ShowCaret();//显示光标
		m_edit.SetSel(-1);//使光标移到最后面
	}
	*pResult = 0;
}


void CDlgDB_Connect::OnEnKillfocusEdit1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	m_edit.GetWindowText(str);//取得编辑框的内容
	vPassword[m_row] = str;
	if (m_column == 3)
	{
		std::string s(str.GetLength(),'*');
		m_wndList.SetItemText(m_row,m_column,CString(s.c_str()));//将该内容更新到CListCtrl中
	}
	else
		m_wndList.SetItemText(m_row,m_column,str);//将该内容更新到CListCtrl中
	m_edit.ShowWindow(SW_HIDE);//隐藏编辑框
}


void CDlgDB_Connect::OnBnClickedButtonNew()
{
	// TODO: 在此添加控件通知处理程序代码
	int m = m_wndList.GetItemCount();
	m_wndList.InsertItem(m, _T("1"));

	m_wndList.SetItemText (m, 0, _T("<NULL>"));
	m_wndList.SetItemText (m, 1, _T("<NULL>"));
	m_wndList.SetItemText (m, 2, _T("<NULL>"));
	m_wndList.SetItemText (m, 3, _T("******"));
	m_wndList.SetItemText (m, 4, _T("未连接"));
	m_wndList.SetHotItem(m_wndList.GetItemCount()-1);

	vPassword.push_back(_T("<NULL>"));
}


void CDlgDB_Connect::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	int n = m_wndList.GetSelectionMark();
	if (n == -1)
	{
		return;
	}
	if (m_wndList.GetItemText(n,4) == _T("已连接"))
	{
		mysql_close(&myCont);
		setNetState(false);
		m_staticInfo.SetWindowTextW(_T("未连接任何服务器"));

		m_rowAbled = -1;
	}
	m_wndList.DeleteItem(n);
	m_wndList.SetSelectionMark(-1);
	if (bConnect)
	{
		if (n<m_rowAbled)
		{
			m_rowAbled -= 1;
		}
	}

	vPassword.erase(vPassword.begin()+n);
}


void CDlgDB_Connect::OnBnClickedButtonConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	USES_CONVERSION;
	int m = m_wndList.GetSelectionMark();
	if (m == -1)
	{
		return;
	}
	AfxGetMainWnd()->BeginWaitCursor();
	if (bConnect)
	{
		mysql_close(&myCont);
		setNetState(false);
		for (int i = 0;i<m_wndList.GetItemCount();++i)
		{
			m_wndList.SetItemText(i,4,_T("未连接"));
		}
	}
	m_wndList.SetItemText(m_row,4,_T("正在连接"));
	UpdateWindow();
	mysql_init(&myCont);
	IP_SERVER = string(W2A(m_wndList.GetItemText(m_row,0))).c_str();
	USER_NAME = string(W2A(m_wndList.GetItemText(m_row,2))).c_str();
	USER_PASSWORD = string(W2A(vPassword[m_row])).c_str();
	PORT_NUM = atoi(string(W2A(m_wndList.GetItemText(m_row,1))).c_str());
	if(mysql_real_connect(&myCont,IP_SERVER.c_str(),USER_NAME.c_str(),USER_PASSWORD.c_str(),DATABASE_NAME.c_str(),PORT_NUM,NULL,0))
	{
		setNetState(true);
		isOK = true;
		m_wndList.SetItemText(m_row,4,_T("已连接"));
		CString s;
		m_combox.GetLBText(m_combox.GetCurSel(),s);
		m_staticInfo.SetWindowTextW(_T("已连接服务器：") + m_wndList.GetItemText(m_row,0) +  _T("(")+ s + _T("秒刷新)"));
		m_rowAbled = m_row;
		CWinThread* sqlThread =  AfxBeginThread(StartThreadForSQL, 
			this,
			THREAD_PRIORITY_NORMAL,
			0,
			0,
			NULL);  
		sqlThread->ResumeThread();
	}
	else
	{
		setNetState(false);
		m_wndList.SetItemText(m_row,4,_T("连接失败"));
		m_staticInfo.SetWindowTextW(_T("未连接任何服务器"));
		m_rowAbled = -1;
	}
	AfxGetMainWnd()->EndWaitCursor();
}

void CDlgDB_Connect::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;

	if(pNMListView->iItem!=-1)
	{
		m_row=pNMListView->iItem;//m_row为被选中行的行序号（int类型成员变量）
		m_column=pNMListView->iSubItem;//m_column为被选中行的列序号（int类型成员变量）
	}
	*pResult = 0;
}


void CDlgDB_Connect::OnBnClickedButtonDisconnect()
{
	// TODO: 在此添加控件通知处理程序代码
	if (bConnect && m_row == m_rowAbled)
	{
		mysql_close(&myCont);
		setNetState(false);
		m_wndList.SetItemText(m_row,4,_T("未连接"));
		m_staticInfo.SetWindowTextW(_T("未连接任何服务器"));

		FILE* outfile = fopen("temp.dat","w");
		fprintf(outfile,"%d",0);
		fclose(outfile);
	}
}


void CDlgDB_Connect::OnBnClickedButtonApply()
{
	// TODO: 在此添加控件通知处理程序代码
	USES_CONVERSION;
	int n = m_combox.GetCurSel();
	if(n == -1)
	{
		return;
	}
	KillTimer(1);
	CString s;
	m_combox.GetLBText(n,s);
	string s1 = W2A(s);
	int k = atoi(s1.c_str());
	SetTimer(1,k*1000,NULL);
	if (bConnect)
	{
		m_staticInfo.SetWindowTextW(_T("已连接服务器：") + m_wndList.GetItemText(m_rowAbled,0) +  _T("(")+ s + _T("秒刷新)"));
	}
}


void CDlgDB_Connect::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ShowWindow(SW_HIDE);
	//CDialogEx::OnClose();
}


BOOL CDlgDB_Connect::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)  
	{  
		switch(pMsg->wParam)  
		{  
		case VK_ESCAPE: //Esc按键事件 
			OnClose();
			return true;  
		case VK_RETURN: //Enter按键事件 
			OnClose();
			return true;  
		default:  
			;  
		}  
	}  
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDlgDB_Connect::OnBnClickedButtonExit()
{
	// TODO: 在此添加控件通知处理程序代码
	OnClose();
}

CDlgDB_Connect* g_DBconnector;

//线程函数  统计数据库信息
UINT StartThreadForStatistics(void* param)
{
	if (!gConnect)
	{
		return 1;
	}
	CDlgDB_Connect* dbc = (CDlgDB_Connect*)param;
	dbc->KillTimer(3);
	StatisticsInfoFromDB();
	return 0;
}

UINT StatisticsInfoFromDB()
{
	if (!gConnect)
	{
		return 1;
	}
	MYSQL sqlCont;
	MYSQL_RES *result = NULL,*iresult = NULL;
	MYSQL_ROW sql_row;
	int res;

	std::vector<pair<std::string,std::string> >vecPair;
	std::vector<std::string> vecID;
	//单独建立连接访问数据库
	mysql_init(&sqlCont);
	if(!mysql_real_connect(&sqlCont,IP_SERVER.c_str(),USER_NAME.c_str(),USER_PASSWORD.c_str(),DATABASE_NAME.c_str(),PORT_NUM,NULL,0))
	{
		return 1;
	}
	mysql_query(&sqlCont, "SET NAMES GBK"); //设置编码格式,否则在cmd下无法显示中文

	/***************查询所有的房间号*************/
	res = mysql_query(&sqlCont, "SELECT BUILDING_NAME,DOOR_NAME FROM lxc.acs_house");
	if (!res)//查询成功
	{
		result=mysql_store_result(&sqlCont);//保存查询到的数据到result
		if(result)
		{
			while(sql_row=mysql_fetch_row(result))//获取具体的数据
			{
				char sBUILDING_NAME[255];
				char sDOOR_NAME[255];
				sprintf(sBUILDING_NAME,"%s",sql_row[0]);
				sprintf(sDOOR_NAME,"%s",sql_row[1]);
				vecPair.push_back(make_pair<std::string,std::string>(std::string(sBUILDING_NAME),std::string(sDOOR_NAME)));
			}
		}
	}
	if (vecPair.empty())
	{
		return 1;
	}

	mysql_query(&sqlCont, "delete from lxc.house_stats");
	
	/***************根据房间号查询信息*************/
	for (int i = 0;i<vecPair.size();++i)
	{
		std::string query = "select lxc.acs_resident.PEOPLE_IDCARD from lxc.acs_resident where lxc.acs_resident.ID in         \
			(select lxc.acs_resident_house_rel.RESIDENT_ID from lxc.acs_resident_house_rel where lxc.acs_resident_house_rel.HOUSE_ID in          \
			(select lxc.acs_house.ID from lxc.acs_house where lxc.acs_house.DOOR_NAME = '" + vecPair[i].second + 
			"' and acs_house.BUILDING_NAME = '" + vecPair[i].first + "') )";
		res = mysql_query(&sqlCont,query.c_str());//查询
		if (!res)
		{
			result = mysql_store_result(&sqlCont);//保存查询到的数据到result
			if(result)
			{
				vecID.clear();
				while(sql_row=mysql_fetch_row(result))//获取具体的数据
				{
					char sIDCARD[255];
					sprintf(sIDCARD,"%s",sql_row[0]);
					vecID.push_back(std::string(sIDCARD));
				}
				if (!vecID.empty())
				{
					int maxAge = COMFUNC::findOlder(vecID);
					int familyMem = vecID.size();
					int inout = -1;
					std::string s = COMFUNC::getDoorName(vecPair[i].first,vecPair[i].second);
					char qry[255];
					sprintf_s(qry,"INSERT INTO lxc.house_stats VALUES('%s', '%d', '%d','%d')",s.c_str(), maxAge,familyMem,inout);
					res = mysql_query(&sqlCont,qry);//插入表数据	
					if (res)
					{
						continue;;
					}
					iresult = mysql_store_result(&sqlCont);
					mysql_free_result(iresult);
				}
			}
		}
	}	
	return 0;
}

//线程函数  
UINT StartThreadForInsertDB(void *param)
{
	return 0;
}
//线程函数  访问数据库
UINT StartThreadForSQL(void *param)
{
	if (!gConnect)
	{
		return 1;
	}
	getDataFromDB();
	return 0;
}

UINT getDataFromDB()
{
	if (!gConnect)
	{
		return 1;
	}
	MYSQL sqlCont;
	MYSQL_RES *result = NULL;
	MYSQL_ROW sql_row;
	int res;
	//单独建立连接访问数据库
	mysql_init(&sqlCont);
	if(!mysql_real_connect(&sqlCont,IP_SERVER.c_str(),USER_NAME.c_str(),USER_PASSWORD.c_str(),DATABASE_NAME.c_str(),PORT_NUM,NULL,0))
	{
		isOK = false;
		return 1;
	}
	mysql_query(&sqlCont, "SET NAMES GBK"); //设置编码格式,否则在cmd下无法显示中文
	//定义查询命令
	std::string query1("select * from lxc_real_data where RECORD_TIME not between '1999-12-31 11:00:00' and '");
	std::string query2("' order by RECORD_TIME");
	std::string sQuery;
	sQuery += query1;
	sQuery += latestUpdateTime;
	sQuery += query2;
	
	res=mysql_query(&sqlCont,sQuery.c_str());//查询
	
	if(!res)
	{
		isOK = true;
		result=mysql_store_result(&sqlCont);//保存查询到的数据到result
		if(result)
		{
			int j;
			j=mysql_num_fields(result);
			if (j<4 )
			{
				mysql_free_result(result);//释放结果资源
				return 1;//数据异常
			}
			bool isNew = false;
			while(sql_row=mysql_fetch_row(result))//获取具体的数据
			{
				isNew = true;
				char sID[50];
				char sData[1024];
				char sTime[50];
				sprintf(sID,"%s",sql_row[1]);
				sprintf(sTime,"%s",sql_row[2]);
				sprintf(sData,"%s",sql_row[3]);
				if (strcmp(sID,"lxc_entrance") == 0)
				{
					vlxcclrk.LData.push_back(string(sData));
					vlxcclrk.lastUpdateTime = std::string(sTime);
					if (vlxcclrk.LData.size()>5)
					{
						vlxcclrk.LData.pop_front();
					}
				}
				else if (strcmp(sID,"lxc_exit") == 0)
				{
					vlxcclck.LData.push_back(string(sData));
					vlxcclck.lastUpdateTime = std::string(sTime);
					if (vlxcclck.LData.size()>recordNum)
					{
						vlxcclck.LData.pop_front();
					}
				}
				else if (strcmp(sID,"swipe_records") == 0)
				{
					vdoor6.LData.push_back(string(sData));
					vdoor6.lastUpdateTime = std::string(sTime);
					if (vdoor6.LData.size()>recordNum)
					{
						vdoor6.LData.pop_front();
					}
				}
				else if (strcmp(sID,"person_location") == 0)
				{
					vpolices.LData.push_back(string(sData));
					vpolices.lastUpdateTime = std::string(sTime);
				}
				else if (strcmp(sID,"man_hole") == 0)//井盖
				{
					vmanHole.LData.push_back(string(sData));
					vmanHole.lastUpdateTime = std::string(sTime);
					if (vmanHole.LData.size()>recordNum)
					{
						vmanHole.LData.pop_front();
					}
				}
				else if (strcmp(sID,"smoke_sensor") == 0)//烟感
				{
					vsmokeSensor.LData.push_back(string(sData));
					vsmokeSensor.lastUpdateTime = std::string(sTime);
					if (vsmokeSensor.LData.size()>recordNum)
					{
						vsmokeSensor.LData.pop_front();
					}
				}
				else if (strcmp(sID,"door_sensor") == 0)//门磁
				{
					vdoorSensor.LData.push_back(string(sData));
					vdoorSensor.lastUpdateTime = std::string(sTime);
					if (vdoorSensor.LData.size()>recordNum)
					{
						vdoorSensor.LData.pop_front();
					}
				}
				else if (strcmp(sID,"ele_meter") == 0)//电表
				{
					veleMeter.LData.push_back(string(sData));
					veleMeter.lastUpdateTime = std::string(sTime);
					if (veleMeter.LData.size()>recordNum)
					{
						veleMeter.LData.pop_front();
					}
				}
				else
					continue;
				latestUpdateTime = std::string(sTime);//更新时间				
			}
			if (isNew)
			{
				save_data(sql3con);
			}
			mysql_free_result(result);//释放结果资源
		}	
	}
	else
		isOK = false;
	mysql_close(&sqlCont);
	return 0;
}

