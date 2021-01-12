#include "PromotEvent.h"

PromotEvent::PromotEvent(int AT, int ID) : Event(AT, ID)
{
}

void PromotEvent::Execute(LinkedList<Process>& Sys, LinkedList<Process>& Inter, LinkedQueue<Process>& Comp)
{
	PromoteInteractiveToSystem(ID, Sys, Inter);
}

void PromotEvent::PromoteInteractiveToSystem(int ID, LinkedList<Process>& Sys, LinkedList<Process>& Inter)
{
	if (!Inter.isEmpty())
	{
		Process process;
		Node<Process>* p = Inter.Head;
		Node<Process>* R = p->getNext();
		if (p->getItem().GetID() == ID)
		{
			process = p->getItem();
			process.SetProcessType(System);
			p->setItem(process);
			Sys.InsertSorted(p->getItem(), p->getItem().GetPriority());
			p = p->getNext();
			delete Inter.Head;
			Inter.Head = p;
			Inter.count--;
			return;
		}
		else if (R)
		{
			while (R)
			{
				if (R->getItem().GetID() == ID)
				{
					Sys.InsertSorted(R->getItem(), R->getItem().GetPriority());
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
