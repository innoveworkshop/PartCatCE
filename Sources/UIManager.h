/**
 * UIManager.h
 * Manages all of the components in the application and also offloads some of
 * the UI stuff.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef _UI_MANAGER_H
#define _UI_MANAGER_H

#include <windows.h>
#include "TreeView.h"
#include "Directory.h"
#include "Workspace.h"

class UIManager {
protected:
	HWND *hwndMain;
	HWND *hwndDetail;
	Workspace *workspace;
	TreeView *treeView;

public:
	// Constructors and destructors.
	UIManager();
	UIManager(HWND *hwndMain, Workspace *workspace, TreeView *treeView,
			  HWND *hwndDetail);

	// TreeView and detail view.
	void PopulateTreeView();
	void ClearDetailView();
	void PopulateDetailView(size_t nIndex);
	LRESULT TreeViewSelectionChanged(HWND hWnd, UINT wMsg, WPARAM wParam,
									 LPARAM lParam);

	// Workspace operations.
	LRESULT OpenWorkspace();
	LRESULT RefreshWorkspace();
	LRESULT CloseWorkspace();
};

#endif  // _UI_MANAGER_H