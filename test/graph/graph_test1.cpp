#include "gdwg/graph.hpp"

#include <catch2/catch.hpp>
#include <vector>
#include <list>
#include <sstream>

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
	SECTION("find"){
		auto g = gdwg::graph<std::string, int>{};
		g.insert_node("hello");
		g.insert_node("how");
		g.insert_node("are");

		g.insert_edge("hello", "how", 5);
		g.insert_edge("hello", "are", 8);
		g.insert_edge("hello", "are", 2);
		auto it = g.find("hello", "how", 5);
		CHECK((*it).from == "hello");
		CHECK((*it).to == "how");
		CHECK((*it).weight == 5);

		auto it2 = g.find("hello", "how", 3);
		CHECK(it2 == g.end());

		auto it3 = g.find("ASFD", "how", 3);
		CHECK(it3 == g.end());
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
		CHECK(src_edges == std::vector<std::string>{"are", "you?"});
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
		auto g = gdwg::graph<std::string, int>{};
		g.insert_node("hello");
		g.insert_node("how");
		g.insert_node("are");
		g.insert_node("you?");
		g.insert_edge("hello", "are", 8);
		g.insert_edge("hello", "are", 2);
		g.insert_edge("hello", "you?", 1);
		g.insert_edge("how", "hello", 4);

		auto old_edge_vec = g.connections("hello");

		CHECK(g.replace_node("hello", "Hey") == true);
		CHECK(g.is_node("Hey") == true);
		auto new_edge_vec = g.connections("Hey");
		CHECK(old_edge_vec == new_edge_vec);
		CHECK(g.weights("Hey", "are") == std::vector<int>{2,8});
		CHECK(g.weights("Hey", "you?") == std::vector<int>{1});
		CHECK(g.weights("how", "Hey") == std::vector<int>{4});

		CHECK(g.replace_node("Hey", "how") == false);
		CHECK_THROWS_AS(g.replace_node("C", "how"), std::runtime_error);
		CHECK_THROWS_WITH(g.replace_node("C", "how"),"Cannot call gdwg::graph<N, E>::replace_node on a node that doesn't exist");

	}
	SECTION("merge_replace_node"){
		auto g = gdwg::graph<std::string, int>{};
		g.insert_node("A");
		g.insert_node("B");
		g.insert_node("C");
		g.insert_node("D");
		g.insert_edge("B", "B", -1);
		g.insert_edge("A", "B", 1);
		g.insert_edge("B", "A", 3);
		g.insert_edge("B", "C", 2);
		g.insert_edge("B", "D", 4);
		g.insert_edge("D", "B", 4);

		g.merge_replace_node("B", "A");
		CHECK(g.is_node("B") == false);
		CHECK(g.is_node("A") == true);
		auto edge_vec = g.connections("A");
		CHECK(edge_vec == std::vector<std::string>{"A", "C", "D"});
		CHECK(g.weights("A", "A") == std::vector<int>{-1,1,3});
		CHECK(g.weights("A", "C") == std::vector<int>{2});
		CHECK(g.weights("A", "D") == std::vector<int>{4});
		CHECK(g.weights("D", "A") == std::vector<int>{4});
		CHECK_THROWS_AS(g.merge_replace_node("C", "how"), std::runtime_error);
		CHECK_THROWS_WITH(g.merge_replace_node("C", "how"),"Cannot call gdwg::graph<N, E>::merge_replace_node on old or new data if they don't exist in the graph");
		CHECK_THROWS_AS(g.merge_replace_node("how", "C"), std::runtime_error);
		CHECK_THROWS_WITH(g.merge_replace_node("how", "C"),"Cannot call gdwg::graph<N, E>::merge_replace_node on old or new data if they don't exist in the graph");

	}
	SECTION("erase_node"){
		auto g = gdwg::graph<std::string, int>{};
		g.insert_node("A");
		g.insert_node("B");
		g.insert_node("C");
		g.insert_node("D");
		g.insert_edge("A", "B", 1);
		g.insert_edge("B", "A", 3);
		g.insert_edge("B", "C", 2);
		g.insert_edge("B", "D", 4);
		g.insert_edge("D", "B", 4);
		g.insert_edge("D", "A", 5);

		CHECK(g.erase_node("B") == true);
		CHECK(g.is_node("B") == false);
		CHECK(g.connections("A") == std::vector<std::string>{});
		CHECK(g.connections("C") == std::vector<std::string>{});
		CHECK(g.connections("D") == std::vector<std::string>{"A"});

		CHECK(g.erase_node("B") == false);
	}
	SECTION("erase_edge -> bool"){
		auto g = gdwg::graph<std::string, int>{};
		g.insert_node("A");
		g.insert_node("B");
		g.insert_node("C");
		g.insert_node("D");
		g.insert_edge("B", "D", 4);
		g.insert_edge("D", "B", 4);
		g.insert_edge("D", "B", 5);
		g.insert_edge("D", "A", 5);

		CHECK(g.erase_edge("D","B",4) == true);
		CHECK(g.connections("B") == std::vector<std::string>{"D"});
		CHECK(g.connections("D") == std::vector<std::string>{"A", "B"});

		CHECK(g.erase_edge("B","A", 4) == false);
		CHECK_THROWS_AS(g.erase_edge("C", "how", 4), std::runtime_error);
		CHECK_THROWS_WITH(g.erase_edge("C", "how",4),"Cannot call gdwg::graph<N, E>::erase_edge on src or dst if they don't exist in the graph");
		CHECK_THROWS_AS(g.erase_edge("how", "C",4), std::runtime_error);
		CHECK_THROWS_WITH(g.erase_edge("how", "C",4),"Cannot call gdwg::graph<N, E>::erase_edge on src or dst if they don't exist in the graph");
	}
	SECTION("erase_edge(single) -> iterator"){
		auto g = gdwg::graph<std::string, int>{};
		g.insert_node("A");
		g.insert_node("B");
		g.insert_node("C");
		g.insert_node("D");
		g.insert_edge("B", "D", 4);
		g.insert_edge("D", "B", 4);
		g.insert_edge("D", "B", 5);
		g.insert_edge("D", "A", 5);


		auto it = g.erase_edge(g.end());
		CHECK(it == g.end());

		auto original_begin = g.begin();
		auto next_val = original_begin;
		++next_val;
		auto it2 = g.erase_edge(original_begin);
		CHECK(it2 == next_val);
		CHECK(g.begin() == next_val);

		auto g2 = gdwg::graph<std::string, int>{};
		g2.insert_node("A");
		g2.insert_node("B");
		g2.insert_node("C");
		g2.insert_node("D");
		g2.insert_edge("B", "D", 4);
		g2.insert_edge("D", "B", 4);
		g2.insert_edge("D", "B", 5);
		g2.insert_edge("D", "A", 5);
		auto val_it = g2.find("D", "B", 4);
		auto it3 = g2.erase_edge(val_it);
		CHECK((*it3).from == "D");
		CHECK((*it3).to == "B");
		CHECK((*it3).weight == 5);
		auto weight_vec = g2.weights("D", "B");
		CHECK(weight_vec == std::vector<int>{5});

	}
	SECTION("erase_edge(range) -> iterator"){
		auto g = gdwg::graph<std::string, int>{};
		g.insert_node("A");
		g.insert_node("B");
		g.insert_node("C");
		g.insert_node("D");
		g.insert_edge("B", "D", 4);
		g.insert_edge("D", "B", 4);
		g.insert_edge("D", "B", 5);
		g.insert_edge("D", "A", 5);


		auto it = g.erase_edge(g.end(), g.end());
		CHECK(it == g.end());

		auto it2 = g.erase_edge(g.begin(), g.end());
		CHECK(it2 == g.end());
		CHECK(g.is_connected("B", "D") == false);
		CHECK(g.is_connected("D", "B") == false);
		CHECK(g.is_connected("D", "A") == false);

		auto g2 = gdwg::graph<std::string, int>{};
		g2.insert_node("A");
		g2.insert_node("B");
		g2.insert_node("C");
		g2.insert_node("D");
		g2.insert_edge("B", "D", 4);
		g2.insert_edge("D", "B", 4);
		g2.insert_edge("D", "B", 5);
		g2.insert_edge("D", "A", 5);
		auto it3 = g2.erase_edge(g2.find("D", "A", 5), g2.find("D", "B", 5));
		CHECK(g2.is_connected("B", "D") == true);
		CHECK(g2.is_connected("D", "B") == false);
		CHECK(g2.is_connected("D", "A") == false);
		CHECK(it3 == g.end());

		auto g3 = gdwg::graph<std::string, int>{};
		g3.insert_node("A");
		g3.insert_node("B");
		g3.insert_node("C");
		g3.insert_node("D");
		g3.insert_edge("B", "D", 4);
		g3.insert_edge("D", "B", 4);
		g3.insert_edge("D", "B", 5);
		g3.insert_edge("D", "A", 5);
		auto it4 = g.erase_edge(g3.find("B", "D", 4), g3.find("D", "B", 4));
		CHECK(g3.is_connected("B", "D") == false);
		CHECK(g3.is_connected("D", "B") == true);
		CHECK(g3.is_connected("D", "A") == false);
		auto weight_vec = g3.weights("D", "B");
		CHECK(weight_vec == std::vector<int>{5});
		CHECK(it4 == g.end());
	}
	SECTION("clear"){
		auto g = gdwg::graph<std::string, int>{};
		g.insert_node("A");
		g.insert_node("B");
		g.insert_node("C");
		g.insert_node("D");
		g.insert_edge("B", "D", 4);
		g.insert_edge("D", "B", 4);
		g.insert_edge("D", "A", 5);
		CHECK_NOTHROW(g.clear());
		CHECK(g.empty() == true);
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
		auto g = gdwg::graph<int, int>();
		g.insert_node(1);
		g.insert_node(2);
		g.insert_node(3);
		g.insert_node(4);
		g.insert_node(5);
		g.insert_node(6);
		g.insert_node(64);

		g.insert_edge(4, 1, -4);
		g.insert_edge(3, 2, 2);
		g.insert_edge(2, 4, 2);
		g.insert_edge(2, 1, 1);
		g.insert_edge(6, 2, 5);
		g.insert_edge(6, 3, 10);
		g.insert_edge(1, 5, -1);
		g.insert_edge(3, 6, -8);
		g.insert_edge(4, 5, 3);
		g.insert_edge(5, 2, 7);
		auto out = std::ostringstream{};
		out << g;
	}
}
TEST_CASE("Iterator Unit Tests") {
	SECTION("empty Graph"){
		auto g = gdwg::graph<int, int>();
		auto it = g.begin();
		CHECK(it == g.end());
	}
	SECTION("A full (every node has an edge) Graph"){
		auto g = gdwg::graph<int, int>();
		g.insert_node(1);
		g.insert_node(2);
		g.insert_node(3);

		g.insert_edge(1, 1, -1);
		g.insert_edge(1, 2, 2);
		g.insert_edge(3, 2, 2);
		g.insert_edge(3, 3, -8);

		g.insert_edge(2, 3, 2);
		g.insert_edge(2, 1, 1);
		auto it = g.begin();
		CHECK((*it).from == 1);
		CHECK((*it).to == 1);
		CHECK((*it).weight == -1);
		++it;

		CHECK((*it).from == 1);
		CHECK((*it).to == 2);
		CHECK((*it).weight == 2);
		++it;

		CHECK((*it).from == 2);
		CHECK((*it).to == 1);
		CHECK((*it).weight == 1);
		++it;

		CHECK((*it).from == 2);
		CHECK((*it).to == 3);
		CHECK((*it).weight == 2);
		++it;

		CHECK((*it).from == 3);
		CHECK((*it).to == 2);
		CHECK((*it).weight == 2);
		++it;

		CHECK((*it).from == 3);
		CHECK((*it).to == 3);
		CHECK((*it).weight == -8);
		++it;

		CHECK(it == g.end());
	}
	SECTION("nodes with no edge Graph"){
		auto g = gdwg::graph<int, int>();
		g.insert_node(1);
		g.insert_node(2);
		g.insert_node(3);

		auto it = g.begin();
		CHECK(it == g.end());
	}
	SECTION("starting nodes has no edge Graph"){
		auto g = gdwg::graph<int, int>();
		g.insert_node(1);
		g.insert_node(2);
		g.insert_node(3);

		g.insert_edge(3, 2, 2);
		g.insert_edge(3, 3, -8);

		g.insert_edge(2, 3, 2);
		g.insert_edge(2, 1, 1);
		auto it = g.begin();
		CHECK((*it).from == 2);
		CHECK((*it).to == 1);
		CHECK((*it).weight == 1);
		++it;

		CHECK((*it).from == 2);
		CHECK((*it).to == 3);
		CHECK((*it).weight == 2);
		++it;

		CHECK((*it).from == 3);
		CHECK((*it).to == 2);
		CHECK((*it).weight == 2);
		++it;

		CHECK((*it).from == 3);
		CHECK((*it).to == 3);
		CHECK((*it).weight == -8);
		++it;

		CHECK(it == g.end());
	}
	SECTION("middle nodes has no edge Graph"){
		auto g = gdwg::graph<int, int>();
		g.insert_node(1);
		g.insert_node(2);
		g.insert_node(3);

		g.insert_edge(1, 1, -1);
		g.insert_edge(1, 2, 2);
		g.insert_edge(3, 2, 2);
		g.insert_edge(3, 3, -8);

		auto it = g.begin();
		CHECK((*it).from == 1);
		CHECK((*it).to == 1);
		CHECK((*it).weight == -1);
		++it;

		CHECK((*it).from == 1);
		CHECK((*it).to == 2);
		CHECK((*it).weight == 2);
		++it;

		CHECK((*it).from == 3);
		CHECK((*it).to == 2);
		CHECK((*it).weight == 2);
		++it;

		CHECK((*it).from == 3);
		CHECK((*it).to == 3);
		CHECK((*it).weight == -8);
		++it;

		CHECK(it == g.end());
	}
	SECTION("ending nodes has no edge Graph"){
		auto g = gdwg::graph<int, int>();
		g.insert_node(1);
		g.insert_node(2);
		g.insert_node(3);

		g.insert_edge(1, 1, -1);
		g.insert_edge(1, 2, 2);

		g.insert_edge(2, 3, 2);
		g.insert_edge(2, 1, 1);
		auto it = g.begin();
		CHECK((*it).from == 1);
		CHECK((*it).to == 1);
		CHECK((*it).weight == -1);
		++it;

		CHECK((*it).from == 1);
		CHECK((*it).to == 2);
		CHECK((*it).weight == 2);
		++it;

		CHECK((*it).from == 2);
		CHECK((*it).to == 1);
		CHECK((*it).weight == 1);
		++it;

		CHECK((*it).from == 2);
		CHECK((*it).to == 3);
		CHECK((*it).weight == 2);
		++it;

		CHECK(it == g.end());
	}
	SECTION("starting & ending nodes has edges Graph"){
		auto g = gdwg::graph<int, int>();
		g.insert_node(1);
		g.insert_node(2);
		g.insert_node(3);

		g.insert_edge(2, 3, 2);
		g.insert_edge(2, 1, 1);
		auto it = g.begin();
		CHECK((*it).from == 2);
		CHECK((*it).to == 1);
		CHECK((*it).weight == 1);
		++it;

		CHECK((*it).from == 2);
		CHECK((*it).to == 3);
		CHECK((*it).weight == 2);
		++it;

		CHECK(it == g.end());
	}

	SECTION("operator--"){
		auto g = gdwg::graph<int, int>();
		g.insert_node(1);
		g.insert_node(2);
		g.insert_node(3);

		g.insert_edge(1, 1, -1);
		g.insert_edge(1, 2, 2);
		g.insert_edge(3, 2, 2);
		g.insert_edge(3, 3, -8);

		g.insert_edge(2, 3, 2);
		g.insert_edge(2, 1, 1);
		auto it = g.end();
		--it;

		CHECK((*it).from == 3);
		CHECK((*it).to == 3);
		CHECK((*it).weight == -8);
		--it;

		CHECK((*it).from == 3);
		CHECK((*it).to == 2);
		CHECK((*it).weight == 2);
		--it;

		CHECK((*it).from == 2);
		CHECK((*it).to == 3);
		CHECK((*it).weight == 2);
		--it;

		CHECK((*it).from == 2);
		CHECK((*it).to == 1);
		CHECK((*it).weight == 1);
		--it;

		CHECK((*it).from == 1);
		CHECK((*it).to == 2);
		CHECK((*it).weight == 2);
		--it;

		CHECK((*it).from == 1);
		CHECK((*it).to == 1);
		CHECK((*it).weight == -1);

		CHECK(it == g.begin());
	}

	SECTION("operator=="){
		auto g = gdwg::graph<int, int>();
		g.insert_node(1);
		g.insert_node(2);
		g.insert_node(3);

		g.insert_edge(1, 1, -1);
		g.insert_edge(1, 2, 2);
		g.insert_edge(3, 2, 2);
		g.insert_edge(3, 3, -8);

		g.insert_edge(2, 3, 2);
		g.insert_edge(2, 1, 1);
		auto it = g.begin();
		auto it2 = g.begin();
		CHECK(it == it2);
		++it;
		CHECK(it != it2);
		++it2;
		CHECK(it == it2);
	}
}
