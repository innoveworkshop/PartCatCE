/**
 * ComponentManager.cpp
 * Manages all of the components in the application and also offloads some of
 * the UI stuff.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include <algorithm>
#include "ComponentManager.h"

/**
 * Initializes an empty component manager.
 */
ComponentManager::ComponentManager() {
}

/**
 * Initializes a component manager with a workspace directory and a TreeView
 * control to manage.
 *
 * @param workspace PartCat workspace .
 * @param treeView  TreeView control to manage.
 */
ComponentManager::ComponentManager(Workspace *workspace, TreeView *treeView) {
	this->workspace = workspace;
	this->treeView = treeView;
}

/**
 * Populates the TreeView with components.
 */
void ComponentManager::PopulateTreeView() {
	size_t i, j;
	bool bHasUncategorized = false;

	// Clear the TreeView and get components.
	treeView->Clear();
	vector<Component> arrComponents = workspace->GetComponents();

	// Populate the categories.
	vector<wstring> arrCategories;
	for (i = 0; i < arrComponents.size(); i++) {
		Property *prop = arrComponents[i].GetProperty(L"Category");
		bool bUnique = true;

		// Check if it's an uncategorized component.
		if (prop == NULL) {
			bHasUncategorized = true;
			continue;
		}
		
		// Check if this category is new.
		wstring swName(prop->GetValue());
		for (j = 0; j < arrCategories.size(); j++) {
			if (arrCategories[j].compare(swName) == 0) {
				bUnique = false;
				break;
			}
		}

		if (bUnique)
			arrCategories.push_back(swName);
	}

	// Add the category nodes and populate them with components.
	for (i = 0; i < arrCategories.size(); i++) {
		HTREEITEM nodeCategory = treeView->AddItem(NULL, arrCategories[i].c_str(),
			NULL, 0, (LPARAM)i);

		for (j = 0; j < arrComponents.size(); j++) {
			Property *prop = arrComponents[j].GetProperty(L"Category");
			if (prop == NULL)
				continue;

			// This component belongs inside this category.
			if (wcscmp(prop->GetValue(), arrCategories[i].c_str()) == 0)
				treeView->AddItem(nodeCategory, arrComponents[j].ToString(),
					NULL, 0, (LPARAM)j);
		}
	}

	// Add the uncategorized folder and its components.
	if (bHasUncategorized) {
		arrCategories.push_back(wstring(L"Uncategorized"));
		HTREEITEM nodeCategory = treeView->AddItem(NULL, L"Uncategorized",
			NULL, 0, (LPARAM)(arrCategories.size() - 1));

		for (j = 0; j < arrComponents.size(); j++) {
			Property *prop = arrComponents[j].GetProperty(L"Category");
			if (prop == NULL)
				treeView->AddItem(nodeCategory, arrComponents[j].ToString(),
					NULL, 0, (LPARAM)j);
		}
	}
	// Populate the components.
/*	for (size_t i = 0; i < arr.size(); i++) {
		Component comp = arr[i];
		treeView->AddItem(NULL, comp.ToString(), NULL, 0, (LPARAM)i);

		vector<Property> props = arr[i].GetProperties();
		for (size_t j = 0; j < props.size(); j++) {
			OutputDebugString(props[j].GetName());
			OutputDebugString(L"\r\n");
			OutputDebugString(props[j].GetValue());
			OutputDebugString(L"\r\n\r\n");
		}
	}
	*/
}
