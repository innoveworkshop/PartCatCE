/**
 * Settings.cpp
 * Manages the settings and its accompained UI counterparts.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "Settings.h"
#include "Constants.h"
#include "FileUtils.h"
#include "resource.h"
#include "commdlg.h"

// Registry storage definitions.
#define SETTINGS_ROOT        L"Software\\Innove Workshop\\PartCat"
#define REGVAL_LASTWORKSPACE L"LastWorkspace"

extern "C" {
	void *lpSettingsThis;
}

/**
 * Initializes an empty object.
 */
Settings::Settings() {
	Initialize();
}

/**
 * Initializes a creation dialog and immediately shows it.
 *
 * @param hInstance  Application instance handle.
 * @param hwndParent Parent window handle.
 */
Settings::Settings(HINSTANCE *hInstance, HWND *hwndParent) {
	Initialize();
	this->hInstance = hInstance;
	this->hwndParent = hwndParent;
}

/**
 * Initializes the settings object.
 */
void Settings::Initialize() {
	HKEY hKey;
	DWORD dwDisposition;
	LONG lResult;

	// Clear everything.
	lpSettingsThis = NULL;
	hwndDialog = NULL;
	szLastWorkspace[0] = L'\0';

	// Open the registry key.
	lResult = RegCreateKeyEx(HKEY_CURRENT_USER, SETTINGS_ROOT, 0, NULL, 0, 0, NULL,
			&hKey, &dwDisposition);
	if (lResult != ERROR_SUCCESS) {
		MessageBox(NULL, L"Couldn't open the the settings registry key.",
			L"Registry Error", MB_OK | MB_ICONERROR);
		return;
	}

	// Check if we just created the key.
	if (dwDisposition == REG_OPENED_EXISTING_KEY) {
		DWORD dwLength = MAX_PATH;

		// Get the last opened workspace key.
		lResult = RegQueryValueEx(hKey, REGVAL_LASTWORKSPACE, NULL, NULL,
			(LPBYTE)szLastWorkspace, &dwLength);
		if (lResult != ERROR_SUCCESS)
			szLastWorkspace[0] = '\0';
	}

	// Close the registry key.
	RegCloseKey(hKey);
}

/**
 * Sets a registry value in the appropriate place.
 * @remark This function will show error message boxes in case of an error.
 *
 * @param  szRegValue Registry value name.
 * @param  dwType     Type of the registry value.
 * @param  lpData     Registry value data to be set.
 * @param  dwLength   Length of the data.
 * @return            RegSetValueEx return code.
 */
long Settings::SetRegistryValue(LPCTSTR szRegValue, DWORD dwType,
								const BYTE *lpData, DWORD dwLength) {
	HKEY hKey;
	LONG lResult;

	// Open the registry key.
	lResult = RegOpenKeyEx(HKEY_CURRENT_USER, SETTINGS_ROOT, 0, 0, &hKey);
	if (lResult != ERROR_SUCCESS) {
		MessageBox(NULL, L"Couldn't open the the settings registry key.",
			L"Registry Error", MB_OK | MB_ICONERROR);
		return lResult;
	}

	// Set the registry value.
	lResult = RegSetValueEx(hKey, szRegValue, 0, REG_SZ, lpData, dwLength);

	// Close the registry key and return.
	return RegCloseKey(hKey);
}

/**
 * Sets a REG_SZ registry value in the appropriate place.
 * @remark This function will show error message boxes in case of an error.
 *
 * @param  szRegValue Registry value name.
 * @param  szData     String data for the value contents.
 * @return            RegSetValueEx return code.
 */
long Settings::SetRegistryValue(LPCTSTR szRegValue, LPCTSTR szData) {
	return SetRegistryValue(szRegValue, REG_SZ, (const BYTE*)szData, sizeof(WCHAR) * (wcslen(szData) + 1));
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
 * Populates the dialog with settings.
 *
 * @return 0 if everything went fine.
 */
int Settings::PopulateDialog() {
	return 0;
}

/**
 * Gets the path to the last opened workspace.
 *
 * @return Last opened workspace path or NULL if one wasn't set or moved.
 */
LPCTSTR Settings::GetLastOpenedWorkspace() {
	// Check if the last workspace is empty.
	if (szLastWorkspace[0] == L'\0')
		return NULL;

	// Check if the last workspace still exists.
	if (!FileUtils::Exists(szLastWorkspace))
		return NULL;

	return szLastWorkspace;
}

/**
 * Sets the last opened workspace path.
 *
 * @param szPath Path to the last opened workspace.
 */
void Settings::SetLastOpenedWorkspace(LPCTSTR szPath) {
	// Set the last workspace locally.
	wcscpy(szLastWorkspace, szPath);

	// Set the Last Workspace registry value.
	SetRegistryValue(REGVAL_LASTWORKSPACE, szLastWorkspace);
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