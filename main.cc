#include <iostream>
#include <set>
#include <cassert>
#include <algorithm>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Trie.h"

using namespace std;

bool traverse(vector<string> a, vector<string>& b, string& s, int k, int i, int j) {
  if (i < 0 || i >= a.size() || j < 0 || j >= a[0].size() || a[i][j] != s[k]) {
    return false;
  }
  a[i][j] = '*';
  if (k == s.size() - 1) {
    b = a;
    return true;
  }

  char c = a[i][j];
  bool ans = traverse(a, b, s, k + 1, i + 1, j) ||
    traverse(a, b, s, k + 1, i, j + 1) ||
    traverse(a, b, s, k + 1, i, j - 1) ||
    traverse(a, b, s, k + 1, i - 1, j) ||
    traverse(a, b, s, k + 1, i + 1, j + 1) ||
    traverse(a, b, s, k + 1, i - 1, j - 1) ||
    traverse(a, b, s, k + 1, i - 1, j + 1) ||
    traverse(a, b, s, k + 1, i + 1, j - 1);
  a[i][j] = c;
  return ans;
}

void drop(vector<string> &puzzle) {
  const int columnSize = puzzle.size(), rowSize = puzzle[0].size();
  for (int columnIndex = 0; columnIndex < columnSize; columnIndex++) {
    int i = rowSize - 1;
    int j = rowSize - 1;
    while (true) {
      while (i >= 0 && puzzle[i][columnIndex] != '*') {
        i--;
        j--;
      }
      while (j >= 0 && puzzle[j][columnIndex] == '*') {
        j--;
      }
      if (j < 0) {
        break;
      } else {
        puzzle[i][columnIndex] = puzzle[j][columnIndex];
        puzzle[j][columnIndex] = '*';
      }
    }
  }
}

bool exist(vector<string>& a, vector<string>& b, string s) {
  for (int i = 0; i < a.size(); i++) {
    for (int j = 0; j < a[0].size(); j++) {
      //if (a[i][j] == 'v') cout << s[0] << endl;
      if (traverse(a, b, s, 0, i, j)) return true;
    }
  }
  return false;
}

vector<string> split (const string &s, char delim = ' ') {
  vector<string> result;
  stringstream ss (s);
  string item;

  while (getline (ss, item, delim)) {
    result.push_back (item);
  }

  return result;
}

map<string, string> from_word;

bool solve(vector<string>& puzzle, vector<vector<string>>& wordlist, bool& ans, int level = 0, int cnt = 0, string last = "") {
  if (level == wordlist.size() && cnt == puzzle[0].size() * puzzle.size()){
    ans = true;
    while (last != "") {
      cout << last << ' ';
      last = from_word[last];
    }
    cout << endl;
    return true;
  }
  drop(puzzle);
  auto p = puzzle;
  for (auto& it : wordlist[level]) {
    vector<string> b;
    if (exist(p, b, it)) {
      from_word[it] = last;
      if (!solve(b, wordlist, ans, level + 1, cnt + it.size(), it)) {
        from_word[it] = "";
      }
    }
  }
  return ans;
}

int main(int argc, char** argv) {
  if (argc > 1) {
    ifstream f1(string{argv[1]});
    ifstream f2(string{argv[2]});
    string word;
    Trie* small = new Trie();
    Trie* large = new Trie();
    while (f1 >> word) small->addWord(word);
    while (f2 >> word) large->addWord(word);
    string res;
    string line;
    vector<string> tmp;
    vector<pair<vector<string>, vector<string>>> p;
    while (getline(cin, line)) {
      if (line.find(" ") != string::npos) {
        p.push_back({tmp, split(line)});
        tmp.clear();
      }
      else {
        tmp.push_back(line);
      }
    }
    for (auto& [f, s] : p) {
      vector<vector<string>> sm, lg;
      for (auto& it : s) {
        vector<string> t_sm, t_lg;
        small->search(it, t_sm);
        large->search(it, t_lg);
        sm.push_back(t_sm);
        lg.push_back(t_lg);
      }
      bool ans = false;
      solve(f, sm, ans);
      if (!ans) {
        solve(f, lg, ans);
        if (!ans) cout << "Couldn't solve :c\n";
        else cout << ".\n";
      }
      else cout << ".\n";
    }
  }
}
