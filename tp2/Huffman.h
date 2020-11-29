#pragma once
#include "HuffmanCode.h"
#include "HuffmanNode.h"
#include "PriorityQueue.h"
#include "Queue.h"
#include "Stack.h"
#include <fstream>
#include <stdio.h>
#include <string>
#include <math.h>

using namespace std;

class Huffman {
private:

	static HuffmanCode** buildCodes(HuffmanNode* huffmanTree, Stack<char>* codeStack = new Stack<char>(), Queue<HuffmanCode*>* huffQueue = new Queue<HuffmanCode*>()) {
		HuffmanNode* runner = huffmanTree;
		if (runner->left) {
			codeStack->push('0');
			buildCodes(runner->left, codeStack, huffQueue);
		}
		if (runner->right) {
			codeStack->push('1');
			buildCodes(runner->right, codeStack, huffQueue);
		}
	
		char* code = codeStack->list();
		if(codeStack->size())
			codeStack->pop();
		
		if (runner->data != '_') {
			huffQueue->push(new HuffmanCode(runner->data, code));	
		}
	
		HuffmanCode** huffcode = new HuffmanCode*[huffQueue->size()];
		for (size_t i = 0; i < huffQueue->size(); i++) {
			huffcode[i] = huffQueue->get(i);
		}
		return huffcode;
	}
	

	static PriorityQueue<HuffmanNode*>* buildKey(const char* fileName) {
		printf("%s\n", "Building Key...");
		string file = fileName;
		file = file.substr(0, file.find("."));

		PriorityQueue<HuffmanNode*>* queue = new PriorityQueue<HuffmanNode*>;
		char byte;
		size_t tab[256];
		ifstream inputFile;
		ofstream outputFile;

		outputFile.open(file + ".hk", ios::binary);
		inputFile.open(fileName, ios::binary | ios::ate);
		streampos size = inputFile.tellg();
		inputFile.seekg(0, ios::beg);
		
		for (int i = 0; i < 256; i++) {
			tab[i] = 0;
		}
		for (int i = 0; i < size; i++) {
			inputFile.read(&byte, 1);
			tab[(unsigned char)byte]++;
		}
		for (int i = 0; i < 256; i++) {
			outputFile.write((const char*) &tab[i], 8);
			if(tab[i] > 0)
				queue->push(new HuffmanNode(i), tab[i]);
		}
		
		outputFile.close();
		inputFile.close();
		return queue;
	}

	static PriorityQueue<HuffmanNode*>* loadKey(const char* fileName) {
		ifstream inputFile;
		PriorityQueue<HuffmanNode*>* queue = new PriorityQueue<HuffmanNode*>;
		size_t byte;

		inputFile.open(fileName, ios::binary | ios::ate);
		streampos size = inputFile.tellg();
		inputFile.seekg(0, ios::beg);

		for (int i = 0; i < size; i++) {
			inputFile.read((char*)&byte, 8);
			if (byte) {
				queue->push(new HuffmanNode(i), byte);
			}
		}

		inputFile.close();
		return queue;
	}

	static HuffmanNode* buildTree(PriorityQueue<HuffmanNode*>* huffmanQueue) {
		printf("%s\n", "Building Tree...");
		while (huffmanQueue->size() > 1)  {
			HuffmanNode* node = new HuffmanNode('_', huffmanQueue->front());
			size_t priority = huffmanQueue->frontPriority();
			huffmanQueue->pop();
			node->right = huffmanQueue->front();
			priority += huffmanQueue->frontPriority();
			huffmanQueue->pop();
			huffmanQueue->push(node, priority);
		}
		return huffmanQueue->front();
	}

	static char* getCode(HuffmanCode** huff, char data) {
		unsigned char count = 0;
		char* code = 0;
		while (!code) {
			if (huff[count]->data == data) {
				code = huff[count]->code;
			}
			count++;
		}
		return code;
	}

	static string decimalToBinary(int num) {
		string binary = "";
		string temp = "";
		
		for (int i = 0; i < 8; i++) {
			temp = (num % 2) + '0';
			binary.insert(0,temp);
			num /= 2;
		}
		return binary;
	}

public:

	static void encode(const char* fileName) {
		string text;
		char byte;
		HuffmanCode** huff = buildCodes(buildTree(buildKey(fileName)));
		printf("%s\n", "Building Codes...");
		ifstream inputFile;
		ofstream outputFile;

		string file = fileName;
		file = file.substr(0, file.find("."));

		inputFile.open(fileName, ios::binary | ios::ate);
		outputFile.open(file + ".hc", ios::binary);
		streampos size = inputFile.tellg();
		inputFile.seekg(0, ios::beg);
		printf("%s\n", "Encoding...");

		for (int i = 0; i < size; i++) {
			inputFile.read(&byte, 1);
			text += getCode(huff, byte);
		}
		
		for (size_t i = 0; i < text.size(); i+=8) {
			while (i > (text.size() - 8)) {
				text += '0';
			}
			string oct = text.substr(i, 8);
			int num = ((oct[0] - '0') * pow(2, 7)) + ((oct[1] - '0') * pow(2,6)) + ((oct[2] - '0') * pow(2,5)) + ((oct[3] - '0') * pow(2,4)) + ((oct[4] - '0') * pow(2,3)) + ((oct[5] - '0') * pow(2,2)) + ((oct[6] - '0') * pow(2,1)) + ((oct[7] - '0') * pow(2,0));
			outputFile.write(to_string(num).c_str(),to_string(num).size());
		}
	
		inputFile.close();
		outputFile.close();
	}

	static void decode(const char* fileName, const char* outputName) {
		ifstream inputFile;
		ofstream outputFile;
		char byte;

		string file = fileName;
		file = file.substr(0, file.find("."));
		
		inputFile.open(file + ".hc", ios::binary | ios::ate);
		outputFile.open(outputName, ios::binary);
		streampos size = inputFile.tellg();
		inputFile.seekg(0, ios::beg);
		
		size_t total = 0; //Nombre de caractères à écrire
		PriorityQueue<HuffmanNode*>* queue = loadKey(fileName);
		size_t queueSize = queue->size();
		for (size_t i = 0; i < queueSize; i++) {
			total += queue->frontPriority();
			queue->pop();
		}

		printf("%s\n", "Loading Key...");
		HuffmanNode* tree = buildTree(loadKey(fileName));
		string decimal = "";
		string binary = "";
		string tmp = "";
		
		printf("%s\n", "Decoding...");
		for (size_t i = 0; i <= size; i++) {
			inputFile.read(&byte, 1);
			decimal += byte;

			if (stoi(decimal) > 255 || byte == '\0') {
				tmp = decimal.substr(0, decimal.size() - 1);
				decimal = decimal[decimal.size() - 1];
				binary += decimalToBinary(stoi(tmp));
			}
		}
				
		size_t longueur = 0; //Nombre de caractères écrit
		size_t count = 0; //Bit actuel dans la string binaire 
		while(longueur < total) {
			HuffmanNode* runner = tree;
			while (runner->left || runner->right) {
				if (binary[count] == '0') {
					runner = runner->left;
				}
				else {
					runner = runner->right;
				}
				count++;
			}
			longueur++;
			outputFile.write((const char*)&runner->data, 1);
		}

		inputFile.close();
		outputFile.close();
	}
};
