/**
 * ImageUtils.cpp
 * A bunch of utilities to deal with images.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "ImageUtils.h"

/**
 * Loads a bitmap from a file.
 *
 * @param  szPath Path to the bitmap file.
 * @return        HBITMAP structure or NULL if an error occured.
 */
HBITMAP ImageUtils::LoadBitmap(LPCTSTR szPath) {
	return SHLoadDIBitmap(szPath);
}

/**
 * Resizes a bitmap.
 * @remark The returned bitmap must be freed using DeleteObject().
 *
 * @param  hbmpOriginal Original bitmap.
 * @param  nWidth       New width.
 * @param  nHeight      New height.
 * @return              Resized bitmap.
 */
HBITMAP ImageUtils::ResizeBitmap(HBITMAP hbmpOriginal, int nWidth, int nHeight) {
	// Get the original bitmap into a structure.
	BITMAP bmp = {0};
	GetObject(hbmpOriginal, sizeof(BITMAP), &bmp);

	// Get some device contexts for the operation.
	HDC hdcScreen = GetWindowDC(NULL);
	HDC hdcNewBmp = CreateCompatibleDC(hdcScreen);
	HDC hdcOldBmp = CreateCompatibleDC(hdcScreen);

	// Get some objects together for the new bitmap.
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight);
	HGDIOBJ hOldBmp = SelectObject(hdcNewBmp, hBitmap);
	
	// Perform the resize operation.
	SelectObject(hdcOldBmp, hbmpOriginal);
	StretchBlt(hdcNewBmp, 0, 0, nWidth, nHeight, hdcOldBmp, 0, 0, bmp.bmWidth,
		bmp.bmHeight, SRCCOPY);

	// Clean up.
	ReleaseDC(NULL, (HDC)hOldBmp);
	DeleteDC(hdcOldBmp);
	DeleteDC(hdcNewBmp);

	return hBitmap;
}