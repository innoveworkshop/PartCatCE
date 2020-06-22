/**
 * Category.h
 * Abstract class that represents a component category in PartCat.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef _CATEGORY_H
#define _CATEGORY_H

#include <windows.h>
#include <vector>
#include "Component.h"

using namespace std;

class Category {
protected:
	WCHAR szName[MAX_PATH];
	vector<wstring> arrSubCategories;

public:
	// Constructors and destructors.
	Category();
	Category(LPCTSTR szName);
	Category(LPCTSTR szName, vector<Component> *arrComponents);

	// Geters and seters.
	LPCTSTR GetName();
	vector<wstring> GetSubCategories();
	void PopulateSubCategories(vector<Component> *arrComponents);

	// Comparisons.
	bool Equals(Category category);
	bool Equals(LPCTSTR szName);
	bool Equals(wstring swName);
};

#endif  // _CATEGORY_H