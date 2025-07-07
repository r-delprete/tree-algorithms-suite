#ifndef NODE_HPP
#define NODE_HPP

#include <climits>
#include <iostream>

class Node {
  int key, frequency;
  char ch;
  Node *left, *right, *parent;

public:
  Node(int key, char ch = '*', int frequency = INT_MAX, Node* left = nullptr, Node* right = nullptr,
       Node* parent = nullptr)
      : key(key), ch(ch), frequency(frequency), left(left), right(right), parent(parent) {}

  int get_key() const { return key; }
  int get_frequency() const { return frequency; }
  char get_character() const { return ch; }
  Node*& get_left() { return left; }
  Node*& get_right() { return right; }
  Node*& get_parent() { return parent; }

  void set_key(int key) { this->key = key; }
  void set_character(char ch) { this->ch = ch; }
  void set_frequency(int frequency) { this->frequency = frequency; }
  void set_left(Node* left) { this->left = left; }
  void set_right(Node* right) { this->right = right; }
  void set_parent(Node* parent) { this->parent = parent; }

  void print(std::ostream& out = std::cout) const {
    out << "Node (Key: " << key << " - Character: " << ch << ") => frequency: " << frequency << " - left: (";

    if (left)
      out << "Key: " << left->get_key() << " - Character: " << left->get_character();
    else
      out << "NULL";
    out << ")";

    out << " - right: (";
    if (right)
      out << "Key: " << right->get_key() << " - Character: " << right->get_character();
    else
      out << "NULL";
    out << ")";

    out << " - parent: (";
    if (parent)
      out << "Key: " << parent->get_key() << " - Character: " << parent->get_character();
    else
      out << "NULL";
    out << ")";

    out << std::endl;
  }

  bool is_leaf() { return !left && !right; }
};

#endif