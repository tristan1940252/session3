#include "SLNode.h"

template <typename T>
class Queue {
private:
	size_t count;
	SLNode<T>* first, * last;
public:
	Queue() {
		count = 0;
		first = last = nullptr;
	}

	~Queue() {
		while (last) {
			pop();
		}
	}

	void push(T data) {
		
		if (first) {
			last = last->next = new SLNode<T>(data, nullptr);
		}
		else {
			first = last = new SLNode<T>(data, nullptr);
		}
		count++;
	}

	void pop() {
		if (first) {
			SLNode<T>* toDelete = first;
			first = first->next;
			delete toDelete;
			count--;
		}
	}

	size_t size() {
		return count;
	}

	T* list() {
		int counter = 0;
		if (first) {
			T* list = new T[this->size()];
			SLNode<T>* runner = first;
			while (runner != nullptr) {
				list[counter++] = runner->data;
				runner = runner->next;
			}
			return list;
		}
		return nullptr;
	}

	T get(size_t index) {
		SLNode<T>* runner = first;
		if (index < this->size()) {
			for (int i = 0; i < index; i++) {
				runner = runner->next;
			}
			return runner->data;
		}
		return first->data;
	}

	T front() {
		return (first) ? first->data : 0;
	}

	T back() {
		return (last) ? last->data : 0;
	}

};
