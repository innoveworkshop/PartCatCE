/**
 * Property.cpp
 * Abstract class that represents a property in a component in PartCat.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "Property.h"
#include "Constants.h"

/**
 * Initializes an empty property.
 */
Property::Property() {
	ClearFields();
}

/**
 * Initializes a property and populate it based on a line.
 *
 * @param szLine Line from a manifest file.
 */
Property::Property(LPCTSTR szLine) {
	ClearFields();
	ParseLine(szLine);
}

/**
 * Initializes a property and populate it based on a line.
 *
 * @param swLine Line from a manifest file.
 */
Property::Property(wstring swLine) {
	ClearFields();
	ParseLine(swLine);
}

/**
 * Parses a line from a manifest file.
 *
 * @param  szLine Manifest file line.
 * @return        TRUE if the line was parsed sucessfully.
 */
bool Property::ParseLine(LPCTSTR szLine) {
	return ParseLine(wstring(szLine));
}

/**
 * Parses a line from a manifest file.
 *
 * @param  swLine Manifest file line.
 * @return        TRUE if the line was parsed sucessfully.
 */
bool Property::ParseLine(wstring swLine) {
	// Search for the separator.
	wstring::size_type pos = swLine.find_first_of(L":");
	if (pos == wstring::npos)
		return false;

	// Copy the name and value strings to the object.
	wcscpy(szName, swLine.substr(0, pos).c_str());
	wcscpy(szValue, swLine.substr(pos + 2).c_str());

	return true;
}

/**
 * Gets the name of the property.
 *
 * @return Property name.
 */
LPCTSTR Property::GetName() {
	return szName;
}

/**
 * Gets the name of the property in a much more human-readable format.
 * @remark The returned string should be freed by the user.
 *
 * @return Human-readable property name.
 */
LPTSTR Property::GetHumanName() {
	LPTSTR szName;

	// Allocate memory for the string and copy it.
	szName = (LPTSTR)LocalAlloc(LMEM_FIXED, (wcslen(this->szName) + 1) *
		sizeof(WCHAR));
	wcscpy(szName, this->szName);

	// Substitute the dashes for spaces.
	LPTSTR szBuffer = szName;
	for (; *szBuffer != L'\0'; szBuffer++) {
		if (*szBuffer == L'-')
			*szBuffer = L' ';
	}

	// Remove Value- part of the string.
	LPCTSTR szValueKey = L"Value ";
	size_t nKeyLen = wcslen(szValueKey);
	if (wcsncmp(szValueKey, szName, nKeyLen) == 0) {
		size_t i;
		for (i = nKeyLen; i < wcslen(szName); i++) {
			szName[i - nKeyLen] = szName[i];
		}

		szName[i - nKeyLen] = L'\0';
	}

	return szName;
}

/**
 * Sets the name of the property using a human-readable format.
 *
 * @param  szName Human-readable property name.
 */
void Property::SetHumanName(LPCTSTR szName) {
	wstring swBuffer(szName);

	// Firstly convert all spaces back into dashes.
	for (size_t i = 0; i < swBuffer.length(); i++) {
		if (swBuffer[i] == L' ')
			swBuffer[i] = L'-';
	}

	// Check for keys that should have Value- prepended.
	if ((swBuffer.compare(PROPERTY_CATEGORY) != 0) &&
		(swBuffer.compare(PROPERTY_SUBCATEGORY) != 0) &&
		(swBuffer.compare(PROPERTY_PACKAGE) != 0)) {
		swBuffer.insert(0, L"-");
		swBuffer.insert(0, PROPERTY_VALUE);
	}

	// Set the name.
	SetName(swBuffer.c_str());
}

/**
 * Sets the name of the property.
 *
 * @param  szName Property name.
 */
void Property::SetName(LPCTSTR szName) {
	wcscpy(this->szName, szName);
}

/**
 * Gets the property value.
 *
 * @return Value of the property.
 */
LPCTSTR Property::GetValue() {
	return szValue;
}

/**
 * Sets the property value.
 *
 * @param szValue Property value.
 */
void Property::SetValue(LPCTSTR szValue) {
	wcscpy(this->szValue, szValue);
}

/**
 * Clears all the fields in the object.
 */
void Property::ClearFields() {
	szName[0] = L'\0';
	szValue[0] = L'\0';
}

/**
 * Checks if the property is empty.
 *
 * @return TRUE if the property name is empty.
 */
bool Property::IsEmpty() {
	return wcslen(szName) == 0;
}

/**
 * Gets a string representation of the property.
 * @remark The string returned should be freed after being used.
 *
 * @return Property line as it should look in a manifest file.
 */
LPTSTR Property::ToString() {
	LPTSTR szName = GetHumanName();

	// Allocate the memory for the string.
	size_t nLen = wcslen(szName) + wcslen(szValue) + 3;
	LPTSTR szBuffer = (LPTSTR)LocalAlloc(LMEM_FIXED, nLen * sizeof(WCHAR));

	// Build the string.
	wcscpy(szBuffer, szName);
	wcscat(szBuffer, L": ");
	wcscat(szBuffer, szValue);

	LocalFree(szName);
	return szBuffer;
}
