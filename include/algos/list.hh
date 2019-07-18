#pragma once


template <class T>
class list {
public:
    class node {
    public:
        node() : prev{nullptr}, next{nullptr}, value{} {};
        explicit node(T value) : prev{nullptr}, next{nullptr}, value(value) {}

        node *prev;
        node *next;
        T value;
    };

    list() : root(new node), count(0) {
        root->prev = root;
        root->next = root;
    };

    explicit list(size_t n)
    : list() {
        for (; n; n--) {
            push_back({});
        }
    }

    explicit list(size_t n, int v)
    : list() {
        for (; n; n--) {
            push_back(v);
        }
    }

    virtual ~list() {
        auto *cur = root->next;
        while (cur != root) {
            cur = cur->next;
            delete cur->prev;
        }
        delete root;
    }

    class iterator {
    public:
        using self = iterator;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        iterator(node *pos) : pos(pos) {}
        self& operator++() { pos = pos->next; return *this; }
        self operator++(int) { auto ret = *this; ++*this; return ret; }
        self& operator--() { pos = pos->prev; return *this; }
        self operator--(int) { auto ret = *this; --*this; return ret; }
        reference operator*() const { return pos->value; }
        pointer operator->() const { return &pos->value; }
        bool operator==(const iterator& rhs) const { return pos == rhs.pos; }
        bool operator!=(const iterator& rhs) const { return pos != rhs.pos; }
    public:
        node *pos;
    };

    iterator begin() { return iterator(root->next); }

    iterator end() { return iterator(root); }

    size_t size() { return count; }

    // reference back() {
    //     iterator it = end();
    //     --it;
    //     return it;
    // }

    void insert(iterator position) {
        insert(position, {});
    }

    /*
     * insert() в данной имплементации работает как 'вставить ДО этого итератора'.
     *
     *     auto it = l.begin();
     *     l.insert(it, 10)
     * insert() вставит ДО l.begin(), сам it не изменится, а вот последующий вызов l.begin() изменится.
     * Не камера движется, а мир вокруг камеры.
     */
    iterator insert(iterator position, T v) {
        node *tmp = new node(v);
        tmp->next = position.pos;
        tmp->prev = position.pos->prev;
        position.pos->prev->next = tmp;
        position.pos->prev = tmp;
        count++;
        return {tmp};
    }

    iterator erase(iterator position) {
        position.pos->next->prev = position.pos->prev;
        position.pos->prev->next = position.pos->next;
        iterator ret = position;
        ret++;
        delete position.pos;
        count--;
        return ret;
    }

    void push_back(T v) {
        insert(end(), v);
    }

public:
    node *root;
    size_t count;
};
