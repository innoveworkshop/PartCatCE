/**
 * Directory.cpp
 * A helper class to abstract directories and perform operations.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "Directory.h"

/**
 * Retrieves the directory contents.
 */
void Directory::GetContents() {
	OutputDebugString(L"Hello World!\r\n");
	OutputDebugString(ToString());
	OutputDebugString(L"\r\n");
}