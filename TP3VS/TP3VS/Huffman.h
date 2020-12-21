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
		
		if (huffmanTree->left) {
			codeStack->push('0');
			buildCodes(huffmanTree->left, codeStack, huffQueue);
		}
		if (huffmanTree->right) {
			codeStack->push('1');
			buildCodes(huffmanTree->right, codeStack, huffQueue);
		}
	
		char* code = codeStack->list();
		codeStack->pop();
		
		if (huffmanTree->data != '_') {
			huffQueue->push(new HuffmanCode(huffmanTree->data, code));	
		}
	
		HuffmanCode** huffcode = new HuffmanCode*[huffQueue->size()];
		for (size_t i = 0; i < huffQueue->size(); i++) {
			huffcode[i] = huffQueue->get(i);
		}
		return huffcode;
	}
	

	static PriorityQueue<HuffmanNode*>* buildKey(const char* fileName) {
		printf("%s\n", "Build Key [####################]");
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

	static PriorityQueue<HuffmanNode*>* buildKeyString(string toEncode) {
		

		PriorityQueue<HuffmanNode*>* queue = new PriorityQueue<HuffmanNode*>;
		char byte;
		size_t tab[256];
		string output;

		for (int i = 0; i < 256; i++) {
			tab[i] = 0;
		}
		for (int i = 0; i < toEncode.size(); i++) {
			byte = toEncode.at(i);
			tab[(unsigned char)byte]++;
		}
		for (int i = 0; i < 256; i++) {
			output += to_string(tab[i]);
			if (tab[i] > 0)
				queue->push(new HuffmanNode(i), tab[i]);
		}
		printf("%s", output);
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

	static PriorityQueue<HuffmanNode*>* loadKeyString(string toDecode) {
		
		PriorityQueue<HuffmanNode*>* queue = new PriorityQueue<HuffmanNode*>;
		size_t byte;
		int tab[20] = { -1 };
		int allo = 0;
		toDecode = toDecode.substr(toDecode.find_first_of(">") + 1, toDecode.find_last_of("<") - 3);
		//string lol = toDecode.substr(toDecode.find_first_of(" "), toDecode.fin)
		for (int i = 0; i < toDecode.size(); i++) {
			if (toDecode[i] == (char)32) {
				tab[allo++] = i;
			}
		}
		for (int i = 0; i < toDecode.size() - 1; i) {
			
			string run = toDecode.substr(tab[i] + 1, tab[i] + 1);
			toDecode = toDecode.substr(tab[i] + 2, toDecode.length() - 1);
			string car = toDecode.substr(tab[i] + 1, tab[i] + 1);
			toDecode = toDecode.substr(tab[i] + 2, toDecode.length() - 1);
			
			queue->push(new HuffmanNode(car.at(0)), stoi(run));
		}
		/*
		for (int i = 0; i < toDecode.size(); i++) {
			byte = toDecode.at(i);
			if (byte) {
				queue->push(new HuffmanNode(byte), i);
			}
		}
		*/
		
		return queue;
	}

	static HuffmanNode* buildTree(PriorityQueue<HuffmanNode*>* huffmanQueue) {
		printf("%s\n", "Build Tree [####################]");
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

	static string getKey() {

	}

public:

	static void encode(const char* fileName) {
		string file = fileName;
		file = file.substr(0, file.find("."));
		
		HuffmanCode** huff = buildCodes(buildTree(buildKey(fileName)));
		printf("%s\n", "Build Codes [####################]");
		printf("%s\n", "Encode [####################]");
		string text;
		char byte;
		
		ifstream inputFile;
		ofstream outputFile;

		inputFile.open(fileName, ios::binary | ios::ate);
		outputFile.open(file + ".hc", ios::binary);
		streampos size = inputFile.tellg();
		inputFile.seekg(0, ios::beg);

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

	static string encodeString(string toEncode) {
		PriorityQueue<HuffmanNode*>* queue = buildKeyString(toEncode);
		HuffmanCode** huff = buildCodes(buildTree(buildKeyString(toEncode)));
		
		
		string output = "<K> ";
		size_t queueSize = queue->size();
		for (size_t i = 0; i < queueSize; i++) {
			output += to_string(queue->frontPriority());
			output += " ";
			output += (char)queue->front()->data;
			output += " ";
			queue->pop();
		}
		output += "</K> ";

		
		string text;
		char byte;

		

		for (int i = 0; i < toEncode.size(); i++) {
			byte = toEncode.at(i);
			text += getCode(huff, byte);
		}

		for (size_t i = 0; i < text.size(); i += 8) {
		
			while (text.size() % 8 > 0) {
				text += '0';
			}
			string oct = text.substr(i, 8);
			int num = ((oct[0] - '0') * pow(2, 7)) + ((oct[1] - '0') * pow(2, 6)) + ((oct[2] - '0') * pow(2, 5)) + ((oct[3] - '0') * pow(2, 4)) + ((oct[4] - '0') * pow(2, 3)) + ((oct[5] - '0') * pow(2, 2)) + ((oct[6] - '0') * pow(2, 1)) + ((oct[7] - '0') * pow(2, 0));
			output += to_string(num);
		}
		return output;
	}

	static void decode(const char* fileName, const char* outputName) {
		printf("%s\n", "Decode [####################]");
		ifstream inputFile;
		ofstream outputFile;
		char byte;

		string file = fileName;
		file = file.substr(0, file.find("."));
		
		inputFile.open(file + ".hc", ios::binary | ios::ate);
		outputFile.open(outputName, ios::binary);
		streampos size = inputFile.tellg();
		inputFile.seekg(0, ios::beg);

		printf("%s\n", "Load Key [####################]");
		size_t total = 0; //Nombre de caractères à écrire
		PriorityQueue<HuffmanNode*>* queue = loadKey(fileName);
		size_t queueSize = queue->size();
		for (size_t i = 0; i < queueSize; i++) {
			total += queue->frontPriority();
			queue->pop();
		}

		string decimal = "";
		string binary = "";
		
		for (size_t i = 0; i <= size; i++) {
			inputFile.read(&byte, 1);
			decimal += byte;

			if (stoi(decimal) > 255 || byte == '\0') {
				string tmp = decimal.substr(0, decimal.size() - 1);
				decimal = decimal[decimal.size() - 1];
				binary += decimalToBinary(stoi(tmp));
			}
		}

		HuffmanNode* tree = buildTree(loadKey(fileName));	
		size_t longueur = 0; //Nombre de caractères écrient
		size_t count = 0; //Bit actuel dans la string binaire 
		while(longueur < total) {
			while (tree->left || tree->right) {
				if (binary[count] == '0') {
					tree = tree->left;
				}
				else {
					tree = tree->right;
				}
				count++;
			}
			longueur++;
			outputFile.write((const char*)&tree->data, 1);
		}

		inputFile.close();
		outputFile.close();
	}

	static string decodeString(string toDecode) {
		printf("%s\n", "Decode [####################]");
		
		char byte;
		string output;
		string original = toDecode;

		
	

		printf("%s\n", "Load Key [####################]");
		size_t total = 0; //Nombre de caractères à écrire
		PriorityQueue<HuffmanNode*>* queue = loadKeyString(toDecode);
		size_t queueSize = queue->size();
		for (size_t i = 0; i < queueSize; i++) {
			total += queue->frontPriority();
			queue->pop();
		}

		toDecode = toDecode.substr(toDecode.find_last_of(" ") + 1, toDecode.size() - 1);
		

		string decimal = "";
		string binary = "";

		for (size_t i = 0; i < toDecode.size(); i++) {
			byte = toDecode.at(i);
			decimal += byte;

			if (stoi(decimal) > 255 || i == toDecode.size() - 1) {
				if (stoi(decimal) > 255) {
					string tmp = decimal.substr(0, decimal.size() - 1);
					decimal = decimal[decimal.size() - 1];
					binary += decimalToBinary(stoi(tmp));
				}
				else {
					binary += decimalToBinary(stoi(decimal));
				}
				
			}
		}

		HuffmanNode* tree = buildTree(loadKeyString(original));
		
		size_t longueur = 0; //Nombre de caractères écrient
		size_t count = 0; //Bit actuel dans la string binaire 
		while (longueur < total) {
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
			output += runner->data;
		}
		return output;
	}
};
