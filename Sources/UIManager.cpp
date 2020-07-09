/**
 * UIManager.cpp
 * Manages all of the components in the application and also offloads some of
 * the UI stuff.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include <algorithm>
#include "UIManager.h"
#include "Category.h"
#include "ImageUtils.h"
#include "PropertyEditor.h"
#include "CreationDialog.h"
#include "resource.h"
#include "commdlg.h"

/**
 * Initializes an empty component manager.
 */
UIManager::UIManager() {
	SetDirty(false);
	iSelComponent = -1;
}

/**
 * Initializes a component manager with a workspace directory and a TreeView
 * control to manage.
 *
 * @param hInst        Application instance handle.
 * @param hwndMain     Main window handle.
 * @param workspace    PartCat workspace .
 * @param treeView     TreeView control manager.
 * @param hIml         ImageList handle.
 * @param hwndDetail   Detail dialog view handle.
 * @param lpDetailProc Detail view dialog procedure.
 */
UIManager::UIManager(HINSTANCE *hInst, HWND *hwndMain, Settings *settings,
					 Workspace *workspace, TreeView *treeView, HIMAGELIST *hIml,
					 HWND *hwndDetail, DLGPROC lpDetailProc) {
	this->settings = settings;
	this->workspace = workspace;
	this->treeView = treeView;
	this->hInst = hInst;
	this->hIml = hIml;
	this->hwndMain = hwndMain;
	this->hwndDetail = hwndDetail;
	this->lpDetailProc = lpDetailProc;

	ClearDetailView(true);
}

/**
 * Checks for unsaved changes and displays a message box if there are any.
 *
 * @return TRUE if we should abort the current operation.
 */
bool UIManager::CheckForUnsavedChanges() {
	int iSelection;

	// Check for dirtiness.
	if (IsDirty()) {
		// Show the message box and get the user selection.
		iSelection = MessageBox(NULL, L"You have unsaved changes. Do you want "
			L"to save your changes?", L"Unsaved Changes",
			MB_YESNOCANCEL | MB_ICONWARNING);

		// Check which button the user clicked.
		switch (iSelection) {
		case IDYES:
			SaveComponent(false);
			return false;
		case IDNO:
			SetDirty(false);
			return false;
		case IDCANCEL:
			return true;
		}
	}

	return false;
}

/**
 * Creates a component for the user.
 *
 * @return 0 if the operation was successful.
 */
LRESULT UIManager::CreateComponent() {
	if (CheckForUnsavedChanges())
		return 1;

	CreationDialog dialog(*hInst, hwndMain, L"Component");
	if (dialog.Created()) {
		if (!Component::Create(workspace->GetDirectory(), dialog.GetName())) {
			MessageBox(*hwndMain, L"An error occured while creating the component.",
				L"Component Creation Error", MB_OK | MB_ICONERROR);
		}

		RefreshWorkspace();
	}

	return 0;
}

/**
 * Creates a workspace for the user.
 *
 * @return 0 if the operation was successful.
 */
LRESULT UIManager::CreateWorkspace() {
	if (CheckForUnsavedChanges())
		return 1;

	CreationDialog dialog(*hInst, hwndMain, L"Workspace Path");
	if (dialog.Created()) {
		if (!Workspace::Create(dialog.GetName())) {
			MessageBox(*hwndMain, L"An error occured while creating the workspace.",
				L"Workspace Creation Error", MB_OK | MB_ICONERROR);
		}

		// Try to open the new workspace.
		if (!workspace->Open(Directory(dialog.GetName()))) {
			MessageBox(*hwndMain, L"An error occured while trying to open the workspace.",
				L"Open Workspace Error", MB_OK | MB_ICONERROR);
			return 1;
		}

		RefreshWorkspace();
	}

	return 0;
}

/**
 * Opens a workspace for the user.
 *
 * @param  bRefresh Are we in a refresh operation?
 * @return          0 if the operation was successful.
 */
LRESULT UIManager::OpenWorkspace(bool bRefresh) {
	OPENFILENAME ofn = {0};
    WCHAR szPath[MAX_PATH] = L"";

	if (CheckForUnsavedChanges())
		return 1;

	// Clear the screen.
	ClearDetailView(true);
	treeView->Clear();

	// Check if we are not refreshing.
	if (!bRefresh) {
		// Close the workspace first.
		workspace->Close();

		// Populate the structure.
		ofn.lStructSize = sizeof(ofn);
		ofn.lpstrTitle = L"Open PartCat Workspace";
		ofn.hwndOwner = *hwndMain;
		ofn.lpstrFilter = L"PartCat Workspace (PartCat.pcw)\0PartCat.pcw\0"
			L"Workspace Files (*.pcw)\0*.pcw\0All Files (*.*)\0*.*\0";
		ofn.lpstrFile = szPath;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
		ofn.lpstrDefExt = L"pcw";

		// Open the open folder dialog.
		if (!GetOpenFileName(&ofn))
			return 1;

		// Try to open the workspace.
		ShowLoading();
		if (!workspace->Open(Path(szPath))) {
			HideLoading();
			MessageBox(*hwndMain, L"An error occured while trying to open the workspace.",
				L"Open Workspace Error", MB_OK | MB_ICONERROR);
			return 1;
		}

		// Save Last Workspace settings.
		settings->SetLastOpenedWorkspace(szPath);
	}

	// Populate and dress up the window.
	SetApplicationSubTitle(workspace->GetName());
	PopulateTreeView();
	HideLoading();

	return 0;
}

/**
 * Refreshes the workspace for the user.
 *
 * @return 0 if the operation was successful.
 */
LRESULT UIManager::RefreshWorkspace() {
	if (CheckForUnsavedChanges())
		return 1;

	ClearDetailView(true);
	treeView->Clear();
	if (!workspace->Refresh()) {
		MessageBox(*hwndMain, L"An error occured while refreshing the workspace.",
			L"Workspace Refresh Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	OpenWorkspace(true);
	return 0;
}

/**
 * Closes the workspace for the user.
 *
 * @return 0 if the operation was successful.
 */
LRESULT UIManager::CloseWorkspace() {
	if (CheckForUnsavedChanges())
		return 1;

	// Clear the UI.
	ClearDetailView(true);
	treeView->Clear();
	SetApplicationSubTitle(NULL);

	// Close the workspace and clear the last workspace settings.
	workspace->Close();
	settings->SetLastOpenedWorkspace(L"");

	return 0;
}

/**
 * Checks if there's a component opened in the detail view.
 *
 * @return TRUE if there is a component opened.
 */
bool UIManager::IsComponentOpened() {
	return iSelComponent >= 0;
}

/**
 * Syncs the detail view controls changes with the opened component object.
 *
 * @param  component  Component to sync.
 * @param  bSaveNotes Should we save the notes?
 */
void UIManager::SyncDetailViewWithComponent(Component *component, bool bSaveNotes) {
	LPTSTR szBuffer;

	// Sync quantity.
	GetEditText(GetDlgItem(*hwndDetail, IDC_EDQUANTITY), &szBuffer);
	component->SetQuantity(szBuffer);
	LocalFree(szBuffer);

	// Sync notes.
	if (bSaveNotes) {
		GetEditText(GetDlgItem(*hwndDetail, IDC_EDNOTES), &szBuffer);
		component->SaveNotes(szBuffer);
		LocalFree(szBuffer);
	}
}

/**
 * Saves the opened component object.
 *
 * @param  bSaveAs Are we saving this as another component?
 * @return         0 if the operation was successful.
 */
LRESULT UIManager::SaveComponent(bool bSaveAs) {
	// Get component.
	Component *component = workspace->GetComponent(iSelComponent);
	if (component == NULL) {
		MessageBox(*hwndMain, L"Couldn't retrieve the currently selected component.",
			L"Component Retrieval Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	// Sync changes and save if we aren't doing a Save As.
	SyncDetailViewWithComponent(component, !bSaveAs);
	if (!bSaveAs) {
		if (!component->Save()) {
			MessageBox(*hwndMain, L"An error occured while trying to save the component.",
				L"Component Save Error", MB_OK | MB_ICONERROR);
			return 1;
		}
	}

	// Check if we should Save As or if a rename is required.
	LPTSTR szName;
	GetEditText(GetDlgItem(*hwndDetail, IDC_EDNAME), &szName);
	if (bSaveAs) {
		// Check if the names are different.
		if (wcscmp(szName, component->GetName()) == 0) {
			MessageBox(*hwndMain, L"The name of the new component should be "
				L"different than the current one. Change the component name "
				L"before trying to \"Save As\".", L"Trying to Save As with Same Name",
				MB_OK | MB_ICONERROR);
			LocalFree(szName);

			return 1;
		}

		// Actually save the component as a new one.
		component->SetName(szName);
		if (!component->Save(component->GetDirectory().Parent(), true)) {
			MessageBox(*hwndMain, L"An error occured while trying to save the component.",
				L"Component Save Error", MB_OK | MB_ICONERROR);
			LocalFree(szName);

			return 1;
		}
		SyncDetailViewWithComponent(component, true);
	} else if (wcscmp(szName, component->GetName()) != 0) {
		if (!component->Rename(szName)) {
			MessageBox(*hwndMain, L"An error occured while renaming the component.",
				L"Component Rename Error", MB_OK | MB_ICONERROR);
			LocalFree(szName);

			return 1;
		}
	}
	LocalFree(szName);

	// Refresh the workspace.
	SetDirty(false);
	RefreshWorkspace();

	return 0;
}

/**
 * Deletes the opened component.
 *
 * @return 0 if the operation was successful.
 */
LRESULT UIManager::DeleteComponent() {
	// Ask the user politely.
	if (MessageBox(*hwndMain, L"Are you sure you want to permanently delete this component?",
			L"Delete Component", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 |
			MB_APPLMODAL) != IDYES)
		return 1;

	// Get component.
	Component *component = workspace->GetComponent(iSelComponent);
	if (component == NULL) {
		MessageBox(*hwndMain, L"Couldn't retrieve the currently selected component.",
			L"Component Retrieval Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	// Clear the space.
	ClearDetailView(true);

	// Actually delete the component.
	if (!component->Delete()) {
		MessageBox(*hwndMain, L"An error occured while trying to delete the component.",
			L"Component Deletion Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	// Refresh workspace and return.
	RefreshWorkspace();
	return 0;
}

/**
 * Populates the detail view with data from a selected component.
 *
 * @param nIndex Selected component index.
 */
void UIManager::PopulateDetailView(size_t nIndex) {
	// Clear the view for a new component.
	ClearDetailView(false);

	// Get the component and check if it's valid.
	Component *component = workspace->GetComponent(nIndex);
	if (component == NULL) {
		MessageBox(NULL, L"Looks like you've selected an invalid component",
			L"Invalid Component Selected", MB_OK | MB_ICONERROR);
		return;
	}

	// Set the selected component index.
	iSelComponent = nIndex;

	// Set the name field.
	SetDlgItemText(*hwndDetail, IDC_EDNAME, component->GetName());

	// Set component image.
	SetComponentImage(component);

	// Set the quantity field.
	LPTSTR szQuantity = component->GetQuantityString();
	SetDlgItemText(*hwndDetail, IDC_EDQUANTITY, szQuantity);
	LocalFree(szQuantity);

	// Set the notes field.
	LPTSTR szNotes = component->GetNotes();
	if (szNotes) {
		SetDlgItemText(*hwndDetail, IDC_EDNOTES, szNotes);
		LocalFree(szNotes);
	}

	// Populate the properties list.
	PopulatePropertiesList(component);

	// Make sure we are not dirty.
	SetDirty(false);
}

/**
 * Populates the properties list with data from a component.
 *
 * @param component Component used to populate the list.
 */
void UIManager::PopulatePropertiesList(Component *component) {
	vector<Property> arrProperties = component->GetProperties();

	for (size_t i = 0; i < arrProperties.size(); i++) {
		Property prop = arrProperties[i];
		LPTSTR szCaption = prop.ToHumanString();

		// Append the string to the list box.
		int pos = (int)SendDlgItemMessage(*hwndDetail, IDC_LSPROPS,
			LB_ADDSTRING, 0, (LPARAM)szCaption);
		SendDlgItemMessage(*hwndDetail, IDC_LSPROPS, LB_SETITEMDATA,
			(WPARAM)pos, (LPARAM)i);

		LocalFree(szCaption);
	}
}

/**
 * Clears the detail view.
 *
 * @param bClose Should we also destroy the detail dialog?
 */
void UIManager::ClearDetailView(bool bClose) {
	// Clear edit boxes.
	SetDlgItemText(*hwndDetail, IDC_EDNAME, L"");
	SetDlgItemText(*hwndDetail, IDC_EDQUANTITY, L"");
	SetDlgItemText(*hwndDetail, IDC_EDNOTES, L"");

	// Clear the list box and image.
	SendDlgItemMessage(*hwndDetail, IDC_LSPROPS, LB_RESETCONTENT, 0, 0);
	ClearImage();

	// Reset flags.
	iSelComponent = -1;
	SetDirty(false);

#ifdef SHELL_AYGSHELL
	if (bClose)
		DestroyWindow(*hwndDetail);
#endif
}

/**
 * Sets the component image label in the detail view.
 *
 * @param component Component to get the image for.
 */
void UIManager::SetComponentImage(Component *component) {
	int cx, cy;

	// Clear the current image.
	ClearImage();

	// Grab the path to the component image.
	LPTSTR szImagePath = component->GetImage();
	if (szImagePath == NULL)
		return;

	// Load the new image.
	HBITMAP hBmp = ImageUtils::LoadBitmap(szImagePath);
	LocalFree(szImagePath);
	if (hBmp == NULL) {
		MessageBox(*hwndMain, L"An error occured while loading the component image.",
			L"Image Loading Error", MB_OK | MB_ICONERROR);
		return;
	}

	// Set the optimal size for the component image.
#ifdef SHELL_AYGSHELL
	cx = 82;
	cy = 85;
#else
	cx = 86;
	cy = 83;
#endif

	// Resize it and display it.
	hbmpComponent = ImageUtils::ResizeBitmap(hBmp, cx, cy);
	SendDlgItemMessage(*hwndDetail, IDC_PICOMP, STM_SETIMAGE, IMAGE_BITMAP,
		(LPARAM)hbmpComponent);
	ShowWindow(GetDlgItem(*hwndDetail, IDC_LBNOIMAGE), SW_HIDE);
	ShowWindow(GetDlgItem(*hwndDetail, IDC_PICOMP), SW_SHOW);
}

/**
 * Clears the component image from the view.
 */
void UIManager::ClearImage() {
	SendDlgItemMessage(*hwndDetail, IDC_PICOMP, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL);
	ShowWindow(GetDlgItem(*hwndDetail, IDC_PICOMP), SW_HIDE);
	DeleteObject(hbmpComponent);
	ShowWindow(GetDlgItem(*hwndDetail, IDC_LBNOIMAGE), SW_SHOW);
}

/**
 * Creates a new component property.
 *
 * @return 0 if everything worked.
 */
LRESULT UIManager::CreateProperty() {
	HWND hwndList = GetDlgItem(*hwndDetail, IDC_LSPROPS);

	// Create property editor.
	Property prop;
	Component *component = workspace->GetComponent(iSelComponent);
	PropertyEditor editor(*hInst, hwndMain, &prop);

	// Check if any changes were made to the property.
	if (editor.Updated()) {
		SendMessage(hwndList, LB_RESETCONTENT, 0, 0);

		if (!prop.IsEmpty()) {
			component->AddProperty(prop);
			PopulatePropertiesList(component);

			SetDirty(true);
		}
	}

	return 0;
}

/**
 * Edits the selected property in the list.
 *
 * @return 0 if everything worked.
 */
LRESULT UIManager::EditSelectedProperty() {
	HWND hwndList = GetDlgItem(*hwndDetail, IDC_LSPROPS);

	// Get selected item and its associated property index.
	int iSelected = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);
	size_t iProp = (size_t)SendMessage(hwndList, LB_GETITEMDATA, iSelected, 0);

	// Get property and create property editor.
	Component *component = workspace->GetComponent(iSelComponent);
	Property *prop = component->GetProperty(iProp);
	PropertyEditor editor(*hInst, hwndMain, prop);

	// Check if any changes were made to the property.
	if (editor.Updated()) {
		SendMessage(hwndList, LB_RESETCONTENT, 0, 0);
		SetDirty(true);

		if (!prop->IsEmpty()) 
			PopulatePropertiesList(component);
	}

	return 0;
}

/**
 * Deletes the selected property in the list.
 *
 * @return 0 if everything worked.
 */
LRESULT UIManager::DeleteSelectedProperty() {
	// Ask the user politely.
	if (MessageBox(*hwndMain, L"Are you sure you want to permanently delete this property?",
			L"Delete Property", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 |
			MB_APPLMODAL) != IDYES)
		return 1;

	// Get selected item and its associated property index.
	HWND hwndList = GetDlgItem(*hwndDetail, IDC_LSPROPS);
	int iSelected = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);
	size_t iProp = (size_t)SendMessage(hwndList, LB_GETITEMDATA, iSelected, 0);

	// Get component and remove the selected property.
	Component *component = workspace->GetComponent(iSelComponent);
	component->RemoveProperty(iProp);
	SetDirty(true);

	// Update the properties list.
	SendMessage(hwndList, LB_RESETCONTENT, 0, 0);
	PopulatePropertiesList(component);

	return 0;
}

/**
 * Shows the currently selected component datasheet.
 *
 * @param szSubTitle Application sub-title.
 */
LRESULT UIManager::ShowDatasheet() {
	SHELLEXECUTEINFO lpExecInfo = {0};

	// Get component.
	Component *component = workspace->GetComponent(iSelComponent);
	if (component == NULL) {
		MessageBox(*hwndMain, L"Couldn't retrieve the currently selected component.",
			L"Component Retrieval Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	// Get datasheet path and setup its opening.
	Path pathDatasheet = component->GetDirectory().Concatenate(DATASHEET_FILE);
	lpExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	lpExecInfo.lpFile = pathDatasheet.ToString();
	lpExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	lpExecInfo.lpVerb = L"open";
	lpExecInfo.nShow = SW_SHOWNORMAL;
	lpExecInfo.hwnd = NULL;

	// Open the file.
	ShellExecuteEx(&lpExecInfo);
	return 0;
}

/**
 * Initializes the main application image list.
 *
 * @param  hInstance Application instance.
 * @return           Image list handle.
 */
HIMAGELIST UIManager::InitializeImageList(HINSTANCE hInstance) {
	HIMAGELIST hIml;
	HBITMAP hBmp;

	// Create an Image List.
	hIml = ImageList_Create(16, 16, 0, 2, 0);

	// Load the folder image into the Image List.
	hBmp = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_FOLDER));
	ImageList_Add(hIml, hBmp, NULL);

	// Load the chip image into the Image List.
	hBmp = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_CHIP));
	ImageList_Add(hIml, hBmp, NULL);

	return hIml;
}

/**
 * Sets the application sub-title.
 *
 * @param szSubTitle Application sub-title.
 */
void UIManager::SetApplicationSubTitle(LPCTSTR szSubTitle) {
	WCHAR szAppTitle[MAX_PATH];

	// Load application title and append the sub-title if needed.
	LoadString(*hInst, IDS_APP_TITLE, szAppTitle, MAX_PATH);
	if (szSubTitle != NULL) {
		wcscat(szAppTitle, L" - ");
		wcscat(szAppTitle, szSubTitle);
	}

	SetWindowText(*hwndMain, szAppTitle);
}

/**
 * Process the TVN_SELCHANGED message for the component TreeView.
 *
 * @param  hWnd   Window handler.
 * @param  wMsg   Message type.
 * @param  wParam Message parameter.
 * @param  lParam Message parameter.
 * @return        0 if everything worked.
 */
LRESULT UIManager::TreeViewSelectionChanged(HWND hWnd, UINT wMsg,
											WPARAM wParam, LPARAM lParam) {
	TVITEM tvItem;
	NMTREEVIEW* pnmTreeView = (LPNMTREEVIEW)lParam;
	size_t nIndex;

	if (CheckForUnsavedChanges())
		return 1;

	// Get item information.
	tvItem.hItem = pnmTreeView->itemNew.hItem;
	tvItem.mask = TVIF_PARAM | TVIF_IMAGE;
	treeView->GetItem(&tvItem);

	// Check if the selected item is a component.
	if (tvItem.lParam == -1) {
		ClearDetailView(true);
		return 0;
	}

#ifdef SHELL_AYGSHELL
		// Create and show the detail view dialog.
		*hwndDetail = CreateDialog(*hInst, MAKEINTRESOURCE(IDD_DETAILPPC), *hwndMain,
			(DLGPROC)lpDetailProc);
		ShowWindow(*hwndDetail, SW_SHOW);
#endif
	
	// Get component index from parameter and show it in the detail view.
	nIndex = (size_t)tvItem.lParam;
	PopulateDetailView(nIndex);

	return 0;
}

/**
 * Populates the TreeView with components.
 */
void UIManager::PopulateTreeView() {
	size_t i, j;
	bool bHasUncategorized = false;

	// Clear the TreeView and get components.
	treeView->Clear();
	vector<Component> arrComponents = workspace->GetComponents();

	// Populate the categories.
	vector<Category> arrCategories;
	for (i = 0; i < arrComponents.size(); i++) {
		LPCTSTR szCategory = arrComponents[i].GetCategory();
		bool bUnique = true;

		// Check if it's an uncategorized component.
		if (szCategory == NULL) {
			bHasUncategorized = true;
			continue;
		}
		
		// Check if this category is new.
		for (j = 0; j < arrCategories.size(); j++) {
			if (arrCategories[j].Equals(szCategory)) {
				bUnique = false;
				break;
			}
		}

		// Looks like we have a new category.
		if (bUnique) {
			Category cat(szCategory);
			cat.PopulateSubCategories(arrComponents);

			arrCategories.push_back(cat);
		}
	}

	// Add the category nodes and populate them with components.
	for (i = 0; i < arrCategories.size(); i++) {
		// Add category to the TreeView.
		HTREEITEM nodeCategory = treeView->AddItem(NULL, arrCategories[i].GetName(),
			NULL, ILI_FOLDER, (LPARAM)-1);

		// Go through sub-categories and populate its components.
		vector<wstring> arrSubCategories = arrCategories[i].GetSubCategories();
		for (j = 0; j < arrSubCategories.size(); j++) {
			HTREEITEM nodeSubCategory = treeView->AddItem(nodeCategory,
				arrSubCategories[j].c_str(), NULL, ILI_FOLDER, (LPARAM)-1);

			// Go through components searching for the ones that belong here.
			for (size_t k = 0; k < arrComponents.size(); k++) {
				LPCTSTR szCategory = arrComponents[k].GetCategory();
				if (szCategory == NULL)
					continue;

				// This component belongs inside this category.
				if (arrCategories[i].Equals(szCategory)) {
					LPCTSTR szSubCategory = arrComponents[k].GetSubCategory();
					
					// This component doesn't have a sub-category.
					if (szSubCategory == NULL)
						continue;

					// Check if this component fits into this sub-category.
					if (arrSubCategories[j].compare(wstring(szSubCategory)) == 0)
						treeView->AddItem(nodeSubCategory, arrComponents[k].ToString(),
							NULL, ILI_CHIP, (LPARAM)k);
				}
			}

			// Expand the sub-category node.
			treeView->ExpandNode(nodeSubCategory);
		}

		// Search for components without a sub-category.
		if (arrCategories[i].HasComponentsWithNoSubCategory()) {
			for (j = 0; j < arrComponents.size(); j++) {
				LPCTSTR szCategory = arrComponents[j].GetCategory();
				if (szCategory == NULL)
					continue;

				// This component belongs inside this category.
				if (arrCategories[i].Equals(szCategory)) {
					LPCTSTR szSubCategory = arrComponents[j].GetSubCategory();

					if (szSubCategory == NULL)
						treeView->AddItem(nodeCategory, arrComponents[j].ToString(),
							NULL, ILI_CHIP, (LPARAM)j);
				}
			}
		}

		// Expand the category node.
		treeView->ExpandNode(nodeCategory);
	}

	// Add the uncategorized folder and its components.
	if (bHasUncategorized) {
		arrCategories.push_back(Category(L"Uncategorized"));
		HTREEITEM nodeCategory = treeView->AddItem(NULL, L"Uncategorized",
			NULL, ILI_FOLDER, (LPARAM)-1);

		// Go through components searching for uncategorized ones.
		for (j = 0; j < arrComponents.size(); j++) {
			LPCTSTR szCategory = arrComponents[j].GetCategory();
			if (szCategory == NULL)
				treeView->AddItem(nodeCategory, arrComponents[j].ToString(),
					NULL, ILI_CHIP, (LPARAM)j);
		}

		// Expand the node.
		treeView->ExpandNode(nodeCategory);
	}
}

/**
 * Shows the classic loading/waiting hourglass.
 */
void UIManager::ShowLoading() {
	SetCursor(LoadCursor(NULL, IDC_WAIT));
}

/**
 * Hides the classic loading/waiting hourglass.
 */
void UIManager::HideLoading() {
	SetCursor(NULL);
}

/**
 * Gets the text from a control on screen.
 * @remark This function allocates the string. Remember to free it later.
 *
 * @param  hwndControl Control to get the text from.
 * @param  szBuffer    Pointer to a string to store the text.
 * @return             TRUE if the operation was successful.
 */
bool UIManager::GetEditText(HWND hwndControl, LPTSTR *szBuffer) {
	int nLength;

	// Get the string length and allocate it.
	nLength = GetWindowTextLength(hwndControl) + 1;
	*szBuffer = (LPTSTR)LocalAlloc(LMEM_FIXED, nLength * sizeof(WCHAR));

	// Get the string from the control and return.
	return GetWindowText(hwndControl, *szBuffer, nLength) != 0;
}

/**
 * Set the dirtiness flag.
 *
 * @param bDirty Are we dirty?
 */
void UIManager::SetDirty(bool bDirty) {
	this->bDirty = bDirty;
}

/**
 * Checks if we are dirty.
 *
 * @return TRUE if we are dirty.
 */
bool UIManager::IsDirty() {
	return bDirty;
}
