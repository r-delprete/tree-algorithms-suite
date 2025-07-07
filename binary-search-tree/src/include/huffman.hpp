#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <fstream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "node.hpp"

class Huffman {
  std::vector<Node*> nodes;
  Node* root;
  std::unordered_map<char, std::string> codes;

  struct Compare {
    bool operator()(Node* node1, Node* node2) { return node1->get_frequency() > node2->get_frequency(); }
  };

  void delete_tree(Node*& node) {
    if (!node) return;

    delete_tree(node->get_left());
    delete_tree(node->get_right());
    delete node;
    node = nullptr;
  }

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

  void build_tree() {
    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;
    for (auto& node : nodes) pq.push(node);

    while (pq.size() > 1) {
      Node* left = pq.top();
      pq.pop();

      Node* right = pq.top();
      pq.pop();

      Node* node = new Node(-1, '*', left->get_frequency() + right->get_frequency(), left, right, nullptr);
      left->set_parent(node);
      right->set_parent(node);
      pq.push(node);
    }

    root = pq.top();
    pq.pop();
  }

  void generate_codes(Node* node, std::string path = "") {
    if (!node) return;

    if (node->is_leaf()) codes[node->get_character()] = path;
    generate_codes(node->get_left(), path + "0");
    generate_codes(node->get_right(), path + "1");
  }

public:
  Huffman(std::ifstream& input) : root(nullptr) {
    load(input);
    build_tree();
    generate_codes(root);
  }

  ~Huffman() {
    delete_tree(root);
    nodes.clear();
  }

  void load(std::ifstream& input) {
    delete_tree(root);
    input.clear();
    input.seekg(0, std::ios::beg);

    std::string line;
    while (std::getline(input, line)) {
      format_line(line);
      std::istringstream iss(line);

      int frequency;
      char ch;
      iss >> frequency >> ch;
      nodes.push_back(new Node(-1, ch, frequency));

      clear_stream(iss);
    }
  }

  std::string encode(std::string input) {
    std::string encoded;

    for (auto& c : input) {
      if (codes.find(c) != codes.end())
        encoded += codes[c] + (input.back() == c ? "" : " ");
      else
        return "";
    }

    return encoded;
  }

  std::string decode(std::string encoded) {
    std::string decoded;
    Node* current = root;

    for (auto& bit : encoded) {
      if (bit == '0')
        current = current->get_left();
      else if (bit == '1')
        current = current->get_right();

      if (current->is_leaf()) {
        decoded += current->get_character();
        current = root;
      }
    }

    return decoded;
  }

  void print_codes(std::ostream& out = std::cout) {
    out << "Huffman codes" << std::endl;
    for (auto& pair : codes) out << pair.first << " => " << pair.second << std::endl;
    out << std::endl;
  }

  void print_encode(std::string input, std::ostream& out = std::cout) {
    std::string encoded = encode(input);
    (encoded.empty() ? out << "Encoded string is empty"
                     : out << "Encoded string for input \"" << input << "\" => " << encoded);
    out << std::endl;
  }

  void print_decode(std::string encoded, std::ostream& out = std::cout) {
    std::string decoded = decode(encoded);
    (decoded.empty() ? out << "Decoded string is empty"
                     : out << "Decoded string for encoded string \"" << encoded << "\" => " << decoded);
    out << std::endl;
  }
};

#endif