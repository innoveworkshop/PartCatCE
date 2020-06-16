/**
 * Path.cpp
 * A helper class to abstract file paths and perform operations.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "Path.h"
#include <string.h>

/**
 * Initializes an empty path.
 */
Path::Path() {
	szPath[0] = L'\0';
}

/**
 * Initializes a path with a pre-defined value.
 *
 * @param szPath String representation of the path.
 */
Path::Path(LPCTSTR szPath) {
	this->szPath[0] = L'\0';
	wcscpy(this->szPath, szPath);
}

/**
 * Appends a path to the end of this path. (This function will automatically
 * prepend a path separator to the child path)
 *
 * @param szChildPath String to be appended to this path.
 */
void Path::Concatenate(LPCTSTR szChildPath) {
	wcscat(szPath, L"\\");  // TODO: Optimize this.
	wcscat(szPath, szChildPath);
}

/**
 * Returns a string representation of the path.
 *
 * @return String representation of the path.
 */
LPCTSTR Path::ToString() {
	return szPath;
}
