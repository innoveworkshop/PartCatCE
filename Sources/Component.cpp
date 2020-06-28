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
		AddProperty(Property(swLine));
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
 * Gets a property from the component by its index.
 *
 * @param  index Property index.
 * @return       A component property or NULL if the index is invalid.
 */
Property* Component::GetProperty(size_t index) {
	if (index >= arrProperties.size())
		return NULL;

	return &arrProperties[index];
}

/**
 * Gets a property from the component by its name.
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
 * Gets the component properties in editable form.
 *
 * @return Component properties.
 */
vector<Property>* Component::GetEditableProperties() {
	return &arrProperties;
}

/**
 * Adds a property to the component properties array.
 *
 * @param  property Property to be added.
 */
void Component::AddProperty(Property property) {
	arrProperties.push_back(property);
}

/**
 * Removes a property at a specified index from the properties array.
 *
 * @param index Index of the property to be removed.
 */
void Component::RemoveProperty(size_t index) {
	if ((index >= 0) && (index < arrProperties.size()))
		arrProperties.erase(arrProperties.begin() + index);
}

/**
 * Retrieves the component image path.
 * @remark The user should free the returned string using LocalFree.
 *
 * @return Path to the image or NULL if there isn't one associated with it.
 */
LPTSTR Component::GetImage() {
	Path pathImage = dirPath.Concatenate(IMAGE_FILE);

	// Image file is present.
	if (pathImage.Exists()) {
		// Get the image name and the associated image file.
		LPTSTR szImageName;
		if (!FileUtils::ReadContents(pathImage.ToString(), &szImageName))
			return NULL;
		pathImage = GetImageFilePath(szImageName);
		LocalFree(szImageName);

		// Check if the image bitmap exists.
		if (pathImage.Exists()) {
			LPTSTR szImagePath;
			
			// Allocate memory for the string.
			size_t nLen = wcslen(pathImage.ToString()) + 1;
			szImagePath = (LPTSTR)LocalAlloc(LMEM_FIXED, nLen * sizeof(WCHAR));

			// Copy the string over and return.
			wcscpy(szImagePath, pathImage.ToString());
			return szImagePath;
		}

		return NULL;
	}

	// Check if the component has a Package property for an image.
	Property *prop = GetProperty(PROPERTY_PACKAGE);
	if (prop) {
		pathImage = GetImageFilePath(prop->GetValue());

		// Check if the image bitmap exists.
		if (pathImage.Exists()) {
			LPTSTR szImagePath;
			
			// Allocate memory for the string.
			size_t nLen = wcslen(pathImage.ToString()) + 1;
			szImagePath = (LPTSTR)LocalAlloc(LMEM_FIXED, nLen * sizeof(WCHAR));

			// Copy the string over and return.
			wcscpy(szImagePath, pathImage.ToString());
			return szImagePath;
		}
	}

	return NULL;
}

/**
 * Gets an image file path according to the desired image name for this
 * component.
 * @remark This function doesn't check for the existance of the image,
 *         it just builds the path to it.
 *
 * @param  szImageName Name of the image to create a path for.
 * @return             Path to the image file.
 */
Path Component::GetImageFilePath(LPCTSTR szImageName) {
	Path pathImage;

	// Navigate to the images folder and append the image file.
	pathImage = dirPath.Parent().Parent();
	pathImage = pathImage.Concatenate(ASSETS_ROOT).Concatenate(IMAGES_DIR);
	pathImage = pathImage.Concatenate(szImageName);
	pathImage.AppendString(IMAGE_EXTENSION);

	return pathImage;
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

	OutputDebugString(L"Properties:\r\n");
	for (size_t i = 0; i < arrProperties.size(); i++) {
		Property prop = arrProperties[i];
		LPTSTR szBuffer = prop.GetHumanName();

		OutputDebugString(L"    ");
		OutputDebugString(prop.GetName());
		OutputDebugString(L" (");
		OutputDebugString(szBuffer);
		OutputDebugString(L"): ");
		OutputDebugString(prop.GetValue());
		OutputDebugString(L"\r\n");

		LocalFree(szBuffer);
	}
	OutputDebugString(L"\r\n");


	LocalFree(szQuantity);
	LocalFree(szNotes);
}
