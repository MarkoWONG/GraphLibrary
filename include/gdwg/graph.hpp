#ifndef GDWG_GRAPH_HPP
#define GDWG_GRAPH_HPP
#include <map>
// This will not compile straight away
namespace gdwg {
	template<typename N, typename E>
	class graph{
		public:
			// Constructor
			graph();
			graph(std::initializer_list<N> il);
			graph(graph&& other) noexcept;
			auto operator=(graph&& other) noexcept -> graph&;
			graph(graph const& other) noexcept;
			auto operator=(graph const& other) -> graph&;

			// Modifiers
			auto insert_node(N const& value) -> bool;
			auto insert_edge(N const& src, N const& dst, E const& weight) -> bool;
			auto replace_node(N const& old_data, N const& new_data) -> bool;
			auto merge_replace_node(N const& old_data, N const& new_data) -> void;
			auto erase_node(N const& value) -> bool;
			auto erase_edge(N const& src, N const& dst, E const& weight) -> bool;
			// auto erase_edge(iterator i) -> iterator;
			// auto erase_edge(iterator i, iterator s) -> iterator;
			auto clear() noexcept -> void;

			// Accessors
			[[nodiscard]] auto is_node(N const& value) -> bool;
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
			friend auto operator<<(std::ostream& os, graph const& g) -> std::ostream&;

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
			std::unique_ptr<std::map<N,E>> values_;
	};

} // namespace gdwg

#endif // GDWG_GRAPH_HPP
