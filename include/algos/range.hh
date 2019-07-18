

struct range {
    range(int to)
    : range(0, to) {}

    range(int from, int to)
    : range(from, to, 1) {}

    range(int from, int to, int step)
    : start(from), stop(to), step(step) {}

    class iterator {
        int pos;
        int step;
    public:
        using value = int;
        iterator(int pos, int step) : pos(pos), step(step) {}
        iterator& operator++() { pos += step; return *this; }
        iterator operator++(int) { auto ret = *this; ++*this; return ret; }
        value operator*() const { return pos; }
        iterator operator+(int v) const { return {pos + v, step}; }
        bool operator==(const iterator& rhs) const { return pos == rhs.pos; }
        bool operator!=(const iterator& rhs) const { return pos != rhs.pos; }
    };

    iterator begin() { return iterator(start, step); }
    iterator end() { auto r = (stop-start)%step; return iterator(r?stop-r+step:stop, step); }

    int start;
    int step;
    int stop;
};

