#pragma once
#include "PQNode.h"

template <typename T>
class PriorityQueue {
private:
	PQNode<T>* first;
	size_t count;
public:

	PriorityQueue(PQNode<T>* first = nullptr) {
		this->first = first;
		this->count = 0;
	}

	~PriorityQueue() {
		while (!first) {
			pop();
		}
	}

	T front() {
		return (first) ? first->data : 0;
	}

	size_t frontPriority() {
		return (first) ? first->priority : 0;
	}

	void pop() {
		if (first) {
			PQNode<T>* toDelete = first;
			first = first->next;
			delete toDelete;
			count--;
		}
	}

	void push(T data, size_t priority) {
		if (first) {
			if (first->priority > priority) {
				PQNode<T>* tmp = first;
				first = new PQNode<T>(data, priority, tmp);
			}
			else {
				PQNode<T>* runner = first;
				while (runner->next != nullptr && runner->next->priority <= priority) {
					runner = runner->next;
				}
				PQNode<T>* tmp = runner->next;
				runner->next = new PQNode<T>(data, priority, tmp);
			}
		}
		else
			first = new PQNode<T>(data, priority);
		count++;
	}

	size_t size() {
		return count;
	}
};