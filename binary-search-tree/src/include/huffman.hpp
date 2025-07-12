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
  std::vector<shared_node_ptr> nodes;
  shared_node_ptr root;
  std::unordered_map<char, std::string> codes;

  struct Compare {
    bool operator()(const shared_node_ptr& node1, const shared_node_ptr& node2) {
      return node1->get_frequency() > node2->get_frequency();
    }
  };

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

  void generate_huffman_tree() {
    std::priority_queue<shared_node_ptr, std::vector<shared_node_ptr>, Compare> pq;
    for (auto& node : nodes) pq.push(node);

    while (pq.size() > 1) {
      auto left = pq.top();
      pq.pop();

      auto right = pq.top();
      pq.pop();

      auto node = make_node_shared(-1, '*', left->get_frequency() + right->get_frequency());
      left->set_parent(node);
      right->set_parent(node);
      node->set_left(left);
      node->set_right(right);

      pq.push(node);
      nodes.push_back(node);
    }

    root = pq.top();
    pq.pop();

    generate_codes(root);
  }

  void generate_codes(const shared_node_ptr& node, std::string path = "") {
    if (!node) return;

    if (node->is_leaf()) codes[node->get_character()] = path;
    generate_codes(node->get_left(), path + "0");
    generate_codes(node->get_right(), path + "1");
  }

public:
  Huffman(std::ifstream& input) { load(input); }

  void reset() { nodes.clear(); }

  void load(std::ifstream& input) {
    reset();
    input.clear();
    input.seekg(0, std::ios::beg);

    std::string line;
    while (std::getline(input, line)) {
      format_line(line);
      std::istringstream iss(line);
      int key;
      char ch;
      iss >> key >> ch;
      nodes.push_back(make_node_shared(key, ch));
      clear_stream(iss);
      line.clear();
    }

    generate_huffman_tree();
  }

  std::string encode(const std::string& input) {
    std::string encoded;

    for (int i = 0; i < input.length(); i++) {
      auto ch = input[i];

      if (codes.find(ch) != codes.end())
        encoded += codes[ch] + (i == input.size() - 1 ? "" : " ");
      else
        return "";
    }

    return encoded;
  }

  std::string decode(const std::string& encoded) {
    std::string decoded;
    auto current = root;

    for (auto& bit : encoded) {
      if (bit == '0')
        current = current->get_left();
      else if (bit == '1')
        current = current->get_right();
      else if (!current)
        return "";

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

  void print_encode(const std::string& input, std::ostream& out = std::cout) {
    const std::string encoded = encode(input);
    (encoded.empty() ? out << "Encoded string is empty"
                     : out << "Encoded string for input \"" << input << "\" => " << encoded);
    out << std::endl;
  }

  void print_decode(const std::string& encoded, std::ostream& out = std::cout) {
    const std::string decoded = decode(encoded);
    (decoded.empty() ? out << "Decoded string is empty"
                     : out << "Decoded string for encoded string \"" << encoded << "\" => " << decoded);
    out << std::endl;
  }
};

#endif