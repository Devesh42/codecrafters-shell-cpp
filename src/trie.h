#ifndef TRIE_H
#define TRIE_H

#include <string>
#define BUFFER_SIZE 256

class TrieNode{
    public:
    TrieNode* children[BUFFER_SIZE];
    bool isWord;

    TrieNode(){
        for(int i=0; i<BUFFER_SIZE; i++)
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