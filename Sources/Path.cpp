/**
 * Path.cpp
 * A helper class to abstract file paths and perform operations.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "Path.h"
#include <string>

using namespace std;

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
	wcscpy(this->szPath, szPath);
}

/**
 * Appends a raw string to the end of the path.
 *
 * @param szString String to be appended to the end of the path.
 */
void Path::AppendString(LPCTSTR szString) {
	wcscat(szPath, szString);
}

/**
 * Appends a path to the end of this path. (This function will automatically
 * prepend a path separator to the child path)
 *
 * @param szChildPath String to be appended to this path.
 */
Path Path::Concatenate(LPCTSTR szChildPath) {
	Path path(szPath);

	path.AppendString(L"\\");  // TODO: Optimize this.
	path.AppendString(szChildPath);

	return path;
}

/**
 * Goes to the parent directory.
 *
 * @return Parent directory.
 */
Path Path::Parent() {
	wstring swPath(szPath);
	size_t iSeparator = swPath.find_last_of(L'\\', swPath.size() - 2);

	// Check if we are already at the root of the filesystem.
	if (iSeparator == wstring::npos) {
		return *this;
	} else if (iSeparator == 0) {
		iSeparator++;
	}

	swPath.erase(iSeparator);
	return Path(swPath.c_str());
}

/**
 * Checks if the path exists.
 *
 * @return TRUE if the path exists.
 */
bool Path::Exists() {
	return GetFileAttributes(szPath) != 0xFFFFFFFF;
}

/**
 * Gets just the last filename from the path.
 *
 * @return A pointer to the filename.
 */
LPCTSTR Path::FileName() {
	LPCTSTR szTemp = szPath;
	LPCTSTR szLastPos = szPath;

	// Go through the path looking for the separators.
	for (; *szTemp != L'\0'; szTemp++) {
		if (*szTemp == L'\\')
			szLastPos = szTemp;
	}

	// Check if we stopped at a separator.
	if (*szLastPos == L'\\')
		szLastPos++;

	return szLastPos;
}

/**
 * Returns a string representation of the path.
 *
 * @return String representation of the path.
 */
LPCTSTR Path::ToString() {
	return szPath;
}
