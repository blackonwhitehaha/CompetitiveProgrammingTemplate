template<class Info, class Tag>
struct SegmentTree {
    int n;
    vector<Info> d;
    vector<Tag> tag;
    SegmentTree() : n(0) {}
    SegmentTree(int n, Info info) {
        init(vector<Info>(n, info));
    }
    SegmentTree(vector<Info> a) {
        init(a);
    }
    void init(vector<Info> a) {
        n = a.size();
        d.assign((n << 2) + 10, Info());
        tag.assign((n << 2) + 10, Tag());
        function<void(int, int, int)> build = [&](int o, int l, int r) {//[l, r)
            if (r - l == 1) {
                d[o] = a[l];
                return ;
            }
            int mid = l + r >> 1;
            build(o << 1, l, mid);
            build(o << 1 | 1, mid, r);
            pushUp(o);
        }
        build(1, 0, n);
    }

    void pushUp(int o) {
        d[o] = d[o << 1] + d[o << 1 | 1];
    }
    void apply(int o, const Tag &v) {
        d[o].apply(v);
        tag[o].apply(v);
    }
    void pushDown(int o) {
        apply(o << 1, tag[o]);
        apply(o << 1 | 1, tag[o]);
        tag[o] = Tag();
    }
    
    void updPoint(int o, int l, int r, int p, const Info &v) {
        if (r - l == 1) {
            d[o] = v;
            return ;
        }
        int mid = l + r >> 1;
        pushDown(o);
        if (p < mid) updPoint(o << 1, l, mid, p, v);
        else updPoint(o << 1 | 1, mid, r, p, v);
        pushUp(o);
    }
    void updPoint(int p, Info &v) { updPoint(1, 0, n, p, v); }

    void updRange(int o, int l, int r, int s, int t, const Tag &v) {
        if (s <= l && r <= t) {
            apply(o, v);
            return ;
        }
        int mid = l + r >> 1;
        pushDown(o);
        if (s < mid) updRange(o << 1, l, mid, s, t, v);
        if (t > mid) updRange(o << 1 | 1, mid, r, s, t, v);
        pushUp(o);
    }
    void updRange(int s, int t, const Tag &v) { updRange(1, 0, n, s, t, v); }

    Info qry(int o, int l, int r, int s, int t) {
        if (s <= l && r <= t) {
            return d[o];
        }
        Info res();
        int mid = l + r >> 1;
        if (s < mid) res += qry(o << 1, l, mid, s, t);
        if (t > mid) res += qry(o << 1 | 1, mid, r, s, t);
        return res;
    }
    Info qry(int s, int t) { return qry(1, 0, n, s, t); }
};
struct Info {
    i64 v = 0;
    void apply(const Tag &t) {
        
    }
};
Info operator + (const Info &a, const Info &b) {
    return {};
}
struct Tag {
    i64 v = 0;
    void apply(const Tag &t) {

    }
};