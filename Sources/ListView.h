/**
 * ListView.h
 * A helper class to deal with ListView components.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef  _LISTVIEW_H
#define  _LISTVIEW_H

#include <windows.h>
#include <commctrl.h>
#include <vector>
#include <string>

using namespace std;

class ListView {
private:
	HWND *hWnd;
	int nColumns;
	int nRows;

public:
	// Contructors and destructor.
	ListView();
	ListView(HWND *hWnd);

	// Column operations.
	bool AddColumn(LPCTSTR szCaption, int nWidth);
	bool AddColumn(LPCTSTR szCaption);

	// Row operations.
	bool AddRow(vector<wstring> arrValues, LPARAM lParam);
	bool AddRow(vector<wstring> arrValues);
	bool Clear();
};

#endif // _LISTVIEW_H
