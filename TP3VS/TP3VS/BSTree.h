#pragma once
#include "TLNode.h"
#include <stdio.h>
#include "Queue.h"

template <typename T>
class BSTree {
private:
	TLNode<T>* root;

	void infixTraversal(TLNode<T>* node, Queue<T>* traversal) {
		if (node->left) {
			infixTraversal(node->left, traversal);
		}
		traversal->push(node->data);
		if (node->right) {
			infixTraversal(node->right, traversal);
		}
	}
	void update(TLNode<T>* node) {
		if (node->left) {
			update(node->left);
		}
		node->data--;
		if (node->right) {
			update(node->right);
		}
	}
	
public:
	BSTree() {
		root = nullptr;
	}

	~BSTree() {
		delete root;
	}

	Queue<T>* infixTraversal() {
		if (root) {
			Queue<T>* traversal = new Queue<T>();
			infixTraversal(root, traversal);
			return traversal;
		}
		return nullptr;
	}

	void update() {
		if (root) {
			update(root);
		}
	}
	
	void add(T data) {
		if (root) {
			TLNode<T>* tmp = runningTree(root, data);
			if(data > tmp->data)
				tmp->right = new TLNode<T>(data,tmp);
			else
				tmp->left = new TLNode<T>(data,tmp);
		}
		else {
			root = new TLNode<T>(data,nullptr);
		}
	}

	bool search(T data) {
		if (root) {
			return (runningTree(root, data)->data == data) ? true : false;
		}
		else
			return false;
	}

	void remove(T data) {
		if (this->search(data)) {
			TLNode<T>* toDelete = runningTree(root, data);
			
			if (toDelete->left == nullptr && toDelete->right == nullptr) {
				if (toDelete != root) {
					if (toDelete->data < toDelete->up->data) {
						toDelete->up->left = nullptr;
					}
					else {
						toDelete->up->right = nullptr;
					}
					delete toDelete;
				}
				else {
					root = nullptr;
				}
				
			}
			//Une branche
			else if (toDelete->left == nullptr || toDelete->right == nullptr) {
				if (toDelete != root) {
					if (toDelete->left == nullptr && toDelete->right != nullptr) {
						toDelete->right->up = toDelete->up;
						toDelete->up->right = toDelete->right;
					}
					else {
						toDelete->left->up = toDelete->up;
						toDelete->up->left = toDelete->left;
					}
				}
				else {
					if (toDelete->left == nullptr && toDelete->right != nullptr) {
						root = toDelete->right;
					}
					else {
						root = toDelete->left;
					}
				}
				delete toDelete;
			}
			
			//Deux branches
			else {
				Queue<T>* queue = new Queue<T>();
				infixTraversal(toDelete, queue);
				toDelete->data = queue->back();
			
			}
		}
	}
	/*
	void remove(T data) {
		if (this->search(data)) {
			DLNode<T>* toDelete = runningTree(root,data);
			DLNode<T>* tmp = new DLNode<T>(toDelete->data, toDelete->left, toDelete->right);
			delete toDelete;
			DLNode<T>* parent = runningTree(root, data);


			if (tmp->left || tmp->right) {
				//Réassignation
				if (parent->data > tmp->data) {
					parent->left = (tmp->right) ? tmp->right : tmp->left;
				}
				else {
					parent->right = (tmp->right) ? tmp->right : tmp->left;
				}
				
			}
			else {
				if (parent->data > tmp->data) {
					parent->left = nullptr;
				}
				else
					parent->right = nullptr;
				delete tmp;
			}
			printf("%i", parent->data);
		}
		
	}
	*/
	TLNode<T>* runningTree(TLNode<T>* root, T data) {
		TLNode<T>* runner = root;
		if (data < runner->data) {
			if (runner->left) {
				return runningTree(runner->left,data);
			}
			else {
				return runner;
			}
		}
		else if(data > runner->data){
			if (runner->right) {
				return runningTree(runner->right,data);
			}
			else {
				return runner;
			}
		}
		else {
			return runner;
		}
	}
};