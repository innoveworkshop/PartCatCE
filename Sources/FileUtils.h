/**
 * FileUtils.h
 * A bunch of utilities to deal with files.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef _FILE_UTILS_H
#define _FILE_UTILS_H

#include <windows.h>
#include <string>

using namespace std;

class FileUtils {
private:
	FileUtils() {}

public:
	// Reading and writing.
	static bool ReadLine(HANDLE hFile, wstring *swLine);
	static bool ReadContents(LPCTSTR szPath, LPTSTR *szFileContents);
	static bool SaveContents(LPCTSTR szFilePath, LPCTSTR szContents);

	// Existance.
	static bool Exists(LPCTSTR szPath);
};

#endif  // _FILE_UTILS_H