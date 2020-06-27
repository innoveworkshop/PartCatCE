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

// PartCat MANIFEST property keys.
#define PROPERTY_CATEGORY    L"Category"
#define PROPERTY_SUBCATEGORY L"Sub-Category"
#define PROPERTY_VALUE       L"Value"
#define PROPERTY_PACKAGE     L"Package"

// File types.
#define IMAGE_EXTENSION L".bmp"

#endif  //  _CONSTANTS_H