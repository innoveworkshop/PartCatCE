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
	static bool ReadLine(HANDLE hFile, wstring *swLine);
};

#endif  // _FILE_UTILS_H