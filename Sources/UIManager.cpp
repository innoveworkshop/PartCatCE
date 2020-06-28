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
#include "resource.h"
#include "commdlg.h"

/**
 * Initializes an empty component manager.
 */
UIManager::UIManager() {
}

/**
 * Initializes a component manager with a workspace directory and a TreeView
 * control to manage.
 *
 * @param hInst      Application instance handle.
 * @param hwndMain   Main window handle.
 * @param workspace  PartCat workspace .
 * @param treeView   TreeView control manager.
 * @param hwndDetail Detail dialog view handle.
 */
UIManager::UIManager(HINSTANCE *hInst, HWND *hwndMain, Workspace *workspace,
					 TreeView *treeView, HWND *hwndDetail) {
	this->workspace = workspace;
	this->treeView = treeView;
	this->hInst = hInst;
	this->hwndMain = hwndMain;
	this->hwndDetail = hwndDetail;

	ClearDetailView();
}

/**
 * Opens a workspace for the user.
 *
 * @return 0 if the operation was successful.
 */
LRESULT UIManager::OpenWorkspace() {
	ClearDetailView();
/*	OPENFILENAME ofn = {0};
    WCHAR szPath[MAX_PATH] = L"";

	// Populate the structure.
    ofn.lStructSize = sizeof(ofn);
	ofn.lpstrTitle = L"Open PartCat Workspace";
    ofn.hwndOwner = *hwndMain;
    ofn.lpstrFile = szPath;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER;

	// Open the open folder dialog.
	if (!GetOpenFileName(&ofn))
		return 1;

	// Try to open the workspace.
	if (!workspace->Open(Directory(szPath))) {
		MessageBox(*hwndMain, L"An error occured while trying to open the workspace.",
			L"Open Workspace Error", MB_OK | MB_ICONERROR);
		return 1;
	}
*/
	return 0;
}

/**
 * Refreshes the workspace for the user.
 *
 * @return 0 if the operation was successful.
 */
LRESULT UIManager::RefreshWorkspace() {
	ClearDetailView();

	if (!workspace->Refresh()) {
		MessageBox(*hwndMain, L"An error occured while refreshing the workspace.",
			L"Workspace Refresh Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	return 0;
}

/**
 * Closes the workspace for the user.
 *
 * @return 0 if the operation was successful.
 */
LRESULT UIManager::CloseWorkspace() {
	ClearDetailView();
	workspace->Close();
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
 * @param  component Component to sync.
 */
void UIManager::SyncDetailViewWithComponent(Component *component) {
	LPTSTR szBuffer;

	// Sync quantity.
	GetEditText(GetDlgItem(*hwndDetail, IDC_EDQUANTITY), &szBuffer);
	component->SetQuantity(szBuffer);
	LocalFree(szBuffer);

	// Sync notes.
	GetEditText(GetDlgItem(*hwndDetail, IDC_EDNOTES), &szBuffer);
	component->SaveNotes(szBuffer);
	LocalFree(szBuffer);

	// Sync properties.
	vector<Property> *arrProperties = component->GetEditableProperties();
	arrProperties->clear();
	int nCount = SendDlgItemMessage(*hwndDetail, IDC_LSPROPS, LB_GETCOUNT, 0, 0);
	for (int i = 0; i < nCount; i++) {
		// Get the size of the string and allocate memory for it.
		int nStrLen = SendDlgItemMessage(*hwndDetail, IDC_LSPROPS,
			LB_GETTEXTLEN, (WPARAM)i, 0);
		LPTSTR szLine = (LPTSTR)LocalAlloc(LMEM_FIXED,
			(nStrLen + 1) * sizeof(WCHAR));

		// Get the string and push it into the array.
		SendDlgItemMessage(*hwndDetail, IDC_LSPROPS, LB_GETTEXT, (WPARAM)i,
			(LPARAM)szLine);
		arrProperties->push_back(Property(szLine));
		LocalFree(szLine);
	}
}

/**
 * Saves the opened component object.
 *
 * @return 0 if the operation was successful.
 */
LRESULT UIManager::SaveComponent() {
	// Get component.
	Component *component = workspace->GetComponent(iSelComponent);
	if (component == NULL) {
		MessageBox(*hwndMain, L"Couldn't retrieve the currently selected component.",
			L"Component Retrieval Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	// Sync changes.
	SyncDetailViewWithComponent(component);
	if (!component->Save()) {
		MessageBox(*hwndMain, L"An error occured while trying to save the component.",
			L"Component Save Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	// Refresh the workspace.
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
	ClearDetailView();

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
 */
void UIManager::ClearDetailView() {
	// Clear edit boxes.
	SetDlgItemText(*hwndDetail, IDC_EDNAME, L"");
	SetDlgItemText(*hwndDetail, IDC_EDQUANTITY, L"");
	SetDlgItemText(*hwndDetail, IDC_EDNOTES, L"");

	// Clear the list box and image.
	SendDlgItemMessage(*hwndDetail, IDC_LSPROPS, LB_RESETCONTENT, 0, 0);
	ClearImage();

	iSelComponent = -1;
}

/**
 * Sets the component image label in the detail view.
 *
 * @param component Component to get the image for.
 */
void UIManager::SetComponentImage(Component *component) {
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

	// Resize it and display it.
	hbmpComponent = ImageUtils::ResizeBitmap(hBmp, 103, 83);
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
	HWND hwndList = GetDlgItem(*hwndDetail, IDC_LSPROPS);

	// Get selected item and its associated property index.
	int iSelected = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);
	size_t iProp = (size_t)SendMessage(hwndList, LB_GETITEMDATA, iSelected, 0);

	// Get component and remove the selected property.
	Component *component = workspace->GetComponent(iSelComponent);
	component->RemoveProperty(iProp);

	// Update the properties list.
	SendMessage(hwndList, LB_RESETCONTENT, 0, 0);
	PopulatePropertiesList(component);

	return 0;
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

	// Get item information.
	tvItem.hItem = pnmTreeView->itemNew.hItem;
	tvItem.mask = TVIF_PARAM | TVIF_IMAGE;
	treeView->GetItem(&tvItem);

	// Check if the selected item is a component.
	if (tvItem.lParam == -1) {
		ClearDetailView();
		return 0;
	}
	
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
			NULL, 0, (LPARAM)-1);

		// Go through sub-categories and populate its components.
		vector<wstring> arrSubCategories = arrCategories[i].GetSubCategories();
		for (j = 0; j < arrSubCategories.size(); j++) {
			HTREEITEM nodeSubCategory = treeView->AddItem(nodeCategory,
				arrSubCategories[j].c_str(), NULL, 0, (LPARAM)-1);

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
							NULL, 0, (LPARAM)k);
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
							NULL, 0, (LPARAM)j);
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
			NULL, 0, (LPARAM)-1);

		// Go through components searching for uncategorized ones.
		for (j = 0; j < arrComponents.size(); j++) {
			LPCTSTR szCategory = arrComponents[j].GetCategory();
			if (szCategory == NULL)
				treeView->AddItem(nodeCategory, arrComponents[j].ToString(),
					NULL, 0, (LPARAM)j);
		}

		// Expand the node.
		treeView->ExpandNode(nodeCategory);
	}
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
