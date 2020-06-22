/**
 * FileUtils.cpp
 * A bunch of utilities to deal with files.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "FileUtils.h"

// Character definitions.
#define CR '\r'
#define LF '\n'

/**
 * Reads a line from a file and returns it wituout the newline character.
 * Remark: This function will increment the file hanfle cursor.
 *
 * @param  hFile  File handle.
 * @param  swLine Pointer to the string that will receive the read line.
 * @return        TRUE if we read a line or FALSE if we reached the EOF.
 */
bool FileUtils::ReadLine(HANDLE hFile, wstring *swLine) {
	DWORD nBytesRead;
	BOOL bResult;
	char c;

	// Start fresh.
	swLine->clear();

	// Go through the file looking for a newline character.
	bResult = ReadFile(hFile, &c, 1, &nBytesRead, NULL);
	while (bResult && (nBytesRead != 0)) {
		// Found a newline character.
		if (c == LF)
			return true;

		// Append character to the string.
		if (c != CR)
			swLine->push_back(c);

		// Read the next character.
		bResult = ReadFile(hFile, &c, 1, &nBytesRead, NULL);
	}

	// Check if we have a file without a terminating newline.
	if (swLine->length() > 0)
		return true;

	return false;
}