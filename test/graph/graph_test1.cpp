#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>

TEST_CASE("basic test") {
	// This will not compile straight away
	auto g = gdwg::graph<int, std::string>{};
	auto n = 5;
	g.insert_node(n);
	CHECK(g.is_node(n));
}
TEST_CASE("Constructor Unit Tests") {
	SECTION(){}
}

TEST_CASE("Accessor Unit Tests") {
}
TEST_CASE("Modifier Unit Tests") {
}

TEST_CASE("Operatir Unit Tests") {
}
TEST_CASE("Iterator Unit Tests") {
}
