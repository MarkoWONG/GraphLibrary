#include <gdwg/graph.hpp>

#include <iostream>
#include <string>
#include <vector>

auto main() -> int {
	auto v = std::vector<int>{1,2,5,69,96,100};
	auto g = gdwg::graph<int, int>(v.begin(), v.end());
	// g.print_key();
	auto g2 = gdwg::graph<int, int>{123,3242,3565,695235,96,102340};
	g2.print_key();
	auto g3 = std::move(g2);
	g3.print_key();

	auto g4 = g3;
	g4.print_key();
	g3.print_key();

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
