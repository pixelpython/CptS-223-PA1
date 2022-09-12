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
	void setHead(node<T>* head);
	int getLength();
	node<T>* getNodeAtPosition(int n);

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
	pHead = nullptr;
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
	return (pHead == nullptr);
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
		pHead = nullptr;
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

template <class T>
void List<T>::setHead(node<T>* head) {
	pHead = head;
}

template <class T>
int List<T>::getLength() {
	int count = 0;
	node<T>* pCurrent = this->getHead();

	// iterate over array and count nodes
	while (pCurrent != nullptr) {
		pCurrent = pCurrent->next;
		count++;
	}

	return count;
}

template <class T>
node<T>* List<T>::getNodeAtPosition(int n) {
	if (n <= this->getLength() - 1) {
		node<T>* pCurrent = pHead;
		for (int i = 0; i < n; i++) {
			pCurrent = pCurrent->next;
		}

		return pCurrent;
	}
	else {
		return nullptr;
	}
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
	node<T>* pNew = new node<T>;
	pNew->data = data;

	// check memory successfully allocated
	if (pNew != 0) {
		if (isEmpty()) {
			pHead = pNew;
		}
		else {
			pNew->next = pHead;
			pHead = pNew;
		}

		return true;
	}

	return false;
}