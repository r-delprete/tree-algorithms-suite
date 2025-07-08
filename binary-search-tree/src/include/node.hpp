#ifndef NODE_HPP
#define NODE_HPP

#include <climits>
#include <iostream>

class Node {
  int key, frequency;
  char character;
  Node *left, *right, *parent;

public:
  Node(int key, char character, int frequency = INT_MAX, Node* left = nullptr, Node* right = nullptr,
       Node* parent = nullptr)
      : key(key), character(character), frequency(frequency), left(left), right(right), parent(parent) {}

  int get_key() const { return key; }
  char get_character() const { return character; }
  int get_frequency() const { return frequency; }
  Node* get_left() const { return left; }
  Node* get_right() const { return right; }
  Node* get_parent() const { return parent; }
  Node*& get_left() { return left; }
  Node*& get_right() { return right; }
  Node*& get_parent() { return parent; }

  void set_key(int key) { this->key = key; }
  void set_character(char character) { this->character = character; }
  void set_frequency(int frequency) { this->frequency = frequency; }
  void set_left(Node* left) { this->left = left; }
  void set_right(Node* right) { this->right = right; }
  void set_parent(Node* parent) { this->parent = parent; }

  void print(std::ostream& out = std::cout) {
    out << "Node (" << key << " - " << character << ") => frequency: " << frequency;

    out << " - left: (";
    if (left)
      out << left->get_key() << " - " << left->get_character();
    else
      out << "NULL";
    out << ")";

    out << " - right: (";
    if (right)
      out << right->get_key() << " - " << right->get_character();
    else
      out << "NULL";
    out << ")";

    out << " - parent: (";
    if (parent)
      out << parent->get_key() << " - " << parent->get_character();
    else
      out << "NULL";
    out << ")";

    out << std::endl;
  }

  bool is_leaf() { return !left && !right; }
};

#endif