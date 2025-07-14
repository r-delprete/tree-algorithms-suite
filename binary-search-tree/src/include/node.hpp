#ifndef NODE_HPP
#define NODE_HPP

#include <climits>
#include <iostream>
#include <memory>

class Node {
  friend class BinarySearchTree;

  int key, frequency;
  char character;
  std::weak_ptr<Node> parent;
  std::shared_ptr<Node> left, right;

protected:
  std::shared_ptr<Node>& get_left_ref() { return left; }
  std::shared_ptr<Node>& get_right_ref() { return right; }

public:
  Node(int key, char character = '*', int frequency = INT_MAX) : key(key), character(character), frequency(frequency) {}

  int get_key() const { return key; }
  int get_frequency() const { return frequency; }
  char get_character() const { return character; }

  const std::weak_ptr<Node>& get_parent() const { return parent; }
  const std::shared_ptr<Node>& get_left() const { return left; }
  const std::shared_ptr<Node>& get_right() const { return right; }

  void set_key(const int key) { this->key = key; }
  void set_character(const char character) { this->character = character; }
  void set_frequency(const int frequency) { this->frequency = frequency; }
  void set_parent(const std::shared_ptr<Node>& node) { this->parent = node; }
  void set_left(const std::shared_ptr<Node>& node) { this->left = node; }
  void set_right(const std::shared_ptr<Node>& node) { this->right = node; }

  bool is_leaf() const { return !left && !right; }

  void print(std::ostream& out = std::cout) const {
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
    auto p = parent.lock();
    if (p)
      out << p->get_key() << " - " << p->get_character();
    else
      out << "NULL";
    out << ")";

    out << std::endl;
  }
};

using shared_node = std::shared_ptr<Node>;
inline shared_node create_node(const int key, const char character = '*', const int frequency = INT_MAX) {
  return std::make_shared<Node>(key, character, frequency);
}

#endif