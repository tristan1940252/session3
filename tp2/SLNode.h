#ifndef SLNODE_H
#define SLNODE_H

template <typename T>
class SLNode {
public:
	SLNode<T>* next;
	T data;

	SLNode(T data, SLNode<T>* next) {
		this->data = data;
		this->next = next;
	}
};
#endif