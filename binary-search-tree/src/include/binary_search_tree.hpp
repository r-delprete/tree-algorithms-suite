#ifndef BINARY_SEARCH_TREE
#define BINARY_SEARCH_TREE

#include <fstream>
#include <sstream>
#include <string>

enum Visit { preorder, postorder, inorder };

#include "node.hpp"

inline void format_line(std::string& line) {
  if (line.empty()) return;
  if (line.front() == '<') line = line.substr(1);
  if (line.back() == '>') line.pop_back();
  for (auto& ch : line) {
    if (ch == ',') ch = ' ';
  }
}

inline void clear_stream(std::istringstream& stream) {
  stream.clear();
  stream.str("");
}

class BinarySearchTree {
  shared_node root;

  void preorder_visit(const shared_node& node, std::ostream& out = std::cout) const {
    if (!node) return;

    node->print(out);
    preorder_visit(node->get_left(), out);
    preorder_visit(node->get_right(), out);
  }

  void inorder_visit(const shared_node& node, std::ostream& out = std::cout) const {
    if (!node) return;

    inorder_visit(node->get_left(), out);
    node->print(out);
    inorder_visit(node->get_right(), out);
  }

  void postorder_visit(const shared_node& node, std::ostream& out = std::cout) const {
    if (!node) return;

    postorder_visit(node->get_left(), out);
    postorder_visit(node->get_right(), out);
    node->print(out);
  }

  void insert_recursive(shared_node& start, const shared_node& node) {
    if (!start) {
      start = node;
      return;
    }

    if (node->get_key() < start->get_key()) {
      insert_recursive(start->get_left_ref(), node);
      start->get_left()->set_parent(start);
    } else {
      insert_recursive(start->get_right_ref(), node);
      start->get_right()->set_parent(start);
    }
  }

public:
  BinarySearchTree(std::ifstream& input) : root(nullptr) { load(input); }

  void delete_subtree(shared_node& node) {
    if (!node) return;

    delete_subtree(node->get_left_ref());
    delete_subtree(node->get_right_ref());
    node = nullptr;
  }

  shared_node get_root() const { return root; }

  void load(std::ifstream& input) {
    delete_subtree(root);
    input.clear();
    input.seekg(0, std::ios::beg);

    std::string line;
    while (std::getline(input, line)) {
      format_line(line);
      std::istringstream iss(line);
      int key;
      char ch;
      iss >> key >> ch;
      insert(create_node(key, ch));
      clear_stream(iss);
      line.clear();
    }
  }

  void insert(const shared_node& node) { insert_recursive(root, node); }

  shared_node tree_maximum(const shared_node& node) const {
    auto tmp = node;

    while (tmp->get_right()) tmp = tmp->get_right();
    return tmp;
  }

  shared_node tree_minimum(const shared_node& node) const {
    auto tmp = node;

    while (tmp->get_left()) tmp = tmp->get_left();
    return tmp;
  }

  shared_node get_predecessor(const shared_node& node) const {
    if (!node) {
      std::cerr << "[get_predecessor ERROR] Invalid node" << std::endl;
      return nullptr;
    }

    if (node->get_left()) return tree_maximum(node->get_left());

    auto tmp = node;
    auto parent = node->get_parent().lock();
    while (parent && tmp == parent->get_left()) {
      tmp = parent;
      parent = parent->get_parent().lock();
    }

    return parent;
  }

  shared_node get_successor(const shared_node& node) const {
    if (!node) {
      std::cerr << "[get_successor ERROR] Invalid node" << std::endl;
      return nullptr;
    }

    if (node->get_right()) return tree_minimum(node->get_right());

    auto tmp = node;
    auto parent = node->get_parent().lock();
    while (parent && tmp == parent->get_right()) {
      tmp = parent;
      parent = parent->get_parent().lock();
    }

    return parent;
  }

  shared_node search(const shared_node& node, const int key) const {
    if (!node) return nullptr;

    if (node->get_key() == key) return node;
    if (key < node->get_key()) return search(node->get_left(), key);
    return search(node->get_right(), key);
  }

  void print_predecessor(const shared_node& node, std::ostream& out = std::cout) const {
    auto predecessor = get_predecessor(node);

    out << "Predecessor for node (" << node->get_key() << ") => " << "(";
    if (predecessor)
      out << predecessor->get_key() << " - " << predecessor->get_character();
    else
      out << "NULL";
    out << ")" << std::endl;
  }

  void print_successor(const shared_node& node, std::ostream& out = std::cout) const {
    auto successor = get_successor(node);

    out << "Successor for node (" << node->get_key() << ") => " << "(";
    if (successor)
      out << successor->get_key() << " - " << successor->get_character();
    else
      out << "NULL";
    out << ")" << std::endl;
  }

  void visit(const shared_node& node, Visit visit, std::ostream& out = std::cout) const {
    out << (visit == Visit::inorder     ? "Inorder"
            : visit == Visit::postorder ? "Postorder"
                                        : "Preorder")
        << " visit" << std::endl;

    switch (visit) {
      case Visit::inorder: {
        inorder_visit(node, out);
        break;
      }

      case Visit::postorder: {
        postorder_visit(node, out);
        break;
      }

      case Visit::preorder: {
        preorder_visit(node, out);
        break;
      }
    }
  }
};

#endif