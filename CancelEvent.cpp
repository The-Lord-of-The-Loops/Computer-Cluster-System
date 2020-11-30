#include "CancelEvent.h"

CancelEvent::CancelEvent(int AT, int ID) : Event(AT, ID) {}

void CancelEvent::deletSysProcess(int ID, LinkedList<Process> &Sys)
{
	if (!Sys.isEmpty())
	{
		Node<Process>* p = Sys.Head;
		Node<Process>* R = p->getNext();
		if (p->getItem().GetID() == ID)
		{
			p = p->getNext();
			delete Sys.Head;
			Sys.Head = p;
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
					return;
				}
				p = R;
				R = R->getNext();
			}
		}
	}
}

void CancelEvent::Execute(LinkedList<Process> &Sys, LinkedQueue<Process> &Inter, LinkedQueue<Process> &Comp)
{
	deletSysProcess(ID, Sys);
}
