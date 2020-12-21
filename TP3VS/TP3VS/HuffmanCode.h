#pragma once
class HuffmanCode {
public:
	char data;
	char* code;

	HuffmanCode(char data = '?', char* code = nullptr) {
		this->data = data;
		this->code = code;
	}
};