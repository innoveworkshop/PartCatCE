/**
 * ListView.cpp
 * A helper class to deal with ListView components.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "ListView.h"

/**
 * Creates an empty ListView object.
 */
ListView::ListView() {
}

/**
 * Initializes the ListView object with a control.
 *
 * @param hWnd A ListView handle.
 */
ListView::ListView(HWND *hWnd) {
    this->hWnd = hWnd;
}

/**
 * Appends a column to the ListView with a default width.
 *
 * @param  szCaption Caption of the column.
 * @return           TRUE if the operation was successful.
 */
bool ListView::AddColumn(LPCTSTR szCaption) {
	return AddColumn(szCaption, 100);
}

/**
 * Appends a column to the ListView.
 *
 * @param  szCaption Caption of the column.
 * @param  nWidth    Width of the column
 * @return           TRUE if the operation was successful.
 */
bool ListView::AddColumn(LPCTSTR szCaption, int nWidth) {
	LVCOLUMN lvc;

	// Set the column properties.
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt = LVCFMT_CENTER;
	lvc.pszText = (LPTSTR)szCaption;
	lvc.cx = nWidth;

	// Insert the column.
	return ListView_InsertColumn(*hWnd, nColumns++, &lvc) != -1;
}

/**
 * Clears the ListView rows.
 *
 * @return TRUE if the operation was successful.
 */
bool ListView::Clear() {
	return true;
}
