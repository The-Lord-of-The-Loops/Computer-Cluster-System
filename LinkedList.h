#ifndef _LINKEDLIST
#define _LINKEDLIST

#include "Node.h"
#include <iostream>
using namespace std;

template <typename T>
class LinkedList
{
	
public:

	Node<T>* Head;	//Pointer to the head of the list
	//You can add tail pointer too (depending on your problem)
	int count;	//Number of nodes in the list

	LinkedList()
	{
		count = 0;
		Head = nullptr;
	}

	//List is being destructed ==> delete all items in the list
	~LinkedList()
	{
		DeleteAll();
	}

	/*
	* Function: PrintList.
	* prints the values of all nodes in a linked list.
	*/
	void PrintList()	const
	{
		if (this == nullptr) // added line for some cases
		{
			cout << "Error! List not found" << endl;
			return;
		}
		cout << "\nList has " << count << " nodes";
		cout << "\nprinting list contents:\n\n";
		Node<T>* p = Head;

		while (p)
		{
			cout << "[ " << p->getItem() << " ]";
			cout << "--->";
			p = p->getNext();
		}
		cout << "*\n";
	}

	/*
	* Function: InsertBeg.
	* Creates a new node and adds it to the beginning of a linked list.
	*
	* Parameters:
	*	- data : The value to be stored in the new node.
	*/
	void InsertBeg(const T& data)
	{
		Node<T>* R = new Node<T>(data);
		R->setNext(Head);
		Head = R;
		count++;

	}

	/*
	* Function: DeleteAll.
	* Deletes all nodes of the list.
	*/
	void DeleteAll()
	{
		Node<T>* P = Head;
		while (Head)
		{
			P = Head->getNext();
			delete Head;
			Head = P;
		}
		count = 0;
	}

	////////////////     Requirements   ///////////////////
	//
	// Implement the following member functions


	//[1]InsertEnd 
	//inserts a new node at end if the list
	void InsertLast(const T& data) {

		if (Head == nullptr) {
			InsertBeg(data);
			return;
		}
		else {
			Node<T>* p = Head;
			while (p->getNext()) {
				p = p->getNext();
			}
			Node<T>* R = new Node<T>(data);
			p->setNext(R);
			count++;
			return;

		}

	}

	//[2]Find 
	//searches for a given value in the list, returns true if found; false otherwise.
	bool Find(T Key) {
		Node<T>* p = Head;
		while (p) {
			if (p->getItem() == Key) {
				cout << "Found the element " << Key << endl;
				return true;
			}
			p = p->getNext();
		}
		cout << "\nElement " << Key << " not found" << endl;
		return false;
	}

	//[3]CountOccurance
	//returns how many times a certain value appeared in the list
	int CountOccurance(const T& value) {
		int occs = 0;
		Node<T>* p = Head;
		while (p) {
			if (p->getItem() == value) {
				occs++;
			}
			p = p->getNext();
		}
		cout << "\nElement " << value << " was found " << occs << " times" << endl;
		return occs;
	}

	//[4] DeleteFirst
	//Deletes the first node in the list
	void DeleteFirst() {
		cout << "\nDeleting the first element" << endl;
		if (Head) {
			Node<T>* p = Head->getNext();
			delete Head;
			Head = p;
			count--;
		}
		return;
	}

	//[5] DeleteLast
	//Deletes the last node in the list
	void DeleteLast() {
		cout << "\nDeleting the last element" << endl;
		if (Head == nullptr) {
			return;
		}
		else if (Head->getNext() == nullptr) {
			DeleteFirst();
			return;
		}
		else {
			Node<T>* p = Head;
			Node<T>* q = p->getNext();
			while (q->getNext()) {
				p = q;
				q = p->getNext();
			}
			delete q;
			p->setNext(nullptr);
			count--;
			return;

		}
	}

	//[6] DeleteNode
	//deletes the first node with the given value (if found) and returns true
	//if not found, returns false
	//Note: List is not sorted
	bool DeleteNode(const T& value) {
		cout << "\nDeleting the element " << value << endl;
		if (Head == nullptr) {
			return false;
		}

		if (Head->getItem() == value) {
			DeleteFirst();
			return true;
		}
		Node<T>* p = Head;
		Node<T>* q = p->getNext();

		while (p) {
			if (q->getItem() == value) {
				p->setNext(q->getNext());
				delete q;
				count--;
				return true;
			}
			p = q;
			q = p->getNext();
		}
		return false;
	}

	//[7] DeleteNodes
	//deletes ALL node with the given value (if found) and returns true
	//if not found, returns false
	//Note: List is not sorted
	bool DeleteNodes(const T& value) {
		cout << "\nDeleting all the elements of value " << value << endl;
		if (Head == nullptr) { return false; }
		bool flag = false;
		while (Head && Head->getItem() == value) {
			DeleteFirst();
			flag = true;
		}
		Node<T>* p = Head;
		while (p && p->getNext()) {
			if ((p->getNext())->getItem() == value) {
				Node<T>* R = (p->getNext())->getNext();
				delete p->getNext();
				p->setNext(R);
				flag = true;
				count--;

			}
			else {
				p = p->getNext();
			}
		}
		return flag;
	}

	//[8]Merge
	//Merges the current list to another list L by making the last Node in the current list 
	//point to the first Node in list L
	Node<T>* getHead() const {
		return Head;
	}

	void setHead(Node<T>* newHead) {
		Head = newHead;
	}

	int getCount() {
		return count;
	}

	void setCount(int newCount) {
		count = newCount;
	}

	void Merge(LinkedList& L) {
		Node<T>* p = Head;
		if (Head == nullptr) {
			Head = L.getHead();
			L.setHead(nullptr);
			L.setCount(0);
			return;
		}
		while (p->getNext()) {
			p = p->getNext();
		}
		p->setNext(L.Head);
		count = count + L.getCount();

		L.setHead(nullptr);
		L.setCount(0);
		return;
	}

	//[9] Reverse
	//Reverses the linked list (without allocating any new Nodes)
	void Reverse() {
		cout << "\nReversing the list" << endl;
		if (Head == nullptr) { return; }
		Node<T>* p = Head;
		Node<T>* nextP = Head->getNext();
		Head->setNext(nullptr);
		while (nextP) {
			Node<T>* temp = nextP->getNext();
			nextP->setNext(p);
			p = nextP;
			nextP = temp;
		}
		Head = p;
		return;
	}

	void PrintKth(int k)
	{
		if (Head == nullptr)
			cout << "List is Empty" << endl;
		else if (k > count)
			cout << "Beyond List Length" << endl;
		else {
			Node<T>* p = Head;
			for (int i = 0; i < k - 1; i++)
			{
				p = p->getNext();
			}
			cout << "the item number " << k << " is " << p->getItem() << endl;
		}
	}

	bool InsertSorted(int item) {
		Node<T>* p = Head;
		while (p)
		{
			if (item == p->getItem())
				return false;
			p = p->getNext();
		}
		p = Head;
		if (p == Head && item < p->getItem()) {
			Node<T>* inserted = new Node<T>(item);
			inserted->setNext(p);
			Head = inserted;
			count++;
			return true;
		}
		Node<T>* R = Head;
		while (p->getNext())
		{
			p = p->getNext();

			if (item < p->getItem()) {
				Node<T>* inserted = new Node<T>(item);
				inserted->setNext(p);
				R->setNext(inserted);
				count++;
				return true;
			}

			R = R->getNext();

		}

		Node<T>* inserted = new Node<T>(item);
		p->setNext(inserted);
		count++;
		return true;
	}

	Node<T>* RemoveMin() {
		Node<T>* p = Head;
		Node<T>* min = new Node<T>;
		min->setItem(Head->getItem());
		while (p)
		{
			if (p->getItem() < min->getItem())
			{
				min->setItem(p->getItem());
			}
			p = p->getNext();
		}
		/*p = Head;
		if (Head == min)
		{
			Head = p->getNext();

			count--;
			return min;
		}

		while ((p->getNext())->getNext())
		{
			if ((p->getNext())->getItem() == min->getItem())
			{
				p->setNext((p->getNext())->getNext());
				(p->getNext())->setNext(nullptr);
				count--;
				return min;
			}
			p = p->getNext();
		}

		p->setNext(nullptr);
		count--;*/
		DeleteNodes(min->getItem());
		return min;
	}

	LinkedList<T>* CloneList()
	{
		LinkedList<T>* newL = new LinkedList<T>;
		/*newL.setHead(L.getHead());
		newL.getHead()->setNext(nullptr);*/
		Node<T>* p = Head;
		//Node<T>* p2 = newL.getHead();

		while (p)
		{
			newL->InsertLast(p->getItem());
			p = p->getNext();
		}

		cout << "cloning is done " << endl;
		return newL;
	}

	LinkedList<T>** SignSplit()
	{
		cout << "Splitting List" << endl;
		if (Head == nullptr)
			return nullptr;
		LinkedList<T>** splitted = new LinkedList<T>*;
		LinkedList<T>* Lpos = new LinkedList<T>;
		LinkedList<T>* Lneg = new LinkedList<T>;
		Node<T>* p = Head;
		Node<T>* q = p->getNext();
		while (p) {
			if (p->getItem() < 0)
			{
				Lneg->InsertLast(p->getItem());
				DeleteNode(p->getItem());
				p = q;
				if(p)
				q = p->getNext();
			}
			else if (p->getItem() > 0)
			{
				Lpos->InsertLast(p->getItem());
				DeleteNode(p->getItem());
				p = q;
				if (p)
				q = p->getNext();
			}
			else
			p = p->getNext();
			if (p)
			q = p->getNext();
		}
			splitted[0] = Lpos;
			splitted[1] = Lneg;
			cout << "Splitted Lists Created" << endl;
			return splitted;
	}

	LinkedList<T>* MergeSorted(LinkedList& L)
	{
		LinkedList<T>* newL = new LinkedList<T>;
		Node<T>* p1 = Head;
		Node<T>* p2 = L.getHead();
		Node<T>* min;
		if (p1 == nullptr)
		{
			newL = L.CloneList();
			L.DeleteAll();
			return newL;
		}
		if (p2 == nullptr)
		{
			newL = CloneList();
			DeleteAll();
			return newL;
		}
		if (p2 == nullptr && p1 == nullptr)
		{
			newL = nullptr;
			return newL;
		}
		while (p1)
		{
			while (p2)
			{
				if (p2->getItem() < p1->getItem())
				{
					newL->InsertLast(p2->getItem());
				}
				else if(p2->getItem() > p1->getItem())
				{
					newL->InsertLast(p1->getItem());
					break;
				}
				else if (p2->getItem() == p1->getItem())
				{
					newL->InsertLast(p2->getItem());
					newL->InsertLast(p1->getItem());
					if (p2-> getNext() == nullptr)
					{
						p1 = p1->getNext();
						p2 = p2->getNext();
						break;
					}
					p2 = p2->getNext();
					break;
				}
				p2 = p2->getNext();
			}
			if (p2 == nullptr)
				break;
			p1 = p1->getNext();
		}
		// Since in some cases p1/p2 is finished before p2/p1, we will need to add the rest of p1/p2 (detected by trials)
		if (p2)
		{
			while (p2)
			{
				newL->InsertLast(p2->getItem());
				p2 = p2->getNext();
			}
		}
		if (p1)
		{
			while (p1)
			{
				newL->InsertLast(p1->getItem());
				p1 = p1->getNext();
			}
		}
		L.DeleteAll();
		this->DeleteAll();
		return newL;
	}

	LinkedList<T>* SumLists(LinkedList& L1, LinkedList& L2)
	{
		if (L1.getCount() != L2.getCount())
		{
			cout << "Error! Number of elements is not equal" << endl;
				return nullptr;
		}
		LinkedList<T>* newL = new LinkedList<T>;
		Node<T>* p1 = L1.getHead();
		Node<T>* p2 = L2.getHead();
		while (p1)
		{
			newL->InsertLast(p1->getItem() + p2->getItem());
			p1 = p1->getNext();
			p2 = p2->getNext();
		}
		return newL;
	}

	// I hate this Function
	void Reorder_X(T x) 
	{
		if (Head == nullptr)
			return;
		Node<T>* p = Head;
		Node<T>* R = p->getNext();
		Node<T>* Rn = R->getNext();

		while (R)
		{
			if (R->getItem() == x || R->getItem() < x)
			{
					R->setNext(Head);
					Head = R;
					p->setNext(Rn);
					R = Rn;
					if (Rn != nullptr)
						Rn = Rn->getNext();
			}
			else
			{
				p = p->getNext();
				R = R->getNext();
				if (Rn != nullptr)
					Rn = Rn->getNext();
			}

				//PrintList();
		}
	}

	void ShiftLargest()
	{
		Node<T>* p = Head;
		Node<T>* max = p;
		Node<T>* beforemax = p;
		while (p)
		{
			if (p->getItem() > max->getItem())
			{
				max = p;
			}
			
			p = p->getNext();
		}
		p = Head;
		while(p)
		{
			if (p->getNext() == max)
			{
				beforemax = p;
				break;
			}
			p = p->getNext();
		}
		Node<T>* R = max->getNext();
		if (max == Head)
		{
			max->setNext(R->getNext());
			R->setNext(max);
			Head = R;
			beforemax = Head;
			R = max->getNext();
		}
		else if (max->getNext() == nullptr)
			return;
		Node<T>* Rn = nullptr;
		if (R)
			Rn = R->getNext();
		while (R)
		{
			if (Rn)
				max->setNext(Rn);
			else
				max->setNext(nullptr);
			R->setNext(max);
			beforemax->setNext(R);
			R = Rn;
			beforemax = beforemax->getNext();
			if (Rn)
			Rn = Rn->getNext();
		}
	}

	void RemoveDuplicates()
	{
		bool del;
		Node<T>* p = Head;
		Node<T>* Look;
		while (p)
		{
			del = false;
			Look = p->getNext();
			while (Look)
			{
				if (p->getItem() == Look->getItem()) {
					p = p->getNext();
					DeleteNode(Look->getItem());
					del = true;
					break;
				}
				if(p->getNext())
				Look = Look->getNext();
			}
				if (!del)
				p = p->getNext();
		} 
	}
};

//Specialization
template<>
class LinkedList<Process>
{
public:
	Node<Process>* Head;	//Pointer to the head of the list
		//You can add tail pointer too (depending on your problem)
	int count;	//Number of nodes in the list

	LinkedList()
	{
		count = 0;
		Head = nullptr;
	}

	void InsertBeg(const Process data)
	{
		Node<Process>* R = new Node<Process>(data);
		R->setNext(Head);
		Head = R;
		count++;

	}

	void InsertLast(const Process data) {

		if (Head == nullptr) {
			InsertBeg(data);
			return;
		}
		else {
			Node<Process>* p = Head;
			while (p->getNext()) {
				p = p->getNext();
			}
			Node<Process>* R = new Node<Process>(data);
			p->setNext(R);
			count++;
			return;
		}
	}

	bool InsertSorted(Process process, int item) {
	    if(isEmpty()){
	        InsertBeg(process);
            return true;
	    }

		Node<Process>* p = Head;
		if (p == Head && item < p->getItem().GetPriority())
		{
			Node<Process>* inserted = new Node<Process>(process);
			inserted->setNext(p);
			Head = inserted;
			count++;
			return true;
		}
		Node<Process>* R = Head;
		while (p->getNext())
		{
			p = p->getNext();

			if (item < p->getItem().GetPriority()) {
				Node<Process>* inserted = new Node<Process>(process);
				inserted->setNext(p);
				R->setNext(inserted);
				count++;
				return true;
			}
			R = R->getNext();
		}
		Node<Process>* inserted = new Node<Process>(process);
		p->setNext(inserted);
		count++;
		return true;
	}

	void DeleteFirst() {
		cout << "\nDeleting the first element" << endl;
		if (Head) {
			Node<Process>* p = Head->getNext();
			delete Head;
			Head = p;
			count--;
		}
		return;
	}

	bool DeleteNode(const int value) {
		if (Head == nullptr) {
			return false;
		}

		if (Head->getItem().GetID() == value) {
			DeleteFirst();
			return true;
		}
		Node<Process>* p = Head;
		Node<Process>* q = p->getNext();

		while (p) {
			if (q->getItem().GetID() == value) {
				p->setNext(q->getNext());
				delete q;
				count--;
				return true;
			}
			p = q;
			q = p->getNext();
		}
		return false;
	}

	void DeleteAll()
	{
		Node<Process>* P = Head;
		while (Head)
		{
			P = Head->getNext();
			delete Head;
			Head = P;
		}
		count = 0;
	}

	~LinkedList()
	{
		DeleteAll();
	}

	bool isEmpty()
	{
		if (Head)
			return false;
		else
			return true;
	}

};

#endif