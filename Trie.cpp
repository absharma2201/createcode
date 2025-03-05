// C++ implementation of delete
// operations on Trie using unordered_map
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

// trie node
struct TrieNode {
    unordered_map<char, TrieNode*> children;
    bool isEndOfWord;

    TrieNode() : isEndOfWord(false) {}
};

// Returns new trie node
TrieNode* getNode() {
    return new TrieNode();
}

// If not present, inserts key into trie
// If the key is prefix of trie node, just
// marks leaf node
void insert(TrieNode* root, const string& key) {
    TrieNode* pCrawl = root;

    for (char c : key) {
        if (pCrawl->children.find(c) == pCrawl->children.end()) {
            pCrawl->children[c] = getNode();
        }
        pCrawl = pCrawl->children[c];
    }

    // mark last node as leaf
    pCrawl->isEndOfWord = true;
}

// Returns true if key presents in trie, else
// false
bool search(TrieNode* root, const string& key) {
    TrieNode* pCrawl = root;

    for (char c : key) {
        if (pCrawl->children.find(c) == pCrawl->children.end()) {
            return false;
        }
        pCrawl = pCrawl->children[c];
    }

    return (pCrawl != nullptr && pCrawl->isEndOfWord);
}

// Returns true if root has no children, else false
bool isEmpty(TrieNode* root) {
    return root->children.empty();
}

// Recursive function to delete a key from given Trie
TrieNode* remove(TrieNode* root, const string& key, int depth = 0) {
    // If tree is empty
    if (!root)
        return nullptr;

    // If last character of key is being processed
    if (depth == key.size()) {

        // This node is no more end of word after
        // removal of given key
        if (root->isEndOfWord)
            root->isEndOfWord = false;

        // If given is not prefix of any other word
        if (isEmpty(root)) {
            delete root;
            root = nullptr;
        }

        return root;
    }

    // If not last character, recur for the child
    // obtained using the character
    char c = key[depth];
    if (root->children.find(c) != root->children.end()) {
        root->children[c] = remove(root->children[c], key, depth + 1);
    }

    // If root does not have any child (its only child got
    // deleted), and it is not end of another word.
    if (isEmpty(root) && !root->isEndOfWord) {
        delete root;
        root = nullptr;
    }

    return root;
}

// Driver
int main() {
    // Input keys
    string keys[] = { "the", "a", "there",
                        "answer", "any", "by",
                        "bye", "their", "hero", "heroplane" };
    int n = sizeof(keys) / sizeof(keys[0]);

    TrieNode* root = getNode();

    // Construct trie
    for (int i = 0; i < n; i++)
        insert(root, keys[i]);

    // Search for different keys
    search(root, "the") ? cout << "Yes\n" : cout << "No\n";
    search(root, "these") ? cout << "Yes\n" : cout << "No\n";

    remove(root, "heroplane");
    search(root, "hero") ? cout << "Yes\n" : cout << "No\n";

    return 0;
}
