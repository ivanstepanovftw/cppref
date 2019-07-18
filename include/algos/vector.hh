#pragma once


template <class T>
class vector {
public:
    vector() : start(nullptr), finish(nullptr), end_of_storage(nullptr) {}

    vector(size_t size) : start(new T[size]), finish(start+size), end_of_storage(finish) {}

    vector(size_t size, T d) : vector(size) {
        auto *cursor = start;
        while (cursor != finish) {
            *cursor++ = d;
        }
    }

    virtual ~vector() {
        delete[] start;
    }

    class iterator {
    public:
        using self = iterator;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        iterator(T *pos) : pos(pos) {}
        self& operator++() { ++pos; return *this; }
        self operator++(int) { auto ret = *this; ++*this; return ret; }
        self& operator--() { --pos; return *this; }
        self operator--(int) { auto ret = *this; --*this; return ret; }
        //operator +\-
        reference operator*() const { return *pos; }
        pointer operator->() const { return pos; }
        bool operator==(const iterator& rhs) const { return pos == rhs.pos; }
        bool operator!=(const iterator& rhs) const { return pos != rhs.pos; }
    public:
        T *pos;
    };

    iterator begin() { return iterator(start); }

    iterator end() { return iterator(finish); }

    iterator insert(iterator position, T v) {
        if (finish != end_of_storage) {
            T *cur = finish;
            for (; cur > position.pos; cur--) {
                *(cur) = std::move(*(cur-1));
            }
            auto result = iterator(cur);
            *cur = v;
            finish++;
            return result;
        } else {
            const size_t new_size = size() + 1;
            size_t new_capacity = capacity() * 2;
            if (!new_capacity)
                new_capacity = new_size;
            T *tmp_start = new T[new_capacity];
            T *tmp_finish = tmp_start + new_size;
            T *tmp_end = tmp_start + new_capacity;

            T *s_cur = start;
            T *t_cur = tmp_start;
            for (; s_cur != position.pos; s_cur++, t_cur++) {
                *t_cur = std::move(*s_cur);
            }
            auto result = iterator(t_cur);
            *t_cur = v;
            ++t_cur;
            for (; s_cur != finish; s_cur++, t_cur++) {
                *t_cur = std::move(*s_cur);
            }
            delete[] start;

            start = tmp_start;
            finish = tmp_finish;
            end_of_storage = tmp_end;
            return result;
        }
    }

    iterator erase(iterator position) {
        //todo
    }

    /*
     * Memory usage O(3*size) if size == capacity
     */
    void push_back(const T& v) {
        if (finish != end_of_storage) {
            *finish = v;
            finish++;
        } else {
            const size_t new_size = size() + 1;
            size_t new_capacity = capacity() * 2;
            if (!new_capacity)
                new_capacity = new_size;
            T *tmp_start = new T[new_capacity];
            T *tmp_finish = tmp_start + new_size;
            T *tmp_end = tmp_start + new_capacity;

            T *s_cur = start;
            T *t_cur = tmp_start;
            for (; s_cur != finish; s_cur++, t_cur++) {
                *t_cur = std::move(*s_cur);
            }
            *t_cur = v;
            delete[] start;

            start = tmp_start;
            finish = tmp_finish;
            end_of_storage = tmp_end;
        }
    }

    size_t size() { return finish - start; }

    size_t capacity() { return end_of_storage - start; }

    T *start;
    T *finish;
    T *end_of_storage;
};
