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
	static void AllocStringCopy(LPTSTR *szDestination, LPCTSTR szSource);
};

#endif  // _STRING_UTILS_H