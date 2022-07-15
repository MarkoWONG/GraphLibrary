#include <gdwg/graph.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <cassert>

auto main() -> int {

	struct value_type {
		int from;
		int to;
		int weight;
	};
	using graph = gdwg::graph<int, int>;
	auto const v = std::vector<value_type>{
	{4, 1, -4},
	{3, 2, 2},
	{2, 4, 2},
	{2, 1, 1},
	{6, 2, 5},
	{6, 3, 10},
	{1, 5, -1},
	{3, 6, -8},
	{4, 5, 3},
	{5, 2, 7},
	};

	auto g = graph{};
	for (const auto& [from, to, weight] : v) {
		g.insert_node(from);
		g.insert_node(to);
		g.insert_edge(from, to, weight);
	}

	g.insert_node(64);
	std::cout << g;
	assert (g.erase_node(4) == true);
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
