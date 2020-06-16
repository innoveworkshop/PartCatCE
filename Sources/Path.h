/**
 * Path.h
 * A helper class to abstract file paths and perform operations.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef _PATH_H
#define _PATH_H

#include <windows.h>

class Path {
private:
	WCHAR szPath[MAX_PATH];

public:
	// Constructors and destructors.
	Path();
	Path(LPCTSTR szPath);

	// Operations.
	void Concatenate(LPCTSTR szChildPath);

	// Misc.
	LPCTSTR ToString();
};

#endif  // _PATH_H