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
	// Populate the name and properties.
	SetName(dirPath.FileName());
	PopulateProperties();

	// Populate the quantity.
	LPTSTR szQuantity;
	if (FileUtils::ReadContents(dirPath.Concatenate(QUANTITY_FILE).ToString(), &szQuantity)) {
		nQuantity = _wtol(szQuantity);
		LocalFree(szQuantity);
	}
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
 * @remark Remember to free the string using LocalFree.
 *
 * @return Notes about the component or NULL if none were found.
 */
LPTSTR Component::GetNotes() {
	LPTSTR szNotes;
	
	// Read the contents of the notes file.
	if (!FileUtils::ReadContents(dirPath.Concatenate(NOTES_FILE).ToString(), &szNotes))
		return NULL;

	return szNotes;
}

/**
 * Set the component notes.
 *
 * @param  szNotes Component notes.
 * @return         TRUE if the operation was successful.
 */
bool Component::SaveNotes(LPCTSTR szNotes) {
	return FileUtils::SaveContents(dirPath.Concatenate(NOTES_FILE).ToString(), szNotes);
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
 * Gets the quantity of components as a string.
 * @remark Remember to free this string with LocalFree.
 *
 * @return Number of components available as a string.
 */
LPTSTR Component::GetQuantityString() {
	LPTSTR szQuantity = (LPTSTR)LocalAlloc(LMEM_FIXED, 33 * sizeof(WCHAR));
	_ltow(nQuantity, szQuantity, 10);

	return szQuantity;
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
 * Sets the amount of componets available as a string.
 *
 * @param szQuantity Quantity of components as a string.
 */
void Component::SetQuantity(LPCTSTR szQuantity) {
	SetQuantity((size_t)_wtol(szQuantity));
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

/**
 * Prints some debug information about the component.
 */
void Component::PrintDebug() {
	LPTSTR szQuantity = GetQuantityString();
	LPTSTR szNotes = GetNotes();

	OutputDebugString(L"Name: ");
	OutputDebugString(szName);
	OutputDebugString(L"\r\n");
	OutputDebugString(L"Quantity: ");
	OutputDebugString(szQuantity);
	OutputDebugString(L"\r\n");
	OutputDebugString(L"Notes: \"");
	OutputDebugString(szNotes);
	OutputDebugString(L"\"\r\n");

	OutputDebugString(L"Properties:");
	for (size_t i = 0; i < arrProperties.size(); i++) {
		Property prop = arrProperties[i];

		OutputDebugString(L"    ");
		OutputDebugString(prop.GetName());
		OutputDebugString(L": ");
		OutputDebugString(prop.GetValue());
		OutputDebugString(L"\r\n");
	}
	OutputDebugString(L"\r\n");


	LocalFree(szQuantity);
	LocalFree(szNotes);
}
