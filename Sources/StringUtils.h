/**
 * StringUtils.h
 * A bunch of utilities to deal with strings.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef _STRING_UTILS_H
#define _STRING_UTILS_H

#include <windows.h>

class StringUtils {
private:
	StringUtils() {}

public:
	static void AllocCopy(LPTSTR *szDestination, LPCTSTR szSource);
	static bool AsciiToUnicode(LPTSTR szUnicode, const char *szASCII);
	static bool UnicodeToAscii(char *szASCII, LPCTSTR szUnicode);
};

#endif  // _STRING_UTILS_H