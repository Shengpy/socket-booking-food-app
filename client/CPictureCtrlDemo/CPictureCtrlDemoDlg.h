// CPictureCtrlDemoDlg.h : Headerdatei
//

#pragma once
#include "PictureCtrl.h"
#define PORT 25000
#define WM_SOCKET WM_USER+2


// CCPictureCtrlDemoDlg-Dialogfeld
class CCPictureCtrlDemoDlg : public CDialog
{
// Konstruktion
public:
	CCPictureCtrlDemoDlg(CWnd* pParent = NULL);	// Standardkonstruktor

// Dialogfelddaten
	enum { IDD = IDD_CPICTURECTRLDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung


// Implementierung
protected:
	HICON m_hIcon;

	// Generierte Funktionen für die Meldungstabellen
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void Split(CString src, CString des[2]);
	char* ConvertToChar(const CString& s);
	void mSend(CString Command);
	int mRecv(CString& Command);
	LRESULT SockMsg(WPARAM wParam, LPARAM lParam);
	SOCKET sClient;
	sockaddr_in servAdd;
	CString Command;
	int	buffLength;
	CString strResult[2];
	CString userOnline;
	int level;
	int isLogon;
	//afx_msg void OnBnClickedButton2();
	//afx_msg void OnBnClickedButton3();
// 	afx_msg void OnBnClickedButton4();
	CPictureCtrl m_picCtrl;
	CString m_msgString;
	CString IP;
	CString m_userName;
	CEdit PhoneNumber;
	CEdit edit_mess;
	CComboBox Combo_List;
	CEdit edt_quant;
	CButton BTN_Del;
	CListCtrl List_Ctrl_Ordered;
	/*CEdit Info_txt;*/
	CButton Cash;
	CButton Credit_card;
	CEdit EDT_Card_info;
	CString total_price;
	CSpinButtonCtrl Spin;
	CButton Login;
	CButton Logout;
	afx_msg void OnBnClickedLogin();
	afx_msg void OnBnClickedLogout();
	CButton Send;
	afx_msg void OnBnClickedSend();
	afx_msg void OnBnClickedChooseCombo();
	afx_msg void OnBnClickedBtnBook();
	afx_msg void OnBnClickedBtnDel();
	afx_msg void OnBnClickedRadCash();
	afx_msg void OnBnClickedRadCreditcard();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedBtnBack();
	afx_msg void OnBnClickedBtnNext();
	CEdit FoodName_;
	int Rad_Cash;
	int Rad_Credit_card;
	CStatic Info_txt;
	void deleteDoublyLinkedList();
	afx_msg void OnBnClickedButton1();
	CEdit Mess_show;
};
