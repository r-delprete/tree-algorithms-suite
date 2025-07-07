#ifndef BINARY_SEARCH_TREE_HPP
#define BINARY_SEARCH_TREE_HPP

#include <fstream>
#include <sstream>
#include <string>

#include "logger.hpp"
#include "node.hpp"

enum Visit { inorder, preorder, postorder };

class BinarySearchTree {
  Node* root;

  void format_line(std::string& line) {
    if (line.front() == '<') line = line.substr(1);
    if (line.back() == '>') line.pop_back();
    for (auto& c : line) {
      if (c == ',') c = ' ';
    }
  }

  void delete_tree(Node*& node) {
    if (!node) return;

    delete_tree(node->get_left());
    delete_tree(node->get_right());
    delete node;
    node = nullptr;
  }

  void clear_stream(std::istringstream& stream) {
    stream.clear();
    stream.str("");
  }

  void clear_stream(std::ostringstream& stream) {
    stream.clear();
    stream.str("");
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

  void inorder_visit(Node* node, std::ostream& out) {
    if (!node) return;

    inorder_visit(node->get_left(), out);
    node->print(out);
    inorder_visit(node->get_right(), out);
  }

  void preorder_visit(Node* node, std::ostream& out) {
    if (!node) return;

    node->print(out);
    preorder_visit(node->get_left(), out);
    preorder_visit(node->get_right(), out);
  }

  void postorder_visit(Node* node, std::ostream& out) {
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
      char ch;

      iss >> key >> ch;
      insert_node(new Node(key, ch));
      clear_stream(iss);
    }
  }

  void insert_node(Node* node) { insert_recursive(root, node); }

  void visit(Visit visit, Node* node = nullptr, std::ostream& out = std::cout) {
    if (!node && !root) return;
    if (!node) node = root;

    out << (visit == Visit::inorder     ? "Inorder"
            : visit == Visit::postorder ? "Postorder"
                                        : "Preorder")
        << " visit" << std::endl;
    switch (visit) {
      case Visit::inorder: {
        inorder_visit(node, out);
        return;
      }
      case Visit::postorder: {
        postorder_visit(node, out);
        return;
      }
      case Visit::preorder: {
        preorder_visit(node, out);
        return;
      }
    }
  }

  Node* tree_minimum(Node*& node) {
    while (node->get_left()) node = node->get_left();
    return node;
  }

  Node* tree_maximum(Node*& node) {
    while (node->get_right()) node = node->get_right();
    return node;
  }

  Node* predecessor(Node* node) {
    if (!node) {
      log("[predecessor] => invalid node", LogLevel::ERROR);
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
      log("[successor] => invalid node", LogLevel::ERROR);
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
    std::ostringstream oss;
    if (!node) {
      oss << "[search] => no node found with key " << key;
      log(oss.str(), LogLevel::ERROR);
      clear_stream(oss);

      return nullptr;
    }

    if (node->get_key() == key) {
      oss << "Node (" << node->get_key() << " found";
      log(oss.str());
      clear_stream(oss);

      return node;
    }

    if (key < node->get_key()) return search(node->get_left(), key);
    return search(node->get_right(), key);
  }
};

#endif