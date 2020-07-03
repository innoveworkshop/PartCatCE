/**
 * CreationDialog.cpp
 * Manages the creation dialog box.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "CreationDialog.h"
#include "Constants.h"
#include "resource.h"

extern "C" {
	void *lpCreationDialogThis;
}

/**
 * Initializes an empty object.
 */
CreationDialog::CreationDialog() {
	lpCreationDialogThis = NULL;
	bCreated = false;
}

/**
 * Initializes a creation dialog and immediately shows it.
 *
 * @param hInst      Application instance handle.
 * @param hwndParent Parent window handle.
 * @param szType     Type of object that we are creating.
 */
CreationDialog::CreationDialog(HINSTANCE hInst, HWND *hwndParent, LPCTSTR szType) {
	lpCreationDialogThis = NULL;
	bCreated = false;
	this->hwndParent = hwndParent;
	this->szType = szType;

	// Create dialog.
	DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_NEWOBJECT), *hwndParent,
		CreationDialog::DlgProcWrapper, (LPARAM)this);
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
int CreationDialog::DlgProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam) {
	switch (wMsg) {
	case WM_INITDIALOG: {
		WCHAR szBuffer[MAX_PATH];

		// Set the type of creation we are doing.
		swprintf(szBuffer, L"%s Name:", szType);
		SetDlgItemText(hDlg, IDC_LBNAME, szBuffer);
		swprintf(szBuffer, L"Create %s", szType);
		SetWindowText(hDlg, szBuffer);

		return 0;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK: {
			HWND hCtl;
			int nLength;

			hCtl = GetDlgItem(hDlg, IDC_EDVALUE);
			nLength = GetWindowTextLength(hCtl) + 1;
			bCreated = GetWindowText(hCtl, szName, nLength) != 0;

			EndDialog(hDlg, LOWORD(wParam));
			return 0;
		}
		case IDCANCEL:
			bCreated = false;
			EndDialog(hDlg, LOWORD(wParam));

			return 1;
		}
		break;
	}

    return 0;
}

/**
 * Gets the name the user typed in.
 *
 * @return Name of the thing.
 */
LPCTSTR CreationDialog::GetName() {
	return szName;
}

/**
 * Checks if the user decided to create something.
 *
 * @return TRUE if we should create something.
 */
bool CreationDialog::Created() {
	return bCreated;
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
int CALLBACK CreationDialog::DlgProcWrapper(HWND hDlg, UINT wMsg, WPARAM wParam,
											LPARAM lParam) {
	if (lpCreationDialogThis == NULL)
		lpCreationDialogThis = (void*)lParam;

	CreationDialog *pThis = reinterpret_cast<CreationDialog*>(lpCreationDialogThis);
	return pThis->DlgProc(hDlg, wMsg, wParam, lParam);
}