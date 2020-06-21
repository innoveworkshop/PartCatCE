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
#include <vector>

using namespace std;

class Directory : public Path {
protected:
	size_t GetSubDirectoriesCount();

public:
	// Constructors and destructors.
	Directory() : Path() {};
	Directory(LPCTSTR szPath) : Path(szPath) {};
	Directory(Path path);

	// Information.
	vector<Directory> GetSubDirectories();

	// Misc.
	void SetPath(Path path);
};

#endif  // _DIRECTORY_H