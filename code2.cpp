#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cctype>

using namespace std;

struct TreeNode {
    string word;
    int count;
    TreeNode* left;
    TreeNode* right;
};
 int totalWords=0;

class BST {
private:
    TreeNode* root;
   
    TreeNode* insert(TreeNode* node, const string& word) {
        if (node == nullptr) {
            node = new TreeNode();
            node->word = word;
            node->count = 1;
            node->left = nullptr;
            node->right = nullptr;
        } else if (word < node->word) {
            node->left = insert(node->left, word);
        } else if (word > node->word) {
            node->right = insert(node->right, word);
        } else {
            node->count++;
        }
        return node;
    }

    void traverse(TreeNode* node, ostream& out) {
        if (node != nullptr) {
            traverse(node->left, out);
            out << node->word << " " << node->count << endl;
            traverse(node->right, out);
        }
    }

     void countTotalWords(TreeNode* node) {
        if (node == nullptr) return;
        totalWords += node->count;
        countTotalWords(node->left);
        countTotalWords(node->right);
    }

    int countUniqueWords(TreeNode* node) {
        if (node == nullptr) return 0;
        return 1 + countUniqueWords(node->left) + countUniqueWords(node->right);
    }

    int countLongWords(TreeNode* node) {
        if (node == nullptr) return 0;
        int count = 0;
        if (node->word.length() > 3) count++;
        count += countLongWords(node->left);
        count += countLongWords(node->right);
        return count;
    }

    int calculateTotalWordLength(TreeNode* node) {
        if (node == nullptr) return 0;
        return node->word.length() * node->count +
               calculateTotalWordLength(node->left) +
               calculateTotalWordLength(node->right);
    }

public:
    BST() : root(nullptr) {}

    void insert(const string& word) {
        root = insert(root, word);
    }

    void printIndex(ostream& out) {
        traverse(root, out);
    }

    int getTotalWords() {
        countTotalWords(root);
        return totalWords;
    }

    int getUniqueWords() {
        return countUniqueWords(root);
    }

    int getLongWords() {
        return countLongWords(root);
    }

    double getAverageWordLength() {
        int totalLength = calculateTotalWordLength(root);
        int totalWords = getTotalWords();
        return static_cast<double>(totalLength) / totalWords;
    }

    void clear() {
        clear(root);
        root = nullptr;
    }

    ~BST() {
        clear();
    }

private:
    void clear(TreeNode* node) {
        if (node != nullptr) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }
};

string sanitizeWord(const string& word) {
    string sanitizedWord;
    for (char c : word) {
        if (isalpha(c) || c == '\'' || c == '"') {
            sanitizedWord += tolower(c);
        }
    }
    return sanitizedWord;
}

int main() {
    string filename;
    cout << "Enter the name of the file: ";
    getline(cin, filename);

    ifstream file(filename);
    if (!file) {
        cerr << "Error: Unable to open file " << filename << endl;
        return 1;
    }

    BST bst;

    string word;
    while (file >> word) {
        string sanitized = sanitizeWord(word);
        if (!sanitized.empty()) {
            bst.insert(sanitized);
        }
    }

    file.close();

    ofstream outFile("index.txt");
    if (!outFile) {
        cerr << "Error: Unable to create output file." << endl;
        return 1;
    }

    outFile << "File: " << filename << endl;
    outFile << "Lexicon with word frequencies:\n";
    bst.printIndex(outFile);
    outFile << "Statistical summary:\n";
    outFile << "Total number of words: " << bst.getTotalWords() << endl;
    outFile << "Number of unique words: " << bst.getUniqueWords() << endl;
    outFile << "Number of unique words of more than three letters: " << bst.getLongWords() << endl;
    outFile << "Average word length: " << bst.getAverageWordLength() << endl;

    cout << "Process completed. Results written to index.txt" << endl;

    return 0;
}
