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
 * Saves the workspace file.
 *
 * @return TRUE if the operation was successful.
 */
bool Workspace::Save() {
	LPTSTR szBuffer;

	wstring swProperties;
	for (size_t i = 0; i < arrProperties.size(); i++) {
		szBuffer = arrProperties[i].ToString();

		swProperties += szBuffer;
		swProperties += L"\r\n";

		LocalFree(szBuffer);
	}

	return FileUtils::SaveContents(dirWorkspace.Concatenate(WORKSPACE_FILE).ToString(),
		swProperties.c_str());
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
 * Gets the workspace properties.
 *
 * @return Array of properties.
 */
vector<Property> Workspace::GetProperties() {
	return arrProperties;
}

/**
 * Gets an editable version of the workspace properties.
 *
 * @return Pointer to the array of properties.
 */
vector<Property>* Workspace::GetEditableProperties() {
	return &arrProperties;
}

/**
 * Gets a workspace property by its index.
 *
 * @param  nIndex Index of the property.
 * @return        The requested property or NULL if it wasn't found.
 */
Property* Workspace::GetProperty(size_t nIndex) {
	if (nIndex >= arrProperties.size())
		return NULL;

	return &arrProperties[nIndex];
}

/**
 * Gets a workspace property by its name.
 *
 * @param  szName Property name.
 * @return        The requested property or NULL if it wasn't found.
 */
Property* Workspace::GetProperty(LPCTSTR szName) {
	for (size_t i = 0; i < arrProperties.size(); i++) {
		Property prop = arrProperties[i];

		if (wcscmp(arrProperties[i].GetName(), szName) == 0)
			return &arrProperties[i];
	}

	return NULL;
}

/**
 * Gets the workspace name.
 *
 * @return Workspace name or NULL if there isn't one.
 */
LPCTSTR Workspace::GetName() {
	Property *property = GetProperty(PROPERTY_NAME);
	if (property)
		return property->GetValue();

	return NULL;
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
	PopulateProperties();

	bOpened = true;
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
 * Gets the workspace directory.
 *
 * @return Workspace directory root.
 */
Directory Workspace::GetDirectory() {
	return dirWorkspace;
}

/**
 * Checks if the workspace is currently opened.
 *
 * @return TRUE if the workspace is opened.
 */
bool Workspace::IsOpened() {
	return bOpened;
}