#ifndef GDWG_GRAPH_HPP
#define GDWG_GRAPH_HPP
#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <set>
#include <vector>

namespace gdwg {
	template<typename N, typename E>
	class graph {
		struct value_type {
			N from;
			N to;
			E weight;
		};
		class iterator {
			using outer_iterator = typename std::map<N, std::set<std::pair<N, E>>>::const_iterator;
			using inner_iterator = typename std::set<std::pair<N, E>>::const_iterator;

		public:
			using value_type = graph<N, E>::value_type;
			using reference = value_type;
			using pointer = void;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::bidirectional_iterator_tag;

			// Iterator constructor
			iterator() = default;

			explicit iterator(outer_iterator outer, inner_iterator inner, outer_iterator end)
			: outer_(outer)
			, inner_(inner)
			, outer_end_(end) {}

			// Iterator source
			auto operator*() const -> const reference {
				return reference{outer_->first, inner_->first, inner_->second};
			}

			auto operator->() const -> pointer {
				return &(this->operator*());
			}

			// Iterator traversal
			// Precondition starting node has an edge
			auto operator++() -> iterator& {
				// increment inner_ if not at end and return value
				if (inner_ != outer_->second.cend()) {
					++inner_;
					if (inner_ != outer_->second.cend()) {
						return *this;
					}
				}

				++outer_;
				// reached the end of structure
				if (outer_ == outer_end_) {
					inner_ = inner_iterator();
				}
				else {
					// skips nodes with no edges
					while (outer_->second.empty()) {
						++outer_;
						// reached the end of structure
						if (outer_ == outer_end_) {
							inner_ = inner_iterator();
							return *this;
						}
					}
					inner_ = outer_->second.cbegin();
				}
				return *this;
			}

			auto operator++(int) -> iterator {
				auto ret = *this;
				++*this;
				return ret;
			}
			auto operator--() -> iterator& {
				// skips nodes with no edges
				while (outer_ == outer_end_ || inner_ == outer_->second.cbegin()) {
					--outer_;
					inner_ = outer_->second.cend();
				}
				--inner_;
				return *this;
			}
			auto operator--(int) -> iterator {
				auto ret = *this;
				--*this;
				return ret;
			}

			// Iterator comparison
			auto operator==(const iterator&) const -> bool = default;

		private:
			outer_iterator outer_;
			inner_iterator inner_;
			outer_iterator outer_end_;

			// To allow graph to modify this
			friend class graph<N, E>;
		};

	public:
		// Iterator
		using iterator = graph<N, E>::iterator;
		using const_iterator = const graph<N, E>::iterator;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		[[nodiscard]] auto begin() const noexcept -> iterator {
			// empty graph
			if (nodes_.get()->empty()) {
				// default constructed inner_iterator is a NULL/EOF iterator
				return iterator(nodes_.get()->begin(), {}, nodes_.get()->cend());
			}
			// return the first node with an edge to be used as the start of iterator
			for (auto it = nodes_.get()->begin(); it != nodes_.get()->end(); ++it) {
				if (!it->second.empty()) {
					return iterator(it, it->second.begin(), nodes_.get()->cend());
				}
			}
			// all nodes had no edge
			return iterator(nodes_.get()->cend(), {}, nodes_.get()->cend());
		}

		auto cbegin() const noexcept -> const_iterator {
			// empty graph
			if (nodes_.get()->empty()) {
				// default constructed inner_iterator is a NULL/EOF iterator
				return const_iterator(nodes_.get()->begin(), {}, nodes_.get()->cend());
			}
			// return the first node with an edge to be used as the start of iterator
			for (auto it = nodes_.get()->begin(); it != nodes_.get()->end(); ++it) {
				if (!it->second.empty()) {
					return const_iterator(it, it->second.begin(), nodes_.get()->cend());
				}
			}
			// all nodes had no edge
			return const_iterator(nodes_.get()->cend(), {}, nodes_.get()->cend());
		}

		[[nodiscard]] auto end() const noexcept -> iterator {
			// default constructed inner_iterator is a NULL/EOF iterator
			return iterator(nodes_.get()->cend(), {}, nodes_.get()->cend());
		}

		auto cend() const noexcept -> const_iterator {
			// default constructed inner_iterator is a NULL/EOF iterator
			return const_iterator(nodes_.get()->cend(), {}, nodes_.get()->cend());
		}

		// Constructors
		graph() noexcept {
			nodes_ = std::make_unique<std::map<N, std::set<std::pair<N, E>>>>();
		}
		graph(std::initializer_list<N> list) {
			nodes_ = std::make_unique<std::map<N, std::set<std::pair<N, E>>>>();
			for (auto const& value : list) {
				nodes_.get()->insert({value, std::set<std::pair<N, E>>{}});
			}
		}
		template<typename InputIt>
		graph(InputIt first, InputIt last) {
			nodes_ = std::make_unique<std::map<N, std::set<std::pair<N, E>>>>();
			for (auto itr = first; itr != last; ++itr) {
				nodes_.get()->insert({*itr, std::set<std::pair<N, E>>{}});
			}
		}
		graph(graph&& other) noexcept
		: nodes_{std::exchange(other.nodes_, graph().nodes_)} {};

		auto operator=(graph&& other) noexcept -> graph& {
			nodes_ = std::exchange(other.nodes_, graph().nodes_);
			return *this;
		}

		graph(graph const& other) noexcept {
			nodes_ = std::make_unique<std::map<N, std::set<std::pair<N, E>>>>();
			for (auto it = other.nodes_.get()->begin(); it != other.nodes_.get()->end(); ++it) {
				nodes_.get()->insert({it->first, it->second});
			}
		}
		auto operator=(graph const& other) -> graph& {
			nodes_ = std::make_unique<std::map<N, std::set<std::pair<N, E>>>>();
			for (auto it = other.nodes_.get()->begin(); it != other.nodes_.get()->end(); ++it) {
				nodes_.get()->insert({it->first, it->second});
			}
			return *this;
		}

		// Modifiers
		auto insert_node(N const& value) noexcept -> bool {
			if (is_node(value)) {
				return false;
			}
			nodes_.get()->insert({value, std::set<std::pair<N, E>>{}});
			return true;
		}

		auto insert_edge(N const& src, N const& dst, E const& weight) -> bool {
			if (is_node(src) && is_node(dst)) {
				auto edge_set = nodes_.get()->at(src);
				// insert edge only if edge don't already exist
				if (std::find(edge_set.begin(), edge_set.end(), std::make_pair(dst, weight))
				    == edge_set.end()) {
					nodes_.get()->at(src).insert(std::make_pair(dst, weight));
					return true;
				}
				return false;
			}
			throw std::runtime_error("Cannot call gdwg::graph<N, E>::insert_edge when either src or "
			                         "dst node does not exist");
			return false;
		}

		auto replace_node(N const& old_data, N const& new_data) -> bool {
			if (!is_node(old_data)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::replace_node on a node that "
				                         "doesn't exist");
				return false;
			}
			if (is_node(new_data)) {
				return false;
			}
			// Rename the old_data key to be the new_data
			auto node_handler = nodes_.get()->extract(old_data);
			node_handler.key() = new_data;
			nodes_.get()->insert(std::move(node_handler));

			// rename any other old_data to new_data in other exisiting edges
			for (auto it = nodes_.get()->begin(); it != nodes_.get()->end(); ++it) {
				auto edge_set = nodes_.get()->at(it->first);
				auto new_edge_set = std::set<std::pair<N, E>>();
				for (auto& [dst, weight] : edge_set) {
					if (dst == old_data) {
						new_edge_set.insert(std::make_pair(new_data, weight));
					}
					else {
						new_edge_set.insert(std::make_pair(dst, weight));
					}
				}
				nodes_.get()->at(it->first) = new_edge_set;
			}

			return true;
		}

		auto merge_replace_node(N const& old_data, N const& new_data) -> void {
			if (!(is_node(old_data) && is_node(new_data))) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::merge_replace_node on old or "
				                         "new data if they don't exist in the graph");
			}
			if (old_data == new_data){
				return;
			}
			// For all outgoing edges
			for (auto& [dst, weight] : nodes_.get()->at(old_data)) {
				nodes_.get()->at(new_data).insert(std::make_pair(dst, weight));
			}
			// remove old node
			nodes_.get()->erase(old_data);
			// For all nodes replace outgoing edges with old_node
			for (auto it = nodes_.get()->begin(); it != nodes_.get()->end(); ++it) {
				auto edge_set = nodes_.get()->at(it->first);
				auto new_edge_set = std::set<std::pair<N, E>>();
				for (auto& [dst, weight] : edge_set) {
					if (dst == old_data) {
						new_edge_set.insert(std::make_pair(new_data, weight));
					}
					else {
						new_edge_set.insert(std::make_pair(dst, weight));
					}
				}
				nodes_.get()->at(it->first) = new_edge_set;
			}
		}
		auto erase_node(N const& value) -> bool {
			auto value_removed = nodes_.get()->erase(value);
			// For all nodes and it's edges replace it with a copy of it's edge without the old node
			for (auto it = nodes_.get()->begin(); it != nodes_.get()->end(); ++it) {
				auto edge_set = nodes_.get()->at(it->first);
				auto new_edge_set = std::set<std::pair<N, E>>();
				for (auto& [dst, weight] : edge_set) {
					if (dst != value) {
						new_edge_set.insert(std::make_pair(dst, weight));
					}
				}
				nodes_.get()->at(it->first) = new_edge_set;
			}
			return value_removed;
		}

		auto erase_edge(N const& src, N const& dst, E const& weight) -> bool {
			if (!(is_node(src) && is_node(dst))) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::erase_edge on src or dst if "
				                         "they don't exist in the graph");
				return false;
			}
			auto edge_removed = false;
			auto edge_set = nodes_.get()->at(src);
			auto new_edge_set = std::set<std::pair<N, E>>();
			for (auto& [e_dst, e_weight] : edge_set) {
				if (e_dst == dst && e_weight == weight) {
					edge_removed = true;
				}
				else {
					new_edge_set.insert(std::make_pair(e_dst, e_weight));
				}
			}
			nodes_.get()->at(src) = new_edge_set;
			return edge_removed;
		}

		auto erase_edge(iterator i) noexcept -> iterator {
			if (i == end()) {
				return end();
			}
			auto key = *(i.outer_);
			auto pair = *(i.inner_);
			++i;
			nodes_.get()->at(key.first).erase(pair);
			return iterator(i.outer_, i.inner_, nodes_.get()->end());
		}

		auto erase_edge(iterator i, iterator s) noexcept -> iterator {
			while (i != s) {
				i = erase_edge(i);
			}
			if (s != end()) {
				++s;
			}
			return iterator(s.outer_, s.inner_, nodes_.get()->end());
		}

		auto clear() noexcept -> void {
			nodes_.get()->clear();
		}

		// Accessors
		[[nodiscard]] auto is_node(N const& value) noexcept -> bool {
			return (nodes_.get()->find(value) != nodes_.get()->end());
		}
		[[nodiscard]] auto empty() noexcept -> bool {
			return (nodes_.get()->empty());
		}
		[[nodiscard]] auto is_connected(N const& src, N const& dst) -> bool {
			if (!(is_node(src) && is_node(dst))) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::merge_replace_node on old or "
				                         "new data if they don't exist in the graph");
				return false;
			}
			for (auto& [edg_dst, weight] : nodes_.get()->at(src)) {
				if (edg_dst == dst) {
					return true;
				}
			}
			return false;
		}
		[[nodiscard]] auto nodes() -> std::vector<N> {
			std::vector<N> vec;
			std::transform(nodes_.get()->begin(),
			               nodes_.get()->end(),
			               std::back_inserter(vec),
			               [](auto& map) { return map.first; });
			return vec;
		}
		[[nodiscard]] auto weights(N const& src, N const& dst) -> std::vector<E> {
			if (!(is_node(src) && is_node(dst))) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::weights if src or dst node "
				                         "don't exist in the graph");
			}
			std::vector<E> vec;
			for (auto& [edg_dst, weight] : nodes_.get()->at(src)) {
				if (edg_dst == dst) {
					vec.push_back(weight);
				}
			}
			return vec;
		}

		[[nodiscard]] auto find(N const& src, N const& dst, E const& weight) noexcept -> iterator {
			auto outer = nodes_.get()->find(src);
			if (outer == nodes_.get()->end()) {
				return end();
			}
			auto inner = outer->second.find(std::make_pair(dst, weight));
			if (inner != outer->second.end()) {
				return iterator(outer, inner, nodes_.get()->cend());
			}
			return end();
		}

		[[nodiscard]] auto connections(N const& src) -> std::vector<N> {
			if (!is_node(src)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::connections if src doesn't "
				                         "exist in the graph");
			}
			auto edge_set = nodes_.get()->at(src);
			std::set<N> uni_edge;
			for (auto& [edg_dst, weight] : nodes_.get()->at(src)) {
				uni_edge.insert(edg_dst);
			}
			return std::vector<N>(uni_edge.begin(), uni_edge.end());
		}

		// Comparisons
		[[nodiscard]] auto operator==(graph const& other) const -> bool {
			return (
			   nodes_.get()->size() == other.nodes_.get()->size()
			   && std::equal(nodes_.get()->begin(), nodes_.get()->end(), other.nodes_.get()->begin()));
		}

		// Extractor
		friend auto operator<<(std::ostream& os, graph const& g) noexcept -> std::ostream& {
			for (auto it = g.nodes_.get()->begin(); it != g.nodes_.get()->end(); ++it) {
				os << it->first << " (" << std::endl;
				for (auto const& [to, weight] : it->second) {
					os << "  " << to << " | " << weight << std::endl;
				}
				os << ")" << std::endl;
			}
			return os;
		}

	private:
		// internal structure
		std::unique_ptr<std::map<N, std::set<std::pair<N, E>>>> nodes_;
	};

} // namespace gdwg

#endif // GDWG_GRAPH_HPP
