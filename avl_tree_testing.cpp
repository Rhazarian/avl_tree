#include <set>
#include <random>
#include <gtest/gtest.h>

#include "avl_tree.h"

namespace {

std::mt19937 gen(0); // NOLINT
std::uniform_int_distribution<uint32_t> dist(std::numeric_limits<uint32_t>::min(), // NOLINT
        std::numeric_limits<uint32_t>::max()); // NOLINT

uint32_t random_uint32()
{
    return dist(gen);
}

std::uniform_int_distribution<size_t> iter_dist(0, 100); // NOLINT

size_t random_loop_count()
{
    return iter_dist(gen);
}

}

TEST(correctness, insert_iterate_clear) { // NOLINT
    constexpr size_t LOOP_COUNT = 50;
    avl_tree<uint32_t> tree;
    std::set<uint32_t> set;
    for (size_t i = 0; i < LOOP_COUNT; ++i) {
        size_t elements = random_loop_count();
        for (size_t j = 0; j < elements; ++j) {
            uint32_t element = random_uint32();
            tree.insert(element);
            set.insert(element);
        }
        std::vector<uint32_t> from_tree;
        from_tree.reserve(elements);
        std::vector<uint32_t> from_set;
        from_set.reserve(elements);
        for (auto const& element : tree) {
            from_tree.emplace_back(element);
        }
        EXPECT_EQ(elements, from_tree.size());
        for (auto const& element : set) {
            from_set.emplace_back(element);
        }
        for (size_t j = 0; j < elements; ++j) {
            EXPECT_EQ(from_set[j], from_tree[j]);
        }
        tree.clear();
        set.clear();
    }
}

TEST(correctness, reverse_iterate) { // NOLINT
    constexpr size_t LOOP_COUNT = 50;
    for (size_t i = 0; i < LOOP_COUNT; ++i) {
        avl_tree<uint32_t> tree;
        std::set<uint32_t> set;
        size_t elements = random_loop_count();
        for (size_t j = 0; j < elements; ++j) {
            uint32_t element = random_uint32();
            tree.insert(element);
            set.insert(element);
        }
        std::vector<uint32_t> from_tree;
        from_tree.reserve(elements);
        std::vector<uint32_t> from_set;
        from_set.reserve(elements);
        for (auto it = tree.crbegin(); it != tree.crend(); ++it) {
            from_tree.emplace_back(*it);
        }
        EXPECT_EQ(elements, from_tree.size());
        for (auto it = set.crbegin(); it != set.crend(); ++it) {
            from_set.emplace_back(*it);
        }
        for (size_t j = 0; j < elements; ++j) {
            EXPECT_EQ(from_set[j], from_tree[j]);
        }
    }
}

TEST(correctness, insert_find_erase_empty) { // NOLINT
    constexpr size_t LOOP_COUNT = 50;
    for (size_t i = 0; i < LOOP_COUNT; ++i) {
        std::vector<uint32_t> vector;
        avl_tree<uint32_t> tree;
        size_t elements = random_loop_count();
        for (size_t j = 0; j < elements; ++j) {
            uint32_t element = random_uint32();
            vector.emplace_back(element);
            tree.insert(element);
        }
        std::shuffle(vector.begin(), vector.end(), gen);
        for (auto const& element : vector) {
            auto it = tree.find(element);
            EXPECT_NE(tree.end(), it);
            tree.erase(it);
        }
        EXPECT_EQ(true, tree.empty());
    }
}

TEST(correctness, lower_bound) { // NOLINT
    constexpr size_t LOOP_COUNT = 50;
    for (size_t i = 0; i < LOOP_COUNT; ++i) {
        avl_tree<uint32_t> tree;
        std::set<uint32_t> set;
        size_t elements = random_loop_count();
        for (size_t j = 0; j < elements; ++j) {
            uint32_t element = random_uint32();
            tree.insert(element);
            set.insert(element);
        }
        size_t tests = random_loop_count();
        for (size_t j = 0; j < tests; ++j) {
            uint32_t element = random_uint32();
            auto lb_set = set.lower_bound(element);
            auto lb_tree = tree.lower_bound(element);
            if (lb_set == set.end()) {
                EXPECT_EQ(tree.end(), lb_tree);
            } else {
                EXPECT_EQ(*lb_set, *lb_tree);
            }
        }
    }
}

TEST(correctness, upper_bound) { // NOLINT
    constexpr size_t LOOP_COUNT = 50;
    for (size_t i = 0; i < LOOP_COUNT; ++i) {
        avl_tree<uint32_t> tree;
        std::set<uint32_t> set;
        size_t elements = random_loop_count();
        for (size_t j = 0; j < elements; ++j) {
            uint32_t element = random_uint32();
            tree.insert(element);
            set.insert(element);
        }
        size_t tests = random_loop_count();
        for (size_t j = 0; j < tests; ++j) {
            uint32_t element = random_uint32();
            auto lb_set = set.upper_bound(element);
            auto lb_tree = tree.upper_bound(element);
            if (lb_set == set.end()) {
                EXPECT_EQ(tree.end(), lb_tree);
            } else {
                EXPECT_EQ(*lb_set, *lb_tree);
            }
        }
    }
}

namespace {

void expect_eq_trees(avl_tree<uint32_t> const& lhs, avl_tree<uint32_t> const& rhs) {
    std::vector<uint32_t> from_lhs;
    std::vector<uint32_t> from_rhs;
    for (auto const& element : lhs) {
        from_lhs.emplace_back(element);
    }
    for (auto const& element : rhs) {
        from_rhs.emplace_back(element);
    }
    EXPECT_EQ(from_lhs.size(), from_rhs.size());
    for (size_t i = 0; i < from_lhs.size(); ++i) {
        EXPECT_EQ(from_lhs[i], from_rhs[i]);
    }
}

}

TEST(correctness, copy_swap_assign) { // NOLINT
    constexpr size_t LOOP_COUNT = 50;
    for (size_t i = 0; i < LOOP_COUNT; ++i) {
        avl_tree<uint32_t> tree1;
        size_t elements = random_loop_count();
        for (size_t j = 0; j < elements; ++j) {
            tree1.insert(random_uint32());
        }
        avl_tree<uint32_t> copy_tree1(tree1);
        expect_eq_trees(tree1, copy_tree1);
        avl_tree<uint32_t> tree2;
        elements = random_loop_count();
        for (size_t j = 0; j < elements; ++j) {
            tree2.insert(random_uint32());
        }
        tree2.swap(tree1);
        expect_eq_trees(copy_tree1, tree2);
        copy_tree1 = tree1;
        expect_eq_trees(copy_tree1, tree1);
    }
}

TEST(correctness, empty_assign_empty) { // NOLINT
    avl_tree<uint32_t> empty;
    avl_tree<uint32_t> empty2 = empty;
    EXPECT_EQ(true, empty2.empty());
}

TEST(correctness, insert_find_iterator) { // NOLINT
    constexpr size_t LOOP_COUNT = 50;
    for (size_t i = 0; i < LOOP_COUNT; ++i) {
        avl_tree<uint32_t> tree;
        std::set<uint32_t> set;
        size_t elements = random_loop_count();
        for (size_t j = 0; j < elements; ++j) {
            uint32_t element = random_uint32();
            auto pair = tree.insert(element);
            EXPECT_EQ(set.find(element) == set.end(), pair.second);
            EXPECT_EQ(tree.find(element), pair.first);
            set.insert(element);
        }
    }
}