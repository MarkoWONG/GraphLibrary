#ifndef GDWG_GRAPH_HPP
#define GDWG_GRAPH_HPP
#include <map>
#include <vector>
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
				nodes_ = std::make_unique<std::map<N, std::vector<std::pair<N,E>>>>();
			}
			graph(std::initializer_list<N> list){
				nodes_ = std::make_unique<std::map<N, std::vector<std::pair<N,E>>>>();
				for (auto itr = list.begin(); itr != list.end(); ++itr) {
					nodes_.get()->insert({*itr, std::vector<std::pair<N,E>>{}});
				}
			}
			template<typename InputIt>
			graph(InputIt first, InputIt last){
				nodes_ = std::make_unique<std::map<N, std::vector<std::pair<N,E>>>>();
				for (auto itr = first; itr != last; ++itr) {
					nodes_.get()->insert({*itr, std::vector<std::pair<N,E>>{}});
				}


			}
			graph(graph&& other) noexcept: nodes_{std::exchange(other.nodes_, nullptr)}{};

			auto operator=(graph&& other) noexcept -> graph&{
				nodes_ = std::exchange(other.nodes_, nullptr);
				return *this;
			}

			graph(graph const& other) noexcept{
				nodes_ = std::make_unique<std::map<N, std::vector<std::pair<N,E>>>>();
				for (auto it = other.nodes_.get()->begin(); it != other.nodes_.get()->end(); ++it) {
					nodes_.get()->insert({it->first, it->second});
				}
			}
			auto operator=(graph const& other) -> graph&{
				nodes_ = std::make_unique<std::map<N, std::vector<std::pair<N,E>>>>();
				for (auto it = other.nodes_.get()->begin(); it != other.nodes_.get()->end(); ++it) {
					nodes_.get()->insert({it->first, it->second});
				}
				return *this;
			}

			// Modifiers
			auto insert_node(N const& value) -> bool{
				if (!is_node(value)) {
				// not found
					nodes_.get()->insert({value, std::vector<std::pair<N,E>>{}});
					return true;
				}
				// found
				return false;
			}

			auto insert_edge(N const& src, N const& dst, E const& weight) -> bool{
				if (is_node(src) && is_node(dst)) {
					if (std::find(nodes_.get()->at(src).begin(), nodes_.get()->at(src).end(), std::make_pair(dst, weight)) == nodes_.get()->at(src).end()){
						nodes_.get()->at(src).push_back(std::make_pair(dst, weight));
						return true;
					}
					return false;
				}
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::insert_edge when either src or dst node does not exist");
				return false;
			}
			auto replace_node(N const& old_data, N const& new_data) -> bool;
			auto merge_replace_node(N const& old_data, N const& new_data) -> void;
			auto erase_node(N const& value) -> bool;
			auto erase_edge(N const& src, N const& dst, E const& weight) -> bool;
			// auto erase_edge(iterator i) -> iterator;
			// auto erase_edge(iterator i, iterator s) -> iterator;
			auto clear() noexcept -> void;

			// Accessors
			[[nodiscard]] auto is_node(N const& value) -> bool{
				return (nodes_.get()->find(value)!= nodes_.get()->end());
			}
			[[nodiscard]] auto empty() -> bool;
			[[nodiscard]] auto is_connected(N const& src, N const& dst) -> bool;
			[[nodiscard]] auto nodes() -> std::vector<N>;
			[[nodiscard]] auto weights(N const& src, N const& dst) -> std::vector<E>;
			// [[nodiscard]] auto find(N const& src, N const& dst, E const& weight) -> iterator;
			[[nodiscard]] auto connections(N const& src) -> std::vector<N>;

			// Iterator Access
			// [[nodiscard]] auto begin() const -> iterator;
			// [[nodiscard]] auto end() const -> iterator;

			// Comparisons
			[[nodiscard]] auto operator==(graph const& other) -> bool;

			// Extractor
			friend auto operator<<(std::ostream& os, graph const& g) -> std::ostream&{
				for (auto it = g.nodes_.get()->begin(); it != g.nodes_.get()->end(); ++it) {
				// for (auto const& [from, edges] : g.nodes_.get()) {
					os << "( " << it->first;
					for (auto const& [to, weight]: it->second){
						os << "( " << to << " | " << weight << " )" << std::endl;
					}
					os << " )" << std::endl;
				}
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
			// struct value_type {
			// 	N from;
			// 	N to;
			// 	E weight;
			// };
			std::unique_ptr<std::map<N, std::vector<std::pair<N,E>>>> nodes_;
	};

} // namespace gdwg

#endif // GDWG_GRAPH_HPP
