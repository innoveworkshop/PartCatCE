/**
 * Property.h
 * Abstract class that represents a property in a component in PartCat.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef _PROPERTY_H
#define _PROPERTY_H

#include <windows.h>

class Property {
protected:
	WCHAR szName[MAX_PATH];
	WCHAR szValue[MAX_PATH];

	bool ParseLine(LPCTSTR szLine);

public:
	// Constructors and destructors.
	Property();
	Property(LPCTSTR szLine);

	// Geters and seters.
	LPCTSTR GetName();
	void SetName(LPCTSTR szName);
	LPCTSTR GetValue();
	void SetValue(LPCTSTR szValue);

	// Misc.
	void ClearFields();
	LPTSTR ToString();
};

#endif  // _PROPERTY_H