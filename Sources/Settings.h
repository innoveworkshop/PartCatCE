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
	WCHAR szPDFViewerPath[MAX_PATH];

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

	// PDF viewer.
	LPCTSTR GetPDFViewer();
	void SetPDFViewer(LPCTSTR szPath);
	int SelectPDFProgram();
};

#endif  // _SETTINGS_H