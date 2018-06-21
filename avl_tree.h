#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <cstddef>
#include <memory>
#include <optional>

template<typename T>
struct avl_tree {
private:
    struct avl_tree_node;
    typedef std::shared_ptr<avl_tree_node> node_ptr;

    struct cleaner_t {
        avl_tree_node* fake_end;

        explicit cleaner_t(avl_tree_node*) noexcept;

        void operator()(avl_tree_node*) noexcept;
    };

    struct avl_tree_node {
        std::optional<T> value{};
        ptrdiff_t height = 0;
        node_ptr left = nullptr;
        node_ptr right = nullptr;
        avl_tree_node* parent = nullptr;

        avl_tree_node() noexcept;
        avl_tree_node(T const&, avl_tree_node*) noexcept;
    };

    avl_tree_node fake_end_node{};

    cleaner_t cleaner = cleaner_t(&fake_end_node);

    node_ptr root = node_ptr(&fake_end_node, cleaner);
    avl_tree_node const* min = root.get();

    template<bool is_const_iterator>
    struct const_noconst_iterator : std::iterator<std::bidirectional_iterator_tag, T, ptrdiff_t, T const*, T const&> {
    private:
        avl_tree_node const* ptr = nullptr;

        explicit const_noconst_iterator(avl_tree_node const*) noexcept;

        friend struct avl_tree;
    public:
        const_noconst_iterator();
        const_noconst_iterator(const_noconst_iterator<false> const&) noexcept; // NOLINT

        const_noconst_iterator& operator=(const_noconst_iterator const&) noexcept;

        template<bool any_const_noconst>
        bool operator==(const_noconst_iterator<any_const_noconst> const&) const noexcept;
        template<bool any_const_noconst>
        bool operator!=(const_noconst_iterator<any_const_noconst> const&) const noexcept;

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
    static int cmp(std::optional<T> const&, std::optional<T> const&);
    static ptrdiff_t height(node_ptr) noexcept;
    static void fix_height(node_ptr) noexcept;
    static ptrdiff_t difference(node_ptr) noexcept;
    static node_ptr rr_rotation(node_ptr) noexcept;
    static node_ptr ll_rotation(node_ptr) noexcept;
    static node_ptr rl_rotation(node_ptr) noexcept;
    static node_ptr lr_rotation(node_ptr) noexcept;
    static void balance(node_ptr&) noexcept;
    iterator find(T const&, avl_tree_node const*) const;
    std::pair<iterator, bool> insert(T const&, avl_tree_node*, node_ptr&);
    static node_ptr minimum(node_ptr const&) noexcept;
    static void remove_minimum(node_ptr&) noexcept;
    void remove(T const&, node_ptr&);
    node_ptr copy_subtree(node_ptr const&, avl_tree_node*, avl_tree const&);

public:
    avl_tree() noexcept;
    avl_tree(avl_tree const&);
    avl_tree& operator=(avl_tree const&);
    ~avl_tree();

    iterator find(T const&) const;
    iterator lower_bound(T const&) const;
    iterator upper_bound(T const&) const;
    std::pair<iterator, bool> insert(T const&);
    iterator erase(const_iterator);
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