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
	vector<Component> arrComponents;

	void PopulateComponents();

public:
	// Constructors and destructors.
	Workspace();
	Workspace(Directory dirWorkspace);

	// Components.
	Component* GetComponent(size_t nIndex);
	vector<Component> GetComponents();
};

#endif  // _WORKSPACE_H