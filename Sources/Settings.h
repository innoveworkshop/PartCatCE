/**
 * Settings.h
 * Manages the settings and its accompained UI counterparts.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef _SETTINGS_H
#define _SETTINGS_H

#include <windows.h>

class Settings {
protected:
	HINSTANCE *hInstance;
	HWND *hwndParent;
	HWND hwndDialog;
	WCHAR szLastWorkspace[MAX_PATH];

	// Initialization.
	void Initialize();

	// Registry.
	long SetRegistryValue(LPCTSTR szRegValue, DWORD dwType,
						  const BYTE *lpData, DWORD dwLength);
	long SetRegistryValue(LPCTSTR szRegValue, LPCTSTR szData);

	// Dialog procedure.
	int DlgProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam);
	static int CALLBACK DlgProcWrapper(HWND hDlg, UINT wMsg, WPARAM wParam,
									   LPARAM lParam);

public:
	// Constructors and destructors.
	Settings();
	Settings(HINSTANCE *hInstance, HWND *hwndParent);

	// Dialog box.
	int ShowDialog();
	int PopulateDialog();

	// Last opened workspace.
	LPCTSTR GetLastOpenedWorkspace();
	void SetLastOpenedWorkspace(LPCTSTR szPath);
};

#endif  // _SETTINGS_H