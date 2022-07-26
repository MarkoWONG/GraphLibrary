#include <gdwg/graph.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <cassert>

auto main() -> int {

	auto g = gdwg::graph<std::string, int>{};
	g.insert_node("A");
	g.insert_node("B");
	g.insert_node("C");
	g.insert_node("D");
	g.insert_edge("B", "D", 4);
	g.insert_edge("D", "B", 4);
	g.insert_edge("D", "B", 5);
	g.insert_edge("D", "A", 5);


	auto it = g.erase_edge(g.begin(), g.end());
	std::cout << g;
	if (it == g.end()){
		std::cout << "ok\n";
	}
	// g.erase_edge(it3);


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
	// for (auto const& [from, to, weight] : g) {
	// 	std::cout << from << " -> " << to << " (weight " << weight << ")\n";
	// }

	// for (auto it = g.end(); it != g.begin(); --it) {
    //     if (it != g.end()) {
	// 		std::cout << (*it).from << " -> " << (*it).to << " (weight " << (*it).weight << ")\n";
    //     }
    // }
}
