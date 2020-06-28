/**
 * PropertyEditor.h
 * Manages the property editor dialog box.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef _PROPERTY_EDITOR_H
#define _PROPERTY_EDITOR_H

#include <windows.h>
#include "Property.h"

class PropertyEditor {
protected:
	HWND *hwndParent;
	Property *property;
	bool bUpdated;

	// Misc.
	void UpdateProperty(HWND hDlg);

	// Dialog procedure.
	int DlgProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam);
	static int CALLBACK DlgProcWrapper(HWND hDlg, UINT wMsg, WPARAM wParam,
									   LPARAM lParam);

public:
	// Constructors and destructors.
	PropertyEditor();
	PropertyEditor(HINSTANCE hInst, HWND *hwndParent, Property *property);

	// Misc.
	bool Updated();
};

#endif  // _PROPERTY_EDITOR_H