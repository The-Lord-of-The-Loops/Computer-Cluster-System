#include "CancelEvent.h"

CancelEvent::CancelEvent(int AT, int ID) : Event(AT,ID){}

void CancelEvent::deletSysProcess(int ID, LinkedList<Process> &Inter)
{
	if (!Inter.isEmpty())
	{
		Node<Process>* p = Inter.Head;
		Node<Process>* R = p->getNext();
		if (p->getItem().GetID() == ID)
		{
			p = p->getNext();
			delete Inter.Head;
			Inter.Head = p;
			Inter.count--;
			return;
		}
		else if(R)
		{
			while (R)
			{
				if (R->getItem().GetID() == ID)
				{
					p->setNext(R->getNext());
					delete R;
					Inter.count--;
					return;
				}
				p = R;
				R = R->getNext();
			}
		}
	}
}

void CancelEvent::Execute(LinkedList<Process>& Sys, LinkedList<Process>& Inter, LinkedQueue<Process>& Comp)
{
	deletSysProcess(ID, Inter);
}
