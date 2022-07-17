#include <gdwg/graph.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <cassert>

auto main() -> int {

	// struct value_type {
	// 	int from;
	// 	int to;
	// 	int weight;
	// };
	// using graph = gdwg::graph<int, int>;
	// auto const v = std::vector<value_type> {
	// {4, 1, -4},
	// {3, 2, 2},
	// {2, 4, 2},
	// {2, 1, 1}
	// };
	auto g1 = gdwg::graph<std::string, int>();
	auto g2 = gdwg::graph<std::string, int>();
	if(g1 == g2){
		std::cout << "done";
	};

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

	// std::cout << g << "\n";

	// auto g2 = gdwg::graph<std::string, int>(g);

	// std::cout << g2 << "\n";

	// // This is a structured binding.
	// // https://en.cppreference.com/w/cpp/language/structured_binding
	// // It allows you to unpack your tuple.
	// for (auto const& [from, to, weight] : g) {
	// 	std::cout << from << " -> " << to << " (weight " << weight << ")\n";
	// }
}
