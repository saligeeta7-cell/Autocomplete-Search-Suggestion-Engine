#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Trie Node Structure
struct TrieNode {
    unordered_map<char, TrieNode*> children;
    bool isEndOfWord;
    int frequency;

    TrieNode() {
        isEndOfWord = false;
        frequency = 0;
    }
};

// Structure to store suggestion and frequency for sorting
struct Suggestion {
    string word;
    int frequency;
};

class Trie {
private:
    TrieNode* root;

    // DFS to collect all words starting with the given prefix
    void dfs(TrieNode* node, string prefix, vector<Suggestion>& results) {
        if (node->isEndOfWord) {
            results.push_back({prefix, node->frequency});
        }
        for (auto& pair : node->children) {
            dfs(pair.second, prefix + pair.first, results);
        }
    }

public:
    Trie() {
        root = new TrieNode();
    }

    // Insert a keyword into the Trie with a popularity frequency
    void insert(string word, int freq) {
        TrieNode* node = root;
        for (char ch : word) {
            ch = tolower(ch);
            if (node->children.find(ch) == node->children.end()) {
                node->children[ch] = new TrieNode();
            }
            node = node->children[ch];
        }
        node->isEndOfWord = true;
        node->frequency = freq;
    }

    // Get sorted autocomplete suggestions based on prefix
    vector<string> autocomplete(string prefix) {
        TrieNode* node = root;
        for (char ch : prefix) {
            ch = tolower(ch);
            if (node->children.find(ch) == node->children.end()) {
                return {}; // Prefix not found
            }
            node = node->children[ch];
        }

        vector<Suggestion> results;
        dfs(node, prefix, results);

        // Sort suggestions by frequency in descending order (highest popularity first)
        sort(results.begin(), results.end(), [](const Suggestion& a, const Suggestion& b) {
            return a.frequency > b.frequency;
        });

        vector<string> sortedWords;
        for (auto& s : results) {
            sortedWords.push_back(s.word);
        }
        return sortedWords;
    }
};

int main() {
    Trie trie;

    // Pre-loaded dataset of keywords with frequencies
    trie.insert("data structures", 50);
    trie.insert("database management", 40);
    trie.insert("database security", 30);
    trie.insert("datapath architecture", 20);
    trie.insert("machine learning", 60);
    trie.insert("mac os", 25);
    trie.insert("java programming", 45);
    trie.insert("javascript framework", 35);

    cout << "===========================================" << endl;
    cout << "   AUTOCOMPLETE & SEARCH SUGGESTION ENGINE " << endl;
    cout << "===========================================" << endl;

    while (true) {
        string query;
        cout << "\nEnter search prefix (or type 'exit' to quit): ";
        cin >> query;

        if (query == "exit" || query == "EXIT") {
            break;
        }

        vector<string> suggestions = trie.autocomplete(query);
        cout << "Suggestions for '" << query << "':" << endl;
        if (!suggestions.empty()) {
            int rank = 1;
            for (const string& sug : suggestions) {
                cout << "  " << rank++ << ". " << sug << endl;
            }
        } else {
            cout << "  No suggestions found." << endl;
        }
    }

    return 0;
}
