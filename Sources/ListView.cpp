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
	nColumns = 0;
	nRows = 0;
}

/**
 * Initializes the ListView object with a control.
 *
 * @param hWnd A ListView handle.
 */
ListView::ListView(HWND *hWnd) {
    SetListView(hWnd);
	nColumns = 0;
	nRows = 0;
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
	memset(&lvc, 0, sizeof(lvc));

	// Set the column properties.
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_CENTER;
	lvc.pszText = (LPTSTR)szCaption;
	lvc.cx = nWidth;
	lvc.iSubItem = nColumns;

	// Insert the column.
	return ListView_InsertColumn(*hWnd, nColumns++, &lvc) != -1;
}

/**
 * Adds a row to the ListView.
 *
 * @param  arrValues Array of values for the row.
 * @return           TRUE if the operation was succesful.
 */
bool ListView::AddRow(vector<wstring> arrValues) {
	return AddRow(arrValues, -1);
}

/**
 * Adds a row to the ListView with an specific LPARAM.
 *
 * @param  arrValues Array of values for the row.
 * @param  lParam    LPARAM for this row item.
 * @return           TRUE if the operation was succesful.
 */
bool ListView::AddRow(vector<wstring> arrValues, LPARAM lParam) {
	// Set the common item parameters.
	LVITEM lvi;
	memset(&lvi, 0, sizeof(lvi));
	lvi.mask = LVIF_TEXT | LVIF_PARAM;
	lvi.pszText = (LPTSTR)arrValues[0].c_str();
	lvi.iSubItem = 0;
	lvi.iItem = nRows;
	lvi.lParam = lParam;

	// Insert the item row into the ListView.
	if (ListView_InsertItem(*hWnd, &lvi) == -1) {
		MessageBox(NULL, L"Failed to insert a row into the properties list.",
			L"Properties List Error", MB_OK | MB_ICONERROR);
		return false;
	}

	// Go through the array of values.
	for (size_t i = 1; i < arrValues.size(); i++) {
		// Set the cell text in the row.
		ListView_SetItemText(*hWnd, nRows, i, (LPTSTR)arrValues[i].c_str());
	}

	// Increment row count and return.
	nRows++;
	return true;
}

/**
 * Adds a key-value pair row to the ListView.
 *
 * @param  szKey   Key name of the pair.
 * @param  szValue Value of the pair.
 * @return         TRUE if the operation was succesful.
 */
bool ListView::AddRowKeyValue(LPCTSTR szKey, LPCTSTR szValue) {
	return AddRowKeyValue(szKey, szValue, -1);
}

/**
 * Adds a key-value pair row to the ListView with an specific LPARAM.
 *
 * @param  szKey   Key name of the pair.
 * @param  szValue Value of the pair.
 * @param  lParam  LPARAM for this row item.
 * @return         TRUE if the operation was succesful.
 */
bool ListView::AddRowKeyValue(LPCTSTR szKey, LPCTSTR szValue, LPARAM lParam) {
	// Set the common item parameters.
	LVITEM lvi;
	memset(&lvi, 0, sizeof(lvi));
	lvi.mask = LVIF_TEXT | LVIF_PARAM;
	lvi.pszText = (LPTSTR)szKey;
	lvi.iSubItem = 0;
	lvi.iItem = nRows;
	lvi.lParam = lParam;

	// Insert the item row into the ListView.
	if (ListView_InsertItem(*hWnd, &lvi) == -1) {
		MessageBox(NULL, L"Failed to insert a row into the properties list.",
			L"Properties List Error", MB_OK | MB_ICONERROR);
		return false;
	}

	// Set the value cell in the row.
	ListView_SetItemText(*hWnd, nRows, 1, (LPTSTR)szValue);

	// Increment row count and return.
	nRows++;
	return true;
}

/**
 * Clears the ListView rows.
 */
void ListView::Clear() {
	ListView_DeleteAllItems(*hWnd);
	nRows = 0;
}

/**
 * Sets the ListView window handle.
 *
 * @param listView ListView handle.
 */
void ListView::SetListView(HWND *listView) {
	this->hWnd = listView;
}
