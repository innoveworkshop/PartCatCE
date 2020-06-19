/**
 * Component.h
 * Abstract class that represents a component in PartCat.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef _COMPONENT_H
#define _COMPONENT_H

#include <windows.h>
#include "Directory.h"
#include "Property.h"
#include "Array.h"

class Component {
protected:
	Directory dirPath;
	WCHAR szName[MAX_PATH];
	size_t nQuantity;
	Array<Property> arrProperties;

	// Population.
	void PopulateProperties();
	void PopulateFromDirectory();

public:
	// Constructors and destructors.
	Component();
	Component(Directory dirPath);

	// Geters and seters.
	LPCTSTR GetName();
	BOOL SetName(LPCTSTR szName);
	LPCTSTR GetNotes();
	void SetNotes(LPCTSTR szNotes);
	size_t GetQuantity();
	void SetQuantity(size_t nQuantity);
	Array<Property> GetProperties();

	// Misc.
	void ClearFields();
	LPTSTR ToString();
};

#endif  // _COMPONENT_H