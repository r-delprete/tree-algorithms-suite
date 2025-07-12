#ifndef BINARY_SEARCH_TREE
#define BINARY_SEARCH_TREE

#include <fstream>
#include <sstream>
#include <string>

enum Visit { preorder, postorder, inorder };

#include "node.hpp"

class BinarySearchTree {
  shared_node_ptr root;

  void preorder_visit(const shared_node_ptr& node, std::ostream& out = std::cout) {
    if (!node) return;

    node->print(out);
    preorder_visit(node->get_left(), out);
    preorder_visit(node->get_right(), out);
  }

  void inorder_visit(const shared_node_ptr& node, std::ostream& out = std::cout) {
    if (!node) return;

    inorder_visit(node->get_left(), out);
    node->print(out);
    inorder_visit(node->get_right(), out);
  }

  void postorder_visit(const shared_node_ptr& node, std::ostream& out = std::cout) {
    if (!node) return;

    postorder_visit(node->get_left(), out);
    postorder_visit(node->get_right(), out);
    node->print(out);
  }

  void insert_recursive(shared_node_ptr& start, const shared_node_ptr& node) {
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

  void format_line(std::string& line) {
    if (line.empty()) return;
    if (line.front() == '<') line = line.substr(1);
    if (line.back() == '>') line.pop_back();
    for (auto& ch : line) {
      if (ch == ',') ch = ' ';
    }
  }

  void clear_stream(std::istringstream& stream) {
    stream.clear();
    stream.str("");
  }

public:
  BinarySearchTree(std::ifstream& input) : root(nullptr) { load(input); }

  void delete_subtree(shared_node_ptr& node) {
    if (!node) return;

    delete_subtree(node->get_left_ref());
    delete_subtree(node->get_right_ref());
    node = nullptr;
  }

  const shared_node_ptr get_root() const { return root; }

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
      insert(make_node_shared(key, ch));
      clear_stream(iss);
      line.clear();
    }
  }

  void insert(const shared_node_ptr& node) { insert_recursive(root, node); }

  shared_node_ptr tree_maximum(shared_node_ptr node) {
    while (node->get_right()) node = node->get_right();
    return node;
  }

  shared_node_ptr tree_minimum(shared_node_ptr node) {
    while (node->get_left()) node = node->get_left();
    return node;
  }

  shared_node_ptr get_predecessor(shared_node_ptr node) {
    if (!node) {
      std::cerr << "[get_predecessor ERROR] Invalid node" << std::endl;
      return nullptr;
    }

    if (node->get_left()) return tree_maximum(node->get_left());
    auto parent = node->get_parent().lock();

    while (parent && node == parent->get_left()) {
      node = parent;
      parent = parent->get_parent().lock();
    }

    return parent;
  }

  shared_node_ptr get_successor(shared_node_ptr node) {
    if (!node) {
      std::cerr << "[get_successor ERROR] Invalid node" << std::endl;
      return nullptr;
    }

    if (node->get_right()) return tree_minimum(node->get_right());

    auto parent = node->get_parent().lock();
    while (parent && node == parent->get_right()) {
      node = parent;
      parent = parent->get_parent().lock();
    }

    return parent;
  }

  shared_node_ptr search(const shared_node_ptr& node, const int key) {
    if (!node) return nullptr;

    if (node->get_key() == key) return node;
    if (key < node->get_key()) return search(node->get_left(), key);
    return search(node->get_right(), key);
  }

  void print_predecessor(const shared_node_ptr& node, std::ostream& out = std::cout) {
    auto predecessor = get_predecessor(node);

    out << "Predecessor for node (" << node->get_key() << ") => " << "(";
    if (predecessor)
      out << predecessor->get_key() << " - " << predecessor->get_character();
    else
      out << "NULL";
    out << ")" << std::endl;
  }

  void print_successor(const shared_node_ptr& node, std::ostream& out = std::cout) {
    auto successor = get_successor(node);

    out << "Successor for node (" << node->get_key() << ") => " << "(";
    if (successor)
      out << successor->get_key() << " - " << successor->get_character();
    else
      out << "NULL";
    out << ")" << std::endl;
  }

  void visit(const shared_node_ptr& node, Visit visit, std::ostream& out = std::cout) {
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