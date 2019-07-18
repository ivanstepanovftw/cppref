#pragma once


template <class T>
class binary_search_tree {
public:
    class Node {
    public:
        Node *left;
        Node *right;
        T value;

        Node(const T& v) {
            left = nullptr;
            right = nullptr;
            value = v;
        }
    };

    binary_search_tree()
    : root(nullptr) {};

    binary_search_tree(Node *root)
    : root(root) {};

    virtual ~binary_search_tree() {
        DFS_LRN([](Node *node) { delete node; });
    }

    void insert(const T& v) {
        insert(root, v);
    }

    /* erase(T) and erase(Node) можно сократить, заодно сделав erase_all(T) */
    void erase(const T& v) {
        // find address of `pb.left` (or `pb.right`) to make it null
        if (!root)
            return;
        Node **pb = nullptr;
        DFS_LRN([&](Node *n) {
            if (n->left && n->left->value == v)
                pb = &n->left;
            else if (n->right && n->right->value == v)
                pb = &n->right;
        });
        Node *node = nullptr;
        if (pb) {
            node = *pb;
            if (!node->left && !node->right) {
                delete node;
                *pb = nullptr;
                return;
            }
            else if (!node->left) {
                // remember address before deleting
                Node *d = node->right;
                std::swap(*node, *node->right);
                delete d;
                return;
            }
            else if (!node->right) {
                Node *d = node->left;
                std::swap(*node, *node->left);
                delete d;
                return;
            }
        }
        if (!node && root->value == v) {
            node = root;
        } else
            return;

        // find min node and its pb
        Node *mp = node;
        Node *m = node->right;
        while (m->left != 0) {
            mp = m;
            m = m->left;
        }

        // delete m, moving it to m->right
        ((mp->left == m) ? mp->left : mp->right) = m->right;
        node->value = m->value;
        delete m;
    }

    void erase(Node *&node) {
        if (!node->left && !node->right) {
            // find address of `parent.left` (or `parent.right`) to make it null
            Node **parent = nullptr;
            DFS_NLR([&](Node *n) {
                if (n->left == node)
                    parent = &n->left;
                else if (n->right == node)
                    parent = &n->right;
            });
            if (parent)
                *parent = nullptr;
            delete node;
        }
        else if (!node->left) {
            // remember address before deleting
            Node *d = node->right;
            std::swap(*node, *node->right);
            delete d;
        }
        else if (!node->right) {
            Node *d = node->left;
            std::swap(*node, *node->left);
            delete d;
        }
        else {
            // find min node and its parent
            Node *mp = node;
            Node *m = node->right;
            while (m->left) {
                mp = m;
                m = m->left;
            }

            // delete m, swapping it with m->right
            ((mp->left == m) ? mp->left : mp->right) = m->right;
            node->value = m->value;
            delete m;
        }
    }

    template <class Pred>
    void DFS_NLR(Pred pred) {
        DFS_NLR(root, pred);
    }

    template <class Pred>
    void DFS_LNR(Pred pred) {
        DFS_LNR(root, pred);
    }

    template <class Pred>
    void DFS_LRN(Pred pred) {
        DFS_LRN(root, pred);
    }

    static void insert(Node *&node, const T& v) {
        if (!node) {
            node = new Node(v);
            return;
        }
        if (v > node->value) {
            insert(node->right, v);
        } else {
            insert(node->left, v);
        }
    }

    /* Depth-first search, Pre-order (NLR) */
    template <class Pred>
    static void DFS_NLR(Node *node, Pred pred) {
        if (!node)
            return;
        pred(node);
        DFS_NLR(node->left, pred);
        DFS_NLR(node->right, pred);
    }

    /* Depth-first search, In-order (LNR) */
    template <class Pred>
    static void DFS_LNR(Node *node, Pred pred) {
        if (!node)
            return;
        DFS_LNR(node->left, pred);
        pred(node);
        DFS_LNR(node->right, pred);
    }

    /* Depth-first search, Post-order (LRN) */
    template <class Pred>
    static void DFS_LRN(Node *node, Pred pred) {
        if (!node)
            return;
        DFS_LRN(node->left, pred);
        DFS_LRN(node->right, pred);
        pred(node);
    }

public:
    Node *root;
};