/**
 * StringUtils.cpp
 * A bunch of utilities to deal with strings.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "StringUtils.h"

/**
 * Allocates space for a string and copies it to a destination.
 *
 * @param szDestination Destination string.
 * @param szSource      Source string.
 */
void StringUtils::AllocCopy(LPTSTR *szDestination, LPCTSTR szSource) {
	// Free the destination string first if it is allocated already.
	if (szDestination)
		LocalFree(szDestination);

	size_t len = wcslen(szSource);
	*szDestination = (LPTSTR)LocalAlloc(LMEM_FIXED, (len + 1) * sizeof(WCHAR));
	wcscpy(*szDestination, szSource);
}

/**
 * Converts a regular ASCII string into a Unicode string.
 *
 * @param  szUnicode Pre-allocated Unicode string.
 * @param  szASCII   Original ASCII string.
 * @return           TRUE if the conversion was successful.
 */
bool StringUtils::AsciiToUnicode(LPTSTR szUnicode, const char *szASCII) {
	size_t nLen = strlen(szASCII) + 1;

    if(MultiByteToWideChar(CP_ACP, 0, szASCII, -1, szUnicode, nLen) == 0) {
		return false;
    }
    
    return true;
}

/**
 * Converts a Unicode string into a regular ASCII string.
 *
 * @param  szASCII   Pre-allocated ASCII string.
 * @param  szUnicode Original Unicode string.
 * @return           TRUE if the conversion was successful.
 */
bool StringUtils::UnicodeToAscii(char *szASCII, LPCTSTR szUnicode) {
	size_t nLen = wcslen(szUnicode) + 1;

    if(WideCharToMultiByte(CP_ACP, 0, szUnicode, -1, szASCII, nLen,
		NULL, NULL) == 0) {
		DWORD err = GetLastError();
		return FALSE;
    }

    return TRUE;
}