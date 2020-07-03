/**
 * Settings.cpp
 * Manages the settings and its accompained UI counterparts.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "Settings.h"
#include "Constants.h"
#include "resource.h"

extern "C" {
	void *lpSettingsThis;
}

/**
 * Initializes an empty object.
 */
Settings::Settings() {
	lpSettingsThis = NULL;
}

/**
 * Initializes a creation dialog and immediately shows it.
 *
 * @param hInstance  Application instance handle.
 * @param hwndParent Parent window handle.
 */
Settings::Settings(HINSTANCE *hInstance, HWND *hwndParent) {
	lpSettingsThis = NULL;

	this->hInstance = hInstance;
	this->hwndParent = hwndParent;
}

/**
 * Shows the settings dialog.
 *
 * @return DialogBox return code.
 */
int Settings::ShowDialog() {
	// Create and show dialog.
	return DialogBoxParam(*hInstance, MAKEINTRESOURCE(IDD_SETTINGS),
		*hwndParent, Settings::DlgProcWrapper, (LPARAM)this);
}

/**
 * Mesage handler for the dialog box.
 *
 * @param  hDlg   Dialog window handler.
 * @param  wMsg   Message type.
 * @param  wParam Message parameter.
 * @param  lParam Message parameter.
 * @return        0 if everything worked.
 */
int Settings::DlgProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam) {
	switch (wMsg) {
	case WM_INITDIALOG: {
		WCHAR szBuffer[MAX_PATH];

		// Set the type of creation we are doing.
		wcscpy(szBuffer, L"Hello!");
		SetDlgItemText(hDlg, IDC_EDPDFAPP, szBuffer);
		
		return 0;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BTPDFBROWSE:
			return 0;
		case IDOK: {
/*			HWND hCtl;
			int nLength;

			hCtl = GetDlgItem(hDlg, IDC_EDVALUE);
			nLength = GetWindowTextLength(hCtl) + 1;
			bCreated = GetWindowText(hCtl, szName, nLength) != 0;
*/
			EndDialog(hDlg, LOWORD(wParam));
			return 0;
		}
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return 1;
		}
		break;
	}

    return 0;
}

/**
 * Mesage handler wrapper for the dialog box. Allowing it to interface with 
 * the C++ class.
 *
 * @param  hDlg   Dialog window handler.
 * @param  wMsg   Message type.
 * @param  wParam Message parameter.
 * @param  lParam Class object pointer.
 * @return        0 if everything worked.
 */
int CALLBACK Settings::DlgProcWrapper(HWND hDlg, UINT wMsg, WPARAM wParam,
									  LPARAM lParam) {
	if (lpSettingsThis == NULL)
		lpSettingsThis = (void*)lParam;

	Settings *pThis = reinterpret_cast<Settings*>(lpSettingsThis);
	return pThis->DlgProc(hDlg, wMsg, wParam, lParam);
}