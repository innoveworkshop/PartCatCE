/**
 * FileUtils.cpp
 * A bunch of utilities to deal with files.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "FileUtils.h"
#include "StringUtils.h"

// Character definitions.
#define CR '\r'
#define LF '\n'

/**
 * Reads a line from a file and returns it wituout the newline character.
 * @remark This function will increment the file hanfle cursor.
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

/**
 * Slurps a file and stores its contents inside a buffer.
 * @remark Remember to free the contents buffer with LocalFree.
 *
 * @param  szPath         Path to the file to be read.
 * @param  szFileContents File contents buffer. Allocated globally by this
                          function.
 * @return                TRUE if the operation was successful.
 */
bool FileUtils::ReadContents(LPCTSTR szPath, LPTSTR *szFileContents) {
	DWORD dwFileSize;
	DWORD dwBytesRead;
	HANDLE hFile;
	bool bSuccess = true;
	char *szaBuffer;
	
	// Open the file.
	hFile = CreateFile(szPath, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		// TODO: Use GetLastError.
		MessageBox(NULL, L"Couldn't open file to read contents.",
			L"Read File Error", MB_OK | MB_ICONERROR);
		return false;
	}

	// Get file size to use to read the whole thing in one go.
	dwFileSize = GetFileSize(hFile, NULL);
	
	// Allocate the memory for the file contents.
	*szFileContents = (LPTSTR)LocalAlloc(LMEM_FIXED, (dwFileSize + 1) *
		sizeof(TCHAR));
	szaBuffer = (char*)LocalAlloc(LMEM_FIXED, (dwFileSize + 1) * sizeof(char));
	
	// Read the file into the buffer.
	if (!ReadFile(hFile, szaBuffer, dwFileSize, &dwBytesRead, NULL)) {
		// TODO: Use GetLastError.
		MessageBox(NULL, L"Failed to read the contents of the file.",
			L"Read File Error", MB_OK | MB_ICONERROR);
		bSuccess = false;
	}

	// Terminate the buffer and convert it.
	szaBuffer[dwBytesRead] = '\0';
	if (!StringUtils::AsciiToUnicode(*szFileContents, szaBuffer)) {
		MessageBox(NULL, L"Failed to convert file buffer from ASCII to Unicode",
			L"Conversion Failed", MB_OK | MB_ICONERROR);

		LocalFree(*szFileContents);
		bSuccess = false;
	}
    
	// Clean up.
	CloseHandle(hFile);
	LocalFree(szaBuffer);

	return bSuccess;
}

/**
 * Save contents to a file.
 *
 * @param  szFilePath Path to the file to be overwritten.
 * @param  szContents Contents to place inside the file.
 * @return            TRUE if the operation was successful.
 */
bool FileUtils::SaveContents(LPCTSTR szFilePath, LPCTSTR szContents) {
    HANDLE hFile;
	DWORD dwTextLength;
	DWORD dwBytesWritten;
	char *szaBuffer;

	// Get text length.
	dwTextLength = wcslen(szContents);

	// Open file for writing.
    hFile = CreateFile(szFilePath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
		// TODO: Use GetLastError.
		MessageBox(NULL, L"Couldn't open file to write contents.",
			L"Write File Error", MB_OK | MB_ICONERROR);
		return false;
	}

	// Convert text to ASCII before writing to the file.
	szaBuffer = (char*)LocalAlloc(LMEM_FIXED, (dwTextLength + 1) * sizeof(char));
	if (!StringUtils::UnicodeToAscii(szaBuffer, szContents)) {
		MessageBox(NULL, L"Failed to convert contents buffer from Unicode to "
			L"ASCII.", L"Conversion Failed", MB_OK | MB_ICONERROR);

		CloseHandle(hFile);
		LocalFree(szaBuffer);
		return false;
	}

	// Write to the file.
	if (!WriteFile(hFile, szaBuffer, dwTextLength, &dwBytesWritten, NULL)) {
		// TODO: Use GetLastError.
		MessageBox(NULL, L"Couldn't write contents to file.",
			L"Write File Error", MB_OK | MB_ICONERROR);
		return false;
	}
	
	// Clean up.
	CloseHandle(hFile);
	LocalFree(szaBuffer);

    return true;
}

/**
 * Checks if a file exists.
 *
 * @param  szPath Path to the file to check for existance.
 * @return        TRUE if the file exists.
 */
bool FileUtils::Exists(LPCTSTR szPath) {
	return GetFileAttributes(szPath) != 0xFFFFFFFF;
}