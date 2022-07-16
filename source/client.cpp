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
	struct value_type {
		std::string from;
		std::string to;
		int weight;
	};
	using graph = gdwg::graph<std::string, int>;
	auto const v = std::vector<value_type>{
	{ "B", "A", 3},
	{ "B","C", 2},
	{ "B","D", 4},
	};

	auto g = graph{};
	for (const auto& [from, to, weight] : v) {
		g.insert_node(from);
		g.insert_node(to) ;
		assert (g.insert_edge(from, to, weight) == true);
	}
	std::cout << g;
	g.merge_replace_node("B", "A") ;
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
