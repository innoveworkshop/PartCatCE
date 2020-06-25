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
#include "resource.h"

/**
 * Initializes an empty component manager.
 */
UIManager::UIManager() {
}

/**
 * Initializes a component manager with a workspace directory and a TreeView
 * control to manage.
 *
 * @param hwndMain   Main window handle.
 * @param workspace  PartCat workspace .
 * @param treeView   TreeView control manager.
 * @param hwndDetail Detail dialog view handle.
 */
UIManager::UIManager(HWND *hwndMain, Workspace *workspace, TreeView *treeView,
					 HWND *hwndDetail) {
	this->workspace = workspace;
	this->treeView = treeView;
	this->hwndMain = hwndMain;
	this->hwndDetail = hwndDetail;
}

/**
 * Opens a workspace for the user.
 *
 * @return 0 if the operation was successful.
 */
LRESULT UIManager::OpenWorkspace() {
	return 0;
}

/**
 * Refreshes the workspace for the user.
 *
 * @return 0 if the operation was successful.
 */
LRESULT UIManager::RefreshWorkspace() {
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
	workspace->Close();
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

	// Set the name field.
	SetDlgItemText(*hwndDetail, IDC_EDNAME, component->GetName());

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
	vector<Property> arrProperties = component->GetProperties();
	for (size_t i = 0; i < arrProperties.size(); i++) {
		Property prop = arrProperties[i];
		
		// Build list caption string.
		wstring szCaption(prop.GetName());
		szCaption += L": ";
		szCaption += prop.GetValue();

		// Append the string to the list box.
		int pos = (int)SendDlgItemMessage(*hwndDetail, IDC_LSPROPS,
			LB_ADDSTRING, 0, (LPARAM)szCaption.c_str());
		SendDlgItemMessage(*hwndDetail, IDC_LSPROPS, LB_SETITEMDATA,
			(WPARAM)pos, (LPARAM)i);
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

	// Clear the list box.
	SendDlgItemMessage(*hwndDetail, IDC_LSPROPS, LB_RESETCONTENT, 0, 0);

	// TODO: Image.
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
