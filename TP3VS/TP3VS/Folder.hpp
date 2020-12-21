#include <vector>

#include "Note.hpp"

using namespace std;

class Folder {
private:
	string name;
	vector<Folder*> folders;
	vector<Note*> notes;

public:
	// TODO : Implémentation des méthodes nécessaires
	Folder(string name = "") {
		this->name = name;
	}

	~Folder() {
		while (folders.size()) {
			delete folders.front();
			removeFolder(folders.front());
		}
		while (notes.size()) {
			delete notes.front();
			removeNote(notes.front());
		}
	}
	/*
	void swap(Folder* a, Folder* b) {
		Folder* tmp = a;
		a = b;
		b = tmp;
	}

	int partition(vector<Folder*> arr, int low, int high) {
		string pivot = arr[high]->getName();
		int i = (low - 1);
		int size = 0;
		
		

		for (int j = low; j < high; j++) {
			int l = 0;
			bool change = false;
			if (arr[j]->getName()[l] == pivot[l]) {
				if (arr[j]->getName().size() >= pivot.size()) {
					size = arr[j]->getName().size();
				}
				else
					size = pivot.size();
				while (l < size && !change) {
					if (arr[j]->getName()[l] <= pivot[l]) {
						i++;
						swap(arr[i], arr[j]);
						change = true;
					}
				l++;
				}
			}
			else {
				if (arr[j]->getName()[l] >= pivot[l]) {
					i++;
					swap(arr[i], arr[j]);
				}
			}
			
			
		}
		swap(arr[i + 1], arr[high]);
		return (i + 1);
	}

	void quicksort(vector<Folder*> arr, int low, int high) {
		if (low < high) {
			int pi = partition(arr, low, high);
			quicksort(arr, low, pi - 1);
			quicksort(arr, pi + 1, high);
		}
	}*/


	void sortFolders() {
		quicksortFolders(0, folders.size() - 1);
	}

	void quicksortFolders(int g, int d) {
		Folder* pivot = folders[d];
		Folder* transit;
		int gauche = g;
		int droite = d;

		if (gauche < droite) {
			for (int i = g; i < d; i++) {
				if (folders[i]->name < pivot->name) {
					transit = folders[g];
					folders[g] = folders[i];
					folders[i] = transit;
					g++;
				}
			}
			transit = folders[g];
			folders[g] = folders[d];
			folders[d] = transit;

			int index = g;

			quicksortFolders(gauche, index - 1);
			quicksortFolders(index + 1, droite);
		}
	}

	void quicksortNotes(int g, int d) {
		Note* pivot = notes[d];
		Note* transit;
		int gauche = g;
		int droite = d;

		if (gauche < droite) {
			for (int i = g; i < d; i++) {
				if (notes[i]->getName() < pivot->getName()) {
					transit = notes[g];
					notes[g] = notes[i];
					notes[i] = transit;
					g++;
				}
			}
			transit = notes[g];
			notes[g] = notes[d];
			notes[d] = transit;

			int index = g;

			quicksortNotes(gauche, index - 1);
			quicksortNotes(index + 1, droite);
		}
	}

	void add(Folder* folder) {
		this->folders.push_back(folder);
		quicksortFolders(0, folders.size() - 1);
	}

	void removeFolder(Folder* folder) {
		int index = 0;
		auto it = find(folders.begin(), folders.end(), folder);
		if (it != folders.end()) {
			index = it - folders.begin();
			folders.erase(folders.begin() + index);
		}
	}

	void removeNote(Note* note) {
		int index = 0;
		auto it = find(notes.begin(), notes.end(), note);
		if (it != notes.end()) {
			index = it - notes.begin();
			notes.erase(notes.begin() + index);
		}
	}

	void add(Note* note) {
		this->notes.push_back(note);
		quicksortNotes(0, notes.size() - 1);
	}

	vector<Folder*> getFolders() {
		return this->folders;
	}

	vector<Note*> getNotes() {
		return this->notes;
	}

	int folderSize() {
		return folders.size();
	}

	int noteSize() {
		return notes.size();
	}

	string getName() {
		if (name.length()) { 
			return this->name; 
		}
		else {
			return this->name = " ";
		}
	}

	Folder* get(int index) {
		return folders.at(index);
	}

	Note* getN(int index) {
		return notes.at(index);
	}

	void setName(string name) {
		if (name.length()) {
			this->name = name;
		}
	}


};
