#include <string>
#include "Huffman.h"

using namespace std;

class Note {
private:
	string name;
	string content;
	
public:
	// TODO : Implémentation des méthodes nécessaires
	Note(string name, string content = "") {
		this->name = name;
		this->content = content;
	
	}

	string getName() {
		return this->name;
	}

	string getContent() {
		return this->content;
	}

	void setContent(string content) {
		this->content = content;
	}

	void setName(string name) {
		if (name.length()) {
			this->name = name;
		}
	}

	void encode() {
		if (this->content.compare("") != 0) {
			this->content = Huffman::encodeString(this->content);
		}
		
	}

	void decode() {
		if (this->content.compare("") != 0) {
			this->content = Huffman::decodeString(this->content);
		}
	}

	bool isEncoded() {
		if (content.substr(0, 3).compare("<K>") == 0) {
			return true;
		}
		return false;
	}


};
