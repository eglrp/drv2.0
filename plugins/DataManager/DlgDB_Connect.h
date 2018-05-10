#pragma once
#include "resource.h"
#include "afxcmn.h"
#include "sqlite3.h"
#include "afxwin.h"
#include <vector>
#include "winsock.h" 
#include "mysql.h"

UINT StartThreadForSQL(void *param);//�����̺߳���
UINT StartThreadForStatistics(void*);
UINT StartThreadForReConnect(void *param);//�����̺߳��� �����������ݿ�
UINT StartThreadForInsertDB(void *param);
UINT getDataFromDB();
UINT StatisticsInfoFromDB();

struct DATACONTAINER{
	std::list<std::string> LData;
	std::string lastUpdateTime;
};

struct DATA_UP{
	DATA_UP(std::string s,int a,int io,int f)
	{
		sFiled = s;
		age = a;
		inout = io;
		familyMem = f;
	}
	std::string sFiled;
	int age;
	int inout;
	int familyMem;
};
// CDlgDB_Connect �Ի���

class CDlgDB_Connect : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDB_Connect)

public:
	CDlgDB_Connect(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgDB_Connect();

// �Ի�������
	enum { IDD = IDD_DIALOG_CONNECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	UINT getData();
	void setNetState(bool b);//��������״̬

private:
	int m_row;
	int m_column;

public:
	std::vector<CString> vPassword;
	bool bConnect;
	MYSQL myCont;

public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusEdit1();
	afx_msg void OnBnClickedButtonNew();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonDisconnect();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonExit();

	CListCtrl m_wndList;
	CEdit m_edit;
	CComboBox m_combox;
	CStatic m_staticInfo;
	int m_rowAbled;
};
