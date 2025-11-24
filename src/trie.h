#ifndef TRIE_H
#define TRIE_H

#include <string>

class TrieNode{
    public:
    TrieNode* children[26];
    bool isWord;

    TrieNode(){
        for(int i=0; i<26; i++)
            children[i] = nullptr;
        isWord = false;
    }
};

class Trie{
    public:
    TrieNode* root;
    Trie();
    void insert(std::string word);
    bool search(std::string word);
    std::string autoComplete(std::string prefix);
};


#endif //TRIE_H