/**
 * ComponentManager.cpp
 * Manages all of the components in the application and also offloads some of
 * the UI stuff.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include <algorithm>
#include "ComponentManager.h"
#include "Category.h"

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

		if (bUnique)
			arrCategories.push_back(Category(szCategory));
	}

	// Add the category nodes and populate them with components.
	for (i = 0; i < arrCategories.size(); i++) {
		HTREEITEM nodeCategory = treeView->AddItem(NULL, arrCategories[i].GetName(),
			NULL, 0, (LPARAM)i);

		// Go through components searching for the ones that belong here.
		for (j = 0; j < arrComponents.size(); j++) {
			LPCTSTR szCategory = arrComponents[j].GetCategory();
			if (szCategory == NULL)
				continue;

			// This component belongs inside this category.
			if (arrCategories[i].Equals(szCategory))
				treeView->AddItem(nodeCategory, arrComponents[j].ToString(),
					NULL, 0, (LPARAM)j);
		}

		// Expand the node.
		treeView->ExpandNode(nodeCategory);
	}

	// Add the uncategorized folder and its components.
	if (bHasUncategorized) {
		arrCategories.push_back(Category(L"Uncategorized"));
		HTREEITEM nodeCategory = treeView->AddItem(NULL, L"Uncategorized",
			NULL, 0, (LPARAM)(arrCategories.size() - 1));

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
