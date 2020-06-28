/**
 * PropertyEditor.cpp
 * Manages the property editor dialog box.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "PropertyEditor.h"
#include "Constants.h"
#include "resource.h"

extern "C" {
	void *lpThis;
}

/**
 * Initializes an empty object.
 */
PropertyEditor::PropertyEditor() {
	lpThis = NULL;
	bUpdated = false;
}

/**
 * Initializes a property editor dialog and immediately shows it.
 *
 * @param hInst      Application instance handle.
 * @param hwndParent Parent window handle.
 * @param property   Property to be edited.
 */
PropertyEditor::PropertyEditor(HINSTANCE hInst, HWND *hwndParent, Property *property) {
	lpThis = NULL;
	bUpdated = false;
	this->hwndParent = hwndParent;
	this->property = property;

	// Create dialog.
	DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_PROPEDIT), *hwndParent,
		PropertyEditor::DlgProcWrapper, (LPARAM)this);
}

/**
 * Updates the property according to the values from the dialog box.
 *
 * @param hDlg Dialog window handle.
 */
void PropertyEditor::UpdateProperty(HWND hDlg) {
	HWND hCtl;
	LPTSTR szBuffer;
	int nLength;

	// Sync property name.
	hCtl = GetDlgItem(hDlg, IDC_CBPROP);
	nLength = GetWindowTextLength(hCtl) + 1;
	szBuffer = (LPTSTR)LocalAlloc(LMEM_FIXED, nLength * sizeof(WCHAR));
	if (GetWindowText(hCtl, szBuffer, nLength) != 0)
		property->SetHumanName(szBuffer);
	LocalFree(szBuffer);

	// Sync property value.
	hCtl = GetDlgItem(hDlg, IDC_EDVALUE);
	nLength = GetWindowTextLength(hCtl) + 1;
	szBuffer = (LPTSTR)LocalAlloc(LMEM_FIXED, nLength * sizeof(WCHAR));
	if (GetWindowText(hCtl, szBuffer, nLength) != 0)
		property->SetValue(szBuffer);
	LocalFree(szBuffer);

	bUpdated = true;
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
int PropertyEditor::DlgProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam) {
	switch (wMsg) {
	case WM_INITDIALOG: {
		// Add default property options.
		SendDlgItemMessage(hDlg, IDC_CBPROP, CB_ADDSTRING, 0,
			(LPARAM)PROPERTY_CATEGORY);
		SendDlgItemMessage(hDlg, IDC_CBPROP, CB_ADDSTRING, 0,
			(LPARAM)PROPERTY_SUBCATEGORY);
		SendDlgItemMessage(hDlg, IDC_CBPROP, CB_ADDSTRING, 0,
			(LPARAM)PROPERTY_PACKAGE);

		// Don't populate the edits if the property is empty.
		if (property->IsEmpty())
			return 1;

		// Set the name of the current property.
		LPTSTR szName = property->GetHumanName();
		SetDlgItemText(hDlg, IDC_CBPROP, szName);
		LocalFree(szName);

		// Set the valud of the current property.
		SetDlgItemText(hDlg, IDC_EDVALUE, property->GetValue());

		return 1;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			UpdateProperty(hDlg);
			EndDialog(hDlg, LOWORD(wParam));

			return 1;
		case IDCANCEL:
			bUpdated = false;
			EndDialog(hDlg, LOWORD(wParam));

			return 1;
		}
		break;
	}

    return 0;
}

/**
 * Checks if the property was updated.
 *
 * @return TRUE if there were any changes to the property.
 */
bool PropertyEditor::Updated() {
	return bUpdated;
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
int CALLBACK PropertyEditor::DlgProcWrapper(HWND hDlg, UINT wMsg, WPARAM wParam,
											LPARAM lParam) {
	if (lpThis == NULL)
		lpThis = (void*)lParam;

	PropertyEditor *pThis = reinterpret_cast<PropertyEditor*>(lpThis);
	return pThis->DlgProc(hDlg, wMsg, wParam, lParam);
}