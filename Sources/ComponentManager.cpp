/**
 * ComponentManager.cpp
 * Manages all of the components in the application and also offloads some of
 * the UI stuff.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

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
	// Clear the TreeView to receive our components.
	treeView->Clear();

	// Populate the components.
	Array<Component> arr = workspace->GetComponents();
	for (size_t i = 0; i < arr.Length(); i++) {
		treeView->AddItem(NULL, arr[i]->ToString(), NULL, 0, (LPARAM)i);
	}
}
