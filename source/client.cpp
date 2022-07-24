#include <gdwg/graph.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <cassert>

auto main() -> int {

	auto g = gdwg::graph<int, int>();
	g.insert_node(1);
	g.insert_node(2);
	g.insert_node(3);
	g.insert_node(4);
	g.insert_node(5);
	g.insert_node(6);
	// g.insert_node(64);

	g.insert_edge(6, 2, 5);
	g.insert_edge(6, 3, 10);
	g.insert_edge(1, 5, -1);
	g.insert_edge(1, 5, 2); //temp
	g.insert_edge(1, 5, 3); //temp

	g.insert_edge(4, 1, -4);
	g.insert_edge(3, 2, 2);
	g.insert_edge(2, 4, 2);
	g.insert_edge(2, 1, 1);
	g.insert_edge(3, 6, -8);
	g.insert_edge(4, 5, 3);
	g.insert_edge(5, 2, 7);
	std::cout << g;


	// This will not compile straight away
	// auto g = gdwg::graph<std::string, int>{};
	// g.insert_node("hello");
	// g.insert_node("how");
	// g.insert_node("are");
	// g.insert_node("you?");

	// g.insert_edge("hello", "how", 5);
	// g.insert_edge("hello", "are", 8);
	// g.insert_edge("hello", "are", 2);

	// g.insert_edge("how", "you?", 1);
	// g.insert_edge("how", "hello", 4);

	// g.insert_edge("are", "you?", 3);

	// This is a structured binding.
	// https://en.cppreference.com/w/cpp/language/structured_binding
	// It allows you to unpack your tuple.
	for (auto const& [from, to, weight] : g) {
		std::cout << from << " -> " << to << " (weight " << weight << ")\n";
	}
	// for (auto const& [to, weight] : g){
	// 	std::cout << to << " (weight " << weight << ")\n";
	// }

	// for (auto it = g.end(); it != g.begin(); --it) {
    //     if (it != g.end()) {
    //         std::cout << (*it).first << " ";
    //     }
    // }
}
