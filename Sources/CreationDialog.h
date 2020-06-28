/**
 * CreationDialog.h
 * Manages the creation dialog box.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef _CREATION_DIALOG_H
#define _CREATION_DIALOG_H

#include <windows.h>
#include "Property.h"

class CreationDialog {
protected:
	HWND *hwndParent;
	LPCTSTR szType;
	WCHAR szName[MAX_PATH];
	bool bCreated;

	// Dialog procedure.
	int DlgProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam);
	static int CALLBACK DlgProcWrapper(HWND hDlg, UINT wMsg, WPARAM wParam,
									   LPARAM lParam);

public:
	// Constructors and destructors.
	CreationDialog();
	CreationDialog(HINSTANCE hInst, HWND *hwndParent, LPCTSTR szType);

	// Misc.
	bool Created();
	LPCTSTR GetName();
};

#endif  // _CREATION_DIALOG_H