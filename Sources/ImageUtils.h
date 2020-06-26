/**
 * ImageUtils.h
 * A bunch of utilities to deal with images.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef _IMAGE_UTILS_H
#define _IMAGE_UTILS_H

#include <windows.h>

class ImageUtils {
private:
	ImageUtils() {}

public:
	static HBITMAP LoadBitmap(LPCTSTR szPath);
	static HBITMAP ResizeBitmap(HBITMAP hbmpOriginal, int nWidth, int nHeight);
};

#endif  // _IMAGE_UTILS_H