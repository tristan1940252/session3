#include "SLNode.h"

template <typename T>
class Stack {
private:
	SLNode<T>* sp;
	size_t count;
public:
	Stack() {
		count = 0;
		sp = nullptr;
	}

	~Stack() {
		while (sp) {
			pop();
		}
	}

	T* list() {
		int counter = 0;
		int counterTwo = 0;
		if (sp) {
			T* list = new T[this->size() + 1];
			T* listTwo = new T[this->size() + 1];
			SLNode<T>* runner = sp;
			while (runner != nullptr) {
				list[counter++] = runner->data;
				runner = runner->next;
			}
			while (counter > 0) {
				listTwo[counterTwo++] = list[--counter];
			}
			listTwo[counterTwo] = 0;
			return listTwo;
		}
		return nullptr;
	}

	void push(T data) {
		sp = new SLNode<T>(data, sp);
		count++;
	}

	void pop() {
		if (sp) {
			SLNode<T>* toDelete = sp;
			sp = sp->next;
			delete toDelete;
			count--;
		}
	}

	size_t size() {
		return count;
	}

	T top() {
		return (count) ? sp->data : 0;
	}
};
