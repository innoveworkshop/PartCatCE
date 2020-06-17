/**
 * Workspace.cpp
 * Manages the workspace stuff for the application and also offloads some of
 * the UI stuff.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "Workspace.h"

/**
 * Initializes an empty PartCat workspace.
 */
Workspace::Workspace() {
}

/**
 * Initializes a PartCat workspace from a directory.
 *
 * @param dirWorkspace A PartCat workspace directory.
 */
Workspace::Workspace(Directory dirWorkspace) {
	this->dirWorkspace = Directory(dirWorkspace);
	PopulateComponents();
}

/**
 * Gets the components array.
 *
 * @return The array of components in this workspace.
 */
Array<Component> Workspace::GetComponents() {
	return arrComponents;
}

/**
 * Populates the components array.
 */
void Workspace::PopulateComponents() {
	Directory dirComponents(dirWorkspace.Concatenate(COMPONENTS_ROOT).ToString());
	Array<Directory> *subDirs = dirComponents.GetSubDirectories();

	// Populate components array.
	for (size_t i = 0; i < subDirs->Length(); i++) {
		Directory dir = *(*subDirs)[i];
		arrComponents.Push(Component(dir));
	}

	delete subDirs;
}