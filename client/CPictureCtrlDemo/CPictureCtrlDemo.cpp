// CPictureCtrlDemo.cpp : Definiert das Klassenverhalten für die Anwendung.
//

#include "stdafx.h"
#include "CPictureCtrlDemo.h"
#include "CPictureCtrlDemoDlg.h"
#include "afxsock.h"
//#include "afxsock.inl"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCPictureCtrlDemoApp

BEGIN_MESSAGE_MAP(CCPictureCtrlDemoApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCPictureCtrlDemoApp-Erstellung

CCPictureCtrlDemoApp::CCPictureCtrlDemoApp()
{
	// TODO: Hier Code zur Konstruktion einfügen
	// Alle wichtigen Initialisierungen in InitInstance positionieren
}


// Das einzige CCPictureCtrlDemoApp-Objekt

CCPictureCtrlDemoApp theApp;


// CCPictureCtrlDemoApp-Initialisierung

BOOL CCPictureCtrlDemoApp::InitInstance()
{
	// InitCommonControlsEx() ist für Windows XP erforderlich, wenn ein Anwendungsmanifest
	// die Verwendung von ComCtl32.dll Version 6 oder höher zum Aktivieren
	// von visuellen Stilen angibt. Ansonsten treten beim Erstellen von Fenstern Fehler auf.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Legen Sie dies fest, um alle allgemeinen Steuerelementklassen einzubeziehen,
	// die Sie in Ihrer Anwendung verwenden möchten.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Standardinitialisierung
	// Wenn Sie diese Features nicht verwenden und die Größe
	// der ausführbaren Datei verringern möchten, entfernen Sie
	// die nicht erforderlichen Initialisierungsroutinen.
	// Ändern Sie den Registrierungsschlüssel, unter dem Ihre Einstellungen gespeichert sind.
	// TODO: Ändern Sie diese Zeichenfolge entsprechend,
	// z.B. zum Namen Ihrer Firma oder Organisation.

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	SetRegistryKey(_T("Vom lokalen Anwendungs-Assistenten generierte Anwendungen"));

	CCPictureCtrlDemoDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Fügen Sie hier Code ein, um das Schließen des
		//  Dialogfelds über "OK" zu steuern
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Fügen Sie hier Code ein, um das Schließen des
		//  Dialogfelds über "Abbrechen" zu steuern
	}

	// Da das Dialogfeld geschlossen wurde, FALSE zurückliefern, sodass wir die
	//  Anwendung verlassen, anstatt das Nachrichtensystem der Anwendung zu starten.
	return FALSE;
}
