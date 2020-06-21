/**
 * Component.cpp
 * Abstract class that represents a component in PartCat.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "Component.h"
#include "StringUtils.h"

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
	arrProperties.clear();

	// TODO: Open file and populate it.
	arrProperties.push_back(Property(L"Category: Test"));
	arrProperties.push_back(Property(L"Sub-Category: Sub Test"));
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
BOOL Component::SetName(LPCTSTR szName) {
	wcscpy(this->szName, szName);
	return TRUE;
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
LPTSTR Component::ToString() {
	return szName;
}
