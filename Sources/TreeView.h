/**
 * TreeView.h
 * A helper class to deal with TreeView components.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef  _TREEVIEW_H
#define  _TREEVIEW_H

#include <windows.h>
#include <commctrl.h>

class TreeView {
private:
	HWND hWnd;

public:
	// Contructors and destructor.
	TreeView();
	TreeView(HINSTANCE hInst, HWND hwndParent,
			 RECT rcClient, HMENU hTreeViewID);

	// Node operations.
	HTREEITEM AddItem(HTREEITEM hParent, LPCTSTR szText,
					  HTREEITEM hInsAfter, int iImage,
					  LPARAM lParam);
	BOOL GetItem(TVITEM *tvItem);
	BOOL ExpandNode(HTREEITEM hNode);
	BOOL Clear();

	// Misc.
	void SetImageList(HIMAGELIST hIml);
};

#endif // _TREEVIEW_H
