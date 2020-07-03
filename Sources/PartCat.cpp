/**
 * PartCat.c
 * Electronic components cataloger and organizer application for Windows CE.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

// Enable development mode.
#define DEVELOP

#include "stdafx.h"
#include "PartCat.h"
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>

#include "Directory.h"
#include "TreeView.h"
#include "Workspace.h"
#include "UIManager.h"

// Styling stuff.
#ifdef SHELL_AYGSHELL
#define DEFAULT_UI_MARGIN 7
#else
#define DEFAULT_UI_MARGIN 5
#endif

// CommandBar buttons.
const TBBUTTON tbButtons[] = {
//   BitmapIndex                         Command         State            Style       UserData String
    { 0,                                 0,              0,               TBSTYLE_SEP,       0,   0 },
    { STD_FILENEW,                       IDC_BTNEWCOMP,  TBSTATE_ENABLED, TBSTYLE_BUTTON,    0,   0 },
    { STD_FILESAVE,                      IDC_BTSAVECOMP, TBSTATE_ENABLED, TBSTYLE_BUTTON,    0,   0 },
    { STD_DELETE,                        IDC_BTDELCOMP,  TBSTATE_ENABLED, TBSTYLE_BUTTON,    0,   0 },
    { 0,                                 0,              0,               TBSTYLE_SEP,       0,   0 },
	{ STD_BMPS_LEN + VIEW_NEWFOLDER,     IDC_BTNEWWS,    TBSTATE_ENABLED, TBSTYLE_BUTTON,    0,   0 },
    { STD_FILEOPEN,                      IDC_BTOPENWS,   TBSTATE_ENABLED, TBSTYLE_BUTTON,    0,   0 },
    { 0,                                 0,              0,               TBSTYLE_SEP,       0,   0 },
    { STD_BMPS_LEN + VIEW_NETCONNECT,    IDC_BTNEWPROP,  TBSTATE_ENABLED, TBSTYLE_BUTTON,    0,   0 },
    { STD_PROPERTIES,                    IDC_BTEDITPROP, TBSTATE_ENABLED, TBSTYLE_BUTTON,    0,   0 },
    { STD_BMPS_LEN + VIEW_NETDISCONNECT, IDC_BTDELPROP,  TBSTATE_ENABLED, TBSTYLE_BUTTON,    0,   0 }
};

// Global variables.
HINSTANCE hInst;
HWND hwndMain;
TreeView treeView;
Workspace workspace;
UIManager uiManager;
HWND hwndDetail;

#ifdef SHELL_AYGSHELL
// Pocket PC specific components.
HWND hwndMenuBar;
SHACTIVATEINFO sai;
#else
// Handheld PC specific components.
HWND hwndCB;
#endif

#ifdef DEVELOP
/**
 * Simple test workspace that is loaded for debugging quickly.
 *
 * @return 0 if everything went OK.
 */
LRESULT LoadTestWorkspace() {
	// Initialize everything.
	workspace = Workspace(Path(TEST_WORKSPACE));
	uiManager = UIManager(&hInst, &hwndMain, &workspace, &treeView, &hwndDetail);

	// Populate the TreeView.
	uiManager.OpenWorkspace(true);

	return 0;
}
#endif

/**
 * Application's main entry point.
 *
 * @param  hInstance     Program instance.
 * @param  hPrevInstance Ignored: Leftover from Win16.
 * @param  lpCmdLine     String with command line text.
 * @param  nShowCmd      Initial state of the program's main window.
 * @return               wParam of the WM_QUIT message.
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPWSTR lpCmdLine, int nShowCmd) {
	MSG msg;
	HACCEL hAccel;
	int rc;

	// Initialize the application.
	rc = InitializeApplication(hInstance);
	if (rc)
		return 0;

	// Initialize this single instance.
	hwndMain = InitializeInstance(hInstance, lpCmdLine, nShowCmd);
	if (hwndMain == 0)
		return 0x10;

	// Load accelerators.
	hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PARTCAT));

	// Application message loop.
	while (GetMessage(&msg, NULL, 0, 0)) {
		// Translate accelerators.
		if (!TranslateAccelerator(hwndMain, hAccel, &msg)) {
			// Translate message.
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// Clean up.
	return TerminateInstance(hInstance, msg.wParam);
}

/**
 * Initializes the application and registers the application class.
 *
 * @param  hInstance Application instance.
 * @return           TRUE if the class was registered.
 */
int InitializeApplication(HINSTANCE hInstance) {
	WNDCLASS wc;
	TCHAR szWindowClass[MAX_LOADSTRING];

	// Load the application name.
	LoadString(hInst, IDC_PARTCAT, szWindowClass, MAX_LOADSTRING);

	
#ifdef SHELL_AYGSHELL
	// Only allow one instance of the application.
	HWND hWnd = FindWindow(szWindowClass, NULL);
	if (hWnd) {
		SetForegroundWindow((HWND)(((DWORD)hWnd) | 0x01));
		return 1;
	}
#endif

	// Register the application's main window class.
	wc.style = CS_VREDRAW | CS_HREDRAW;  // Window style.
	wc.lpfnWndProc = MainWindowProc;     // Main window procedure.
	wc.cbClsExtra = 0;				     // Extra class data.
	wc.cbWndExtra = 0;				     // Extra window data.
	wc.hInstance = hInstance;		     // Owner handle.
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPICON));
	wc.hCursor = NULL;				     // Default cursor. (Always NULL)
	wc.hbrBackground = (HBRUSH)GetSysColorBrush(COLOR_STATIC);
	wc.lpszMenuName = NULL;              // Menu name. (Always NULL)
	wc.lpszClassName = szWindowClass;    // Window class name.

	// Check if the class registration worked.
	if (!RegisterClass(&wc)) {
        MessageBox(NULL, L"Window Registration Failed!", L"Error",
			MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }

	return 0;
}

/**
 * Initializes the instance and creates the window.
 *
 * @param  hInstance     Program instance.
 * @param  lpCmdLine     String with command line text.
 * @param  nShowCmd      Initial state of the program's main window.
 * @return               Window handler.
 */
HWND InitializeInstance(HINSTANCE hInstance, LPWSTR lpCmdLine, int nCmdShow) {
	HWND hWnd;
	TCHAR szWindowClass[MAX_LOADSTRING];
	TCHAR szAppTitle[MAX_LOADSTRING];
	hInst = hInstance;

	// Load the application name.
	LoadString(hInst, IDC_PARTCAT, szWindowClass, MAX_LOADSTRING);
	LoadString(hInst, IDS_APP_TITLE, szAppTitle, MAX_LOADSTRING);

#ifdef SHELL_AYGSHELL
	// Initialize PocketPC controls.
	SHInitExtraControls();
#endif

	// Create the main window.
	hWnd = CreateWindow(szWindowClass,  // Window class.
						szAppTitle,     // Window title.
						WS_VISIBLE,		// Style flags.
						CW_USEDEFAULT,  // X position.
						CW_USEDEFAULT,  // Y position.
						CW_USEDEFAULT,  // Initial width,
						CW_USEDEFAULT,  // Initial height.
						NULL,			// Parent window.
						NULL,			// Menu class. (Always NULL)
						hInstance,		// Application instance.
						NULL);			// Pointer to create parameters.


	// Check if the window creation worked.
	if (!IsWindow(hWnd)) {
		MessageBox(NULL, L"Window Creation Failed!", L"Error",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
	}

	// Set the window task switching icon.
	HANDLE hIcon = LoadImage(hInst, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON,
		GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), 0);
	SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

	// Set window taskbar icon.
	hIcon = LoadImage(hInst, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON,
		GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), 0);
	SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

	// Show and update the window.
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

/**
 * Terminates the application instance.
 *
 * @param  hInstance Application instance.
 * @param  nDefRC    Return code.
 * @return           Previous return code.
 */
int TerminateInstance(HINSTANCE hInstance, int nDefRC) {
	return nDefRC;
}

/**
 * Main window procedure.
 *
 * @param  hWnd   Window handler.
 * @param  wMsg   Message type.
 * @param  wParam Message parameter.
 * @param  lParam Message parameter.
 * @return        0 if everything worked.
 */
LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam) {
	switch (wMsg) {
	case WM_CREATE:
		return WndMainCreate(hWnd, wMsg, wParam, lParam);
	case WM_COMMAND:
		return WndMainCommand(hWnd, wMsg, wParam, lParam);
	case WM_INITMENUPOPUP:
		return WndMainInitMenuPopUp(hWnd, wMsg, wParam, lParam);
	case WM_NOTIFY:
		return WndMainNotify(hWnd, wMsg, wParam, lParam);
	case WM_SETTINGCHANGE:
		return WndMainSettingChange(hWnd, wMsg, wParam, lParam);
	case WM_ACTIVATE:
		return WndMainActivate(hWnd, wMsg, wParam, lParam);
	case WM_CLOSE:
		return WndMainClose(hWnd, wMsg, wParam, lParam);
	case WM_DESTROY:
		return WndMainDestroy(hWnd, wMsg, wParam, lParam);
	}

	return DefWindowProc(hWnd, wMsg, wParam, lParam);
}

/**
 * Process the WM_CREATE message for the window.
 *
 * @param  hWnd   Window handler.
 * @param  wMsg   Message type.
 * @param  wParam Message parameter.
 * @param  lParam Message parameter.
 * @return        0 if everything worked.
 */
LRESULT WndMainCreate(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam) {
	// Ensure that the common controls DLL is loaded. 
    InitCommonControls();

	// Initialize the Image List.
	//HIMAGELIST hIml = InitializeImageList(hInst);

#ifdef SHELL_AYGSHELL
	SHMENUBARINFO mbi = {0};
	SIPINFO si = {0};
	int cx, cy;

	// Initialize the shell to activate the info structure.
	memset(&sai, 0, sizeof(sai));
	sai.cbSize = sizeof(sai);

	// Setup the menu bar.
	mbi.cbSize = sizeof(SHMENUBARINFO);  // Size field.
	mbi.hwndParent = hWnd;               // Parent window.
	mbi.nToolBarId = IDR_MENUBAR;        // ID of the toolbar resource.
	mbi.hInstRes = hInst;                // Instance handle of our application.
	mbi.nBmpId = 0;                      // Bitmap resource ID.
	mbi.cBmpImages = 0;                  // Number of images in the bitmap.
	mbi.hwndMB = 0;                      // Returned handle of the menu bar.
	
	// Create the menu bar.
	if (!SHCreateMenuBar(&mbi)) {
		MessageBox(hWnd, L"Couldn't create the menu bar.", L"UI Error",
			MB_OK | MB_ICONERROR);
		DestroyWindow(hWnd);
	}

	// Save the menu bar handle.
	hwndMenuBar = mbi.hwndMB;

	// Query the SIP state and size our window appropriately.
	si.cbSize = sizeof(si);
	SHSipInfo(SPI_GETSIPINFO, 0, (PVOID)&si, 0);
	cx = si.rcVisibleDesktop.right - si.rcVisibleDesktop.left;
	cy = si.rcVisibleDesktop.bottom - si.rcVisibleDesktop.top;

	// Correct the window height based on the menu bar height.
	if (!(si.fdwFlags & SIPF_ON) || ((si.fdwFlags & SIPF_ON) && (si.fdwFlags & SIPF_DOCKED))) {
		RECT rcMenuBar;
		GetWindowRect(hwndMenuBar, &rcMenuBar);

		cy -= (rcMenuBar.bottom - rcMenuBar.top);
	}

	// Resize our window appropriately.
	SetWindowPos(hWnd, NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOZORDER);
#else
	// Create CommandBar.
	hwndCB = CommandBar_Create(hInst, hWnd, IDC_CMDBAR);
	
    // Add the Standard and View bitmaps to the toolbar.
    CommandBar_AddBitmap(hwndCB, HINST_COMMCTRL, IDB_STD_SMALL_COLOR,
		STD_BMPS_LEN, 16, 16);
    CommandBar_AddBitmap(hwndCB, HINST_COMMCTRL, IDB_VIEW_SMALL_COLOR,
		VIEW_BMPS_LEN, 16, 16);

	// Insert menu bar, toolbar buttons, and the exit button.
	CommandBar_InsertMenubar(hwndCB, hInst, IDM_MAINMENU, 0);
    CommandBar_AddButtons(hwndCB, sizeof(tbButtons) / sizeof(TBBUTTON),
		tbButtons);
	CommandBar_AddAdornments(hwndCB, 0, 0);
#endif

	// Calculate the TreeView control size and position.
	RECT rcTreeView;
	GetClientRect(hWnd, &rcTreeView);
#if !defined(SHELL_AYGSHELL)
	rcTreeView.top += CommandBar_Height(hwndCB);
	rcTreeView.bottom -= rcTreeView.top;
	rcTreeView.right = (LONG)(rcTreeView.right / 3.5);
#endif

	// Create the TreeView control.
	treeView = TreeView(hInst, hWnd, rcTreeView, (HMENU)IDC_TREEVIEW);
	//treeView.SetImageList(hIml);

#if !defined(SHELL_AYGSHELL)
	// Calculate the detail view dialog size and position.
	RECT rcDetailView;
	GetClientRect(hWnd, &rcDetailView);
	rcDetailView.top = rcTreeView.top;
	rcDetailView.bottom = rcTreeView.bottom;
	rcDetailView.left = rcTreeView.right + DEFAULT_UI_MARGIN;
	rcDetailView.right -= rcDetailView.left;

	// Load and embed the detail view dialog.
	HRSRC hResDialog = FindResource(hInst, MAKEINTRESOURCE(IDD_DETAILVIEW), RT_DIALOG);
	HGLOBAL hDialogGlobal = LoadResource(hInst, hResDialog);
	hwndDetail = CreateDialogIndirectParam(hInst, (LPCDLGTEMPLATE)hDialogGlobal,
		hWnd, DetailDlgProc, (LPARAM)&rcDetailView);
	SetWindowPos(hwndDetail, HWND_TOP, rcDetailView.left, rcDetailView.top,
		rcDetailView.right, rcDetailView.bottom, SWP_SHOWWINDOW);
#endif

#ifdef DEVELOP
	// Load the test workspace.
	LoadTestWorkspace();
#endif

	return 0;
}

/**
 * Process the WM_INITMENUPUP message for the window.
 *
 * @param  hWnd   Window handler.
 * @param  wMsg   Message type.
 * @param  wParam Message parameter.
 * @param  lParam Message parameter.
 * @return        0 if everything worked.
 */
LRESULT WndMainInitMenuPopUp(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam) {
#ifdef SHELL_AYGSHELL
	HMENU hmenuRes = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENUBAR));
	HMENU hMenu = (HMENU)SendMessage(hwndMenuBar, SHCMBM_GETMENU, 0, 0);
#else
	HMENU hMenu = CommandBar_GetMenu(hwndCB, 0);
#endif

	// Enable and disable workspace related items.
	if (workspace.IsOpened()) {
		EnableMenuItem(hMenu, IDM_FILE_NEW_COMP, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(hMenu, IDM_FILE_REFRESHWS, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(hMenu, IDM_FILE_CLOSEWS, MF_BYCOMMAND | MF_ENABLED);
	} else {
		EnableMenuItem(hMenu, IDM_FILE_NEW_COMP, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(hMenu, IDM_FILE_REFRESHWS, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(hMenu, IDM_FILE_CLOSEWS, MF_BYCOMMAND | MF_GRAYED);
	}

	// Enable and disable component related items.
	if (uiManager.IsComponentOpened() && workspace.IsOpened()) {
		EnableMenuItem(hMenu, IDM_COMP_SAVE, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(hMenu, IDM_COMP_SAVEAS, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(hMenu, IDM_COMP_DELETE, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(hMenu, IDM_COMP_NEWPROP, MF_BYCOMMAND | MF_ENABLED);

		LONG lCurSel = SendDlgItemMessage(hwndDetail, IDC_LSPROPS, LB_GETCURSEL, 0, 0);
		if (lCurSel != LB_ERR) {
			EnableMenuItem(hMenu, IDM_COMP_EDTPROP, MF_BYCOMMAND | MF_ENABLED);
			EnableMenuItem(hMenu, IDM_COMP_DELPROP, MF_BYCOMMAND | MF_ENABLED);
		} else {
			EnableMenuItem(hMenu, IDM_COMP_EDTPROP, MF_BYCOMMAND | MF_GRAYED);
			EnableMenuItem(hMenu, IDM_COMP_DELPROP, MF_BYCOMMAND | MF_GRAYED);
		}
	} else {
		EnableMenuItem(hMenu, IDM_COMP_SAVE, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(hMenu, IDM_COMP_SAVEAS, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(hMenu, IDM_COMP_DELETE, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(hMenu, IDM_COMP_NEWPROP, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(hMenu, IDM_COMP_EDTPROP, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(hMenu, IDM_COMP_DELPROP, MF_BYCOMMAND | MF_GRAYED);
	}

	return 0;
}

/**
 * Process the WM_COMMAND message for the window.
 *
 * @param  hWnd   Window handler.
 * @param  wMsg   Message type.
 * @param  wParam Message parameter.
 * @param  lParam Message parameter.
 * @return        0 if everything worked.
 */
LRESULT WndMainCommand(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam) {
	switch (GET_WM_COMMAND_ID(wParam, lParam)) {
	case IDC_BTNEWCOMP:
	case IDM_FILE_NEW_COMPONENT:
		return uiManager.CreateComponent();
	case IDC_BTNEWWS:
	case IDM_FILE_NEW_WORKSPACE:
		return uiManager.CreateWorkspace();
	case IDC_BTOPENWS:
	case IDM_FILE_OPENWS:
		return uiManager.OpenWorkspace(false);
	case IDM_FILE_REFRESHWS:
		return uiManager.RefreshWorkspace();
	case IDM_FILE_CLOSEWS:
		return uiManager.CloseWorkspace();
	case IDM_FILE_EXIT:
		return SendMessage(hWnd, WM_CLOSE, 0, 0);
	case IDC_BTSAVECOMP:
	case IDM_COMP_SAVE:
		return uiManager.SaveComponent(false);
	case IDM_COMP_SAVEAS:
		return uiManager.SaveComponent(true);
	case IDC_BTDELCOMP:
	case IDM_COMP_DELETE:
		return uiManager.DeleteComponent();
	case IDC_BTNEWPROP:
	case IDM_COMP_NEWPROP:
		return uiManager.CreateProperty();
	case IDC_BTEDITPROP:
	case IDM_COMP_EDTPROP:
		return uiManager.EditSelectedProperty();
	case IDC_BTDELPROP:
	case IDM_COMP_DELPROP:
		return uiManager.DeleteSelectedProperty();
	case IDM_HELP_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, (DLGPROC)AboutDlgProc);
		return 0;
	}

	return DefWindowProc(hWnd, wMsg, wParam, lParam);
}

/**
 * Process the WM_NOTIFY message for the window.
 *
 * @param  hWnd   Window handler.
 * @param  wMsg   Message type.
 * @param  wParam Message parameter.
 * @param  lParam Message parameter.
 * @return        0 if everything worked.
 */
LRESULT WndMainNotify(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam) {
	switch (((LPNMHDR)lParam)->code) {
	case TVN_SELCHANGED:
#ifdef SHELL_AYGSHELL
		// Create and show the detail view dialog.
		hwndDetail = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DETAILPPC), hWnd,
			(DLGPROC)DetailDlgProc);
		ShowWindow(hwndDetail, SW_SHOW);
#endif

		return uiManager.TreeViewSelectionChanged(hWnd, wMsg, wParam, lParam);
	}

	return 0;
}

/**
 * Process the WM_SETTINGCHANGE message for the window.
 *
 * @param  hWnd   Window handler.
 * @param  wMsg   Message type.
 * @param  wParam Message parameter.
 * @param  lParam Message parameter.
 * @return        0 if everything worked.
 */
LRESULT WndMainSettingChange(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam) {
#ifdef SHELL_AYGSHELL
	// Notify shell of our setting change message.
	SHHandleWMSettingChange(hWnd, wParam, lParam, &sai);
#endif

	return 0;
}

/**
 * Process the WM_ACTIVATE message for the window.
 *
 * @param  hWnd   Window handler.
 * @param  wMsg   Message type.
 * @param  wParam Message parameter.
 * @param  lParam Message parameter.
 * @return        0 if everything worked.
 */
LRESULT WndMainActivate(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam) {
#ifdef SHELL_AYGSHELL
	// Notify shell of our activate message.
	SHHandleWMActivate(hWnd, wParam, lParam, &sai, 0);
#endif

	return 0;
}

/**
 * Process the WM_CLOSE message for the window.
 *
 * @param  hWnd   Window handler.
 * @param  wMsg   Message type.
 * @param  wParam Message parameter.
 * @param  lParam Message parameter.
 * @return        0 if everything worked.
 */
LRESULT WndMainClose(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam) {
	// Check for unsaved changes.
	if (uiManager.CheckForUnsavedChanges())
		return 1;

	// Send window destruction message.
	DestroyWindow(hWnd);
	DestroyWindow(hwndDetail);

	return 0;
}

/**
 * Process the WM_DESTROY message for the window.
 *
 * @param  hWnd   Window handler.
 * @param  wMsg   Message type.
 * @param  wParam Message parameter.
 * @param  lParam Message parameter.
 * @return        0 if everything worked.
 */
LRESULT WndMainDestroy(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam) {
	// Post quit message and return.
	PostQuitMessage(0);
	return 0;
}

/**
 * Mesage handler for the About dialog box.
 *
 * @param  hDlg   Dialog window handler.
 * @param  wMsg   Message type.
 * @param  wParam Message parameter.
 * @param  lParam Message parameter.
 * @return        0 if everything worked.
 */
LRESULT CALLBACK AboutDlgProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam) {
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;

	switch (wMsg) {
		case WM_INITDIALOG:
			// trying to center the About dialog
			if (GetWindowRect(hDlg, &rt1)) {
				GetClientRect(GetParent(hDlg), &rt);
				DlgWidth	= rt1.right - rt1.left;
				DlgHeight	= rt1.bottom - rt1.top ;
				NewPosX		= (rt.right - rt.left - DlgWidth)/2;
				NewPosY		= (rt.bottom - rt.top - DlgHeight)/2;
				
				// if the About box is larger than the physical screen 
				if (NewPosX < 0) NewPosX = 0;
				if (NewPosY < 0) NewPosY = 0;
				SetWindowPos(hDlg, 0, NewPosX, NewPosY,
					0, 0, SWP_NOZORDER | SWP_NOSIZE);
			}
			return 1;

		case WM_COMMAND:
			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL)) {
				EndDialog(hDlg, LOWORD(wParam));
				return 1;
			}
			break;
	}

    return 0;
}

/**
 * Mesage handler for the detail view.
 *
 * @param  hDlg   Dialog window handler.
 * @param  wMsg   Message type.
 * @param  wParam Message parameter.
 * @param  lParam Message parameter.
 * @return        0 if everything worked.
 */
int CALLBACK DetailDlgProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam) {
	switch (wMsg) {
	case WM_INITDIALOG: {
#ifdef SHELL_AYGSHELL
		// Setup the dialog as full-screen.
		SHINITDLGINFO shidi = {0};
		shidi.dwMask = SHIDIM_FLAGS;
		shidi.dwFlags = SHIDIF_DONEBUTTON | SHIDIF_SIZEDLGFULLSCREEN;
		shidi.hDlg = hDlg;

		SHInitDialog(&shidi);
		SetActiveWindow(hDlg);
#endif

		BringWindowToTop(GetDlgItem(hDlg, IDC_LBNOIMAGE));
		ShowWindow(GetDlgItem(hDlg, IDC_LBNOIMAGE), SW_SHOW);
		return 0;
	}
	case WM_SIZE: {
		// Get the dialog position and size.
		RECT rcDialog;
		GetWindowRect(hDlg, &rcDialog);

		// Calculate the properties list size and position.
		HWND hwndList = GetDlgItem(hDlg, IDC_LSPROPS);
		RECT rcList;
		GetWindowRect(hwndList, &rcList);
#ifdef SHELL_AYGSHELL
		rcList.top -= rcDialog.top;
		rcList.right = rcDialog.right - rcDialog.left - rcList.left - DEFAULT_UI_MARGIN;
		rcList.bottom = rcDialog.bottom - rcDialog.top - rcList.top - DEFAULT_UI_MARGIN;
#else
		rcList.left -= rcDialog.left;
		rcList.top -= rcDialog.top;
		rcList.right = rcDialog.right - rcDialog.left - rcList.left - DEFAULT_UI_MARGIN;
		rcList.bottom = rcDialog.bottom - rcDialog.top - rcList.top - DEFAULT_UI_MARGIN;
#endif

		// Calculate the notes edit size and position.
		HWND hwndNotes = GetDlgItem(hDlg, IDC_EDNOTES);
		RECT rcNotes;
		GetWindowRect(hwndNotes, &rcNotes);
#ifdef SHELL_AYGSHELL
		rcNotes.top -= rcDialog.top;
		rcNotes.right = rcDialog.right - rcDialog.left - rcNotes.left - DEFAULT_UI_MARGIN;
		rcNotes.bottom -= rcDialog.top + rcNotes.top;
#else
		rcNotes.left -= rcDialog.left;
		rcNotes.top -= rcDialog.top;
		rcNotes.right = rcList.left - (DEFAULT_UI_MARGIN * 2);
		rcNotes.bottom = rcDialog.bottom - rcDialog.top - rcNotes.top - DEFAULT_UI_MARGIN;
#endif

#ifdef SHELL_AYGSHELL
		// Calculate the name edit size and position.
		HWND hwndName = GetDlgItem(hDlg, IDC_EDNAME);
		RECT rcName;
		GetWindowRect(hwndName, &rcName);
		rcName.top -= rcDialog.top;
		rcName.right = rcDialog.right - rcDialog.left - rcName.left - DEFAULT_UI_MARGIN;
		rcName.bottom -= rcDialog.top + rcName.top;

		// Calculate the quantity edit size and position.
		HWND hwndQuantity = GetDlgItem(hDlg, IDC_EDQUANTITY);
		RECT rcQuantity;
		GetWindowRect(hwndQuantity, &rcQuantity);
		rcQuantity.top -= rcDialog.top;
		rcQuantity.right = rcDialog.right - rcDialog.left - rcQuantity.left - DEFAULT_UI_MARGIN;
		rcQuantity.bottom -= rcDialog.top + rcQuantity.top;
#endif

		// Expand the dynamic controls in the view.
		SetWindowPos(hwndList, HWND_TOP, rcList.left, rcList.top, rcList.right,
			rcList.bottom, SWP_SHOWWINDOW);
		SetWindowPos(hwndNotes, HWND_TOP, rcNotes.left, rcNotes.top,
			rcNotes.right, rcNotes.bottom, SWP_SHOWWINDOW);
#ifdef SHELL_AYGSHELL
		SetWindowPos(hwndName, HWND_TOP, rcName.left, rcName.top,
			rcName.right, rcName.bottom, SWP_SHOWWINDOW);
		SetWindowPos(hwndQuantity, HWND_TOP, rcQuantity.left, rcQuantity.top,
			rcQuantity.right, rcQuantity.bottom, SWP_SHOWWINDOW);
#endif

		return 0;
	}
	case WM_COMMAND:
		switch (GET_WM_COMMAND_ID(wParam, lParam)) {
		case IDC_LSPROPS:
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK:
				return uiManager.EditSelectedProperty();
			}

			break;
		case IDC_EDNAME:
		case IDC_EDQUANTITY:
		case IDC_EDNOTES:
			if (HIWORD(wParam) == EN_CHANGE) {
				uiManager.SetDirty(true);
			}

			break;
		case IDOK:
			// Check for unsaved changes.
			if (uiManager.CheckForUnsavedChanges())
				return 1;

			return DestroyWindow(hDlg) == 0;
		}

		break;
	}

    return 0;
}
