// CPictureCtrlDemoDlg.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "CPictureCtrlDemo.h"
#include "PictureCtrl.h"
#include "CPictureCtrlDemoDlg.h"
#include "odbcinst.h"
#include "afxdb.h"
#include <time.h> 
#include <fstream>
#include <string>
#include <vector>
#include <direct.h>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg-Dialogfeld für Anwendungsbefehl "Info"

const int highest = 1000;
const int lowest = 1;
int Price = 0;
int supPrice = 0;
bool logIn = 0;
bool booked = 0;
bool cash = 1;
int total = 0;


struct node
{
	struct node* prev;
	CString path;
	CString name;
	struct node* next;
};

struct node* head;

CString PicName = _T("");
CString Path;
struct node* current;
struct node* back;

int loaded = 0;

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialogfelddaten
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

// Implementierung
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCPictureCtrlDemoDlg-Dialogfeld

CCPictureCtrlDemoDlg::CCPictureCtrlDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCPictureCtrlDemoDlg::IDD, pParent)
	, m_msgString(_T(""))
	, IP(_T(""))
	, m_userName(_T(""))
	//, cash(1)
	//, credit_card(0)
	, total_price(_T(""))
	, Rad_Cash(0)
	, Rad_Credit_card(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCPictureCtrlDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PICTURE, m_picCtrl);
	DDX_Text(pDX, IDC_MSGBOX, m_msgString);
	DDX_Text(pDX, IDC_IP, IP);
	DDX_Text(pDX, IDC_USER, m_userName);
	DDX_Control(pDX, IDC_Phone, PhoneNumber);
	DDX_Control(pDX, IDC_Request, edit_mess);
	DDX_Control(pDX, IDC_COMBO_Food, Combo_List);
	DDX_Control(pDX, IDC_EDT_Quantity, edt_quant);
	DDX_Control(pDX, IDC_BTN_Del, BTN_Del);
	DDX_Control(pDX, IDC_LIST_CTRL_Ordered, List_Ctrl_Ordered);
	//DDX_Check(pDX, IDC_RAD_Cash, cash);
	//DDX_Check(pDX, IDC_RAD_Credit_card, credit_card);
	DDX_Control(pDX, IDC_EDT_Card_Info, EDT_Card_info);
	DDX_Text(pDX, IDC_EDIT3, total_price);
	DDX_Control(pDX, IDC_SPIN1, Spin);
	DDX_Control(pDX, IDC_LOGIN, Login);
	DDX_Control(pDX, IDC_LOGOUT, Logout);
	DDX_Control(pDX, IDC_Send, Send);
	DDX_Control(pDX, IDC_EDIT1, FoodName_);
	DDX_Check(pDX, IDC_RAD_Cash, Rad_Cash);
	DDX_Check(pDX, IDC_RAD_Credit_card, Rad_Credit_card);
	DDX_Control(pDX, IDC_Card_Info, Info_txt);
	DDX_Control(pDX, IDC_MSGBOX, Mess_show);
}

BEGIN_MESSAGE_MAP(CCPictureCtrlDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SOCKET, SockMsg)
	//}}AFX_MSG_MAP
	
/*	ON_BN_CLICKED(IDC_BUTTON4, &CCPictureCtrlDemoDlg::OnBnClickedButton4)*/
ON_BN_CLICKED(IDC_LOGIN, &CCPictureCtrlDemoDlg::OnBnClickedLogin)
ON_BN_CLICKED(IDC_LOGOUT, &CCPictureCtrlDemoDlg::OnBnClickedLogout)
ON_BN_CLICKED(IDC_Send, &CCPictureCtrlDemoDlg::OnBnClickedSend)
ON_BN_CLICKED(IDC_Choose_Combo, &CCPictureCtrlDemoDlg::OnBnClickedChooseCombo)
ON_BN_CLICKED(IDC_BTN_Book, &CCPictureCtrlDemoDlg::OnBnClickedBtnBook)
ON_BN_CLICKED(IDC_BTN_Del, &CCPictureCtrlDemoDlg::OnBnClickedBtnDel)
ON_BN_CLICKED(IDC_RAD_Cash, &CCPictureCtrlDemoDlg::OnBnClickedRadCash)
ON_BN_CLICKED(IDC_RAD_Credit_card, &CCPictureCtrlDemoDlg::OnBnClickedRadCreditcard)
ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CCPictureCtrlDemoDlg::OnDeltaposSpin1)
ON_BN_CLICKED(IDC_BTN_Back, &CCPictureCtrlDemoDlg::OnBnClickedBtnBack)
ON_BN_CLICKED(IDC_BTN_Next, &CCPictureCtrlDemoDlg::OnBnClickedBtnNext)
ON_BN_CLICKED(IDC_BUTTON1, &CCPictureCtrlDemoDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CCPictureCtrlDemoDlg-Meldungshandler

BOOL CCPictureCtrlDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Hinzufügen des Menübefehls "Info..." zum Systemmenü.

	// IDM_ABOUTBOX muss sich im Bereich der Systembefehle befinden.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Symbol für dieses Dialogfeld festlegen. Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden

	// TODO: Hier zusätzliche Initialisierung einfügen
	Spin.SetRange(lowest, highest);
	Spin.SetPos(lowest);
	edt_quant.SetSel(0);
	EDT_Card_info.SetLimitText(10);
	PhoneNumber.SetLimitText(10);
	Rad_Cash = 1;

	List_Ctrl_Ordered.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	List_Ctrl_Ordered.InsertColumn(0, _T("Food"), LVCFMT_LEFT, 100); // 100 = width
	List_Ctrl_Ordered.InsertColumn(1, _T("Quantity"), LVCFMT_LEFT, 100);
	List_Ctrl_Ordered.InsertColumn(2, _T("Price"), LVCFMT_LEFT, 100);

	IP = "127.0.0.1";
	GetDlgItem(IDC_LOGOUT)->EnableWindow(FALSE);
	UpdateData(FALSE);

	return TRUE;  // Geben Sie TRUE zurück, außer ein Steuerelement soll den Fokus erhalten
}

void CCPictureCtrlDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// Wenn Sie dem Dialogfeld eine Schaltfläche "Minimieren" hinzufügen, benötigen Sie 
//  den nachstehenden Code, um das Symbol zu zeichnen. Für MFC-Anwendungen, die das 
//  Dokument/Ansicht-Modell verwenden, wird dies automatisch ausgeführt.

void CCPictureCtrlDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Gerätekontext zum Zeichnen

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Symbol in Clientrechteck zentrieren
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Symbol zeichnen
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// Die System ruft diese Funktion auf, um den Cursor abzufragen, der angezeigt wird, während der Benutzer
//  das minimierte Fenster mit der Maus zieht.
HCURSOR CCPictureCtrlDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCPictureCtrlDemoDlg::Split(CString src, CString des[2])
{
	int p1, p2;

	p1 = src.Find(_T("\r\n"), 0);
	des[0] = src.Mid(0, p1);

	//p2=src.Find(_T("\r\n"),p1+1);
	//des[1]=src.Mid(p1+2,p2-(p1+2));
	des[1] = src.Mid(p1 + 2, src.GetLength() - 1);

}

char* CCPictureCtrlDemoDlg::ConvertToChar(const CString& s)
{
	int nSize = s.GetLength();
	char* pAnsiString = new char[nSize + 1];
	memset(pAnsiString, 0, nSize + 1);
	wcstombs(pAnsiString, s, nSize + 1);
	return pAnsiString;
}

void CCPictureCtrlDemoDlg::mSend(CString Command)
{
	int Len = Command.GetLength();
	Len += Len;
	PBYTE sendBuff = new BYTE[1000];
	memset(sendBuff, 0, 1000);
	memcpy(sendBuff, (PBYTE)(LPCTSTR)Command, Len);
	send(sClient, (char*)&Len, sizeof(Len), 0);
	send(sClient, (char*)sendBuff, Len, 0);
	delete sendBuff;
}

int CCPictureCtrlDemoDlg::mRecv(CString& Command)
{
	PBYTE buffer = new BYTE[1000000];
	memset(buffer, 0, 1000000);
	recv(sClient, (char*)&buffLength, sizeof(int), 0);
	recv(sClient, (char*)buffer, buffLength, 0);
	TCHAR* ttc = (TCHAR*)buffer;
	Command = ttc;

	if (Command.GetLength() == 0)
		return -1;
	return 0;
}

LRESULT CCPictureCtrlDemoDlg::SockMsg(WPARAM wParam, LPARAM lParam)
{

	if (WSAGETSELECTERROR(lParam))
	{
		// Display the error and close the socket
		closesocket(wParam);
	}
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_READ:
	{
		CString temp;
		if (mRecv(temp) < 0)
			break;

		Split(temp, strResult);
		int flag1 = _ttoi(strResult[0]);
		int flag2 = _ttoi(strResult[1]);
		switch (flag1)
		{
		case 1:
		{
			if (flag2 == 1)
			{
				m_msgString += _T("Dang nhap thanh cong\r\n");
				GetDlgItem(IDC_LOGOUT)->EnableWindow(TRUE);
				GetDlgItem(IDC_LOGIN)->EnableWindow(FALSE);
				logIn = 1;
			}
			else
				m_msgString += _T("Dang nhap that bai\r\n");


			UpdateData(FALSE);
			break;
		}

		case 2:
		{
			m_msgString += "Server: ";
			m_msgString += strResult[1];
			m_msgString += "\r\n";
			UpdateData(FALSE);
			Mess_show.LineScroll(Mess_show.GetLineCount());
			break;
		}

		case 3: // add food to combolist
		{
			CString kind_Food;
			for (int i = 0; i < strResult[1].GetLength(); i++) {
				if (strResult[1][i] == '-')
				{
					Combo_List.AddString(kind_Food);
					kind_Food = _T("");
				}
				else
					kind_Food += strResult[1][i];
			}
			Combo_List.SetCurSel(0);
			UpdateData(FALSE);
			break;
		}
		//price when click on choose button
		case 4:
		{
			CString price = strResult[1];
			int Price = _ttoi(price);
			// calculation based on quantity
			CString quant = List_Ctrl_Ordered.GetItemText(0, 1);
			int Quant = _ttoi(quant);
			int Final_Price = Price * Quant;
			CString final_price;
			final_price.Format(L"%d", Final_Price);
			List_Ctrl_Ordered.SetItemText(0, 2, final_price);
			total += Final_Price;
			CString Total_Str;
			Total_Str.Format(L"%d", total);
			total_price = Total_Str + _T(".000");
			UpdateData(FALSE);
			break;
		}
		//Anounce for client result
		case 5:
		{
			//MessageBox(strResult[1]);
			if (strResult[1] == "Book failed! Time exceeded!")
			{
				m_msgString += _T("Book failed! Time exceeded!");
				m_msgString += _T("\r\n");
				break;
			}
			///
			else 
			{
				CString CheckBook;
				CString BankAccount;
				CString Price_;
				CString data = _T("");
				int count = 0;
				for (int i = 0; i < strResult[1].GetLength(); i++)
				{
					if (strResult[1][i] == '-' && count < 3)
					{
						if (count == 0)
						{
							CheckBook = data;
						}
						else if (count == 1)
						{
							BankAccount = data;
						}
						else if (count == 2)
						{
							Price_ = data;
						}
						data = _T("");
						count++;
						continue;
					}
					data += strResult[1][i];
				}
				if (CheckBook == '1')
				{
					Command = "6\r\n";
					Command += CheckBook + _T("-") + BankAccount + _T("-") + Price_ + _T("-");
					mSend(Command);
				}

				else if (CheckBook == '0')
				{
					m_msgString += _T("You have to pay ") + Price_ + _T(" in total") + _T("\r\n");
				}
			}
			UpdateData(FALSE);
			Mess_show.LineScroll(Mess_show.GetLineCount());
			break;
		}

		case 6: {
			CString name_pic = _T("Images\\");
			CString data = _T("");
			int find = 0;
			for (int i = 0; i < strResult[1].GetLength(); i++) {
				if (strResult[1][i] == '~' && find == 0)
				{
					find = 1;
					continue;
				}
				if (find == 0)
					name_pic += strResult[1][i];
				else
					data += strResult[1][i];
			};
			//convert name_pic
			const TCHAR* name_pic_char = (LPCTSTR)name_pic;
			//convert data
			CT2CA pszConvertedAnsiString(data);
			std::string DataStr(pszConvertedAnsiString);
			ofstream fOut(name_pic_char, ios::trunc | ios::binary);
			if (fOut.is_open())
			{
				ofstream  fO("abc.txt", ios::trunc);
				fO << DataStr;
				fO.close();

				ifstream  fz("abc.txt", ios::beg);
				int a;
				while (!fz.eof()) {
					fz >> a;
					char b = char(a);
					fOut.write((char*)&b, sizeof(b));
				}
				fz.close();
				UpdateData(FALSE);
				fOut.close();
				PicName += name_pic;
				PicName += '~';
			}
			OnBnClickedLoad();
			deleteDoublyLinkedList();
			OnBnClickedLoad();
			break;
		}
		case 7:
		{
			m_msgString += strResult[1];
			List_Ctrl_Ordered.DeleteAllItems();
			total_price = _T("0");
			total = 0;
			EDT_Card_info.Clear();
			Combo_List.SetEditSel(0, -1);
			Combo_List.Clear();
			if (loaded == 1)
			{
				deleteDoublyLinkedList();
				CButton* pButton = (CButton*)GetDlgItem(IDC_BTN_Back);
				pButton->EnableWindow(FALSE);
				CButton* pButton1 = (CButton*)GetDlgItem(IDC_BTN_Next);
				pButton1->EnableWindow(FALSE);
			}
			UpdateData(FALSE);
			break;
		}
		}
		break;
	}
	case FD_CLOSE:
	{
		closesocket(sClient);
		m_msgString += _T("Server da dong ket noi\r\n");
		GetDlgItem(IDC_LOGOUT)->EnableWindow(FALSE);
		GetDlgItem(IDC_LOGIN)->EnableWindow(TRUE);
		UpdateData(FALSE);
		break;
	}

	}
	return 0;
}

void CCPictureCtrlDemoDlg::OnBnClickedLogin()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_userName == "")
	{
		MessageBox(_T("Vui long nhap ten user"));
		return;
	}

	sClient = socket(AF_INET, SOCK_STREAM, 0);
	hostent* host = NULL;
	if (sClient == INVALID_SOCKET)
	{
		MessageBox(_T("socket() failed"), _T("ERROR"), 0);
		return;
	}

	servAdd.sin_family = AF_INET;
	servAdd.sin_port = htons(PORT);

	char* cIP = ConvertToChar(IP);

	servAdd.sin_addr.s_addr = inet_addr(cIP);

	CStringA cpy_IP(IP);

	if (servAdd.sin_addr.s_addr == INADDR_NONE)
	{
		host = (gethostbyname(cpy_IP));
		if (host == NULL)
		{
			MessageBox(_T("Khong the ket noi den server."), _T("ERROR"), 0);
		}
		CopyMemory(&servAdd.sin_addr, host->h_addr_list[0],
			host->h_length);
		return;
	}

	int err = connect(sClient, (struct sockaddr*)&servAdd, sizeof(servAdd));
	if (err == SOCKET_ERROR) {
		MessageBox(_T("Ket noi that bai"), _T("ERROR"), 0);
		return;
	}

	Command = _T("1\r\n");
	Command += m_userName + _T("");

	mSend(Command);

	WSAAsyncSelect(sClient, m_hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
	UpdateData(FALSE);
}

void CCPictureCtrlDemoDlg::OnBnClickedLogout()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	INT_PTR i = MessageBox(_T("Ban muon logout?"), _T("Confirm"), MB_OKCANCEL);
	if (i == IDCANCEL)
		return;
	Command = _T("3\r\n");
	mSend(Command);
	List_Ctrl_Ordered.DeleteAllItems();
	total_price = _T("0");
	total = 0;
	EDT_Card_info.Clear();
	Combo_List.SetEditSel(0, -1);
	Combo_List.Clear();
	if (loaded == 1)
	{
		deleteDoublyLinkedList();
		CButton* pButton = (CButton*)GetDlgItem(IDC_BTN_Back);
		pButton->EnableWindow(FALSE);
		CButton* pButton1 = (CButton*)GetDlgItem(IDC_BTN_Next);
		pButton1->EnableWindow(FALSE);
	}
	logIn = 0;
}

void CCPictureCtrlDemoDlg::OnBnClickedSend()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	Command = _T("2\r\n");
	mSend(Command);
}

void CCPictureCtrlDemoDlg::OnBnClickedChooseCombo()
{
	// TODO: Add your control notification handler code here
	if (logIn == 0)
		return;
	UpdateData();

	CString Food;
	int idx = Combo_List.GetCurSel();
	Combo_List.GetLBText(idx, Food);
	List_Ctrl_Ordered.InsertItem(0, Food);

	CString quant = _T("");
	edt_quant.GetWindowTextW(quant);
	List_Ctrl_Ordered.SetItemText(0, 1, quant);

	CString index;
	index.Format(L"%d", idx);
	UpdateData(FALSE);
	Command = "4\r\n";
	Command += index;
	mSend(Command);
}

void CCPictureCtrlDemoDlg::OnBnClickedBtnBook()
{
	// TODO: Add your control notification handler code here
	if (logIn == 0)
		return;
	// TODO: Add your control notification handler code here
	UpdateData();
	int check_valid_payment = 1;
	if (List_Ctrl_Ordered.GetItemCount() == 0) {
		MessageBox(_T("You have not ordered anything!"), _T("Server"));
		return;
	}
	// pay by card
	if (Rad_Cash == 0) {
		CString cardInfo;
		EDT_Card_info.GetWindowTextW(cardInfo);
		if (cardInfo.GetLength() != 10)
			check_valid_payment = 0;
		else
		{
			for (int i = 0; i < cardInfo.GetLength(); i++)
				if (!isdigit(cardInfo[i]))
				{
					check_valid_payment = 0;
					break;
				}
		}
	}
	if (check_valid_payment) {
		INT_PTR i = MessageBox(_T("Please Confirm!"), _T("Server"), MB_OKCANCEL);
		if (i == IDCANCEL)
			return;
		else
		{
			//Sheng-0904098763-bankaccount-booktime-totalprice
			CString code = m_userName + _T('-');
			CString data;
			PhoneNumber.GetWindowText(data);
			code += data + _T('-');
			if (Rad_Cash == 0) {
				EDT_Card_info.GetWindowText(data);
				code += data + _T('-');
			}
			else
				code += _T("0-");
			time_t now = time(0);
			char str[26];
			ctime_s(str, sizeof(str), &now);
			CString Str = CString(str);
			code += Str + _T('-');
			code += total_price + _T('-');
			for (int i = 0; i < List_Ctrl_Ordered.GetItemCount(); i++) {
				code += List_Ctrl_Ordered.GetItemText(i, 0) + _T('.');
				code += List_Ctrl_Ordered.GetItemText(i, 1) + _T('-');
			}
			Command = "5\r\n";
			Command += code;
			mSend(Command);
			return;
		}
	}
	//payment is not valid
	else {
		MessageBox(_T("10 digits please!"), _T("Server"));
		EDT_Card_info.SetSel(0, -1);
		EDT_Card_info.Clear();
		return;
	}
}

void CCPictureCtrlDemoDlg::OnBnClickedBtnDel()
{
	// TODO: Add your control notification handler code here
	if (logIn == 0)
		return;
	// TODO: Add your control notification handler code here
	if ((booked == 0 && total_price != _T("0.000")) || (total_price != _T("0.000")))
	{
		UpdateData();
		int num;
		_stscanf(List_Ctrl_Ordered.GetItemText(0, 2), _T("%d"), &num);
		total -= num;
		CString Total_Str;
		Total_Str.Format(L"%d", total);
		total_price = Total_Str + _T(".000");
		List_Ctrl_Ordered.DeleteItem(0);
		UpdateData(FALSE);
	}
}

void CCPictureCtrlDemoDlg::OnBnClickedRadCash()
{
	// TODO: Add your control notification handler code here
	EDT_Card_info.ShowWindow(false);
	Info_txt.ShowWindow(false);
	Rad_Cash = 1;
}

void CCPictureCtrlDemoDlg::OnBnClickedRadCreditcard()
{
	// TODO: Add your control notification handler code here
	EDT_Card_info.ShowWindow(true);
	Info_txt.ShowWindow(true);
	Rad_Cash = 0;
}

void CCPictureCtrlDemoDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void initDoublyLinkedList()
{
	head = NULL;
}

void insert(CString Info, CString Name)
{
	struct node* ptr;
	struct node* temp = new node();
	temp->prev = NULL;
	temp->path = Info;
	temp->name = Name;
	temp->next = NULL;

	if (head == NULL)
	{
		head = temp;
	}

	else if (head != NULL)
	{
		ptr = head;

		while (ptr->next != NULL)
		{
			ptr = ptr->next;
		}
		ptr->next = temp;
		temp->prev = ptr;
	}
}

void splitStringFoodAndInsert()
{
	initDoublyLinkedList();
	CString temp = _T("");
	CString kind_Food;
	vector<CString> listOfFood;

	int i = 7;

	while(i < PicName.GetLength()) {
		if (PicName[i] == '.') // Images\\Apple Pie with Cream.jpg~Images\\Fried Rice.jpg~
		{
			listOfFood.push_back(kind_Food);
			kind_Food = _T("");
			i += 12;
		}
		else
		{
			kind_Food += PicName[i];
			++i;
		}
	}

	int k = 0;

	for (int j = 0; j < PicName.GetLength(); j++)
	{
		if (PicName[j] != '~')
		{
			temp += PicName[j];
		}

		else
		{
			insert(temp, listOfFood[k]);
			temp = _T("");
			++k;
		}
	}
}

void CCPictureCtrlDemoDlg::OnBnClickedLoad()
{
	// TODO: Add your control notification handler code here
	if (logIn == 0)
	{
		return;
	}
	else
	{
		splitStringFoodAndInsert();
		struct node* temp = head;
		current = temp;
		back = temp;
		Path = temp->path;
		//Load an Image from File
		m_picCtrl.Load(Path);
		FoodName_.SetWindowTextW(temp->name);
		loaded = 1;
	}
}

void CCPictureCtrlDemoDlg::OnBnClickedBtnBack()
{
	// TODO: Add your control notification handler code here
	if (loaded == 0)
	{
		return;
	}
	else
	{
		back = current;
		while (back->prev != NULL)
		{
			if (back->path == Path)
			{
				back = back->prev;
				break;
			}

			back = back->prev;
		}
		current = back;
		Path = back->path;
		m_picCtrl.Load(Path);
		FoodName_.SetWindowTextW(back->name);
	}
}

void CCPictureCtrlDemoDlg::OnBnClickedBtnNext()
{
	// TODO: Add your control notification handler code here
	if (loaded == 0)
	{
		return;
	}
	else
	{
		if (back == NULL)
		{
			current = head;
		}
		else
		{
			current = back;
		}
		while (current->next != NULL)
		{
			if (current->path == Path)
			{
				current = current->next;
				break;
			}

			current = current->next;
		}
		back = current;
		Path = current->path;
		m_picCtrl.Load(Path);
		FoodName_.SetWindowTextW(current->name);
	}
}

void CCPictureCtrlDemoDlg::deleteDoublyLinkedList()
{
	struct node* temp = head;
	struct node* ptr = head;

	while (temp->next != NULL)
	{
		temp = temp->next;
		delete ptr;
		ptr = NULL;
		ptr = temp;
	} 

	delete temp;
	temp = NULL;
}

void CCPictureCtrlDemoDlg::OnBnClickedButton1()
{
	UpdateData();
	Command = _T("7\r\n");
	CString mess;
	edit_mess.GetWindowText(mess);
	if (mess != "") {
		m_msgString += mess;
		m_msgString += "\r\n";
		edit_mess.SetWindowText(_T(""));
		UpdateData(FALSE);
		Command += mess;
		Command += "\r\n";
		mSend(Command);
		Mess_show.LineScroll(Mess_show.GetLineCount());
	}
}
