#ifndef NODE_HPP
#define NODE_HPP

#include <climits>
#include <iostream>
#include <memory>

class Node {
  int key, frequency;
  char character;
  std::weak_ptr<Node> parent;
  std::shared_ptr<Node> left, right;

public:
  Node(int key = -1, char character = '*', int frequency = INT_MAX)
      : key(key), character(character), frequency(frequency) {}

  int get_key() const { return key; }
  int get_frequency() const { return frequency; }
  char get_character() const { return character; }
  std::shared_ptr<Node> get_left() const { return left; }
  std::shared_ptr<Node> get_right() const { return right; }
  std::shared_ptr<Node>& get_left_ref() { return left; }
  std::shared_ptr<Node>& get_right_ref() { return right; }
  std::weak_ptr<Node> get_parent() const { return parent; }

  void set_key(int key) { this->key = key; }
  void set_frequency(int frequency) { this->frequency = frequency; }
  void set_character(char character) { this->character = character; }
  void set_parent(const std::shared_ptr<Node>& parent) { this->parent = parent; }
  void set_left(const std::shared_ptr<Node>& left) { this->left = left; }
  void set_right(const std::shared_ptr<Node>& right) { this->right = right; }

  bool is_leaf() const { return !left && !right; }

  void print(std::ostream& out = std::cout) {
    out << "(" << key << " - " << character << ") => frequency: " << frequency;

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
    if (parent.lock())
      out << parent.lock()->get_key() << " - " << parent.lock()->get_character();
    else
      out << "NULL";
    out << ")";

    out << std::endl;
  }
};

using shared_node_ptr = std::shared_ptr<Node>;
shared_node_ptr make_node_shared(int key = -1, char character = '*', int frequency = INT_MAX) {
  return std::make_shared<Node>(key, character, frequency);
}

#endif