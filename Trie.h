#pragma once

#include <vector>
#include <algorithm>
#include <string>
#include <map>

class Trie {
  private:
    struct Node {
      char val = '^';
      std::vector<Node*> children;
      bool is_leaf = false;
      Node() : children(26, nullptr) {}
      Node(char v) : val(v), children (26, nullptr) {}
    } *root;
    std::map<Node*, Node*> p;
  public:
    Trie();
    void addWord(std::string);
    bool search(std::string, std::vector<std::string>&, Node* = nullptr);
    std::string recover(Node*);
};

Trie::Trie() : root(new Node()) {
  p[root] = nullptr;
}

void Trie::addWord(std::string word) {
  Node* cur = this->root;
  for (int i = 0; i < word.size(); i++) {
    int idx = word[i] - 'a';
    if (cur->children[idx] == nullptr) {
      cur->children[idx] = new Node(word[i]);
    }
    cur = cur->children[idx];
  }
  cur->is_leaf = true;
}

bool Trie::search(std::string word, std::vector<std::string>& f, Node* cur) {
  if (cur == nullptr) cur = this->root;
  for (int i = 0; i < word.size(); i++) {
    if (word[i] == '*') {
      bool ans = false;
      for (auto it : cur->children) {
        if (it != nullptr) {
          p[it] = cur;
          ans |= search(word.substr(i + 1), f, it);
        }
      }
      return ans;
    }
    int idx = word[i] - 'a';
    if (cur->children[idx] == nullptr) return false;
    p[cur->children[idx]] = cur;
    cur = cur->children[idx];
  }
  if (cur != nullptr && cur->is_leaf) {
    f.push_back(recover(cur));
    return true;
  }
  return false;
}

std::string Trie::recover(Node* cur) {
  std::string res;
  while (cur != nullptr) {
    res.push_back(cur->val);
    cur = p[cur];
  }
  res.pop_back();
  reverse(res.begin(), res.end());
  return res;
}
