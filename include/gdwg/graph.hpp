#ifndef GDWG_GRAPH_HPP
#define GDWG_GRAPH_HPP
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <memory>
#include <iostream>
// This will not compile straight away
namespace gdwg {
	template<typename N, typename E>
	class graph{
		public:
			// Constructor
			graph() noexcept {
				nodes_ = std::make_unique<std::map<N, std::multiset <std::pair<N,E>>>>();
			}
			graph(std::initializer_list<N> list){
				nodes_ = std::make_unique<std::map<N, std::multiset <std::pair<N,E>>>>();
				for (auto itr = list.begin(); itr != list.end(); ++itr) {
					nodes_.get()->insert({*itr, std::multiset <std::pair<N,E>>{}});
				}
			}
			template<typename InputIt>
			graph(InputIt first, InputIt last){
				nodes_ = std::make_unique<std::map<N, std::multiset <std::pair<N,E>>>>();
				for (auto itr = first; itr != last; ++itr) {
					nodes_.get()->insert({*itr, std::multiset <std::pair<N,E>>{}});
				}


			}
			graph(graph&& other) noexcept: nodes_{std::exchange(other.nodes_, graph().nodes_)}{};

			auto operator=(graph&& other) noexcept -> graph&{
				nodes_ = std::exchange(other.nodes_, graph().nodes_);
				return *this;
			}

			graph(graph const& other) noexcept{
				nodes_ = std::make_unique<std::map<N, std::multiset <std::pair<N,E>>>>();
				for (auto it = other.nodes_.get()->begin(); it != other.nodes_.get()->end(); ++it) {
					nodes_.get()->insert({it->first, it->second});
				}
			}
			auto operator=(graph const& other) -> graph&{
				nodes_ = std::make_unique<std::map<N, std::multiset <std::pair<N,E>>>>();
				for (auto it = other.nodes_.get()->begin(); it != other.nodes_.get()->end(); ++it) {
					nodes_.get()->insert({it->first, it->second});
				}
				return *this;
			}

			// Modifiers
			auto insert_node(N const& value) -> bool{
				if (is_node(value)) {
					return false;
				}
				nodes_.get()->insert({value, std::multiset <std::pair<N,E>>{}});
				return true;
			}

			auto insert_edge(N const& src, N const& dst, E const& weight) -> bool{
				if (is_node(src) && is_node(dst)) {
					auto edge_set = nodes_.get()->at(src);
					if (std::find(edge_set.begin(), edge_set.end(), std::make_pair(dst, weight)) == edge_set.end()){
						nodes_.get()->at(src).insert(std::make_pair(dst, weight));
						return true;
					}
					return false;
				}
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::insert_edge when either src or dst node does not exist");
				return false;
			}

			auto replace_node(N const& old_data, N const& new_data) -> bool{
				if(!is_node(old_data)){
					throw std::runtime_error("Cannot call gdwg::graph<N, E>::replace_node on a node that doesn't exist");
					return false;
				}
				if (is_node(new_data)){
					return false;
				}
				// Rename the old_data key to be the new_data
				auto node_handler = nodes_.get()->extract(old_data);
				node_handler.key() = new_data;
				nodes_.get()->insert(std::move(node_handler));

				// rename any other old_data to new_data in other exisiting edges
				for (auto it = nodes_.get()->begin(); it != nodes_.get()->end(); ++it) {
					auto edge_set = nodes_.get()->at(it->first);
					auto new_edge_set = std::multiset<std::pair<N,E>>();
					for (auto& [dst,weight] : edge_set){
						if (dst == old_data){
							new_edge_set.insert(std::make_pair(new_data, weight));
						}
						else{
							new_edge_set.insert(std::make_pair(dst, weight));
						}
					}
					nodes_.get()->at(it->first) = new_edge_set;
				}

				return true;
			}

			auto merge_replace_node(N const& old_data, N const& new_data) -> void{
				if (!(is_node(old_data) && is_node(new_data))){
					throw std::runtime_error("Cannot call gdwg::graph<N, E>::merge_replace_node on old or new data if they don't exist in the graph");
				}
				// For all outgoing edges
				for (auto& [dst, weight] : nodes_.get()->at(old_data)){
					nodes_.get()->at(new_data).insert(std::make_pair(dst, weight));
				}
				// remove old node
				nodes_.get()->erase(old_data);
				// For all nodes replace outgoing edges with old_node
				for (auto it = nodes_.get()->begin(); it != nodes_.get()->end(); ++it) {
					auto edge_set = nodes_.get()->at(it->first);
					auto new_edge_set = std::multiset<std::pair<N,E>>();
					for (auto& [dst,weight] : edge_set){
						if (dst == old_data){
							new_edge_set.insert(std::make_pair(new_data, weight));
						}
						else{
							new_edge_set.insert(std::make_pair(dst, weight));
						}
					}
					nodes_.get()->at(it->first) = new_edge_set;
				}
			}
			auto erase_node(N const& value) -> bool{

				auto value_removed = nodes_.get()->erase(value);
				// For all nodes replace outgoing edges with old_node
				for (auto it = nodes_.get()->begin(); it != nodes_.get()->end(); ++it) {
					auto edge_set = nodes_.get()->at(it->first);
					auto new_edge_set = std::multiset<std::pair<N,E>>();
					for (auto& [dst,weight] : edge_set){
						if (dst == value){
						}
						else{
							new_edge_set.insert(std::make_pair(dst, weight));
						}
					}
					nodes_.get()->at(it->first) = new_edge_set;
				}
				return value_removed;
			}

			auto erase_edge(N const& src, N const& dst, E const& weight) -> bool{
				if (!(is_node(src) && is_node(dst))){
					throw std::runtime_error("Cannot call gdwg::graph<N, E>::erase_edge on src or dst if they don't exist in the graph");
					return false;
				}
				auto edge_removed = false;
				auto edge_set = nodes_.get()->at(src);
				auto new_edge_set = std::multiset<std::pair<N,E>>();
				for (auto& [e_dst,e_weight] : edge_set){
					if (e_dst == dst && e_weight == weight){
						edge_removed = true;
					}
					else{
						new_edge_set.insert(std::make_pair(e_dst, e_weight));
					}
				}
				nodes_.get()->at(src) = new_edge_set;
				return edge_removed;


			}
			// auto erase_edge(iterator i) -> iterator;
			// auto erase_edge(iterator i, iterator s) -> iterator;
			auto clear() noexcept -> void{
				nodes_.get()->clear();
			}

			// Accessors
			[[nodiscard]] auto is_node(N const& value) -> bool{
				return (nodes_.get()->find(value)!= nodes_.get()->end());
			}
			[[nodiscard]] auto empty() -> bool{
				return(nodes_.get()->empty());
			}
			[[nodiscard]] auto is_connected(N const& src, N const& dst) -> bool{
				if (!(is_node(src) && is_node(dst))){
					throw std::runtime_error("Cannot call gdwg::graph<N, E>::merge_replace_node on old or new data if they don't exist in the graph");
					return false;
				}
				for (auto& [edg_dst, weight] : nodes_.get()->at(src)){
					if(edg_dst == dst){
						return true;
					}
				}
				return false;
			}
			[[nodiscard]] auto nodes() -> std::vector<N>{
				std::vector<N> vec;
				std::transform( nodes_.get()->begin(), nodes_.get()->end(), std::back_inserter(vec), [](auto &map){ return map.first;});
				return vec;
			}
			[[nodiscard]] auto weights(N const& src, N const& dst) -> std::vector<E>{
				if (!(is_node(src) && is_node(dst))){
					throw std::runtime_error("Cannot call gdwg::graph<N, E>::weights if src or dst node don't exist in the graph");
				}
				std::vector<E> vec;
				for (auto& [edg_dst, weight] : nodes_.get()->at(src)){
					if(edg_dst == dst){
						vec.push_back(weight);
					}
				}
				return vec;
			}
			// [[nodiscard]] auto find(N const& src, N const& dst, E const& weight) -> iterator;
			[[nodiscard]] auto connections(N const& src) -> std::vector<N>{
				if (!is_node(src)){
					throw std::runtime_error("Cannot call gdwg::graph<N, E>::connections if src doesn't exist in the graph");
				}
				auto edge_set = nodes_.get()->at(src);
				std::vector<N> vec;
				for (auto& [edg_dst, weight] : nodes_.get()->at(src)){
					vec.push_back(edg_dst);
				}
				return vec;
			}

			// Iterator Access
			// [[nodiscard]] auto begin() const -> iterator;
			// [[nodiscard]] auto end() const -> iterator;

			// Comparisons
			[[nodiscard]] auto operator==(graph const& other) const-> bool{
				return (nodes_.get()->size() == other.nodes_.get()->size() && std::equal(nodes_.get()->begin(), nodes_.get()->end(),other.nodes_.get()->begin()));
			}

			// Extractor
			friend auto operator<<(std::ostream& os, graph const& g) -> std::ostream&{
				// os << "(";
				for (auto it = g.nodes_.get()->begin(); it != g.nodes_.get()->end(); ++it) {
				// for (auto const& [from, edges] : g.nodes_.get()) {
					os << it->first << " (" << std::endl;
					for (auto const& [to, weight]: it->second){
						os << "  "  << to << " | " << weight << std::endl;
					}
					os << ")" << std::endl;
				}
				// os << ")" << std::endl;
				return os;
			}

			// // Iterator
			// iterator();
			// explicit iterator(unspecified);

			// // Iterator source
			// auto operator*() -> reference;

			// // Iterator traversal
			// auto operator++() -> iterator&;

			// // Iterator comparison
			// auto operator==(iterator const& other) -> bool;

		private:
			std::unique_ptr<std::map<N, std::multiset<std::pair<N,E>>>> nodes_;
	};

} // namespace gdwg

#endif // GDWG_GRAPH_HPP
