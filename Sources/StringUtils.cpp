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
void StringUtils::AllocStringCopy(LPTSTR *szDestination, LPCTSTR szSource) {
	// Free the destination string first if it is allocated already.
	if (szDestination)
		LocalFree(szDestination);

	size_t len = wcslen(szSource);
	*szDestination = (LPTSTR)LocalAlloc(LMEM_FIXED, (len + 1) * sizeof(WCHAR));
	wcscpy(*szDestination, szSource);
}

