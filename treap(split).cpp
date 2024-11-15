namespace treap {// 无旋treap
    const int maxn = 5e5;
    int rt, tot;
    int fa[maxn + 10], ch[maxn + 10][2], val[maxn + 10], rnk[maxn + 10], sz[maxn + 10], cnt[maxn + 10];
    void pushUp(int x) {
        sz[x] = sz[ch[x][0]] + sz[ch[x][1]] + cnt[x];
    }
    void clear(int x) {
        fa[x] = ch[x][0] = ch[x][1] = val[x] = rnk[x] = sz[x] = cnt[x] = 0;
    }
    pair<int, int> split(int u, int k) {// left: <= k, right > k (val)
        if (!u) return {0, 0};
        if (val[u] <= k) {
            auto tmp = split(ch[u][1], k);
            ch[u][1] = tmp.first;
            if (tmp.first) fa[tmp.first] = u;
            pushUp(u);
            fa[u] = 0;
            return {u, tmp.second};
        } else {
            auto tmp = split(ch[u][0], k);
            ch[u][0] = tmp.second;
            if (tmp.second) fa[tmp.second] = u;
            pushUp(u);
            fa[u] = 0;
            return {tmp.first, u};
        }
    }
    tuple<int, int, int> split_by_rk(int u, int k) {// left: < k, mid: = k, right > k (size)
        if (!u) return {0, 0, 0};
        if (sz[ch[u][0]] + cnt[u] < k) {
            int l, mid, r;
            tie(l, mid, r) = split_by_rk(ch[u][1], k - sz[ch[u][0]] - cnt[u]);
            ch[u][1] = l;
            if (l) fa[l] = u;
            fa[u] = 0;
            pushUp(u);
            return {u, mid, r};
        } else if (sz[ch[u][0]] < k) {
            int l = ch[u][0], mid = u, r = ch[u][1];
            if (l) fa[l] = 0;
            if (r) fa[r] = 0;
            ch[u][0] = ch[u][1] = 0;
            pushUp(u);
            return {l, mid, r};
        } else {
            int l, mid, r;
            tie(l, mid, r) = split_by_rk(ch[u][0], k);
            ch[u][0] = r;
            if (r) fa[r] = u;
            fa[u] = 0;
            pushUp(u);
            return {l, mid, u};
        }
    }
    int merge(int u, int v) {//val_of_tree_u <= val_of_tree_v
        if (!u) return v;
        if (!v) return u;
        if (rnk[u] < rnk[v]) {
            int t = merge(ch[u][1], v);
            ch[u][1] = t;
            if (t) fa[t] = u;
            fa[u] = 0;
            pushUp(u);
            return u;
        } else {
            int t = merge(u, ch[v][0]);
            ch[v][0] = t;
            if (t) fa[t] = v;
            fa[v] = 0;
            pushUp(v);
            return v;
        }
    }
    void ins(int x) {
        int l, mid, r;
        tie(mid, r) = split(rt, x);
        tie(l, mid) = split(mid, x - 1);
        if (mid) cnt[mid]++;
        else {
            val[mid = ++tot] = x;
            rnk[tot] = rand();
            cnt[tot] = 1;
        }
        pushUp(mid);
        rt = merge(l, mid);
        rt = merge(rt, r);
    }
    void del(int x) {//by val
        int l, mid, r;
        tie(mid, r) = split(rt, x);
        tie(l, mid) = split(mid, x - 1);
        if (cnt[mid] > 1) {
            cnt[mid]--;
            pushUp(mid);
        } else {
            clear(mid);
            mid = 0;
        }
        rt = merge(l, mid);
        rt = merge(rt, r);
    }
    int rk(int x) {
        int u, v;
        tie(u, v) = split(rt, x - 1);
        int res = sz[u] + 1;
        rt = merge(u, v);
        return res;
    }
    int kth(int k) {
        int l, mid, r;
        tie(l, mid, r) = split_by_rk(rt, k);
        int res = val[mid];
        rt = merge(l, mid);
        rt = merge(rt, r);
        return res;
    }
    int pre(int x) {
        return kth(rk(x) - 1);
    }
    int nxt(int x) {
        return kth(rk(x + 1));
    }
}