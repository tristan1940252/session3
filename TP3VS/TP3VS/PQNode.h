#pragma once

template <typename T>
class PQNode {
public:
	T data;
	PQNode<T>* next;
	size_t priority;

	PQNode(T data, size_t priority, PQNode<T>* next = nullptr) {
		this->data = data;
		this->priority = priority;
		this->next = next;
	}
};