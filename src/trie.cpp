#include "trie.h"
#include <iostream>

Trie::Trie()
{
    root = new TrieNode();
}
void Trie::insert(std::string word)
{
    TrieNode* curr = root;
    for(char& c : word)
    {
        int i = c - 'a';
        if(!(curr->children[i]))
            curr->children[i] = new TrieNode();
        curr = curr->children[i];
    }
    curr->isWord = true;
}
bool Trie::search(std::string word){
    TrieNode* curr = root;
    for(char& c : word)
    {
        int i = c - 'a';
        if(!(curr->children[i]))
            return false;
        curr = curr->children[i];
    }
    return curr->isWord;
}
std::string Trie::autoComplete(std::string prefix){
    TrieNode* curr = root;
    std::string completeString = prefix;
    for(char& c : prefix)
    {
        int i = c - 'a';
        if(!(curr->children[i]))
            return completeString;
        curr = curr->children[i];
    }
    //Full prefix matched
    if(curr->isWord)
        return completeString;
    else{
        while(!(curr->isWord))
        {
            for(int i=0; i < 26; i++)
            {
                if(curr->children[i])
                {
                    std::cout << char('a' + i);
                    completeString += char('a' + i);
                    curr = curr->children[i];
                }
            }
        }
        return completeString;
    }
}