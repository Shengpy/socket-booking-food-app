// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "odbcinst.h"
#include "afxdb.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CServerDlg dialog
CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
	, m_msgString(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_BOXCHAT, m_msgString);
	DDX_Control(pDX, IDC_Message, edit_mess);
	DDX_Control(pDX, IDC_Orders, List_Orders);
	DDX_Control(pDX, IDC_Menu, List_Menu);
	DDX_Control(pDX, IDC_Payment, List_Payment);
	DDX_Control(pDX, IDC_EDT_FoodName, FoodName);
	DDX_Control(pDX, IDC_EDT_Price, Price);
	DDX_Control(pDX, IDC_BTN_AddFood, AddFood);
	DDX_Control(pDX, IDC_BOXCHAT, Mess_show);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SOCKET, SockMsg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_LISTEN, &CServerDlg::OnBnClickedListen)
	ON_BN_CLICKED(IDC_Mess, &CServerDlg::OnBnClickedMess)
	ON_BN_CLICKED(IDC_BTN_AddFood, &CServerDlg::OnBnClickedBtnAddfood)
	ON_BN_CLICKED(IDC_BUTTON1, &CServerDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CServerDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CServerDlg::OnBnClickedButton3)
	//ON_BN_CLICKED(IDC_BUTTON4, &CServerDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	OnBnClickedUploadmenu();
	Upload_Order();
	Upload_Payment();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CServerDlg::Split(CString src, CString des[2])
{
	int p1, p2;

	p1 = src.Find(_T("\r\n"), 0);
	des[0] = src.Mid(0, p1);

	//p2=src.Find(_T("\r\n"),p1+1);
	//des[1]=src.Mid(p1+2,p2-(p1+2));
	des[1] = src.Mid(p1 + 2, src.GetLength() - 1);

}
char* CServerDlg::ConvertToChar(const CString& s)
{
	int nSize = s.GetLength();
	char* pAnsiString = new char[nSize + 1];
	memset(pAnsiString, 0, nSize + 1);
	wcstombs(pAnsiString, s, nSize + 1);
	return pAnsiString;
}

void CServerDlg::mSend(SOCKET sk, CString Command)
{
	int Len = Command.GetLength();
	Len += Len;
	PBYTE sendBuff = new BYTE[1000000];
	memset(sendBuff, 0, 1000000);
	memcpy(sendBuff, (PBYTE)(LPCTSTR)Command, Len);
	send(sk, (char*)&Len, sizeof(Len), 0);
	send(sk, (char*)sendBuff, Len, 0);
	delete sendBuff;
}
int CServerDlg::mRecv(SOCKET sk, CString& Command)
{
	PBYTE buffer = new BYTE[1000];
	memset(buffer, 0, 1000);
	recv(sk, (char*)&buffLength, sizeof(int), 0);
	recv(sk, (char*)buffer, buffLength, 0);
	TCHAR* ttc = (TCHAR*)buffer;
	Command = ttc;

	if (Command.GetLength() == 0)
		return -1;
	return 0;

}
void CServerDlg::OnBnClickedListen()
{
	// TODO: Add your control notification handler code here

	UpdateData();
	sockServer = socket(AF_INET, SOCK_STREAM, 0);
	serverAdd.sin_family = AF_INET;
	serverAdd.sin_port = htons(PORT);
	serverAdd.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(sockServer, (SOCKADDR*)&serverAdd, sizeof(serverAdd));
	listen(sockServer, 5);
	int err = WSAAsyncSelect(sockServer, m_hWnd, WM_SOCKET, FD_READ | FD_ACCEPT | FD_CLOSE);
	if (err)
		MessageBox((LPCTSTR)"Cant call WSAAsyncSelect");
	GetDlgItem(IDC_LISTEN)->EnableWindow(FALSE);
	number_Socket = 0;
	pSock = new SockName[200];

	srand((unsigned)time(NULL));
	R = rand();
}
int TimeConvert(CString Str)
{
	CString hour;
	CString min;
	CString sec;
	int i = 9;
	while (i < Str.GetLength() - 5)
	{
		while (Str[i] != ' ')
		{
			++i;
		}

		if (Str[i] == ' ')
		{
			++i;
			while (Str[i] != ':')
			{
				hour += Str[i];
				++i;
			}
			++i;
			while (Str[i] != ':')
			{
				min += Str[i];
				++i;
			}
			++i;
			while (Str[i] != ' ')
			{
				sec += Str[i];
				++i;
			}
			++i;
		}
	}
	int Hour = _ttoi(hour);
	int Min = _ttoi(min);
	int Sec = _ttoi(sec);

	int timeInSec = Hour * 3600 + Min * 60 + Sec;

	return timeInSec;
}

CString TotalPrice;
int check_book = 0;
CString ClientName;

namespace fs = std::filesystem;
LRESULT CServerDlg::SockMsg(WPARAM wParam, LPARAM lParam)
{
	if (WSAGETSELECTERROR(lParam))
	{
		// Display the error and close the socket
		closesocket(wParam);
	}
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_ACCEPT:
	{
		pSock[number_Socket].sockClient = accept(wParam, NULL, NULL);
		GetDlgItem(IDC_LISTEN)->EnableWindow(FALSE);
		break;
	}
	case FD_READ:
	{

		int post = -1, dpos = -1;

		for (int i = 0; i < number_Socket; i++)
		{
			if (pSock[i].sockClient == wParam)
			{
				if (i < number_Socket)
					post = i;
			}
		}

		CString temp;
		if (mRecv(wParam, temp) < 0)
			break;
		Split(temp, strResult);
		int flag = _ttoi(strResult[0]);
		char* tem = ConvertToChar(strResult[1]);
		switch (flag)
		{
		case 1: // Login
		{
			int t = 0;
			if (number_Socket > 0)
			{
				for (int i = 0; i < number_Socket; i++)
				{
					if (strcmp(tem, pSock[i].Name) == 0)//Trung ten user
					{
						t = 1;
						break;
					}
				}
			}

			if (t == 0)
			{
				strcpy(pSock[number_Socket].Name, tem);
				Command = _T("1\r\n1\r\n");
				m_msgString += strResult[1] + _T(" login\r\n");
				UpdateData(FALSE);
				Mess_show.LineScroll(Mess_show.GetLineCount());
				number_Socket++;
			}
			else
				Command = _T("1\r\n0\r\n");
			mSend(wParam, Command);
			UpdateData(FALSE);
			break;
		}
		case 2: // Menu
		{
			int post = -1;
			for (int i = 0; i < number_Socket; i++)
			{
				if (pSock[i].sockClient == wParam)
				{
					if (i < number_Socket)
						post = i;
				}
			}

			vector<char> Menu;
			for (int i = 0; i < List_Menu.GetItemCount(); i++)
			{
				CString temp = List_Menu.GetItemText(i, 1);
				for (int j = 0; j < temp.GetLength(); j++)
					Menu.push_back(temp[j]);
				Menu.push_back('-');
			}

			CString menu = CString(Menu.data());
			Command = "3\r\n";
			Command += menu;
			mSend(wParam, Command);

			// send image folder
			std::string path = "Images";
			for (const auto& entry : fs::directory_iterator(path))
			{
				string a = entry.path().string();
				CString name_pic = _T("");
				//get name of directory-----------------
				int find;
				for (find = a.length(); find > 0; find--)
					if (a[find] == 92)
						break;
				find++;
				for (; find < a.length(); find++)
					name_pic += a[find];
				CString src_cs = _T("Images\\") + name_pic;
				const TCHAR* src = (LPCTSTR)src_cs;
				Command = _T("6\r\n");
				Command += name_pic + _T('~');
				ifstream fIn(src, ios::beg | ios::binary);
				if (fIn.is_open())
				{
					char a;
					while (!fIn.eof()) {
						fIn.read((char*)&a, sizeof(a));
						//fIn >> a;
						CString str;
						str.Format(L"%d", int(a));
						Command += str + _T(" ");
					}

					mSend(wParam, Command);
					fIn.close();
				}
			}
			break;
		}
		case 3: // Log out
		{
			int post = -1;
			for (int i = 0; i < number_Socket; i++)
			{
				if (pSock[i].sockClient == wParam)
				{
					if (i < number_Socket)
						post = i;
				}
			}

			m_msgString += pSock[post].Name;
			m_msgString += " logout\r\n";
			closesocket(wParam);
			for (int j = post; j < number_Socket; j++)
			{
				pSock[post].sockClient = pSock[post + 1].sockClient;
				strcpy(pSock[post].Name, pSock[post + 1].Name);
			}
			number_Socket--;
			UpdateData(FALSE);
			Mess_show.LineScroll(Mess_show.GetLineCount());
			break;
		}
		case 4: // choose food
		{
			int post = -1;
			for (int i = 0; i < number_Socket; i++)
			{
				if (pSock[i].sockClient == wParam)
				{
					if (i < number_Socket)
						post = i;
				}
			}

			int index = _ttoi(strResult[1]);
			CString p = List_Menu.GetItemText(index, 2);
			Command = "4\r\n";
			Command += p;
			mSend(wParam, Command);
			UpdateData(FALSE);
			break;
		}
		case 5: // Upload Payment
		{
			//Sheng-0904098763-bankaccount-booktime-totalprice
			int count = 0;
			CString food = _T("");
			CString data = _T(""), NameCustomer, phone, banknumber, booktime, totalprice;
			for (int i = 0; i < strResult[1].GetLength(); i++) {
				if (strResult[1][i] == '-' && count < 7) {
					if (count == 0)
						NameCustomer = data;
					else if (count == 1)
						phone = data;
					else if (count == 2)
						banknumber = data;
					else if (count == 3)
						booktime = data;
					else if (count == 4)
						totalprice = data;
					else if (count > 4)
						food += data + _T("-");
					data = _T("");
					count++;
					continue;
				}
				data += strResult[1][i];
			}

			TotalPrice = totalprice;
			ClientName = NameCustomer;
			//check book first time
			CString book1;
			for (int i = 0; i < List_Payment.GetItemCount(); i++) {
				if (List_Payment.GetItemText(i, 1) == NameCustomer) {
					check_book = 1;
					book1 = List_Payment.GetItemText(i, 4);
					break;
				}
			}
			if (check_book == 1) {
				int b1 = TimeConvert(book1);
				int b2 = TimeConvert(booktime);
				if (b2 - b1 > 7200)
				{
					Command = "5\r\n";
					Command += _T("Book failed! Time exceeded!");
					mSend(wParam, Command);
					break;
				}
				else
				{
					CDatabase database;
					CString SqlString;
					CString sDriver = L"MICROSOFT ACCESS DRIVER (*.mdb)";
					CString sFile = L"Database.mdb";
					CString sDsn;
					// You must change above path if it's different
					int iRec = 0;

					//insert payment
					sDsn.Format(_T("ODBC;DRIVER={%s};DSN='';DBQ=%s"), sDriver, sFile);
					TRY{
						// Open the database
						database.Open(NULL,false,false,sDsn);
					CRecordset recset(&database);
					SqlString.Format(_T("SELECT * FROM Payment"));
					recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly);
					SqlString.Format(_T("UPDATE Payment SET BookTime = '%s' WHERE CustomerName = '%s'"), booktime, NameCustomer);
					database.ExecuteSQL(SqlString);
					// Close the database
					database.Close();
					}CATCH(CDBException, e) {
						// If a database exception occured, show error msg
						AfxMessageBox(_T("Database error") + e->m_strError);
					}
					END_CATCH;
				}
			}
			else
			{
				CDatabase database;
				CString SqlString;
				CString sDriver = L"MICROSOFT ACCESS DRIVER (*.mdb)";
				CString sFile = L"Database.mdb";
				CString sDsn;
				// You must change above path if it's different
				int iRec = 0;

				//insert payment
				sDsn.Format(_T("ODBC;DRIVER={%s};DSN='';DBQ=%s"), sDriver, sFile);
				TRY{
					// Open the database
					database.Open(NULL,false,false,sDsn);
				/*CRecordset recset(&database);
				SqlString.Format(_T("SELECT * FROM Payment"));
				recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly);*/
				SqlString.Format(_T("INSERT INTO Payment (CustomerName, PhoneNumber, BankAccount, BookTime, TotalPrice) VALUES ('%s', '%s', '%s', '%s', '%s')"), NameCustomer, phone, banknumber, booktime, totalprice);
				database.ExecuteSQL(SqlString);
				// Close the database
				database.Close();
				}CATCH(CDBException, e) {
					// If a database exception occured, show error msg
					AfxMessageBox(_T("Database error") + e->m_strError);
				}
				END_CATCH;
			}
			CDatabase database;
			CString SqlString;
			CString sDriver = L"MICROSOFT ACCESS DRIVER (*.mdb)";
			CString sFile = L"Database.mdb";
			CString sDsn;
			// You must change above path if it's different
			int iRec = 0;

			//insert order
			sDsn.Format(_T("ODBC;DRIVER={%s};DSN='';DBQ=%s"), sDriver, sFile);
			TRY{
				// Open the database
			database.Open(NULL,false,false,sDsn);
			/*CRecordset recset(&database);
			SqlString.Format(_T("SELECT * FROM Orders"));
			recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly);*/
			CString quantity = _T(""),name_food = _T(""), str = _T("");
			for (int i = 0; i < food.GetLength(); i++) {
				if (food[i] == '-')
				{
					quantity = str;
					str = _T("");
					SqlString.Format(_T("INSERT INTO Orders (CustomerName, Food, Quantity) VALUES ('%s', '%s', '%s')"), NameCustomer, name_food, quantity);
					database.ExecuteSQL(SqlString);
					continue;
				}
				if (food[i] == '.') {
					name_food = str;
					str = _T("");
					continue;
				}
				str += food[i];
			}
			// Close the database
			database.Close();
			}CATCH(CDBException, e) {
				// If a database exception occured, show error msg
				AfxMessageBox(_T("Database error") + e->m_strError);
			}
			END_CATCH;

			Upload_Payment();
			Upload_Order();
			m_msgString += NameCustomer + _T(" ordered\r\n");
			UpdateData(FALSE);
			Mess_show.LineScroll(Mess_show.GetLineCount());

			CString CheckBook;
			CheckBook.Format(L"%d", check_book);
			Command = "5\r\n";
			//Command += _T("Book succeeded!");
			///
			Command += CheckBook + _T("-") + banknumber + _T("-") + TotalPrice + _T("-");
			///
			mSend(wParam, Command);
			break;
		}
		case 6:
		{
			int post = -1;
			for (int i = 0; i < number_Socket; i++)
			{
				if (pSock[i].sockClient == wParam)
				{
					if (i < number_Socket)
						post = i;
				}
			}

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

			CString oldPrice;
			for (int i = 0; i < List_Payment.GetItemCount(); i++) {
				if (List_Payment.GetItemText(i, 1) == pSock[post].Name) {
					oldPrice = List_Payment.GetItemText(i, 5);
					break;
				}
			}
			//CString oldPrice = List_Payment.GetItemText(List_Payment.GetItemCount() - 1, 5); // find name to get price
			int oPrice = _ttoi(oldPrice);
			int nPrice = _ttoi(TotalPrice);

			int newPrice = oPrice + nPrice;

			CString FinalP;
			FinalP.Format(L"%d", newPrice);
			FinalP += _T(".000");

			CDatabase database;
			CString SqlString;

			//CString strOrderNumber, strCustomerName, strFood, strQuantity, strPrice;
			CString sDriver = L"MICROSOFT ACCESS DRIVER (*.mdb)";
			CString sDsn;
			CString sFile = L"Database.mdb";
			// You must change above path if it's different

			// Build ODBC connection string
			sDsn.Format(L"ODBC;DRIVER={%s};DSN='';DBQ=%s", sDriver, sFile);
			TRY{
				// Open the database
				database.Open(NULL,false,false,sDsn);

				SqlString.Format(_T("UPDATE Payment SET TotalPrice = '%s' WHERE CustomerName = '%s'"), FinalP, ClientName);
				database.ExecuteSQL(SqlString);

				// Close the database
				database.Close();
			}CATCH(CDBException, e) {
				// If a database exception occured, show error msg
				AfxMessageBox(_T("Database error: ") + e->m_strError);
			}
			END_CATCH;

			Upload_Payment();
			if (BankAccount != '0') // card
			{
				Command = "7\r\n";
				Command += _T("You have to pay ") + Price_ + _T(" in total") + _T("\r\n");
				mSend(wParam, Command);
			}
			else if (BankAccount == '0') // cash
			{
				Command = "7\r\n";
				Command += _T("You have to pay ") + FinalP + _T(" in total") + _T("\r\n");
				mSend(wParam, Command);
			}
			break;
		}
		case 7: {
			m_msgString += pSock[post].Name;
			m_msgString += ": ";
			m_msgString += strResult[1];
			UpdateData(FALSE);
			Mess_show.LineScroll(Mess_show.GetLineCount());
		}
		}
		break;
	}

	case FD_CLOSE:
	{
		UpdateData();
		int post = -1;
		for (int i = 0; i < number_Socket; i++)
		{
			if (pSock[i].sockClient == wParam)
			{
				if (i < number_Socket)
					post = i;
			}
		}

		m_msgString += pSock[post].Name;
		m_msgString += " logout\r\n";
		closesocket(wParam);
		for (int j = post; j < number_Socket; j++)
		{
			pSock[post].sockClient = pSock[post + 1].sockClient;
			strcpy(pSock[post].Name, pSock[post + 1].Name);
		}
		number_Socket--;
		UpdateData(FALSE);
		Mess_show.LineScroll(Mess_show.GetLineCount());
		break;
	}

	}
	return 0;
}
void CServerDlg::OnBnClickedMess()
{
	UpdateData();
	CString mess = _T("");
	edit_mess.GetWindowText(mess);
	if (mess != "") {
		m_msgString += mess;
		m_msgString += "\r\n";

		//hwnd.LineScroll(m_editcontrol.GetLineCount());
		for (int i = 0; i < number_Socket; i++)
		{
			Command = "2\r\n";
			Command += mess;
			mSend(pSock[i].sockClient, Command);
		}
		edit_mess.SetWindowText(_T(""));
		UpdateData(FALSE);
		Mess_show.LineScroll(Mess_show.GetLineCount());
	}
}
void CServerDlg::Reset_ListMenu() {
	List_Menu.DeleteAllItems();
	int iNbrOfColumns;
	CHeaderCtrl* pHeader = (CHeaderCtrl*)List_Menu.GetDlgItem(0);
	if (pHeader) {
		iNbrOfColumns = pHeader->GetItemCount();
	}
	for (int i = iNbrOfColumns; i >= 0; i--) {
		List_Menu.DeleteColumn(i);
	}
}
void CServerDlg::OnBnClickedUploadmenu()
{
	// TODO: Add your control notification handler code here
	CDatabase database;
	CString SqlString;
	CString strFood, strPrice, ID;
	CString sDriver = L"MICROSOFT ACCESS DRIVER (*.mdb)";
	CString sFile = L"Database.mdb";
	CString sDsn;
	// You must change above path if it's different
	int iRec = 0;

	sDsn.Format(_T("ODBC;DRIVER={%s};DSN='';DBQ=%s"), sDriver, sFile);
	TRY{
		// Open the database
		database.Open(NULL,false,false,sDsn);

	// Allocate the recordset
	CRecordset recset(&database);

	// Build the SQL statement
	SqlString.Format(_T("SELECT * FROM Food"));

	// Execute the query

	recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly);
	// Reset List control if there is any data
	Reset_ListMenu();

	// populate Grids
	ListView_SetExtendedListViewStyle(List_Menu, LVS_EX_GRIDLINES);

	// Column width and heading
	List_Menu.InsertColumn(1, _T("ID"), LVCFMT_LEFT, 50);
	List_Menu.InsertColumn(2, _T("Food"), LVCFMT_LEFT, 250);
	List_Menu.InsertColumn(3, _T("Price"), LVCFMT_LEFT, 100);
	int n = 0;

	// Loop through each record
	while (!recset.IsEOF()) {
		// Copy each column into a variable
		recset.GetFieldValue(_T("Number"), ID);
		recset.GetFieldValue(_T("Food"), strFood);
		recset.GetFieldValue(_T("Price"), strPrice);

		// Insert values into the list control
		iRec = List_Menu.InsertItem(n, ID, 0);
		List_Menu.SetItemText(n, 1, strFood);
		List_Menu.SetItemText(n, 2, strPrice);
		n++;

		// goto next record
		recset.MoveNext();
	 }
	// Close the database
	database.Close();
	}CATCH(CDBException, e) {
		// If a database exception occured, show error msg
		AfxMessageBox(_T("Database error") + e->m_strError);
	}
	END_CATCH;
	UpdateData(FALSE);
}
void CServerDlg::OnBnClickedBtnAddfood()
{
	// TODO: Add your control notification handler code here
	CString Food_Name;
	FoodName.GetWindowTextW(Food_Name);
	if (Food_Name == _T(""))
	{
		MessageBox(_T("Invalid Name"), _T("Server"));
		return;
	}

	CString Price_Tag;
	Price.GetWindowTextW(Price_Tag);
	if (Price_Tag == _T(""))
	{
		MessageBox(_T("Invalid Price"), _T("Server"));
		return;
	}

	CDatabase database;

	//CString strOrderNumber, strCustomerName, strFood, strQuantity, strPrice;
	CString sDriver = L"MICROSOFT ACCESS DRIVER (*.mdb)";
	CString sDsn;
	CString SqlString;
	CString sFile = L"Database.mdb";
	// You must change above path if it's different
	int iRec = 0;

	// Build ODBC connection string
	sDsn.Format(L"ODBC;DRIVER={%s};DSN='';DBQ=%s", sDriver, sFile);
	TRY{
		// Open the database
		database.Open(NULL,false,false,sDsn);

		SqlString.Format(_T("INSERT INTO Food (Food, Price) VALUES ('%s', '%s')"), Food_Name, Price_Tag);
		database.ExecuteSQL(SqlString);

		// Close the database
		database.Close();
	}CATCH(CDBException, e) {
		// If a database exception occured, show error msg
		AfxMessageBox(_T("Database error: ") + e->m_strError);
	}
	END_CATCH;
	m_msgString += _T("Successfully Added!");
	m_msgString += _T("\r\n");
	FoodName.Clear();
	Price.Clear();
	UpdateData(FALSE);
	OnBnClickedUploadmenu();
	Mess_show.LineScroll(Mess_show.GetLineCount());
}
//delete food 
void CServerDlg::OnBnClickedButton1()
{
	int index = List_Menu.GetNextItem(-1, LVNI_SELECTED);
	CString ID_del = List_Menu.GetItemText(index, 0);
	List_Menu.DeleteItem(index);
	UpdateData(FALSE);

	CDatabase database;
	CString SqlString;
	CString strID, strName, strAge;
	CString sDriver = L"MICROSOFT ACCESS DRIVER (*.mdb)";
	CString sDsn;
	CString sFile = L"Database.mdb";

	// You must change above path if it's different
	int iRec = 0;

	// Build ODBC connection string
	sDsn.Format(L"ODBC;DRIVER={%s};DSN='';DBQ=%s", sDriver, sFile);
	TRY{
		// Open the database
	database.Open(NULL,false,false,sDsn);
	SqlString.Format(L"DELETE FROM Food WHERE Number = %s", ID_del);
	database.ExecuteSQL(SqlString);
	// Execute the query
	database.Close();
	}CATCH(CDBException, e) {
		// If a database exception occured, show error msg
		AfxMessageBox(L"Database error: " + e->m_strError);
	}
	END_CATCH;
}
void CServerDlg::Reset_ListMenuOrder() {
	List_Orders.DeleteAllItems();
	int iNbrOfColumns;
	CHeaderCtrl* pHeader = (CHeaderCtrl*)List_Orders.GetDlgItem(0);
	if (pHeader) {
		iNbrOfColumns = pHeader->GetItemCount();
	}
	for (int i = iNbrOfColumns; i >= 0; i--) {
		List_Orders.DeleteColumn(i);
	}
}
void CServerDlg::Upload_Order() {
	CDatabase database;
	CString SqlString;
	CString strFood, strQuantity, strName, ID;
	CString sDriver = L"MICROSOFT ACCESS DRIVER (*.mdb)";
	CString sFile = L"Database.mdb";
	CString sDsn;
	// You must change above path if it's different
	int iRec = 0;

	sDsn.Format(_T("ODBC;DRIVER={%s};DSN='';DBQ=%s"), sDriver, sFile);
	TRY{
		// Open the database
		database.Open(NULL,false,false,sDsn);

	// Allocate the recordset
	CRecordset recset(&database);

	// Build the SQL statement
	SqlString.Format(_T("SELECT * FROM Orders"));

	// Execute the query

	recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly);
	// Reset List control if there is any data
	Reset_ListMenuOrder();

	// populate Grids
	ListView_SetExtendedListViewStyle(List_Orders, LVS_EX_GRIDLINES);

	// Column width and heading
	List_Orders.InsertColumn(1, _T("ID"), LVCFMT_LEFT, 50);
	List_Orders.InsertColumn(2, _T("Customer name"), LVCFMT_LEFT, 150);
	List_Orders.InsertColumn(3, _T("Food"), LVCFMT_LEFT, 150);
	List_Orders.InsertColumn(4, _T("Quantity"), LVCFMT_CENTER, 75);
	int n = 0;

	// Loop through each record
	while (!recset.IsEOF()) {
		// Copy each column into a variable
		recset.GetFieldValue(_T("OrderNumber"), ID);
		recset.GetFieldValue(_T("CustomerName"), strName);
		recset.GetFieldValue(_T("Food"), strFood);
		recset.GetFieldValue(_T("Quantity"), strQuantity);

		// Insert values into the list control
		iRec = List_Orders.InsertItem(n, ID, 0);
		List_Orders.SetItemText(n, 1, strName);
		List_Orders.SetItemText(n, 2, strFood);
		List_Orders.SetItemText(n, 3, strQuantity);

		n++;

		// goto next record
		recset.MoveNext();
	 }
	// Close the database
	database.Close();
	}CATCH(CDBException, e) {
		// If a database exception occured, show error msg
		AfxMessageBox(_T("Database error") + e->m_strError);
	}
	END_CATCH;
	UpdateData(FALSE);
}
//delete order
void CServerDlg::OnBnClickedButton2()
{
	int index = List_Orders.GetNextItem(-1, LVNI_SELECTED);
	CString ID_del = List_Orders.GetItemText(index, 0);
	List_Orders.DeleteItem(index);
	UpdateData(FALSE);

	CDatabase database;
	CString SqlString;
	CString strID, strName, strAge;
	CString sDriver = L"MICROSOFT ACCESS DRIVER (*.mdb)";
	CString sDsn;
	CString sFile = L"Database.mdb";

	// You must change above path if it's different
	int iRec = 0;

	// Build ODBC connection string
	sDsn.Format(L"ODBC;DRIVER={%s};DSN='';DBQ=%s", sDriver, sFile);
	TRY{
		// Open the database
	database.Open(NULL,false,false,sDsn);
	SqlString.Format(L"DELETE FROM Orders WHERE OrderNumber = %s", ID_del);
	database.ExecuteSQL(SqlString);
	// Execute the query
	database.Close();
	}CATCH(CDBException, e) {
		// If a database exception occured, show error msg
		AfxMessageBox(L"Database error: " + e->m_strError);
	}
	END_CATCH;
}
void CServerDlg::ResetListPayment() {
	List_Payment.DeleteAllItems();
	int iNbrOfColumns;
	CHeaderCtrl* pHeader = (CHeaderCtrl*)List_Payment.GetDlgItem(0);
	if (pHeader) {
		iNbrOfColumns = pHeader->GetItemCount();
	}
	for (int i = iNbrOfColumns; i >= 0; i--) {
		List_Payment.DeleteColumn(i);
	}
}
void CServerDlg::Upload_Payment() {
	CDatabase database;
	CString SqlString;
	CString Name, Phone, Bank, time, price, ID;
	CString sDriver = L"MICROSOFT ACCESS DRIVER (*.mdb)";
	CString sFile = L"Database.mdb";
	CString sDsn;
	// You must change above path if it's different
	int iRec = 0;

	sDsn.Format(_T("ODBC;DRIVER={%s};DSN='';DBQ=%s"), sDriver, sFile);
	TRY{
		// Open the database
		database.Open(NULL,false,false,sDsn);

	// Allocate the recordset
	CRecordset recset(&database);

	// Build the SQL statement
	SqlString.Format(_T("SELECT * FROM Payment"));

	// Execute the query

	recset.Open(CRecordset::forwardOnly,SqlString,CRecordset::readOnly);
	// Reset List control if there is any data
	ResetListPayment();

	// populate Grids
	ListView_SetExtendedListViewStyle(List_Payment, LVS_EX_GRIDLINES);

	// Column width and heading
	List_Payment.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 50);
	List_Payment.InsertColumn(1, _T("Customer name"), LVCFMT_LEFT, 150);
	List_Payment.InsertColumn(2, _T("Phone"), LVCFMT_LEFT, 85);
	List_Payment.InsertColumn(3, _T("Bank"), LVCFMT_CENTER, 85);
	List_Payment.InsertColumn(4, _T("Time"), LVCFMT_CENTER, 100);
	List_Payment.InsertColumn(5, _T("Total Price"), LVCFMT_CENTER, 75);
	int n = 0;

	// Loop through each record
	while (!recset.IsEOF()) {
		// Copy each column into a variable
		recset.GetFieldValue(_T("Number"), ID);
		recset.GetFieldValue(_T("CustomerName"), Name);
		recset.GetFieldValue(_T("PhoneNumber"), Phone);
		recset.GetFieldValue(_T("BankAccount"), Bank);
		recset.GetFieldValue(_T("BookTime"), time);
		recset.GetFieldValue(_T("TotalPrice"), price);

		// Insert values into the list control
		iRec = List_Payment.InsertItem(n, ID, 0);
		List_Payment.SetItemText(n, 1, Name);
		List_Payment.SetItemText(n, 2, Phone);
		List_Payment.SetItemText(n, 3, Bank);
		List_Payment.SetItemText(n, 4, time);
		List_Payment.SetItemText(n, 5, price);

		n++;
		// goto next record
		recset.MoveNext();
	 }
	// Close the database
	database.Close();
	}CATCH(CDBException, e) {
		// If a database exception occured, show error msg
		AfxMessageBox(_T("Database error") + e->m_strError);
	}
	END_CATCH;
	UpdateData(FALSE);
}
//delete payment
void CServerDlg::OnBnClickedButton3()
{
	int index = List_Payment.GetNextItem(-1, LVNI_SELECTED);
	CString ID_del = List_Payment.GetItemText(index, 0);
	List_Payment.DeleteItem(index);
	UpdateData(FALSE);

	CDatabase database;
	CString SqlString;
	CString strID, strName, strAge;
	CString sDriver = L"MICROSOFT ACCESS DRIVER (*.mdb)";
	CString sDsn;
	CString sFile = L"Database.mdb";

	// You must change above path if it's different
	int iRec = 0;

	// Build ODBC connection string
	sDsn.Format(L"ODBC;DRIVER={%s};DSN='';DBQ=%s", sDriver, sFile);
	TRY{
		// Open the database
	database.Open(NULL,false,false,sDsn);
	SqlString.Format(L"DELETE FROM Payment WHERE Number = %s", ID_del);
	database.ExecuteSQL(SqlString);
	// Execute the query
	database.Close();
	}CATCH(CDBException, e) {
		// If a database exception occured, show error msg
		AfxMessageBox(L"Database error: " + e->m_strError);
	}
	END_CATCH;
}

