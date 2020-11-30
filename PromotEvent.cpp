#include "PromotEvent.h"

PromotEvent::PromotEvent(int AT, int ID) : Event(AT, ID)
{
}

void PromotEvent::Execute(LinkedList<Process>& Sys, LinkedQueue<Process>& Inter, LinkedQueue<Process>& Comp)
{
	//PromoteSystemToComputationallyIntensive(ID, Sys, Comp);
}

void PromotEvent::PromoteSystemToComputationallyIntensive(int ID, LinkedList<Process>& Sys, LinkedQueue<Process>& Inter)
{
	if (!Sys.isEmpty())
	{
		Node<Process>* p = Sys.Head;
		Node<Process>* R = p->getNext();
		if (p->getItem().GetID() == ID)
		{
			Inter.enqueue(p->getItem());
			p = p->getNext();
			delete Sys.Head;
			Sys.Head = p;
			Sys.count--;
			return;
		}
		else if (R)
		{
			while (R)
			{
				if (R->getItem().GetID() == ID)
				{
					Inter.enqueue(R->getItem());
					p->setNext(R->getNext());
					delete R;
					Sys.count--;
					return;
				}
				p = R;
				R = R->getNext();
			}
		}
	}
}
