/**
 * Directory.cpp
 * A helper class to abstract directories and perform operations.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "Directory.h"

/**
 * Retrieves the subdirectories of this directory.
 *
 * @return An array of subdirectories.
 */
Array<Directory> Directory::GetSubDirectories() {
	HANDLE hFind;
	WIN32_FIND_DATA fndData;

	// Initialize array with enough space for everything.
	Array<Directory> arr(GetSubDirectoriesCount());

	// Find the first file in the directory.
	hFind = FindFirstFile(this->Concatenate(L"\\*").ToString(), &fndData);

	// Read directory contents.
	while (hFind != INVALID_HANDLE_VALUE) {
		// Looks like we hit a directory.
		if (fndData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
			// Push directory into the array.
			arr.Push(Directory(this->Concatenate(fndData.cFileName).ToString()));
		}

		// Continue to the next file.
		if (FindNextFile(hFind, &fndData) == 0) {
			if (GetLastError() == ERROR_NO_MORE_FILES) {
				// If there are no more files, close the handle and return.
				FindClose(hFind);
				hFind = INVALID_HANDLE_VALUE;
			} else {
				MessageBox(NULL, L"Error while listing directory", L"Error",
					MB_OK | MB_ICONERROR);
				return 0;
			}
		}
	}

	return arr;
}

/**
 * Gets the number of subdirectories inside this directory.
 *
 * @return Subdirectory count.
 */
size_t Directory::GetSubDirectoriesCount() {
	UINT count = 0;
	HANDLE hFind;
	WIN32_FIND_DATA fndData;

	// Find the first file in the directory.
	hFind = FindFirstFile(this->Concatenate(L"\\*").ToString(), &fndData);

	// Read directory contents.
	while (hFind != INVALID_HANDLE_VALUE) {
		// Looks like we hit a directory.
		if (fndData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
			// Count up.
			count++;
		}

		// Continue to the next file.
		if (FindNextFile(hFind, &fndData) == 0) {
			if (GetLastError() == ERROR_NO_MORE_FILES) {
				// If there are no more files, close the handle and return.
				FindClose(hFind);
				hFind = INVALID_HANDLE_VALUE;
			} else {
				MessageBox(NULL, L"Error while listing directory", L"Error",
					MB_OK | MB_ICONERROR);
				return 0;
			}
		}
	}

	return count;
}

/**
 * Sets the directory path.
 *
 * @param path Path object to the desired directory path.
 */
void Directory::SetPath(Path path) {
	wcscpy(szPath, path.ToString());
}
