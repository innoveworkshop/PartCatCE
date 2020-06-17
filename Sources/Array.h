/**
 * Array.h
 * A simple array implementation since eVC doesn't have std::vector.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef _ARRAY_H
#define _ARRAY_H

#include <stdlib.h>

template <class T>
class Array {
protected:
	T *lpContainer;
	size_t nCount;

private:
	/**
	 * Allocates memory for the array.
	 *
	 * @param nSize Size of the array.
	 */
	void Initialize(size_t nSize) {
		lpContainer = (T*)LocalAlloc(LMEM_FIXED, nSize * sizeof(T));
	}

public:
	/**
	 * Initializes an empty array of 0 size.
	 */
	Array() {
		// Initialize the container to avoid weird problems with realloc later.
		Initialize(1);
		nCount = 0;
	}

	/**
	 * Initializes an empty array with a predefined size.
	 *
	 * @param nInitialSize Initial size of the array.
	 */
	Array(size_t nInitialSize) {
		Initialize(nInitialSize);
		nCount = nInitialSize;
	}

	/**
	 * Makes sure the array is properly freed.
	 */
	virtual ~Array() {
		LocalFree(lpContainer);
		nCount = 0;
	}

	/**
	 * Gets the length of the array.
	 *
	 * @return Length of the array.
	 */
	size_t Length() {
		return nCount;
	}

	/**
	 * Grows the size of the array.
	 *
	 * @param nSize How much should we grow the array.
	 */
	void Grow(size_t nSize) {
		nCount += nSize;
		lpContainer = (T*)LocalReAlloc(lpContainer, nCount * sizeof(T), LMEM_MOVEABLE);
	}

	/**
	 * Appends an item to the array.
	 *
	 * @param item Item to be appended to the array.
	 */
	void Push(const T item) {
		Grow(1);
		//lpContainer[nCount - 1] = item;
		memcpy(&lpContainer[nCount - 1], &item, sizeof(item));
	}

	/**
	 * Gets an item from the array by its index.
	 *
	 * @param  index Index of the item to be retrieved.
	 * @return       Pointer to the item retrieved or NULL if it doesn't exist.
	 */
	T* Get(size_t index) {
		if (index >= nCount)
			return NULL;

		return &lpContainer[index];
	}
};

#endif  // _ARRAY_H