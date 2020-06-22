/**
 * Category.cpp
 * Abstract class that represents a component category in PartCat.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#include "Category.h"

/**
 * Initializes an empty component category.
 */
Category::Category() {
}

/**
 * Initializes a component category with a name.
 *
 * @param szName Category name.
 */
Category::Category(LPCTSTR szName) {
	wcscpy(this->szName, szName);
}

/**
 * Initializes a component category and populates it with sub-categories.
 *
 * @param szName        Category name.
 * @param arrComponents Array of components to search for sub-categories.
 */
Category::Category(LPCTSTR szName, vector<Component> *arrComponents) {
	wcscpy(this->szName, szName);
	PopulateSubCategories(arrComponents);
}

/**
 * Populates the category sub-categories from a list of components.
 *
 * @param arrComponents Array of components to search for sub-categories.
 */
void Category::PopulateSubCategories(vector<Component> *arrComponents) {
}

/**
 * Checks if this category has the same name as another.
 *
 * @param category Category to be tested.
 */
bool Category::Equals(Category category) {
	return wcscmp(szName, category.GetName()) == 0;
}

/**
 * Checks if this category is the same as the name provided.
 *
 * @param szName Name to be checked.
 */
bool Category::Equals(LPCTSTR szName) {
	return wcscmp(this->szName, szName) == 0;
}

/**
 * Checks if this category is the same as the name provided.
 *
 * @param szName Name to be checked.
 */
bool Category::Equals(wstring swName) {
	return wcscmp(szName, swName.c_str()) == 0;
}

/**
 * Gets the name of the category.
 *
 * @return Category name.
 */
LPCTSTR Category::GetName() {
	return szName;
}

/**
 * Gets the sub-category names.
 *
 * @return Array of sub-category names.
 */
vector<wstring> Category::GetSubCategories() {
	return arrSubCategories;
}