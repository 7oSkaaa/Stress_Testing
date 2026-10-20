#pragma once
#include <vector>
#include <random>
#include <algorithm>
#include <cassert>
#include <numeric>
#include <set>
#include <tuple>
#include <unordered_set>

namespace gen_graphs {
    // Edge list from a set, shuffled so tests do not depend on sorted edge order
    template < typename E >
    std::vector < E > shuffled_edges (const std::set < E > &edge_set, std::mt19937_64 &rng) {
        std::vector < E > edges(edge_set.begin(), edge_set.end());
        std::shuffle(edges.begin(), edges.end(), rng);
        return edges;
    }

    // Tree generator (edges as pairs)
    inline std::vector < std::pair < int, int > > tree (int n, std::mt19937_64 &rng) {
        std::vector < int > nodes(n);
        std::iota(nodes.begin(), nodes.end(), 1);
        std::shuffle(nodes.begin(), nodes.end(), rng);
        std::vector < std::pair < int, int > > edges;
        for (int i = 1; i < n; ++i) {
            int u = nodes[i], v = nodes[std::uniform_int_distribution < int > (0, i - 1)(rng)];
            edges.emplace_back(u, v);
        }
        std::shuffle(edges.begin(), edges.end(), rng);
        return edges;
    }
    // Simple connected graph
    inline std::vector < std::pair < int, int > > simple_graph (int n, int m, std::mt19937_64 &rng) {
        assert(n - 1 <= m && m <= (long long)n * (n - 1) / 2);
        std::set < std::pair < int, int > > edge_set;
        for (const auto &e : tree (n, rng)) edge_set.insert(std::minmax(e.first, e.second));
        while ((int)edge_set.size() < m) {
            int u = std::uniform_int_distribution < int > (1, n)(rng);
            int v = std::uniform_int_distribution < int > (1, n)(rng);
            if (u != v) edge_set.insert(std::minmax(u, v));
        }
        auto edges = shuffled_edges (edge_set, rng);
        for (auto &e : edges) if (rng() & 1) std::swap(e.first, e.second);
        return edges;
    }

    // Generate a random connected weighted graph
    inline std::vector < std::tuple < int, int, int > > weighted_graph (int n, int m, int min_w, int max_w, std::mt19937_64 &rng) {
        auto edges = simple_graph (n, m, rng);
        std::vector < std::tuple < int, int, int > > weighted;
        for (const auto &e : edges) {
            int w = std::uniform_int_distribution < int > (min_w, max_w)(rng);
            weighted.emplace_back(e.first, e.second, w);
        }
        return weighted;
    }

    // Generate a random directed graph (no self-loops, no multi-edges)
    inline std::vector < std::pair < int, int > > directed_graph (int n, int m, std::mt19937_64 &rng) {
        assert(m <= (long long)n * (n - 1));
        std::set < std::pair < int, int > > edge_set;
        while ((int)edge_set.size() < m) {
            int u = std::uniform_int_distribution < int > (1, n)(rng);
            int v = std::uniform_int_distribution < int > (1, n)(rng);
            if (u != v) edge_set.emplace(u, v);
        }
        return shuffled_edges (edge_set, rng);
    }

    // Generate a random DAG (Directed Acyclic Graph)
    inline std::vector < std::pair < int, int > > dag (int n, int m, std::mt19937_64 &rng) {
        assert(m <= (long long)n * (n - 1) / 2);
        std::vector < int > nodes(n);
        std::iota(nodes.begin(), nodes.end(), 1);
        std::shuffle(nodes.begin(), nodes.end(), rng);
        std::set < std::pair < int, int > > edge_set;
        while ((int)edge_set.size() < m) {
            int u = std::uniform_int_distribution < int > (0, n - 2)(rng);
            int v = std::uniform_int_distribution < int > (u + 1, n - 1)(rng);
            edge_set.emplace(nodes[u], nodes[v]);
        }
        return shuffled_edges (edge_set, rng);
    }

    // Generate a random bipartite graph
    inline std::vector < std::pair < int, int > > bipartite (int n1, int n2, int m, std::mt19937_64 &rng) {
        assert(m <= (long long)n1 * n2);
        std::set < std::pair < int, int > > edge_set;
        while ((int)edge_set.size() < m) {
            int u = std::uniform_int_distribution < int > (1, n1)(rng);
            int v = std::uniform_int_distribution < int > (1, n2)(rng) + n1;
            edge_set.emplace(u, v);
        }
        return shuffled_edges (edge_set, rng);
    }

    // Generate a random star graph (center node connected to all others)
    inline std::vector < std::pair < int, int > > star (int n, std::mt19937_64 &rng, int center = 1) {
        std::vector < std::pair < int, int > > edges;
        for (int i = 1; i <= n; ++i) {
            if (i != center) edges.emplace_back(center, i);
        }
        std::shuffle(edges.begin(), edges.end(), rng);
        return edges;
    }

    // Generate a random cycle graph
    inline std::vector < std::pair < int, int > > cycle (int n, std::mt19937_64 &rng) {
        assert(n >= 3);
        std::vector < int > nodes(n);
        std::iota(nodes.begin(), nodes.end(), 1);
        std::shuffle(nodes.begin(), nodes.end(), rng);
        std::vector < std::pair < int, int > > edges;
        for (int i = 0; i < n; ++i) {
            edges.emplace_back(nodes[i], nodes[(i + 1) % n]);
        }
        return edges;
    }

    // Generate a random complete graph (all pairs)
    inline std::vector < std::pair < int, int > > complete (int n) {
        std::vector < std::pair < int, int > > edges;
        for (int i = 1; i <= n; ++i) {
            for (int j = i + 1; j <= n; ++j) {
                edges.emplace_back(i, j);
            }
        }
        return edges;
    }

    // Generate a random regular graph (each node has degree d)
    inline std::vector < std::pair < int, int > > regular (int n, int d, std::mt19937_64 &rng) {
        assert(0 <= d && d < n && (long long)n * d % 2 == 0);
        if (2 * d > n - 1) {
            // Dense: the complement of a random (n - 1 - d)-regular graph is d-regular, and swaps there rarely get rejected.
            std::vector < std::vector < bool > > missing(n + 1, std::vector < bool > (n + 1));
            for (const auto &e : regular (n, n - 1 - d, rng)) missing[e.first][e.second] = missing[e.second][e.first] = true;
            std::vector < std::pair < int, int > > edges;
            for (int u = 1; u <= n; ++u)
                for (int v = u + 1; v <= n; ++v)
                    if (!missing[u][v]) edges.emplace_back(rng() & 1 ? std::make_pair(u, v) : std::make_pair(v, u));
            std::shuffle(edges.begin(), edges.end(), rng);
            return edges;
        }
        // Start from a circulant d-regular graph: i joins i +- 1..d/2, plus the opposite node when d is odd.
        std::vector < std::pair < int, int > > edges;
        for (int i = 0; i < n; ++i)
            for (int k = 1; k <= d / 2; ++k) edges.emplace_back(i, (i + k) % n);
        if (d % 2 == 1)
            for (int i = 0; i < n / 2; ++i) edges.emplace_back(i, i + n / 2);
        if (edges.empty()) return edges;

        // Randomize with degree-preserving swaps: a-b, c-e becomes a-c, b-e when that stays simple.
        auto key = [n](int u, int v) { return (long long)std::min(u, v) * n + std::max(u, v); };
        std::unordered_set < long long > edge_set;
        edge_set.reserve(edges.size() * 2);
        for (const auto &e : edges) edge_set.insert(key(e.first, e.second));
        std::uniform_int_distribution < size_t > pick(0, edges.size() - 1);
        // ponytail: 10 swaps per edge is a rule-of-thumb mixing time; raise it if graphs still look structured
        for (size_t step = 0; step < 10 * edges.size(); ++step) {
            size_t i = pick(rng), j = pick(rng);
            int a = edges[i].first, b = edges[i].second, c = edges[j].first, e = edges[j].second;
            if (rng() & 1) std::swap(c, e);
            if (a == c || b == e || edge_set.count(key(a, c)) || edge_set.count(key(b, e))) continue;
            edge_set.erase(key(a, b));
            edge_set.erase(key(c, e));
            edge_set.insert(key(a, c));
            edge_set.insert(key(b, e));
            edges[i] = {a, c};
            edges[j] = {b, e};
        }

        std::vector < int > label(n);
        std::iota(label.begin(), label.end(), 1);
        std::shuffle(label.begin(), label.end(), rng);
        for (auto &edge : edges) {
            edge = {label[edge.first], label[edge.second]};
            if (rng() & 1) std::swap(edge.first, edge.second);
        }
        std::shuffle(edges.begin(), edges.end(), rng);
        return edges;
    }

    // Generate a random tree with a given diameter
    inline std::vector < std::pair < int, int > > tree_with_diameter (int n, int diameter, std::mt19937_64 &rng) {
        if (diameter >= n) diameter = n - 1;
        assert(diameter >= 2 || diameter == n - 1);
        std::vector < int > nodes(n);
        std::iota(nodes.begin(), nodes.end(), 1);
        std::shuffle(nodes.begin(), nodes.end(), rng);
        std::vector < std::pair < int, int > > edges;
        for (int i = 0; i < diameter; ++i) {
            edges.emplace_back(nodes[i], nodes[i + 1]);
        }
        // nodes[0..diameter] form the longest path; hanging leaves off its inner nodes keeps it longest
        for (int i = diameter + 1; i < n; ++i) {
            int attach = std::uniform_int_distribution < int > (1, diameter - 1)(rng);
            edges.emplace_back(nodes[attach], nodes[i]);
        }
        std::shuffle(edges.begin(), edges.end(), rng);
        return edges;
    }

    // Generate a chain tree (path tree)
    inline std::vector < std::pair < int, int > > chain_tree (int n) {
        std::vector < std::pair < int, int > > edges;
        for (int i = 2; i <= n; ++i) {
            edges.emplace_back(i - 1, i);
        }
        return edges;
    }
}
