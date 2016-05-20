//============================================================================
// Name        : patricia.cpp
// Author      : Andrey Sudnik
// Version     :
// Copyright   : 
// Description : Patricia (Radix) tree example
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/trie_policy.hpp>
#include <ext/pb_ds/tag_and_trait.hpp>

using namespace std;
using namespace __gnu_pbds;

typedef string_trie_e_access_traits<> cmp_fn;
typedef trie<string, null_mapped_type, cmp_fn, pat_trie_tag,
        trie_prefix_search_node_update> trie_type;

class Trie {
public:
    Trie() {};
    void print_prefix_match(const string&);
    void populate_from_file(const string&);
    void depopulate_from_file(const string&);
private:
    trie_type t;
};

void Trie::print_prefix_match(const string& key) {
    typedef trie_type::const_iterator const_iterator;
    typedef pair<const_iterator, const_iterator> pair_type;

    const pair_type match_range = t.prefix_range(key);
    for (const_iterator it = match_range.first; it != match_range.second; ++it)
        cout << *it << ' ';
    cout << endl;
}

string read_word(ifstream &file) {
    string word;
    char c;
    while (file.good()) {
        c = file.get();
        if (file.good()) {
            if (c != ' ' && c != '\n' && c != '.' && c != ',' && c != ':' &&
                c != ';' && c != '?' && c != '!' && c != ')' && c != '('  &&
                c != '>' && c != '<')
            {
                if (!(c == '-' && word.length() == 0))
                    word += c;
            } else {
                return word;
            }
        }
    }
    return word;
}

void Trie::populate_from_file(const string& fname) {
    cout<< "Reading " << fname << endl;
    ifstream infile;
    infile.open(fname.c_str(), ifstream::in);
    while (infile.good()) {
        t.insert(read_word(infile));
    }
    infile.close();
    cout<< "Tree size " << t.size()<< endl;
}

void Trie::depopulate_from_file(const string& fname) {
    ifstream infile;
    infile.open(fname.c_str(), ifstream::in);
    while (infile.good()) {
        t.erase(read_word(infile));
    }
    infile.close();
    cout<< "Tree size after erase " << t.size()<< endl;
}

int main() {
    Trie mybase;
    mybase.populate_from_file("GuideGalaxy01.txt");
    mybase.populate_from_file("GuideGalaxy02.txt");
    mybase.populate_from_file("GuideGalaxy03.txt");
    mybase.populate_from_file("GuideGalaxy04.txt");
    mybase.populate_from_file("GuideGalaxy05.txt");
    mybase.populate_from_file("Intel-1.txt");
    mybase.populate_from_file("Intel-2.txt");

    cout << "Ready to search" << endl;
    char line[256];
    do {
        cin.getline(line, 256);
        mybase.print_prefix_match(string(line));
    } while (string(line).compare(":q"));

    return 0;
}

