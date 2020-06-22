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
