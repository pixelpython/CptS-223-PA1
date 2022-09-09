#pragma once

#include <iostream>

template <typename T>
struct node {
    T data;
    node *next;
};

template <class T>
class List {
public:
	List();
	~List();

	bool isEmpty();
	void clearList();
	node<T>* getHead();

	bool insertAtFront(T& data);

private:
	node<T>* pHead;
};

/*************************************************************
 * Function: List ()                                         *
 * Description: default constructor for List class           *
 * Input parameters:                                         *
 * Returns:                                                  *
 * Preconditions:                                            *
 * Postconditions:                                           *
 *************************************************************/
template <class T>
List<T>::List() {
	pHead = NULL;
}

/*************************************************************
 * Function: ~List ()                                        *
 * Description: destructor for List class                    *
 * Input parameters:                                         *
 * Returns:                                                  *
 * Preconditions:                                            *
 * Postconditions:                                           *
 *************************************************************/
template <class T>
List<T>::~List() {
	delete pHead;
}

/*************************************************************
 * Function: isEmpty ()                                      *
 * Description: determines whether list is empty             *
 * Input parameters:                                         *
 * Returns: bool                                             *
 * Preconditions:                                            *
 * Postconditions:                                           *
 *************************************************************/
template <class T>
bool List<T>::isEmpty() {
	return (pHead == NULL);
}

/*************************************************************
 * Function: clearList ()                                    *
 * Description: deletes all nodes in list and resets pHead   *
 *		to NULL                                              *
 * Input parameters:                                         *
 * Returns:                                                  *
 * Preconditions:                                            *
 * Postconditions:                                           *
 *************************************************************/
template <class T>
void List<T>::clearList() {
	if (!isEmpty()) {
		delete pHead;
		pHead = NULL;
	}
}

/*************************************************************
 * Function: getHead ()                                      *
 * Description: get head pointer of list                     *
 * Input parameters:                                         *
 * Returns: node<T>*                                         *
 * Preconditions:                                            *
 * Postconditions:                                           *
 *************************************************************/
template <class T>
node<T>* List<T>::getHead() {
	return pHead;
}

/*************************************************************
 * Function: insertAtFront ()                                *
 * Description: inserts new data at front of List            *
 * Input parameters: T& data                                 *
 * Returns: bool                                             *
 * Preconditions:                                            *
 * Postconditions: returns true if allocated successfully,   *
 *		false otherwise                                      *
 *************************************************************/
template <class T>
bool List<T>::insertAtFront(T& data) {
	node<T>* pNew = new node<T>(data);

	// check memory successfully allocated
	if (pNew != 0) {
		if (isEmpty()) {
			pHead = pNew;
		}
		else {
			pNew->setNext(pHead);
			pHead = pNew;
		}

		return true;
	}

	return false;
}