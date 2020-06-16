/**
 * TreeView.cpp
 * A helper class to deal with TreeView components.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "TreeView.h"

/**
 * Creates an empty TreeView object.
 */
TreeView::TreeView() {
}

/**
 * Initializes the TreeView control and places it on its parent.
 *
 * @param hInst      Application interface handle.
 * @param hwndParent Parent window handle.
 * @param rcClient   Client rectagle to place the TreeView.
 */
TreeView::TreeView(HINSTANCE hInst, HWND hwndParent,
				   RECT rcClient, HMENU hTreeViewID) {
    // Ensure that the common control DLL is loaded. 
    InitCommonControls(); 
	
	// Create TreeView window.
	hWnd = CreateWindowEx(0, WC_TREEVIEW, L"Tree View",
		WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT, 
		rcClient.left, rcClient.top, rcClient.right, rcClient.bottom,
		hwndParent, hTreeViewID, hInst, NULL);
}

/**
 * Adds an item to the TreeView.
 *
 * @param  hParent   Parent TreeView item handle.
 * @param  szText    Item caption.
 * @param  hInsAfter Insert after this TreeView item handle.
 * @param  iImage    Index to the item icon in the ImageList.
 * @param  lParam    Item lParam.
 * @return           Added item handle.
 */
HTREEITEM TreeView::AddItem(HTREEITEM hParent, LPTSTR szText,
							HTREEITEM hInsAfter, int iImage,
							LPARAM lParam) {
	HTREEITEM hItem;
	TV_ITEM tvItem;
	TV_INSERTSTRUCT tvInsert;
	
	// Fill out the item structure.
	tvItem.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	tvItem.pszText = szText;
	tvItem.cchTextMax = wcslen(szText);
	tvItem.iImage = iImage;
	tvItem.iSelectedImage = iImage;
	tvItem.lParam = lParam;
	
	// Fill out the insert structure.
	tvInsert.item = tvItem;
	tvInsert.hInsertAfter = hInsAfter;
	tvInsert.hParent = hParent;
	
	// Insert the item into the tree.
	hItem = (HTREEITEM)SendMessage(hWnd, TVM_INSERTITEM, 0, 
		(LPARAM)(LPTV_INSERTSTRUCT)&tvInsert);
	
	return hItem;
}

/**
 * Gets a TreeView item.
 *
 * @param  tvItem TVITEM structure with the information to get.
 * @return        TRUE if the operation was successful.
 */
BOOL TreeView::GetItem(TVITEM *tvItem) {
	return TreeView_GetItem(hWnd, tvItem);
}

/**
 * Expands a node in the TreeView.
 *
 * @param  hNode Handle to the node to expand.
 * @param        TRUE if the operation was successful.
 */
BOOL TreeView::ExpandNode(HTREEITEM hNode) {
	return TreeView_Expand(hWnd, hNode, TVE_EXPAND);
}

/**
 * Clears the entire TreeView contents.
 *
 * @return TRUE if the operation was successful.
 */
BOOL TreeView::Clear() {
	return TreeView_DeleteAllItems(hWnd);
}

/**
 * Associates an ImageList with the control.
 *
 * @param hIml ImageList handler.
 */
void TreeView::SetImageList(HIMAGELIST hIml) {
	TreeView_SetImageList(hWnd, hIml, TVSIL_NORMAL);
}