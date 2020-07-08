/**
 * Directory.cpp
 * A helper class to abstract directories and perform operations.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "Directory.h"

/**
 * Initializes a directory from a Path.
 *
 * @param path Path to the directory.
 */
Directory::Directory(Path path) {
	SetPath(path);
}

/**
 * Deletes the directory and all of it's contents.
 *
 * @return TRUE if the operation was successful.
 */
bool Directory::DeleteRecursively() {
	HANDLE hFind;
	WIN32_FIND_DATA fndData;

	// Find the first file in the directory.
	hFind = FindFirstFile(this->Concatenate(L"\\*").ToString(), &fndData);

	// Go through the files in the directory removing them.
	while (hFind != INVALID_HANDLE_VALUE) {
		if (fndData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
			// Looks like we hit a directory. Recursion!
			if (!Directory(this->Concatenate(fndData.cFileName)).DeleteRecursively())
				return false;
		} else {
			if (!DeleteFile(this->Concatenate(fndData.cFileName).ToString()))
				return false;
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

	// Delete the final empty directory.
	return RemoveDirectory(this->ToString()) != 0;
}

/**
 * Retrieves the subdirectories of this directory.
 *
 * @return Pointer to an array of subdirectories. (MUST BE FREED LATER)
 */
vector<Directory> Directory::GetSubDirectories() {
	HANDLE hFind;
	WIN32_FIND_DATA fndData;

	// Initialize array with enough space for everything.
	vector<Directory> arr;

	// Find the first file in the directory.
	hFind = FindFirstFile(this->Concatenate(L"\\*").ToString(), &fndData);

	// Read directory contents.
	while (hFind != INVALID_HANDLE_VALUE) {
		// Looks like we hit a directory.
		if (fndData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
			// Push directory into the array.
			arr.push_back(Directory(this->Concatenate(fndData.cFileName)));
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
				return vector<Directory>();
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
