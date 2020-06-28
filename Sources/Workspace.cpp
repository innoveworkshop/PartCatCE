/**
 * Workspace.cpp
 * Manages the workspace stuff for the application and also offloads some of
 * the UI stuff.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "Workspace.h"
#include "FileUtils.h"

/**
 * Initializes an empty PartCat workspace.
 */
Workspace::Workspace() {
	bOpened = false;
}

/**
 * Initializes a PartCat workspace from a PartCat workspace file.
 *
 * @param pathWorkspace A PartCat workspace file.
 */
Workspace::Workspace(Path pathWorkspace) {
	Open(pathWorkspace);
}

/**
 * Initializes a PartCat workspace from a directory.
 *
 * @param dirWorkspace A PartCat workspace directory.
 */
Workspace::Workspace(Directory dirWorkspace) {
	Open(dirWorkspace.Concatenate(WORKSPACE_FILE));
}

/**
 * Populates the properties from the workspace file.
 */
void Workspace::PopulateProperties() {
	HANDLE hFile;
	wstring swLine;

	// Clear the properties array.
	arrProperties.clear();

	// Open the MANIFEST file for reading.
	hFile = CreateFile(dirWorkspace.Concatenate(WORKSPACE_FILE).ToString(),
		GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
		NULL);

	// Go through the file line by line.
	while (FileUtils::ReadLine(hFile, &swLine)) {
		AddProperty(Property(swLine));
	}

	// Close the file handle.
	CloseHandle(hFile);
}

/**
 * Adds a property to the workspace properties array.
 *
 * @param  property Property to be added.
 */
void Workspace::AddProperty(Property property) {
	arrProperties.push_back(property);
}

/**
 * Gets a component from the array using an index.
 *
 * @param  nIndex Index of the component in the array.
 * @return        Component if the index was valid or NULL if it doesn't exist.
 */
Component* Workspace::GetComponent(size_t nIndex) {
	if (nIndex >= arrComponents.size())
		return NULL;

	return &arrComponents[nIndex];
}

/**
 * Gets the components array.
 *
 * @return The array of components in this workspace.
 */
vector<Component> Workspace::GetComponents() {
	return arrComponents;
}

/**
 * Populates the components array.
 */
void Workspace::PopulateComponents() {
	Directory dirComponents(dirWorkspace.Concatenate(COMPONENTS_ROOT).ToString());
	vector<Directory> subDirs = dirComponents.GetSubDirectories();

	// Clear the components array.
	arrComponents.clear();

	// Populate components array.
	for (size_t i = 0; i < subDirs.size(); i++) {
		Directory dir = subDirs[i];
		arrComponents.push_back(Component(dir));
	}
}

/**
 * Opens a workspace.
 *
 * @param  pathWorkspace A PartCat workspace file.
 * @return               TRUE if the operation was successful.
 */
bool Workspace::Open(Path pathWorkspace) {
	this->dirWorkspace = Directory(pathWorkspace.Parent());
	PopulateComponents();
	bOpened = true;

	// TODO: Read the workspace manifest.

	return bOpened;
}

/**
 * Opens a workspace by its directory.
 *
 * @param  dirWorkspace A PartCat workspace directory.
 * @return              TRUE if the operation was successful.
 */
bool Workspace::Open(Directory dirWorkspace) {
	return Open(dirWorkspace.Concatenate(WORKSPACE_FILE));
}

/**
 * Refreshes the workspace.
 *
 * @return TRUE if the operation was successful.
 */
bool Workspace::Refresh() {
	Close();
	return Open(dirWorkspace);
}

/**
 * Closes the workspace.
 */
void Workspace::Close() {
	bOpened	= false;
	arrComponents.clear();
}

/**
 * Checks if the workspace is currently opened.
 *
 * @return TRUE if the workspace is opened.
 */
bool Workspace::IsOpened() {
	return bOpened;
}