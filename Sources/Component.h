/**
 * Component.h
 * Abstract class that represents a component in PartCat.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef _COMPONENT_H
#define _COMPONENT_H

#include <windows.h>
#include <vector>
#include "Directory.h"
#include "Property.h"

using namespace std;

class Component {
protected:
	Directory dirPath;
	WCHAR szName[MAX_PATH];
	size_t nQuantity;
	vector<Property> arrProperties;

	// Population.
	void PopulateProperties();
	void PopulateFromDirectory();

public:
	// Constructors and destructors.
	Component();
	Component(Directory dirPath);

	// Geters and seters.
	LPCTSTR GetName();
	bool SetName(LPCTSTR szName);
	LPCTSTR GetNotes();
	void SetNotes(LPCTSTR szNotes);
	size_t GetQuantity();
	void SetQuantity(size_t nQuantity);
	Property* GetProperty(LPCTSTR szName);
	vector<Property> GetProperties();
	LPCTSTR GetCategory();
	LPCTSTR GetSubCategory();

	// Misc.
	void ClearFields();
	LPCTSTR ToString();
};

#endif  // _COMPONENT_H