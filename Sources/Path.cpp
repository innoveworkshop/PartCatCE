/**
 * Path.cpp
 * A helper class to abstract file paths and perform operations.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "Path.h"
#include <string>
#include "FileUtils.h"

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
 * Renames the file/folder at the path.
 *
 * @param  szNewName New name for the file or folder.
 * @return           TRUE if the operation was successful.
 */
bool Path::Rename(LPCTSTR szNewName) {
	WCHAR szNewPath[MAX_PATH];
	LPTSTR szTemp = szNewPath;
	LPTSTR szLastPos = szNewPath;

	// Go through the path looking for the separators.
	wcscpy(szNewPath, szPath);
	for (; *szTemp != L'\0'; szTemp++) {
		if (*szTemp == L'\\')
			szLastPos = szTemp;
	}

	// Place the new name at the end of the path.
	*szLastPos = L'\0';
	wcscpy(szNewPath, Path(szNewPath).Concatenate(szNewName).ToString());

	bool bSuccess = MoveFile(szPath, szNewPath) != 0;
	if (bSuccess)
		wcscpy(szPath, szNewPath);

	return bSuccess;
}

/**
 * Appens a separator to the end of the path.
 */
void Path::AppendSeparator() {
	// Check if we already have a separator at the end.
	if (EndsWithSeparator())
		return;

	// Append separator and terminate the string.
	size_t iStrEnd = wcslen(szPath);
	szPath[iStrEnd++] = L'\\';
	szPath[iStrEnd] = L'\0';
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

	path.AppendSeparator();
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
	return FileUtils::Exists(szPath);
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
 * Removes a separator in the end of a path string in case there's one.
 */
void Path::RemoveEndingSeparator() {
	// Check if we have a separator at the end.
	if (!EndsWithSeparator())
		return;

	// Remove the separator from the string.
	szPath[wcslen(szPath) - 1] = L'\0';
}

/**
 * Checks if this path string ends with a separator.
 *
 * @return TRUE if the path string ends in a separator.
 */
bool Path::EndsWithSeparator() {
	return szPath[wcslen(szPath) - 1] == L'\\';
}

/**
 * Returns a string representation of the path.
 *
 * @return String representation of the path.
 */
LPCTSTR Path::ToString() {
	return szPath;
}
