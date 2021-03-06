/**
 * PartCat.h
 * Electronic components cataloger and organizer application for Windows CE.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef  _PARTCAT_H
#define  _PARTCAT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Constants.h"
#include "resource.h"

// Limits.
#define MAX_LOADSTRING 100

// Control IDs.
#define IDC_CMDBAR   201
#define IDC_TREEVIEW 202

// CommandBar buttons.
#define IDC_BTNEWCOMP  211
#define IDC_BTSAVECOMP 212
#define IDC_BTDELCOMP  213
#define IDC_BTNEWWS    214
#define IDC_BTOPENWS   215
#define IDC_BTNEWPROP  216
#define IDC_BTEDITPROP 217
#define IDC_BTDELPROP  218

// Number of bitmaps in the standard and view image lists.
#define STD_BMPS_LEN  STD_PRINT + 1
#define VIEW_BMPS_LEN VIEW_NEWFOLDER + 1

// Instance operators.
int InitializeApplication(HINSTANCE hInstance);
HWND InitializeInstance(HINSTANCE hInstance, LPWSTR lpCmdLine, int nCmdShow);
int TerminateInstance(HINSTANCE hInstance, int nDefRC);

// Window procedure.
LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK AboutDlgProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam);
int CALLBACK DetailDlgProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam);

// Window message handlers.
LRESULT WndMainCreate(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam);
LRESULT WndMainCommand(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam);
LRESULT WndMainInitMenuPopUp(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam);
LRESULT WndMainNotify(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam);
LRESULT WndMainSettingChange(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam);
LRESULT WndMainActivate(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam);
LRESULT WndMainClose(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam);
LRESULT WndMainDestroy(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam);

#endif // _PARTCAT_H
