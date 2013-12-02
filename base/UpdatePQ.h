#ifndef UPDATEPQ_H
#define UPDATEPQ_H


#include <iostream>
#include <set>
#include <map>

template <class T>
class UpdatePQ
{

	struct Pair
	{
		float priority;
		T value;

		Pair()
		{
		}

		Pair(float initPriority, const T &initValue) : priority(initPriority), value(initValue)
		{
		}

		bool operator<(const Pair &p) const
		{
			return (priority < p.priority) || ((priority == p.priority) && (value < p.value));
		}
	};

	std::set<Pair> pairs;
	std::map<T, float> positions;

public:
	void add(float priority, const T &value)
	{
		Pair p(priority, value);

		pairs.insert(p);
		positions.insert(make_pair(value, priority));
	}

	bool empty() const
	{
		return (pairs.begin() == pairs.end());
	}

	const T &top()
	{
		typename std::set<Pair>::iterator it = pairs.begin();

		return it->value;
	}

	float topPriority()
	{
		typename std::set<Pair>::iterator it = pairs.begin();

		return it->priority;
	}

	void pop()
	{
		typename std::set<Pair>::iterator it = pairs.begin();

		positions.erase(it->value);
		pairs.erase(it);
	}

	bool updatePriority(const T&value, float newPriority)
	{
		if(!remove(value))
			return false;
		add(newPriority, value);

		return true;
	}

	bool remove(const T&value)
	{
		Pair p;
		typename std::set<Pair>::iterator itPair;
		typename std::map<T, float>::iterator itPos;

		itPos = positions.find(value);
		if(itPos == positions.end())
			return false;
		p.value = value;
		p.priority = itPos->second;
		itPair = pairs.find(p);
		if(itPair == pairs.end())
			return false;
		pairs.erase(itPair);
		positions.erase(itPos);

		return true;
	}

};


#endif // UPDATEPQ_H
