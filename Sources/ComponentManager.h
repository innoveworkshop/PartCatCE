/**
 * ComponentManager.h
 * Manages all of the components in the application and also offloads some of
 * the UI stuff.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef _COMPONENT_MANAGER_H
#define _COMPONENT_MANAGER_H

#include <windows.h>
#include "TreeView.h"
#include "Directory.h"
#include "Workspace.h"

class ComponentManager {
protected:
	Workspace *workspace;
	TreeView *treeView;

public:
	// Constructors and destructors.
	ComponentManager();
	ComponentManager(Workspace *workspace, TreeView *treeView);

	// UI stuff.
	void PopulateTreeView();
	LRESULT TreeViewSelectionChanged(HWND hWnd, UINT wMsg, WPARAM wParam,
									 LPARAM lParam);
};

#endif  // _COMPONENT_MANAGER_H