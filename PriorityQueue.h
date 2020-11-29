#pragma once

#include "Node.h"
#include "PQ_Data.h"

template <typename T>
class PriorityQueue
{
private:

	Node<PQ_Data<T>>* frontPtr;
	int count;


public:

	PriorityQueue();
	bool isEmpty() const;
	bool enqueue(const T& newEntry, const int& priority);
	bool dequeue(T& frntEntry);
	bool peek(T& frntEntry)  const;
	PriorityQueue(const PriorityQueue<T>& PQ);
	~PriorityQueue();
	int GetCount() const;
};

// constructor
template <typename T>
PriorityQueue<T>::PriorityQueue() {
	frontPtr = nullptr;
	count = 0;
}

template <typename T>
bool PriorityQueue<T>::isEmpty() const
{
	return (frontPtr == nullptr);

}

template <typename T>
bool PriorityQueue<T>::enqueue(const T& newEntry, const int& priority)
{
	Node<PQ_Data<T>>* newNodePtr = new Node<PQ_Data<T>>(PQ_Data<T>(newEntry, priority));
	if (isEmpty())
	{
		frontPtr = newNodePtr;
	}

	else if (frontPtr->getItem().getPriority() > priority) // If the frontPtr has a lower priority than new node, higher number means lower priority
	{
		newNodePtr->setNext(frontPtr);
		frontPtr = newNodePtr;
	}

	else
	{
		Node<PQ_Data<T>>* temp = frontPtr;
		while (temp->getNext() && temp->getNext()->getItem().getPriority() < priority) // we search for a position to insert new node
		{
			temp = temp->getNext();
		}
		if (temp->getNext() && temp->getNext()->getItem().getPriority() == priority)
			temp = temp->getNext();
		newNodePtr->setNext(temp->getNext());
		temp->setNext(newNodePtr);
	}
	count++;
	return true;
}

template <typename T>
bool PriorityQueue<T>::dequeue(T& frntEntry) {
	if (isEmpty())
		return false;

	Node<PQ_Data<T>>* nodeToDeletePtr = frontPtr;
	frntEntry = frontPtr->getItem().getData();
	frontPtr = frontPtr->getNext();
	// Queue is not empty; remove front

	// Free memory reserved for the dequeued node
	delete nodeToDeletePtr;
	count--;
	return true;
}

template <typename T>
bool PriorityQueue<T>::peek(T& frntEntry) const {
	if (isEmpty())
		return false;

	frntEntry = frontPtr->getItem().getData();
	return true;

}

//copy constructor
template <typename T>
PriorityQueue<T>::PriorityQueue(const PriorityQueue<T>& PQ)
{
	frontPtr = nullptr;
	count = 0;
	Node<PQ_Data<T>>* NodePtr = PQ.frontPtr;	//start at the front node in PQ
	while (NodePtr)
	{
		enqueue(NodePtr->getItem().getData(), NodePtr->getItem().getPriority());	//get data of each node and enqueue it in this queue 
		NodePtr = NodePtr->getNext();
	}
}

template <typename T>
PriorityQueue<T>::~PriorityQueue() {
	T temp;
	while (dequeue(temp));

}

template <typename T>
int PriorityQueue<T>::GetCount() const {
	return count;
}


