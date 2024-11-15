template <typename T>
struct Fenwick {// [0, n)
    int n;
    vector<T> a;

    Fenwick(int _n = 0) {
        init(_n);
    }
    void init(int _n) {
        n = _n;
        a.assign(n, T{});
    }

    void add(int x, const T &v) {// [0, n)
        for (int i = x + 1; i <= n; i += i & -i) {
            a[i - 1] = a[i - 1] + v;
        }
    }

    T sum(int x) {// [0, x)
        T ans{};
        for (int i = x; i > 0; i -= i & -i) {
            ans = ans + a[i - 1];
        }
        return ans;
    }

    T rangeSum(int l, int r) {// [l, r) 
        return sum(r) - sum(l);
    }
};