#ifndef BINARY_SEARCH_TREE
#define BINARY_SEARCH_TREE

#include <fstream>
#include <sstream>
#include <string>

enum Visit { preorder, postorder, inorder };

#include "node.hpp"

class BinarySearchTree {
  Node* root;

  void format_line(std::string& line) {
    if (line.front() == '<') line = line.substr(1);
    if (line.back() == '>') line.pop_back();
    for (auto& c : line) {
      if (c == ',') c = ' ';
    }
  }

  void clear_stream(std::istringstream& stream) {
    stream.clear();
    stream.str("");
  }

  void delete_tree(Node*& node) {
    if (!node) return;

    delete_tree(node->get_left());
    delete_tree(node->get_right());
    delete node;
    node = nullptr;
  }

  void insert_recursive(Node*& start, Node* node) {
    if (!start) {
      start = node;
      return;
    }

    if (node->get_key() < start->get_key()) {
      insert_recursive(start->get_left(), node);
      start->get_left()->set_parent(start);
    } else {
      insert_recursive(start->get_right(), node);
      start->get_right()->set_parent(start);
    }
  }

  void inorder_visit(Node* node, std::ostream& out = std::cout) {
    if (!node) return;

    inorder_visit(node->get_left(), out);
    node->print(out);
    inorder_visit(node->get_right(), out);
  }

  void preorder_visit(Node* node, std::ostream& out = std::cout) {
    if (!node) return;

    node->print(out);
    preorder_visit(node->get_left(), out);
    preorder_visit(node->get_right(), out);
  }

  void postorder_visit(Node* node, std::ostream& out = std::cout) {
    if (!node) return;

    postorder_visit(node->get_left(), out);
    postorder_visit(node->get_right(), out);
    node->print(out);
  }

public:
  BinarySearchTree(std::ifstream& input) : root(nullptr) { load(input); }

  ~BinarySearchTree() { delete_tree(root); }

  Node* get_root() const { return root; }

  void load(std::ifstream& input) {
    delete_tree(root);
    input.clear();
    input.seekg(0, std::ios::beg);

    std::string line;
    while (std::getline(input, line)) {
      format_line(line);
      std::istringstream iss(line);

      int key;
      char character;
      iss >> key >> character;
      insert(new Node(key, character));

      clear_stream(iss);
    }
  }

  void insert(Node* node) {
    insert_recursive(root, node);
    std::cout << "[insert INFO] Inserted node ";
    node->print();
  }

  void visit(Visit type, Node* node = nullptr, std::ostream& out = std::cout) {
    if (!node && !root) {
      std::cerr << "[visit ERROR] Tree is empty" << std::endl;
      return;
    }

    if (!node) node = root;

    switch (type) {
      case Visit::inorder: {
        out << "Inorder visit" << std::endl;
        inorder_visit(node, out);
        break;
      }

      case Visit::preorder: {
        out << "Preorder visit" << std::endl;
        preorder_visit(node, out);
        break;
      }

      case Visit::postorder: {
        out << "Postorder visit" << std::endl;
        postorder_visit(node, out);
        break;
      }

      default: {
        std::cerr << "[visit ERROR] Visit type invalid" << std::endl;
        break;
      }
    }
  }

  Node* tree_minimum(Node* node) {
    while (node->get_left()) node = node->get_left();
    return node;
  }

  Node* tree_maximum(Node* node) {
    while (node->get_right()) node = node->get_right();
    return node;
  }

  Node* predecessor(Node* node) {
    if (!node) {
      std::cerr << "[predecessor ERROR] Invalid node" << std::endl;
      return nullptr;
    }

    if (node->get_left()) return tree_maximum(node->get_left());

    Node* parent = node->get_parent();
    while (parent && node == parent->get_left()) {
      node = parent;
      parent = parent->get_parent();
    }

    return parent;
  }

  Node* successor(Node* node) {
    if (!node) {
      std::cerr << "[successor ERROR] Invalid node" << std::endl;
      return nullptr;
    }

    if (node->get_right()) return tree_minimum(node->get_right());

    Node* parent = node->get_parent();
    while (parent && node == parent->get_right()) {
      node = parent;
      parent = parent->get_parent();
    }

    return parent;
  }

  void print_successor(Node* node, std::ostream& out = std::cout) {
    Node* succ = successor(node);

    if (!succ)
      out << "Node (" << node->get_key() << ") => no successor found";
    else
      out << "Node (" << node->get_key() << ") => successor: (" << succ->get_key() << ")";
    out << std::endl;
  }

  void print_predecessor(Node* node, std::ostream& out = std::cout) {
    Node* pred = predecessor(node);

    if (!pred)
      out << "Node (" << node->get_key() << ") => no predecessor found";
    else
      out << "Node (" << node->get_key() << ") => predecessor: (" << pred->get_key() << ")";
    out << std::endl;
  }

  Node* search(Node* node, int key) {
    if (!node) {
      std::cerr << "[search ERROR] Node (" << key << ") not found" << std::endl;
      return nullptr;
    }

    if (node->get_key() == key) {
      std::cout << "[search INFO] Node (" << key << ") found" << std::endl;
      return node;
    }

    if (key < node->get_key())
      return search(node->get_left(), key);
    else
      return search(node->get_right(), key);
  }
};

#endif