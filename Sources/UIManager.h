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
#include "Settings.h"
#include "TreeView.h"
#include "Directory.h"
#include "Workspace.h"

// Define the Image List image indexes.
#define ILI_FOLDER 0
#define ILI_CHIP   1

class UIManager {
protected:
	HINSTANCE *hInst;
	HWND *hwndMain;
	HWND *hwndDetail;
	HIMAGELIST *hIml;
	HBITMAP hbmpComponent;
	DLGPROC lpDetailProc;
	Settings *settings;
	Workspace *workspace;
	TreeView *treeView;
	long iSelComponent;
	bool bDirty;

public:
	// Constructors and destructors.
	UIManager();
	UIManager(HINSTANCE *hInst, HWND *hwndMain, Settings *settings,
			  Workspace *workspace, TreeView *treeView, HIMAGELIST *hIml,
			  HWND *hwndDetail, DLGPROC lpDetailProc);

	// Loading dialog.
	static void ShowLoading();
	static void HideLoading();

	// Image list.
	static HIMAGELIST InitializeImageList(HINSTANCE hInstance);

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
	LRESULT ShowDatasheet();
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