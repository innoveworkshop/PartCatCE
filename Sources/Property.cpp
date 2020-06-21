/**
 * Property.cpp
 * Abstract class that represents a property in a component in PartCat.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "Property.h"

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
	wcscpy(szName, szLine);
	wcscpy(szValue, szLine);
	//ParseLine(szLine);
}

/**
 * Parses a line from a manifest file.
 *
 * @param szLine Manifest file line.
 */
void Property::ParseLine(LPCTSTR szLine) {
	LPCTSTR szTemp = szLine;
	LPCTSTR szValue = szLine;

	// Go through the line looking for the separator.
	for (; *szTemp != L'\0'; szTemp++) {
		if (*szTemp == L':') {
			szValue = szTemp + 1;
			break;
		}
	}

	// Check if we have a space after the separator.
	if (*szValue == L' ')
		szValue++;

	// Copy strings.
	wcsncpy(szName, szLine, szTemp - szLine);
	wcscpy(this->szValue, szValue);
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
 * Gets a string representation of the property.
 * WARNING: The string returned should be freed after being used.
 *
 * @return Property line as it should look in a manifest file.
 */
LPTSTR Property::ToString() {
	// Allocate the memory for the string.
	size_t nLen = wcslen(szName) + wcslen(szValue) + 3;
	LPTSTR szBuffer = (LPTSTR)LocalAlloc(LMEM_FIXED, nLen * sizeof(WCHAR));

	// Build the string.
	wcscpy(szBuffer, szName);
	wcscat(szBuffer, L": ");
	wcscat(szBuffer, szValue);

	return szBuffer;
}
