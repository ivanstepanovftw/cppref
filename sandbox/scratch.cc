#include <iostream>
#include <list>
#include <vector>
#include <algos/vector.hh>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <memory>
#include <regex>
#include <cmath>
#include <iterator>
#include <algos/binary_search_tree.hh>


int main() {
    using std::cout, std::endl;
    using tree = binary_search_tree<char>;

    std::vector;
    /*
     *       G
     *   C     I
     * A   E  H  K
     *  B D F   J
     */
    tree t;
    t.insert('G');
    t.insert('C');
    t.insert('A');
    t.insert('B');
    t.insert('E');
    t.insert('D');
    t.insert('F');
    t.insert('I');
    t.insert('H');
    t.insert('K');
    t.insert('J');
    t.DFS_LNR([](tree::Node *node) { std::cout << node->value << ","; });
    std::cout<<"\b"<<std::endl;
    // tree::Node *n;
    // t.DFS_LNR([&](tree::Node *node) { if (node->value == 'J') { n=node; } });
    // if (!n)
    //     throw std::runtime_error("node not found :(");
    // std::cout<<"n->value: "<<n->value<<std::endl;
    // t.erase(n);
    t.erase('J');
    t.DFS_LNR([](tree::Node *node) { std::cout << node->value << ","; });
    std::cout<<"\b"<<std::endl;
}
