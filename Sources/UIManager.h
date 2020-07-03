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
	HINSTANCE *hInst;
	HWND *hwndMain;
	HWND *hwndDetail;
	HBITMAP hbmpComponent;
	DLGPROC lpDetailProc;
	Workspace *workspace;
	TreeView *treeView;
	long iSelComponent;
	bool bDirty;

public:
	// Constructors and destructors.
	UIManager();
	UIManager(HINSTANCE *hInst, HWND *hwndMain, Workspace *workspace,
			  TreeView *treeView, HWND *hwndDetail, DLGPROC lpDetailProc);

	// General stuff.
	bool GetEditText(HWND hwndControl, LPTSTR *szBuffer);
	void SetApplicationSubTitle(LPCTSTR szSubTitle);

	// Dirtiness and unsaved changes stuff.
	void SetDirty(bool bDirty);
	bool IsDirty();
	bool CheckForUnsavedChanges();

	// Component operations.
	LRESULT CreateComponent();
	void SyncDetailViewWithComponent(Component *component, bool bSaveNotes);
	LRESULT SaveComponent(bool bSaveAs);
	LRESULT DeleteComponent();
	bool IsComponentOpened();

	// Detail view.
	void ClearDetailView(bool bClose);
	void PopulateDetailView(size_t nIndex);

	// TreeView.
	void PopulateTreeView();
	LRESULT TreeViewSelectionChanged(HWND hWnd, UINT wMsg, WPARAM wParam,
									 LPARAM lParam);

	// Image.
	void ClearImage();
	void SetComponentImage(Component *component);

	// Properties.
	void PopulatePropertiesList(Component *component);
	LRESULT CreateProperty();
	LRESULT EditSelectedProperty();
	LRESULT DeleteSelectedProperty();

	// Workspace operations.
	LRESULT CreateWorkspace();
	LRESULT OpenWorkspace(bool bRefresh);
	LRESULT RefreshWorkspace();
	LRESULT CloseWorkspace();
};

#endif  // _UI_MANAGER_H