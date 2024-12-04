#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <queue>
#include <cerrno>
using namespace std;

struct TreeNode {
    string data;
    string translation;
    string wordType;
    TreeNode* left;
    TreeNode* right;

    TreeNode(string value, string meaning, string type) : data(value), translation(meaning), wordType(type), left(nullptr), right(nullptr) {}
};

struct BinaryTree {
    TreeNode* root = nullptr;

    void printInOrder(TreeNode* node) {
        if (node == nullptr) {
            return;
        }

        printInOrder(node->left);
        cout << node->data << " ";
        printInOrder(node->right);
    }

    void insert(string value, string meaning, string type) {
        TreeNode* newNode = new TreeNode(value, meaning, type);

        if (root == nullptr) {
            root = newNode;
            return;
        }

        TreeNode* currentNode = root;

        while (true) {
            if (value < currentNode->data) {
                if (currentNode->left == nullptr) {
                    currentNode->left = newNode;
                    break;
                }
                currentNode = currentNode->left;
            }
            else {
                if (currentNode->right == nullptr) {
                    currentNode->right = newNode;
                    break;
                }
                currentNode = currentNode->right;
            }
        }
    }

    string searchBST(string word) {
        TreeNode* currentNode = root;

        while (true) {
            if (word < currentNode->data) {
                if (currentNode->left == nullptr) {
                    return "";
                }
                currentNode = currentNode->left;
            }
            else if (word == currentNode->data) return currentNode->translation;
            else {
                if (currentNode->right == nullptr) {
                    return "";
                }
                currentNode = currentNode->right;
            }
        }
    }

    string searchTypeBST(string word) {
        TreeNode* currentNode = root;

        while (true) {
            if (word < currentNode->data) {
                if (currentNode->left == nullptr) {
                    return "";
                }
                currentNode = currentNode->left;
            }
            else if (word == currentNode->data) return currentNode->wordType;
            else {
                if (currentNode->right == nullptr) {
                    return "";
                }
                currentNode = currentNode->right;
            }
        }
    }

    void translate(string sentences) {
        stringstream ss(sentences);
        string kata;
        vector<string> wordList;
        bool wrong = false;

        while (ss >> kata) {
            wordList.push_back(kata);
        }

        string arr[4] = {}; //SPOK
        for (int i = 0; i < wordList.size(); i++) {
            string k = wordList[i];
            string n = wordList[i];
            if (i < wordList.size() - 1) {
                string merged = k + wordList[i + 1];
                if (searchBST(merged) == "") {
                    if (searchBST(k) == "") {
                        cout << "\nKata " << wordList[i] << " belum terdaftar di dictionary";
                        wrong = true;
                        break;
                    }
                    else {
                        k = searchBST(k);
                    }

                }
                else {
                    k = searchBST(merged);
                    n = merged;
                    i++;
                }
            }
            else k = searchBST(k);



            string t = searchTypeBST(n);
            if (arr[0] == "" && t == "s") {
                arr[0] = k;
                cout << k << " ";
            }
            else if (arr[1] == "" && t == "p") {
                arr[1] = k;
                cout << k << " ";
            }
            else if (arr[2] == "" && t == "s") {
                arr[2] = k;
                cout << k << " ";
            }
            else if (arr[3] == "" && t == "k") {
                arr[3] = k;

                cout << k << " ";
            }
        }
        if (!wrong) {
            cout << endl << "Auto-Correct: ";
            for (int i = 0; i < 4; i++)
            {
                if (arr[i] != "") {
                    cout << arr[i] << " ";
                }
            }
        }

        else wrong = false;
        cout << endl;
    }

    bool editWord(const string& oldWord, const string& newWord, const string& newTranslation, const string& newType) {
        TreeNode* nodeToEdit = findNode(root, oldWord);

        if (nodeToEdit != nullptr) {
            string oldValue = nodeToEdit->data;
            string oldTranslation = nodeToEdit->translation;
            string oldType = nodeToEdit->wordType;

            deleteNode(root, oldWord);
            insert(newWord, newTranslation, newType);

            return true;
        }
        else {
            cout << "Word not found in the dictionary." << endl;
            return false;
        }
    }


    bool deleteWord(const string& word) {
        return deleteNode(root, word);
    }


private:
    TreeNode* findNode(TreeNode* node, const string& word) {
        if (node == nullptr) {
            return nullptr;
        }

        if (word < node->data) {
            return findNode(node->left, word);
        }
        else if (word > node->data) {
            return findNode(node->right, word);
        }
        else {
            return node;
        }
    }

    bool deleteNode(TreeNode*& node, const string& word) {
        if (node == nullptr) {
            return false;
        }

        if (word < node->data) {
            return deleteNode(node->left, word);
        }
        else if (word > node->data) {
            return deleteNode(node->right, word);
        }
        else {
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                node = nullptr;
            }
            else if (node->left == nullptr) {
                TreeNode* temp = node;
                node = node->right;
                delete temp;
            }
            else if (node->right == nullptr) {
                TreeNode* temp = node;
                node = node->left;
                delete temp;
            }
            else {
                TreeNode* minNode = findMin(node->right);
                node->data = minNode->data;
                node->translation = minNode->translation;
                node->wordType = minNode->wordType;
                deleteNode(node->right, minNode->data);
            }

            return true;
        }
    }

    TreeNode* findMin(TreeNode* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }
};


struct File {
    void saveToFileRecursive(TreeNode* node, ofstream& file) {
        if (node == nullptr) {
            return;
        }

        file << node->data << " " << node->translation << " " << node->wordType << endl;

        saveToFileRecursive(node->left, file);
        saveToFileRecursive(node->right, file);
    }


    void insertFromFile(const string& filename, BinaryTree& englishTree, BinaryTree& bahasaTree) {
        ifstream file(filename);
        string line;

        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }

        while (getline(file, line)) {
            stringstream ss(line);
            string value, meaning, type;

            ss >> value >> meaning >> type;

            englishTree.insert(value, meaning, type);
            bahasaTree.insert(meaning, value, type);
        }

        file.close();
        ofstream clearFile(filename, ios::trunc);
        clearFile.close();
    }

    void saveToFile(const string& filename, BinaryTree englishTree) {
        ofstream file(filename);

        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }

        saveToFileRecursive(englishTree.root, file);

        file.close();
    }

};


struct Menu
{
    void startMenu(BinaryTree englishTree, BinaryTree bahasaTree) {
        File dictionaryFile;
        dictionaryFile.insertFromFile("C:/Users/aurum/Downloads/dictionary.txt", englishTree, bahasaTree);

        string bahasa, english, type, translation, oldWord, newWord, newTranslation, newType;

        while (true) {
            cout << "Dictionary Menu:\n1. Tambah Kata\n2. Terjemahan English ke Bahasa\n3. Terjemahan Bahasa ke English\n4. Perbarui Kata\n5. Hapus Kata\n6. Daftar Kata\n7. Keluar\nPilihan: ";
            int choice;
            cin >> choice;
            cout << endl;

            cin.ignore();
            if (choice == 1) {
                cout << "Kata dalam Bahasa Indonesia: ";
                cin >> bahasa;

                cout << "Terjemahan dalam Bahasa Inggris: ";
                cin >> english;
                cout << "Tipe Kata (S/P/O/K): ";
                cin >> type;
                if (type == "o") type = "s";

                englishTree.insert(english, bahasa, type);
                bahasaTree.insert(bahasa, english, type);



                cout << "Kata telah disimpan dalam Dictionary!" << endl;
            }
            else if (choice == 2) {
                cout << "English:\n1. Word Translation\n2. Sentence Translation\nChoice: ";
                cin >> choice;
                cin.ignore();
                if (choice == 1) {
                    cout << "Word : ";
                    cin >> english;
                    translation = englishTree.searchBST(english);
                    if (translation != "") {
                        cout << "Translation: " << translation << endl;
                    }
                    else {
                        cout << "Word " << english << " does not exist in Dictionary\n";
                    }
                }
                else if (choice == 2) {
                    cout << "Sentence : ";
                    getline(cin, english);
                    cout << "Translation: ";
                    englishTree.translate(english);
                }
                else {
                    cout << "Choice does not exist!";
                    return;
                }
            }
            else if (choice == 3) {
                cout << "Bahasa:\n1. Terjemahan Kata\n2. Terjemahan Kalimat\nPilihan: ";
                cin >> choice;
                cin.ignore();
                if (choice == 1) {
                    cout << "Kata : ";
                    cin >> bahasa;
                    translation = bahasaTree.searchBST(bahasa);
                    if (translation != "") {
                        cout << "Terjemahan: " << translation << endl;
                    }
                    else {
                        cout << "Kata " << bahasa << " tidak ada di dalam Dictionary\n";
                    }
                }
                else if (choice == 2) {
                    cout << "Kalimat : ";
                    getline(cin, bahasa);
                    cout << "Terjemahan: ";
                    bahasaTree.translate(bahasa);
                }
                else {
                    cout << "Pilihan tidak tersedia!";
                    return;
                }
            }
            else if (choice == 4) {
                cout << "Bahasa:\n1. Input dalam Bahasa Indonesia\n2. Input in English\nChoice: ";
                cin >> choice;
                cin.ignore();
                if (choice == 1) {
                    cout << "Kata yang akan diperbarui:";
                    cin >> oldWord;
                    translation = bahasaTree.searchBST(oldWord);
                    if (translation != "") {
                        cout << "Kata baru: ";
                        cin >> newWord;
                        cout << "Terjemahan kata baru: ";
                        cin >> newTranslation;
                        cout << "Tipe kata baru: ";
                        cin >> newType;
                        englishTree.editWord(translation, newTranslation, newWord, newType);

                        cout << "Kata " << oldWord << " diperbarui";
                    }
                    else {
                        cout << "Kata tidak ditemukan di Dictionary" << endl;
                    }
                }
                else if (choice == 2) {
                    cout << "Word to be update:";
                    cin >> oldWord;
                    translation = englishTree.searchTypeBST(oldWord);
                    if (translation != "") {
                        cout << "New word: ";
                        cin >> newWord;
                        cout << "New translate word: ";
                        cin >> newTranslation;
                        cout << "New word type: ";
                        cin >> newType;
                        englishTree.editWord(oldWord, newWord, newTranslation, newType);

                        cout << "Word " << oldWord << " updated";
                    }
                    else {
                        cout << "Word does not exist in Dictionary" << endl;
                    }
                }
                else {
                    cout << "Pilihan tidak tersedia!";
                    return;
                }
            }
            else if (choice == 5) {
                cout << "Bahasa:\n1. Input in English\n2. Input dalam Bahasa Indonesia\nPilihan: ";
                cin >> choice;
                cin.ignore();
                if (choice == 1) {
                    cout << "Word to be delete:";
                    cin >> oldWord;
                    if (englishTree.deleteWord(oldWord)) {
                        cout << "Word " << oldWord << " deleted" << endl;
                        string temp = englishTree.searchBST(oldWord);
                        bahasaTree.deleteWord(temp);
                    }
                    else {
                        cout << "Word does not exist in Dictionary" << endl;
                    }
                }
                else if (choice == 2) {
                    cout << "Kata yang akan dihapus:";
                    cin >> oldWord;
                    if (bahasaTree.deleteWord(oldWord)) {
                        cout << "Kata " << oldWord << " dihapus" << endl;
                        string temp = bahasaTree.searchBST(oldWord);
                        englishTree.deleteWord(temp);
                    }
                    else {
                        cout << "Kata tidak ada di dalam dictionary" << endl;
                    }
                }
            }
            else if (choice == 6) {
                cout << "Bahasa:\n1. English Tree\n2. Bahasa Tree\nPilihan: ";
                cin >> choice;
                if (choice == 1) {
                    englishTree.printInOrder(englishTree.root);
                }
                else if (choice == 2) {
                    bahasaTree.printInOrder(bahasaTree.root);
                }
                else {
                    cout << "Pilihan tidak tersedia";
                }
            }
            else if (choice == 7) {
                dictionaryFile.saveToFile("C:/Users/aurum/Downloads/dictionary.txt", englishTree);
                break;
            }
            else cout << "Pilihan tidak tersedia\n";
            cout << endl;
        }
    }
};

int main() {
    BinaryTree englishTree;
    BinaryTree bahasaTree;

    Menu Dictionary;

    Dictionary.startMenu(englishTree, bahasaTree);

    return 0;
}