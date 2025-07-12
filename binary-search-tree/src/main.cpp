#include "./include/binary_search_tree.hpp"
#include "./include/huffman.hpp"

using namespace std;

int main() {
  ifstream input("input.txt");
  ifstream exam_input("exam_input.txt");
  ofstream output("output.txt");

  BinarySearchTree bst(exam_input);
  bst.visit(bst.get_root(), Visit::preorder);
  bst.visit(bst.get_root(), Visit::preorder, output);

  cout << endl;
  bst.print_predecessor(bst.get_root());
  bst.print_successor(bst.get_root());

  int key_search = 5;
  auto node_search = bst.search(bst.get_root(), key_search);
  if (node_search) {
    cout << "Found node with key " << key_search << " => ";
    node_search->print();
  } else
    cout << "Node with key " << key_search << " not found";
  cout << endl;

  Huffman h(exam_input);
  h.print_codes();
  string huffman_input_str = "ACE";
  h.print_encode(huffman_input_str);
  h.print_decode(h.encode(huffman_input_str));

  input.close();
  exam_input.close();
  output.close();

  return 0;
}