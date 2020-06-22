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
#include <string.h>

#include "Directory.h"
#include "TreeView.h"
#include "Workspace.h"
#include "ComponentManager.h"

// Global variables.
HINSTANCE hInst;
TreeView treeView;
Workspace workspace;
ComponentManager manComponent;

#ifdef DEVELOP
/**
 * Simple test workspace that is loaded for debugging quickly.
 *
 * @return 0 if everything went OK.
 */
LRESULT LoadTestWorkspace() {
	// Initialize everything.
	workspace = Workspace(Directory(TEST_WORKSPACE));
	manComponent = ComponentManager(&workspace, &treeView);

	// Populate the TreeView.
	manComponent.PopulateTreeView();

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
	HWND hwndMain;
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

	// Register the application's main window class.
	wc.style = 0;					   // Window style.
	wc.lpfnWndProc = MainWindowProc;   // Main window procedure.
	wc.cbClsExtra = 0;				   // Extra class data.
	wc.cbWndExtra = 0;				   // Extra window data.
	wc.hInstance = hInstance;		   // Owner handle.
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PARTCAT));
	wc.hCursor = NULL;				   // Default cursor. (Always NULL)
	wc.hbrBackground = (HBRUSH)GetSysColorBrush(COLOR_STATIC);
	wc.lpszMenuName = NULL;            // Menu name. (Always NULL)
	wc.lpszClassName = szWindowClass;  // Window class name.

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
	hInst = hInstance;

	// Load the application name.
	LoadString(hInst, IDC_PARTCAT, szWindowClass, MAX_LOADSTRING);

	// Create the main window.
	hWnd = CreateWindow(szWindowClass,  // Window class.
						L"WinUki",      // Window title.
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

	// Show and update the window.
	ShowWindow(hWnd, SW_SHOWMAXIMIZED);
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
LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT wMsg, WPARAM wParam,
								LPARAM lParam) {
	switch (wMsg) {
	case WM_CREATE:
		return WndMainCreate(hWnd, wMsg, wParam, lParam);
	case WM_COMMAND:
		return WndMainCommand(hWnd, wMsg, wParam, lParam);
	case WM_INITMENUPOPUP:
		return WndMainInitMenuPopUp(hWnd, wMsg, wParam, lParam);
	case WM_NOTIFY:
		return WndMainNotify(hWnd, wMsg, wParam, lParam);
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
LRESULT WndMainCreate(HWND hWnd, UINT wMsg, WPARAM wParam,
					  LPARAM lParam) {
	HWND hwndCB;
	//HIMAGELIST hIml;
	RECT rcTreeView;
	RECT rcPageView;

	// Ensure that the common control DLL is loaded. 
    InitCommonControls();

	// Initialize the Image List.
	//hIml = InitializeImageList(hInst);

	// Create CommandBar.
	hwndCB = CommandBar_Create(hInst, hWnd, IDC_CMDBAR);
	
    // Add the Standard and View bitmaps to the toolbar.
    CommandBar_AddBitmap(hwndCB, HINST_COMMCTRL, IDB_STD_SMALL_COLOR,
		STD_BMPS_LEN, 16, 16);
    CommandBar_AddBitmap(hwndCB, HINST_COMMCTRL, IDB_VIEW_SMALL_COLOR,
		VIEW_BMPS_LEN, 16, 16);

	// Insert menu bar, toolbar buttons, and the exit button.
	CommandBar_InsertMenubar(hwndCB, hInst, IDM_MAINMENU, 0);
    //CommandBar_AddButtons(hwndCB, sizeof(tbButtons) / sizeof(TBBUTTON),
	//	tbButtons);
	CommandBar_AddAdornments(hwndCB, 0, 0);

	// Calculate the TreeView control size and position.
	GetClientRect(hWnd, &rcTreeView);
	rcTreeView.top += CommandBar_Height(hwndCB);
	rcTreeView.bottom -= rcTreeView.top;
	rcTreeView.right = (LONG)(rcTreeView.right / 3.5);

	// Create the TreeView control.
	treeView = TreeView(hInst, hWnd, rcTreeView, (HMENU)IDC_TREEVIEW);
	//treeView.SetImageList(hIml);

	// Calculate the page view controls size and position.
	GetClientRect(hWnd, &rcPageView);
	rcPageView.top = rcTreeView.top;
	rcPageView.bottom = rcTreeView.bottom;
	rcPageView.left = rcTreeView.right + 5;
	rcPageView.right -= rcPageView.left;

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
LRESULT WndMainInitMenuPopUp(HWND hWnd, UINT wMsg, WPARAM wParam,
							 LPARAM lParam) {
	/*HMENU hMenu = CommandBar_GetMenu(GetDlgItem(hWnd, IDC_CMDBAR), 0);

	// Check if we can undo and enable/disable the menu item accordingly.
	if (SendPageEditMessage(EM_CANUNDO, 0, 0)) {
		EnableMenuItem(hMenu, IDM_EDIT_UNDO, MF_BYCOMMAND | MF_ENABLED);
	} else {
		EnableMenuItem(hMenu, IDM_EDIT_UNDO, MF_BYCOMMAND | MF_GRAYED);
	}

	// Check if editing or viewing a page and change the menu radio group.
	if (IsPageEditorActive()) {
		CheckMenuRadioItem(hMenu, IDM_VIEW_PAGEVIEW, IDM_VIEW_PAGEEDIT,
			IDM_VIEW_PAGEEDIT, MF_BYCOMMAND);
	} else {
		CheckMenuRadioItem(hMenu, IDM_VIEW_PAGEVIEW, IDM_VIEW_PAGEEDIT,
			IDM_VIEW_PAGEVIEW, MF_BYCOMMAND);
	}

	// Enable/disable the Find Next button if there's something in the edit box.
	if (PageEditCanFindNext()) {
		EnableMenuItem(hMenu, IDM_EDIT_FINDNEXT, MF_BYCOMMAND | MF_ENABLED);
	} else {
		EnableMenuItem(hMenu, IDM_EDIT_FINDNEXT, MF_BYCOMMAND | MF_GRAYED);
	}

	// Enable and disable workspace related items.
	if (fWorkspaceOpen) {
		EnableMenuItem(hMenu, IDM_FILE_NEWARTICLE, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(hMenu, IDM_FILE_NEWTEMPLATE, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(hMenu, IDM_FILE_REFRESHWS, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(hMenu, IDM_FILE_CLOSEWS, MF_BYCOMMAND | MF_ENABLED);
	} else {
		EnableMenuItem(hMenu, IDM_FILE_NEWARTICLE, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(hMenu, IDM_FILE_NEWTEMPLATE, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(hMenu, IDM_FILE_REFRESHWS, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(hMenu, IDM_FILE_CLOSEWS, MF_BYCOMMAND | MF_GRAYED);
	}

	// Enable/disable article related items.
	if (IsArticleLoaded() || IsTemplateLoaded()) {
		EnableMenuItem(hMenu, IDM_FILE_SAVE, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(hMenu, IDM_FILE_SAVEAS, MF_BYCOMMAND | MF_ENABLED);
	} else {
		EnableMenuItem(hMenu, IDM_FILE_SAVE, MF_BYCOMMAND | MF_GRAYED);
		EnableMenuItem(hMenu, IDM_FILE_SAVEAS, MF_BYCOMMAND | MF_GRAYED);
	}*/

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
LRESULT WndMainCommand(HWND hWnd, UINT wMsg, WPARAM wParam,
					   LPARAM lParam) {
	switch (GET_WM_COMMAND_ID(wParam, lParam)) {
	case IDM_HELP_ABOUT:
		DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)AboutDlgProc);
		break;
	case IDM_FILE_EXIT:
		return SendMessage(hWnd, WM_CLOSE, 0, 0);
	default:
		return DefWindowProc(hWnd, wMsg, wParam, lParam);
	}

	return 0;
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
LRESULT WndMainNotify(HWND hWnd, UINT wMsg, WPARAM wParam,
					  LPARAM lParam) {
	switch (((LPNMHDR)lParam)->code) {
	case TVN_SELCHANGED:
		return manComponent.TreeViewSelectionChanged(hWnd, wMsg, wParam,
			lParam);
	}

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
LRESULT WndMainClose(HWND hWnd, UINT wMsg, WPARAM wParam,
					 LPARAM lParam) {
	// Check for unsaved changes.
	//if (CheckForUnsavedChanges())
	//	return 1;

	// Send window destruction message.
	DestroyWindow(hWnd);
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
LRESULT WndMainDestroy(HWND hWnd, UINT wMsg, WPARAM wParam,
					   LPARAM lParam) {
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
LRESULT CALLBACK AboutDlgProc(HWND hDlg, UINT wMsg, WPARAM wParam,
							  LPARAM lParam) {
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
