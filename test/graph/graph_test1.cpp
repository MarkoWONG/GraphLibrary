#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <vector>
#include <list>

TEST_CASE("basic test") {
	// This will not compile straight away
	auto g = gdwg::graph<int, std::string>{};
	auto n = 5;
	g.insert_node(n);
	CHECK(g.is_node(n));
}
TEST_CASE("Constructor Unit Tests") {
	SECTION("default"){
		CHECK_NOTHROW(gdwg::graph<int, std::string>());
		auto g1 = gdwg::graph<int, std::string>();
		CHECK(g1.empty() == true);

		CHECK_NOTHROW(gdwg::graph<int, int>());
		auto g2 = gdwg::graph<int, int>();
		CHECK(g2.empty() == true);
	}
	SECTION("initialiser list"){
		auto g1 = gdwg::graph<std::string, int>{"A", "B", "Marko"};
		auto vec1 = g1.nodes();
		CHECK(vec1.size() == 3);
		CHECK(g1.is_node("A") == true);
		CHECK(g1.is_node("B") == true);
		CHECK(g1.is_node("Marko") == true);

		auto g2 = gdwg::graph<int, std::string>{-1234,-3,0,420};
		auto vec2 = g2.nodes();
		CHECK(vec2.size() == 4);
		CHECK(g2.is_node(-1234) == true);
		CHECK(g2.is_node(-3) == true);
		CHECK(g2.is_node(0) == true);
		CHECK(g2.is_node(420) == true);
	}
	SECTION("interator"){
		auto in_vec1 = std::vector<std::string>{"A", "B", "Marko"};
		auto g1 = gdwg::graph<std::string, int>(in_vec1.begin(), in_vec1.end());
		auto vec1 = g1.nodes();
		CHECK(vec1.size() == 3);
		CHECK(g1.is_node("A") == true);
		CHECK(g1.is_node("B") == true);
		CHECK(g1.is_node("Marko") == true);

		auto in_list1 = std::list<int>{-1234,-3,0,420};
		auto g2 = gdwg::graph<int, std::string>(in_list1.begin(), in_list1.end());
		auto vec2 = g2.nodes();
		CHECK(vec2.size() == 4);
		CHECK(g2.is_node(-1234) == true);
		CHECK(g2.is_node(-3) == true);
		CHECK(g2.is_node(0) == true);
		CHECK(g2.is_node(420) == true);
	}
	SECTION("move"){
		auto in_vec1 = std::vector<std::string>{"A", "B", "Marko"};
		auto g1 = gdwg::graph<std::string, int>(in_vec1.begin(), in_vec1.end());
		auto vec1 = g1.nodes();

		auto g2 = gdwg::graph<std::string, int>(std::move(g1));
		auto vec2 = g2.nodes();
		CHECK(vec2.size() == 3);
		CHECK(g2.is_node("A") == true);
		CHECK(g2.is_node("B") == true);
		CHECK(g2.is_node("Marko") == true);
		CHECK(g1.empty() == true);

		auto g3 = std::move(g2);
		auto vec3 = g3.nodes();
		CHECK(vec3.size() == 3);
		CHECK(g3.is_node("A") == true);
		CHECK(g3.is_node("B") == true);
		CHECK(g3.is_node("Marko") == true);
		CHECK(g2.empty() == true);
	}
	SECTION("copy"){
		auto in_vec1 = std::vector<std::string>{"A", "B", "Marko"};
		auto g1 = gdwg::graph<std::string, int>(in_vec1.begin(), in_vec1.end());
		auto vec1 = g1.nodes();
		CHECK(vec1.size() == 3);
		CHECK(g1.is_node("A") == true);
		CHECK(g1.is_node("B") == true);
		CHECK(g1.is_node("Marko") == true);

		auto g2 = gdwg::graph<std::string, int>(g1);
		CHECK(g1 == g2);

		auto g3 = g2;
		CHECK(g3 == g2);
	}
}

TEST_CASE("Accessor Unit Tests") {
	SECTION("is_node"){
		auto g1 = gdwg::graph<std::string, int>{"A", "B", "Marko"};
		CHECK(g1.is_node("A") == true);
		CHECK(g1.is_node("B") == true);
		CHECK(g1.is_node("Marko") == true);
		CHECK(g1.is_node("marko") == false);
	}
	SECTION("empty"){
		auto g1 = gdwg::graph<std::string, int>();
		CHECK(g1.empty() == true);
		auto g2 = gdwg::graph<std::string, int>{"2"};
		CHECK(g2.empty() == false);
	}
	SECTION("is_connected"){
		auto g1 = gdwg::graph<std::string, int>{"A", "B", "Marko"};
		g1.insert_edge("A", "B", 5);
		CHECK(g1.is_connected("A", "B") == true);
		CHECK(g1.is_connected("B", "A") == false);

		CHECK_THROWS_AS(g1.is_connected("C", "A"), std::runtime_error);
		CHECK_THROWS_WITH(g1.is_connected("C", "A"), "Cannot call gdwg::graph<N, E>::merge_replace_node on old or new data if they don't exist in the graph");

		CHECK_THROWS_AS(g1.is_connected("A", "D"), std::runtime_error);
		CHECK_THROWS_WITH(g1.is_connected("A", "D"), "Cannot call gdwg::graph<N, E>::merge_replace_node on old or new data if they don't exist in the graph");
	}
	SECTION("nodes"){
		auto g1 = gdwg::graph<std::string, int>{"B", "A", "Marko"};
		auto src_vec = g1.nodes();
		CHECK(src_vec == std::vector<std::string>{"A", "B", "Marko"});
	}
	SECTION("weights"){
		auto g = gdwg::graph<std::string, int>{};
		g.insert_node("hello");
		g.insert_node("how");
		g.insert_node("are");

		g.insert_edge("hello", "how", 5);
		g.insert_edge("hello", "are", 8);
		g.insert_edge("hello", "are", 2);
		auto src_weights = g.weights("hello", "are");
		CHECK(src_weights == std::vector<int>{2,8});

		CHECK_THROWS_AS(g.weights("C", "are"), std::runtime_error);
		CHECK_THROWS_WITH(g.weights("C", "are"), "Cannot call gdwg::graph<N, E>::weights if src or dst node don't exist in the graph");
		CHECK_THROWS_AS(g.weights("are", "C"), std::runtime_error);
		CHECK_THROWS_WITH(g.weights("are", "C"), "Cannot call gdwg::graph<N, E>::weights if src or dst node don't exist in the graph");
	}
	SECTION("connections"){
		auto g = gdwg::graph<std::string, int>{};
		g.insert_node("hello");
		g.insert_node("how");
		g.insert_node("are");
		g.insert_node("you?");

		g.insert_edge("hello", "are", 8);
		g.insert_edge("hello", "are", 2);
		g.insert_edge("hello", "you?", 1);
		g.insert_edge("how", "hello", 4);

		auto src_edges = g.connections("hello");
		CHECK(src_edges == std::vector<std::string>{"are", "are", "you?"});
		CHECK_THROWS_AS(g.connections("C"), std::runtime_error);
		CHECK_THROWS_WITH(g.connections("C"),"Cannot call gdwg::graph<N, E>::connections if src doesn't exist in the graph");
	}
}
TEST_CASE("Modifier Unit Tests") {
	SECTION("insert_node"){
		auto g = gdwg::graph<std::string, int>{};
		CHECK(g.insert_node("hello") == true);
		CHECK(g.is_node("hello") == true);
		auto vec1 = g.nodes();
		CHECK(vec1.size() == 1);
		CHECK(g.insert_node("hello") == false);
		vec1 = g.nodes();
		CHECK(vec1.size() == 1);
	}
	SECTION("insert_edge"){
		auto g = gdwg::graph<std::string, int>{};
		g.insert_node("hello");
		g.insert_node("how");
		g.insert_node("are");
		g.insert_node("you?");

		CHECK(g.insert_edge("hello", "are", 8) == true);
		CHECK(g.insert_edge("hello", "are", 2) == true);
		CHECK(g.insert_edge("hello", "you?", 1)== true);
		CHECK(g.insert_edge("how", "hello", 4)== true);

		CHECK(g.insert_edge("hello", "are", 8) == false);
		CHECK(g.insert_edge("how", "hello", 4)== false);
		CHECK_THROWS_AS(g.insert_edge("C", "how", 2), std::runtime_error);
		CHECK_THROWS_WITH(g.insert_edge("C", "how",2),"Cannot call gdwg::graph<N, E>::insert_edge when either src or dst node does not exist");
		CHECK_THROWS_AS(g.insert_edge("how", "C", 2), std::runtime_error);
		CHECK_THROWS_WITH(g.insert_edge("how", "C",2),"Cannot call gdwg::graph<N, E>::insert_edge when either src or dst node does not exist");
	}
	SECTION("replace_node"){

	}
	SECTION("merge_replace_node"){

	}
	SECTION("erase_node"){

	}
	SECTION("erase_edge"){

	}
	SECTION("clear"){

	}
}

TEST_CASE("Operator Unit Tests") {
	SECTION("=="){
		auto g1 = gdwg::graph<std::string, int>();
		auto g2 = gdwg::graph<std::string, int>();
		CHECK(g1 == g2);

		auto g3 = gdwg::graph<int, int>{1,2,3,4,65,1234,-2};
		auto g4 = gdwg::graph<int, int>{1234,-2,3,4,65,1,2};
		CHECK(g3 == g4);

		auto g5 = gdwg::graph<int, int>{1,2,3,4,65,1234,-2};
		g5.insert_edge(1,2,90);
		g5.insert_edge(2,1,91);
		g5.insert_edge(2,2,92);
		auto g6 = gdwg::graph<int, int>{1234,-2,3,4,65,1,2};
		g6.insert_edge(1,2,90);
		g6.insert_edge(2,1,91);
		g6.insert_edge(2,2,92);
		CHECK(g5 == g6);
	}
	SECTION("<<"){

	}
}
TEST_CASE("Iterator Unit Tests") {
}
