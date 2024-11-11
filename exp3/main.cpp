#include <iostream>
#include <queue>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

class Bitmap {
private:
    vector<bool> bits;
public:
    Bitmap(int size = 8) : bits(size, false) {}

    void set(int pos, bool value) {
        if (pos < bits.size()) {
            bits[pos] = value;
        }
    }

    bool test(int pos) const {
        return pos < bits.size() ? bits[pos] : false;
    }

    void expand(int newSize) {
        if (newSize > bits.size()) {
            bits.resize(newSize, false);
        }
    }

    void print() const {
        for (bool bit : bits) {
            cout << (bit ? "1" : "0");
        }
    }

    int size() const {
        return bits.size();
    }
};

class HuffCode : public Bitmap {
public:
    HuffCode() : Bitmap(8) {}
    HuffCode(int n) : Bitmap(n) {}
};
struct HuffChar {
    char ch;
    int weight;
    HuffCode code;
    HuffChar(char c = '^', int w = 0) : ch(c), weight(w) {}
};

template <typename T>
struct BinNode {
    T data;
    BinNode<T>* left;
    BinNode<T>* right;

    BinNode(T e) : data(e), left(nullptr), right(nullptr) {}
};

class HuffTree {
private:
    BinNode<HuffChar>* root;

    void buildHuffTree(int* freq, int n) {
        struct CompareNode {
            bool operator()(BinNode<HuffChar>* a, BinNode<HuffChar>* b) {
                return a->data.weight > b->data.weight;
            }
        };

        priority_queue<BinNode<HuffChar>*, vector<BinNode<HuffChar>*>, CompareNode> pq;

        for (int i = 0; i < n; i++) {
            if (freq[i] > 0) {
                pq.push(new BinNode<HuffChar>(HuffChar(i + 0x20, freq[i])));
            }
        }

        while (pq.size() > 1) {
            BinNode<HuffChar>* left = pq.top(); pq.pop();
            BinNode<HuffChar>* right = pq.top(); pq.pop();

            BinNode<HuffChar>* parent = new BinNode<HuffChar>(HuffChar('^', left->data.weight + right->data.weight));
            parent->left = left;
            parent->right = right;

            pq.push(parent);
        }

        root = pq.empty() ? nullptr : pq.top();
    }
    void generateCodes(BinNode<HuffChar>* node, HuffCode& prefix, int length, HuffCode codeTable[]) {
        if (!node) return;

        if (!node->left && !node->right) {
            codeTable[node->data.ch - 0x20] = prefix;
            codeTable[node->data.ch - 0x20].expand(length);
            return;
        }

        if (node->left) {
            prefix.set(length, 0);
            generateCodes(node->left, prefix, length + 1, codeTable);
        }

        if (node->right) {
            prefix.set(length, 1);
            generateCodes(node->right, prefix, length + 1, codeTable);
        }
    }
public:
    HuffTree(int* freq, int n) {
        buildHuffTree(freq, n);
    }
    ~HuffTree() {
        delete root; 
    }
    void generateCodes(HuffCode codeTable[], int n) {
        if (!root) return;
        HuffCode prefix(n * 8);  
        generateCodes(root, prefix, 0, codeTable);
    }
    void printCodes(HuffCode codeTable[], int n) {
        cout << "Huffman编码表：\n";
        for (int i = 0; i < n; i++) {
            if (codeTable[i].size() > 0) {
                cout << char(i + 0x20) << ": ";
                codeTable[i].print();
                cout << "\n";
            }
        }
    }
};
void huffmanExample() {
    const int N_CHAR = 95; 
    string text = "aabaaab";
    ifstream file("word.txt");
    if (!file.is_open()) {
        cerr << "无法打开文件 word.txt" << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        text += line + "\n"; 
    }
    file.close();
    cout << "输入文本：" << text << endl;
    int freq[N_CHAR] = {0}; 
    for (char c : text) {
        if (c >= 0x20 && c <= 0x7e) {  
            freq[c - 0x20]++;
        }
    }
    HuffTree tree(freq, N_CHAR);
    HuffCode codeTable[N_CHAR];
    tree.generateCodes(codeTable, N_CHAR);
    tree.printCodes(codeTable, N_CHAR);
    HuffCode encodedText(text.length() * 8);  
    int encodedLen = 0;

    for (char c : text) {
        if (c >= 0x20 && c <= 0x7e) { 
            HuffCode& code = codeTable[c - 0x20];
            for (int j = 0; j < code.size(); j++) {
                encodedText.set(encodedLen++, code.test(j));  
            }
        }
    }
    cout << "\n编码结果: ";
    for (int i = 0; i < encodedLen; i += 8) {
        for (int j = 0; j < 8 && i + j < encodedLen; j++) {
            cout << (encodedText.test(i + j) ? "1" : "0");
        }
        cout << " ";
    }
    cout << endl;
}
int main() {
    huffmanExample();
    return 0;
}
