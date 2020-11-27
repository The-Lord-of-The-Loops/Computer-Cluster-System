#pragma once

template <typename T>
class PQ_Data
{
private:
	T data;
	int priority; // high number means low priority.
public:

	PQ_Data() {};
	PQ_Data(T data, int priority) : data(data), priority(priority) {};
	T getData() const
	{
		return data;
	}
	int getPriority() const
		{
			return priority;
		}

	PQ_Data(const PQ_Data& D)
	{
		data = D.getData();
		priority = D.getPriority();
	}

	void operator=(const PQ_Data& D)
	{
		data = D.getData();
		priority = D.getPriority();
	}

};
