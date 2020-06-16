/**
 * Directory.h
 * A helper class to abstract directories and perform operations.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef _DIRECTORY_H
#define _DIRECTORY_H

#include <windows.h>
#include "Path.h"

class Directory : public Path {
public:
	// Constructors and destructors.
	Directory() : Path() {};
	Directory(LPCTSTR szPath) : Path(szPath) {};

	// Information.
	UINT GetSubDirectories();

	// Misc.
	void SetPath(Path path);
};

#endif  // _DIRECTORY_H