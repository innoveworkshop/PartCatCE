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
 * @return Number of subdirectories found.
 */
UINT Directory::GetSubDirectories() {
	UINT count = 0;
	HANDLE hFind;
	WIN32_FIND_DATA fndData;

	// Array test.
	Array<Directory> arr;

	// Allocate space for our list if needed.
/*	if (list != NULL) {
		if (list->size == 0) {
			err = n_list_directory_files(0, NULL, path, recursive);
			if (err < 0)
				return err;

			// Allocate listing array.
			list->size = err;
			list->list = (char**)malloc(err * sizeof(char*));
		}
	}
*/
	// Find the first file in the directory.
	hFind = FindFirstFile(this->Concatenate(L"\\*").ToString(), &fndData);

	// Read directory contents.
	while (hFind != INVALID_HANDLE_VALUE) {
		// Looks like we hit a directory.
		if (fndData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
			//OutputDebugString(fndData.cFileName);
			//OutputDebugString(L"\r\n");
			Directory dir(fndData.cFileName);
			arr.Push(dir);

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

	// Print array contents.
	for (size_t i = 0; i < arr.Length(); i++) {
		OutputDebugString(arr.Get(i)->ToString());
		OutputDebugString(L"\r\n");
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
