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
  std::vector<shared_node> nodes;
  std::unordered_map<char, std::string> codes;
  shared_node root;

  struct Compare {
    bool operator()(const shared_node& node1, const shared_node& node2) {
      return node1->get_frequency() > node2->get_frequency();
    }
  };

  void build_tree() {
    std::priority_queue<shared_node, std::vector<shared_node>, Compare> pq;

    for (auto& node : nodes) pq.push(node);
    while (pq.size() > 1) {
      auto left = pq.top();
      pq.pop();

      auto right = pq.top();
      pq.pop();

      auto node = create_node(-1, '*', left->get_frequency() + right->get_frequency());
      node->set_left(left);
      node->set_right(right);
      left->set_parent(node);
      right->set_parent(node);

      nodes.push_back(node);
      pq.push(node);
    }

    root = pq.top();
    pq.pop();
  }

  void generate_codes(const shared_node& node, const std::string path = "") {
    if (!node) return;

    if (node->is_leaf()) codes[node->get_character()] = path;
    generate_codes(node->get_left(), path + "0");
    generate_codes(node->get_right(), path + "1");
  }

public:
  Huffman(std::ifstream& input) {
    load(input);
    build_tree();
    generate_codes(root);
  }

  void reset() { nodes.clear(); }

  void load(std::ifstream& input) {
    reset();
    input.clear();
    input.seekg(0, std::ios::beg);

    std::string line;
    while (std::getline(input, line)) {
      format_line(line);
      std::istringstream iss(line);

      int frequency;
      char ch;
      iss >> frequency >> ch;

      nodes.push_back(create_node(-1, ch, frequency));

      clear_stream(iss);
      line.clear();
    }
  }

  std::string encode(const std::string& input) const {
    std::string encoded;

    for (int i = 0; i < input.size(); i++) {
      auto ch = input[i];

      if (codes.find(ch) != codes.end())
        encoded += codes.at(ch) + (i != input.size() - 1 ? " " : "");
      else
        return "";
    }

    return encoded;
  }

  std::string decode(const std::string& encoded) const {
    std::string decoded;
    auto current = root;

    for (auto& bit : encoded) {
      if (!current) return "";

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

  void print_codes(std::ostream& out = std::cout) const {
    out << "Huffman codes" << std::endl;
    for (auto& pair : codes) out << pair.first << " => " << pair.second << std::endl;
    out << std::endl;
  }

  void print_encode(const std::string& input, std::ostream& out = std::cout) const {
    const std::string encoded = encode(input);
    (encoded.empty() ? out << "Encoded string is empty"
                     : out << "Encoded string for input \"" << input << "\" => " << encoded);
    out << std::endl;
  }

  void print_decode(const std::string& encoded, std::ostream& out = std::cout) const {
    const std::string decoded = decode(encoded);
    (decoded.empty() ? out << "Decoded string is empty"
                     : out << "Decoded string for encoded string \"" << encoded << "\" => " << decoded);
    out << std::endl;
  }
};

#endif