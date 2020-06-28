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
protected:
	WCHAR szPath[MAX_PATH];

	void AppendSeparator();

public:
	// Constructors and destructors.
	Path();
	Path(LPCTSTR szPath);

	// Checking.
	bool EndsWithSeparator();
	bool Exists();

	// Normal operations.
	void RemoveEndingSeparator();
	void AppendString(LPCTSTR szString);
	Path Concatenate(LPCTSTR szChildPath);
	Path Parent();

	// Destructive operations.
	bool Rename(LPCTSTR szNewName);

	// Misc.
	LPCTSTR FileName();
	LPCTSTR ToString();
};

#endif  // _PATH_H