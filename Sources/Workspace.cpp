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
	this->dirWorkspace = dirWorkspace;
	PopulateComponents();
}

/**
 * Gets the components array.
 *
 * @return The array of components in this workspace.
 */
Array<Directory> Workspace::GetComponents() {
	return arrComponents;
}

/**
 * Populates the components array.
 */
void Workspace::PopulateComponents() {
	Directory dirComponents(dirWorkspace.Concatenate(COMPONENTS_ROOT).ToString());
	Array<Directory> subDirs = dirComponents.GetSubDirectories();

	// Initialize component array and populate it.
	arrComponents = Array<Directory>(subDirs.Length());
	for (size_t i = 0; i < subDirs.Length(); i++) {
		arrComponents.Push(*subDirs[i]);
	}
}