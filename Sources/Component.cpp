/**
 * Component.cpp
 * Abstract class that represents a component in PartCat.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include <windows.h>
#include <string>

#include "Constants.h"
#include "Component.h"
#include "FileUtils.h"

using namespace std;

/**
 * Initializes an empty component.
 */
Component::Component() {
	ClearFields();
}

/**
 * Initializes a component and populate it based on its path.
 *
 * @param dirPath Path to the component folder.
 */
Component::Component(Directory dirPath) {
	this->dirPath = Directory(dirPath);
	PopulateFromDirectory();
}

/**
 * Populates the properties from the MANIFEST file.
 */
void Component::PopulateProperties() {
	HANDLE hFile;
	wstring swLine;

	// Clear the properties array.
	arrProperties.clear();

	// Open the MANIFEST file for reading.
	hFile = CreateFile(dirPath.Concatenate(MANIFEST_FILE).ToString(),
		GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
		NULL);

	// Go through the file line by line.
	while (FileUtils::ReadLine(hFile, &swLine)) {
		arrProperties.push_back(Property(swLine));
	}

	// Close the file handle.
	CloseHandle(hFile);
}

/**
 * Populates this component with data from its directory.
 */
void Component::PopulateFromDirectory() {
	SetName(dirPath.FileName());
	PopulateProperties();
}

/**
 * Gets the name of the component.
 *
 * @return Component name.
 */
LPCTSTR Component::GetName() {
	return szName;
}

/**
 * Sets the name of the component.
 *
 * @param  szName Component name.
 * @return        TRUE if the name was valid and was set.
 */
bool Component::SetName(LPCTSTR szName) {
	wcscpy(this->szName, szName);
	return true;
}

/**
 * Get the component notes.
 *
 * @return Notes about the component.
 */
LPCTSTR Component::GetNotes() {
	// TODO: Read from file.
	return L"";
}

/**
 * Sets the component notes.
 *
 * @param szNotes Component notes.
 */
void Component::SetNotes(LPCTSTR szNotes) {
	// TODO: Write to file.
}

/**
 * Gets the quantity of components.
 *
 * @return Number of components available.
 */
size_t Component::GetQuantity() {
	return nQuantity;
}

/**
 * Sets the amount of componets available.
 *
 * @param nQuantity Quantity of components.
 */
void Component::SetQuantity(size_t nQuantity) {
	this->nQuantity = nQuantity;
}

/**
 * Gets the component category.
 *
 * @return Category name or NULL if it's uncategorized.
 */
LPCTSTR Component::GetCategory() {
	Property *prop = GetProperty(PROPERTY_CATEGORY);

	if (prop)
		return prop->GetValue();

	return NULL;
}

/**
 * Gets the component sub-category.
 *
 * @return Sub-category name or NULL if it doesn't have one.
 */
LPCTSTR Component::GetSubCategory() {
	Property *prop = GetProperty(PROPERTY_SUBCATEGORY);

	if (prop)
		return prop->GetValue();

	return NULL;
}

/**
 * Gets a property from the component.
 *
 * @param  szName Name of the property.
 * @return        The requested property or NULL if it wasn't found.
 */
Property* Component::GetProperty(LPCTSTR szName) {
	for (size_t i = 0; i < arrProperties.size(); i++) {
		if (wcscmp(arrProperties[i].GetName(), szName) == 0)
			return &arrProperties[i];
	}

	return NULL;
}

/**
 * Gets the component properties.
 *
 * @return Component properties.
 */
vector<Property> Component::GetProperties() {
	return arrProperties;
}

/**
 * Clears all the fields in the object.
 */
void Component::ClearFields() {
	szName[0] = L'\0';
	nQuantity = 0;
	arrProperties.clear();
}

/**
 * Gets a string representation of the component.
 *
 * @return Component name.
 */
LPCTSTR Component::ToString() {
	return szName;
}
