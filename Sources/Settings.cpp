/**
 * Settings.cpp
 * Manages the settings and its accompained UI counterparts.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "Settings.h"
#include "Constants.h"
#include "resource.h"
#include "commdlg.h"

extern "C" {
	void *lpSettingsThis;
}

/**
 * Initializes an empty object.
 */
Settings::Settings() {
	lpSettingsThis = NULL;
	hwndDialog = NULL;
	szPDFViewerPath[0] = L'\0';
}

/**
 * Initializes a creation dialog and immediately shows it.
 *
 * @param hInstance  Application instance handle.
 * @param hwndParent Parent window handle.
 */
Settings::Settings(HINSTANCE *hInstance, HWND *hwndParent) {
	lpSettingsThis = NULL;
	hwndDialog = NULL;
	szPDFViewerPath[0] = L'\0';

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
 * Selects a PDF viewer program using a file browser.
 *
 * @return 0 if everything worked.
 */
int Settings::SelectPDFProgram() {
	OPENFILENAME ofn = {0};
    WCHAR szPath[MAX_PATH] = L"";

	// Setup the open dialog.
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrTitle = L"Select PDF Viewer Program";
	ofn.hwndOwner = *hwndParent;
	ofn.lpstrFilter = L"Executables (*.exe)\0*.exe\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szPath;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
	ofn.lpstrDefExt = L"exe";

	// Open the open file dialog.
	if (!GetOpenFileName(&ofn))
		return 1;

	// Set the PDF viewer path and refresh the dialog.
	SetPDFViewer(szPath);
	PopulateDialog();

	return 0;
}

/**
 * Populates the dialog with settings.
 *
 * @return 0 if everything went fine.
 */
int Settings::PopulateDialog() {
	SetDlgItemText(hwndDialog, IDC_EDPDFAPP, szPDFViewerPath);
	return 0;
}

/**
 * Gets the path to the default PDF viewer application.
 *
 * @return PDF viewer path or NULL if one wasn't set.
 */
LPCTSTR Settings::GetPDFViewer() {
	if (szPDFViewerPath[0] == L'\0')
		return NULL;

	return szPDFViewerPath;
}

/**
 * Sets the PDF viewer program path.
 *
 * @param szPath Path to the PDF viewer.
 */
void Settings::SetPDFViewer(LPCTSTR szPath) {
	wcscpy(szPDFViewerPath, szPath);
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
	case WM_INITDIALOG:
		hwndDialog = hDlg;
		return PopulateDialog();
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BTPDFBROWSE:
			return SelectPDFProgram();
		case IDOK:
			// TODO: Save the settings to the registry.
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return 0;
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