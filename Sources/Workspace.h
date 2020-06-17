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
#include "Directory.h"
#include "Constants.h"

class Workspace {
protected:
	Directory dirWorkspace;
	Array<Directory> arrComponents;

	void PopulateComponents();

public:
	// Constructors and destructors.
	Workspace();
	Workspace(Directory dirWorkspace);

	// Components.
	Array<Directory> GetComponents();
};

#endif  // _WORKSPACE_H