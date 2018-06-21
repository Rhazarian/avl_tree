#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <cstddef>
#include <memory>

template<typename T>
struct avl_tree {
private:
    struct avl_tree_node;
    typedef std::shared_ptr<avl_tree_node> node_ptr;
    struct avl_tree_node {
        T value;
        ptrdiff_t height;
        node_ptr left;
        node_ptr right;
        avl_tree_node* parent;

        explicit avl_tree_node(T const& value);
    };

    node_ptr root;
    avl_tree_node const* min;

    template<bool is_const_iterator>
    struct const_noconst_iterator : std::iterator<std::bidirectional_iterator_tag, T, ptrdiff_t, T const*, T const&> {
    private:
        node_ptr const* root;
        avl_tree_node const* ptr;

        const_noconst_iterator(node_ptr const* root, avl_tree_node const* node) noexcept;

        friend struct avl_tree;
    public:
        const_noconst_iterator();
        const_noconst_iterator(const_noconst_iterator<false> const& other) noexcept; // NOLINT

        const_noconst_iterator& operator=(const_noconst_iterator const& other) noexcept;

        template<bool any_const_noconst>
        bool operator==(const_noconst_iterator<any_const_noconst> const& other) const noexcept;
        template<bool any_const_noconst>
        bool operator!=(const_noconst_iterator<any_const_noconst> const& other) const noexcept;

        typename const_noconst_iterator::reference operator* () const noexcept;
        typename const_noconst_iterator::pointer operator-> () const noexcept;

        const_noconst_iterator& operator++();
        const_noconst_iterator operator++(int); // NOLINT
        const_noconst_iterator& operator--();
        const_noconst_iterator operator--(int); // NOLINT
    };

public:
    typedef const_noconst_iterator<false> iterator;
    typedef const_noconst_iterator<true> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

private:
    static ptrdiff_t height(node_ptr) noexcept;
    static void fix_height(node_ptr) noexcept;
    static ptrdiff_t difference(node_ptr) noexcept;
    static node_ptr rr_rotation(node_ptr) noexcept;
    static node_ptr ll_rotation(node_ptr) noexcept;
    static node_ptr rl_rotation(node_ptr) noexcept;
    static node_ptr lr_rotation(node_ptr) noexcept;
    iterator find(node_ptr const&, T const&) const;
    std::pair<iterator, bool> insert(node_ptr&, avl_tree_node*, T const&);
    void remove(node_ptr&, T const&);
    static void balance(node_ptr&) noexcept;
    static node_ptr minimum(node_ptr const&) noexcept;
    static node_ptr maximum(node_ptr const&) noexcept;
    static void remove_minimum(node_ptr&) noexcept;
    node_ptr copy_subtree(node_ptr const&, avl_tree_node*);

public:
    avl_tree() noexcept;
    avl_tree(avl_tree const&);
    avl_tree& operator=(avl_tree const&);
    ~avl_tree();
    std::pair<iterator, bool> insert(T const&);
    iterator erase(const_iterator);
    iterator find(T const&) const;
    iterator lower_bound(T const&) const;
    iterator upper_bound(T const&) const;
    bool empty() const noexcept;
    void clear() noexcept;

    void swap(avl_tree&) noexcept;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;
    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crend() const noexcept;
};

template<typename T>
void swap(avl_tree<T>&, avl_tree<T>&) noexcept;

#include <avl_tree.tpp>
#endif //AVL_TREE_H