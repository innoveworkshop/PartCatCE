/**
 * Constants.h
 * A collection of constants used throughout the application.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef  _CONSTANTS_H
#define  _CONSTANTS_H

// Application development.
#ifdef DEVELOP
#define TEST_WORKSPACE L"\\PartCat"
#endif

// Limits.
#define MAX_LOADSTRING 100

// PartCat paths.
#define COMPONENTS_ROOT L"components"
#define ASSETS_ROOT     L"assets"
#define IMAGES_DIR      L"images"

// PartCat component files.
#define MANIFEST_FILE  L"MANIFEST"
#define QUANTITY_FILE  L"QUANTITY"
#define IMAGE_FILE     L"IMAGE"
#define DATASHEET_FILE L"datasheet.pdf"
#define NOTES_FILE     L"notes.txt"

#endif  //  _CONSTANTS_H