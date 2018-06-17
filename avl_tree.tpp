#include <algorithm>

#include "avl_tree.h"

template<typename T>
avl_tree<T>::avl_tree_node::avl_tree_node(T const& value) : value(value), height(0), left(nullptr), right(nullptr) { }

template<typename T>
avl_tree<T>::avl_tree_node::avl_tree_node(T const& value, ptrdiff_t height, avl_tree::node_ptr const& left, avl_tree::node_ptr const& right) :
        value(value), height(height), left(left), right(right) { }

template<typename T>
ptrdiff_t avl_tree<T>::height(node_ptr node) noexcept
{
    return node ? node->height : 0;
}

template<typename T>
void avl_tree<T>::fix_height(node_ptr node) noexcept
{
    node->height = std::max(height(node->left), height(node->right)) + 1;
}

template<typename T>
ptrdiff_t avl_tree<T>::difference(node_ptr node) noexcept
{
    return height(node->left) - height(node->right);
}

template<typename T>
typename avl_tree<T>::node_ptr avl_tree<T>::rr_rotation(node_ptr parent) noexcept
{
    node_ptr node;
    node = parent->right;
    parent->right = node->left;
    node->left = parent;
    fix_height(parent);
    fix_height(node);
    return node;
}

template<typename T>
typename avl_tree<T>::node_ptr avl_tree<T>::ll_rotation(node_ptr parent) noexcept
{
    node_ptr node;
    node = parent->left;
    parent->left = node->right;
    node->right = parent;
    fix_height(parent);
    fix_height(node);
    return node;
}

template<typename T>
typename avl_tree<T>::node_ptr avl_tree<T>::lr_rotation(node_ptr parent) noexcept
{
    node_ptr node;
    node = parent->left;
    parent->left = rr_rotation(node);
    return ll_rotation(parent);
}

template<typename T>
typename avl_tree<T>::node_ptr avl_tree<T>::rl_rotation(node_ptr parent) noexcept
{
    node_ptr node;
    node = parent->right;
    parent->right = ll_rotation(node);
    return rr_rotation(parent);
}

template<typename T>
void avl_tree<T>::balance(node_ptr& node) noexcept
{
    fix_height(node);
    auto diff = difference(node);
    if (diff > 1) {
        if (difference(node->left) > 0) {
            node = ll_rotation(node);
        }
        else {
            node = lr_rotation(node);
        }
    }
    else {
        if (diff < -1) {
            if (difference(node->right) > 0) {
                node = rl_rotation(node);
            }
            else {
                node = rr_rotation(node);
            }
        }
    }
}

template<typename T>
avl_tree<T>::avl_tree() noexcept : root(nullptr), min(nullptr) { }

template<typename T>
avl_tree<T>::~avl_tree() = default;

template<typename T>
template<bool is_const_iterator>
avl_tree<T>::const_noconst_iterator<is_const_iterator>::const_noconst_iterator(avl_tree const* tree, avl_tree<T>::avl_tree_node const* node) noexcept : tree(tree),
        ptr(node) { }

template<typename T>
template<bool is_const_iterator>
avl_tree<T>::const_noconst_iterator<is_const_iterator>::const_noconst_iterator(avl_tree<T>::const_noconst_iterator<false> const& other) noexcept : tree(other.tree), ptr(other.ptr) { }

template<typename T>
template<bool is_const_iterator>
template<bool any_const_noconst>
bool avl_tree<T>::const_noconst_iterator<is_const_iterator>::operator==(avl_tree<T>::const_noconst_iterator<any_const_noconst> const& other) const noexcept {
    return ptr == other.ptr && tree == other.tree;
}

template<typename T>
template<bool is_const_iterator>
template<bool any_const_noconst>
bool avl_tree<T>::const_noconst_iterator<is_const_iterator>::operator!=(avl_tree<T>::const_noconst_iterator<any_const_noconst> const& other) const noexcept {
    return !operator==(other);
}

template<typename T>
template<bool is_const_iterator>
typename avl_tree<T>::template const_noconst_iterator<is_const_iterator>& avl_tree<T>::const_noconst_iterator<is_const_iterator>::operator++() {
    avl_tree_node const* node = tree->root.get();
    avl_tree_node const* successor = nullptr;
    while (node != nullptr) {
        if (node->value > ptr->value) {
            successor = node;
            node = node->left.get();
        }
        else {
            node = node->right.get();
        }
    }
    ptr = successor;
    return *this;
}

template<typename T>
template<bool is_const_iterator>
typename avl_tree<T>::template const_noconst_iterator<is_const_iterator>& avl_tree<T>::const_noconst_iterator<is_const_iterator>::operator--() {
    if (ptr != nullptr) {
        avl_tree_node const* node = tree->root.get();
        avl_tree_node const* successor = nullptr;
        while (node != nullptr) {
            if (node->value < ptr->value) {
                successor = node;
                node = node->right.get();
            }
            else {
                node = node->left.get();
            }
        }
        ptr = successor;
    }
    else {
        ptr = tree->maximum(tree->root).get();
    }
    return *this;
}

template<typename T>
template<bool is_const_iterator>
typename avl_tree<T>::template const_noconst_iterator<is_const_iterator> avl_tree<T>::const_noconst_iterator<is_const_iterator>::operator++(int) {
    const const_noconst_iterator copy(*this);
    ++(*this);
    return copy;
}

template<typename T>
template<bool is_const_iterator>
typename avl_tree<T>::template const_noconst_iterator<is_const_iterator> avl_tree<T>::const_noconst_iterator<is_const_iterator>::operator--(int) {
    const const_noconst_iterator copy(*this);
    --(*this);
    return copy;
}

template<typename T>
template<bool is_const_iterator>
typename avl_tree<T>::template const_noconst_iterator<is_const_iterator>::reference avl_tree<T>::const_noconst_iterator<is_const_iterator>::operator*() const noexcept
{
    return ptr->value;
}
template<typename T>
template<bool is_const_iterator>
typename avl_tree<T>::template const_noconst_iterator<is_const_iterator>& avl_tree<T>::const_noconst_iterator<is_const_iterator>::operator=(
        const avl_tree<T>::const_noconst_iterator<is_const_iterator>& other) noexcept
{
    tree = other.tree;
    ptr = other.ptr;
    return *this;
}

template<typename T>
typename avl_tree<T>::iterator avl_tree<T>::find(node_ptr const& node, T const& value) const
{
    if (node == nullptr || node->value == value)
        return avl_tree::iterator(this, node.get());
    return value < node->value ? find(node->left, value) : find(node->right, value);
}

template<typename T>
typename avl_tree<T>::iterator avl_tree<T>::find(T const& value) const
{
    return find(root, value);
}

template<typename T>
bool avl_tree<T>::empty() const noexcept {
    return root == nullptr;
}

template<typename T>
void avl_tree<T>::clear() noexcept {
    root.reset();
    min = nullptr;
}

template<typename T>
std::pair<typename avl_tree<T>::iterator, bool> avl_tree<T>::insert(node_ptr& node, T const& value)
{
    if (node == nullptr) {
        node.reset(new avl_tree_node(value));
        return {avl_tree::iterator(this, node.get()), true};
    }
    if (value == node->value) {
        return {avl_tree::iterator(this, node.get()), false};
    }
    if (value < node->value) {
        auto tmp = insert(node->left, value);
        balance(node);
        return tmp;
    }
    else {
        auto tmp = insert(node->right, value);
        balance(node);
        return tmp;
    }
}

template<typename T>
std::pair<typename avl_tree<T>::iterator, bool> avl_tree<T>::insert(T const& value)
{
    if (min == nullptr || value < min->value) {
        auto tmp = insert(root, value);
        min = tmp.first.ptr;
        return tmp;
    }
    return insert(root, value);
}

template<typename T>
typename avl_tree<T>::node_ptr avl_tree<T>::minimum(avl_tree::node_ptr const& node) const noexcept
{
    return node->left ? minimum(node->left) : node;
}

template<typename T>
typename avl_tree<T>::node_ptr avl_tree<T>::maximum(avl_tree::node_ptr const& node) const noexcept
{
    return node->right ? maximum(node->right) : node;
}

template<typename T>
void avl_tree<T>::remove_minimum(avl_tree<T>::node_ptr& node) noexcept {
    if (node->left == nullptr) {
        node = node->right;
        return;
    }
    remove_minimum(node->left);
    balance(node);
}

template<typename T>
void avl_tree<T>::remove(avl_tree::node_ptr& node, T const& value)
{
    if (node == nullptr) {
        return;
    }
    if (value < node->value) {
        remove(node->left, value);
    }
    else {
        if (value > node->value) {
            remove(node->right, value);
        }
        else {
            if (node->right == nullptr) {
                node = node->left;
                return;
            }
            node_ptr left = node->left;
            node_ptr right = node->right;
            node = minimum(node->right);
            remove_minimum(right);
            node->left = left;
            node->right = right;
        }
    }
    balance(node);
}

template<typename T>
typename avl_tree<T>::iterator avl_tree<T>::erase(avl_tree<T>::const_iterator it) {
    avl_tree_node const* ptr = it.ptr;
    iterator new_it(this, (++it).ptr);
    if (ptr == min) {
        min = new_it.ptr;
    }
    remove(root, ptr->value);
    return new_it;
}

template<typename T>
typename avl_tree<T>::iterator avl_tree<T>::lower_bound(T const& value) const {
    avl_tree_node const* node = root.get();
    avl_tree_node const* successor = nullptr;
    while (node != nullptr) {
        if (node->value >= value) {
            successor = node;
            node = node->left.get();
        }
        else {
            node = node->right.get();
        }
    }
    return iterator(this, successor);
}

template<typename T>
typename avl_tree<T>::iterator avl_tree<T>::upper_bound(T const& value) const {
    avl_tree_node const* node = root.get();
    avl_tree_node const* successor = nullptr;
    while (node != nullptr) {
        if (node->value > value) {
            successor = node;
            node = node->left.get();
        }
        else {
            node = node->right.get();
        }
    }
    return iterator(this, successor);
}

template<typename T>
void avl_tree<T>::swap(avl_tree& other) noexcept {
    root.swap(other.root);
    std::swap(min, other.min);
}

template<typename T>
typename avl_tree<T>::node_ptr avl_tree<T>::copy_subtree(avl_tree<T>::node_ptr const& node) {
    if (node == nullptr) {
        return nullptr;
    }
    return node_ptr(new avl_tree<T>::avl_tree_node(node->value, node->height, copy_subtree(node->left), copy_subtree(node->right)));
}

template<typename T>
avl_tree<T>::avl_tree(avl_tree const& other) : root(copy_subtree(other.root)) {
    min = minimum(root).get();
}

template<typename T>
avl_tree<T>& avl_tree<T>::operator=(avl_tree const& other)
{
    avl_tree copy(other);
    swap(copy);
    return *this;
}

template<typename T>
typename avl_tree<T>::iterator avl_tree<T>::begin() noexcept {
    return iterator(this, min);
}

template<typename T>
typename avl_tree<T>::const_iterator avl_tree<T>::cbegin() const noexcept {
    return const_iterator(this, min);
}

template<typename T>
typename avl_tree<T>::const_iterator avl_tree<T>::begin() const noexcept {
    return cbegin();
}

template<typename T>
typename avl_tree<T>::iterator avl_tree<T>::end() noexcept {
    return iterator(this, nullptr);
}

template<typename T>
typename avl_tree<T>::const_iterator avl_tree<T>::cend() const noexcept {
    return const_iterator(this, nullptr);
}

template<typename T>
typename avl_tree<T>::const_iterator avl_tree<T>::end() const noexcept {
    return cend();
}

template<typename T>
typename avl_tree<T>::reverse_iterator avl_tree<T>::rbegin() noexcept {
    return avl_tree<T>::reverse_iterator(end());
}

template<typename T>
typename avl_tree<T>::const_reverse_iterator avl_tree<T>::crbegin() const noexcept {
    return avl_tree<T>::const_reverse_iterator(end());
}

template<typename T>
typename avl_tree<T>::const_reverse_iterator avl_tree<T>::rbegin() const noexcept {
    return crbegin();
}

template<typename T>
typename avl_tree<T>::reverse_iterator avl_tree<T>::rend() noexcept {
    return avl_tree<T>::reverse_iterator(begin());
}

template<typename T>
typename avl_tree<T>::const_reverse_iterator avl_tree<T>::crend() const noexcept {
    return avl_tree<T>::const_reverse_iterator(begin());
}

template<typename T>
typename avl_tree<T>::const_reverse_iterator avl_tree<T>::rend() const noexcept {
    return crend();
}

template<typename T>
void swap(avl_tree<T>& lhs, avl_tree<T>& rhs) noexcept
{
    lhs.swap(rhs);
}

#include "avl_tree.h"