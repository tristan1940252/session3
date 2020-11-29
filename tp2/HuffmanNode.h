#pragma once

class HuffmanNode {
public:
	char data;
	HuffmanNode* left;
	HuffmanNode* right;

	HuffmanNode(char data, HuffmanNode* left = nullptr, HuffmanNode* right = nullptr) {
		this->data = data;
		this->left = left;
		this->right = right;
	}
};