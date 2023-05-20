// ServerDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "stdlib.h"
#include "time.h"

#define PORT 25000
#define WM_SOCKET WM_USER+1

// CServerDlg dialog
class CServerDlg : public CDialog
{
	// Construction
public:
	CServerDlg(CWnd* pParent = NULL);	// standard constructor

	// Dialog Data
	enum { IDD = IDD_SERVER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


	// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	LRESULT SockMsg(WPARAM wParam, LPARAM lParam);
	char* ConvertToChar(const CString &s);
	void Split(CString src,CString des[2]);
	void mSend(SOCKET sk, CString Command);
	int mRecv(SOCKET sk, CString &Command);

	struct SockName
	{

		SOCKET sockClient;
		char Name[200];
	};

	SOCKET sockServer,sockClient,flag,sclient;
	struct sockaddr_in serverAdd;
	int msgType;
	int buffLength,t,lenguser,flagsend,kq, count_sock;
	int number_Socket;
	int first_send;
	SockName *pSock;
	CString strResult[2];
	CString Command;
	int R;


	CString m_msgString;
	afx_msg void OnBnClickedListen();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedMess();
protected:
	CEdit edit_mess;
public:
	void CServerDlg::ResetListPayment();
	CListCtrl List_Orders;
	CListCtrl List_Menu;
	CListCtrl List_Payment;
	CListCtrl List_TimeConstraint;
	afx_msg void OnBnClickedUploadmenu();
	CEdit FoodName;
	CEdit Price;
	CButton AddFood;
	afx_msg void OnBnClickedBtnAddfood();
	void CServerDlg::Reset_ListMenu();
	afx_msg void OnBnClickedButton1();
	afx_msg void CServerDlg::Upload_Order();
	afx_msg void CServerDlg::Reset_ListMenuOrder();
	afx_msg void OnBnClickedButton2();
	afx_msg void CServerDlg::Upload_Payment();
	afx_msg void OnBnClickedButton3();
	CButton Upload;
	/*afx_msg void OnBnClickedButton4();*/
	CEdit Mess_show;
};
