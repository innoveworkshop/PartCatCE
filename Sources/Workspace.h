/**
 * Workspace.h
 * Manages the workspace stuff for the application and also offloads some of
 * the UI stuff.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef _WORKSPACE_H
#define _WORKSPACE_H

#include <windows.h>
#include "Constants.h"
#include "Directory.h"
#include "Component.h"

using namespace std;

class Workspace {
protected:
	Directory dirWorkspace;
	vector<Property> arrProperties;
	vector<Component> arrComponents;
	bool bOpened;

	// Population.
	void PopulateProperties();
	void PopulateComponents();

public:
	// Constructors and destructors.
	Workspace();
	Workspace(Path pathWorkspace);
	Workspace(Directory dirWorkspace);

	// Properties.
	bool Save();
	void AddProperty(Property property);
	vector<Property> GetProperties();
	vector<Property>* GetEditableProperties();
	Property* GetProperty(size_t nIndex);
	Property* GetProperty(LPCTSTR szName);
	LPCTSTR GetName();

	// Components.
	Component* GetComponent(size_t nIndex);
	vector<Component> GetComponents();

	// Operations.
	static bool Create(LPCTSTR szPath);
	bool Open(Path pathWorkspace);
	bool Open(Directory dirWorkspace);
	void Close();
	bool Refresh();

	// Status.
	Directory GetDirectory();
	bool IsOpened();
};

#endif  // _WORKSPACE_H