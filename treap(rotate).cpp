namespace treap {//旋转treap, 小根堆
    const int maxn = 1e6;
    int rt, tot;
    int fa[maxn + 10], ch[maxn + 10][2], val[maxn + 10], rnk[maxn + 10], sz[maxn + 10], cnt[maxn + 10];
    void pushUp(int x) {
        sz[x] = sz[ch[x][0]] + sz[ch[x][1]] + cnt[x];
    }
    void clear(int x) { 
        fa[x] = ch[x][0] = ch[x][1] = rnk[x] = 0;
        val[x] = sz[x] = cnt[x] = 0;
    }
    bool get(int x) { return x == ch[fa[x]][1]; }
    void rotate(int x) {
        int y = fa[x], z = fa[y], chk = get(x);
        ch[y][chk] = ch[x][chk ^ 1];
        if (ch[x][chk ^ 1]) fa[ch[x][chk ^ 1]] = y;
        ch[x][chk ^ 1] = y;
        fa[y] = x;
        fa[x] = z;
        if (z) ch[z][y == ch[z][1]] = x;
        pushUp(y);
        pushUp(x);
    }
    void ins(int &u, int x, int fno) {// ins(rt, x, 0)
        if (!u) {
            fa[u = ++tot] = fno;
            val[u] = x;
            rnk[u] = rand();
            cnt[u] = 1;
            pushUp(u);
            return ;
        }
        if (val[u] == x) {
            cnt[u]++;
            pushUp(u);
        } else if (val[u] < x) {
            ins(ch[u][1], x, u);
            if (rnk[ch[u][1]] < rnk[u]) {
                int t = ch[u][1];
                rotate(ch[u][1]);
                u = t;
            } else pushUp(u);
        } else {
            ins(ch[u][0], x, u);
            if (rnk[ch[u][0]] < rnk[u]) {
                int t = ch[u][0];
                rotate(ch[u][0]);
                u = t;
            }
            else pushUp(u);
        }
    }

    void del(int &u, int x) {// del(rt, x)
        if (!u) return ;
        if (val[u] < x) {
            del(ch[u][1], x);
            pushUp(u);
        } else if (val[u] > x) {
            del(ch[u][0], x);
            pushUp(u);
        } else {
            if (cnt[u] > 1) {
                cnt[u]--;
                pushUp(u);
                return ;
            }
            if (!ch[u][0] && !ch[u][1]) {
                clear(u);
                u = 0;
                return ;
            }
            if (!ch[u][0] || !ch[u][1]) {
                int chk = ch[u][1] != 0;
                int t = u;
                u = ch[t][chk];
                fa[u] = fa[t];
                clear(t);
                return ;
            }
            if (rnk[ch[u][0]] < rnk[ch[u][1]]) {
                int t = ch[u][0];
                rotate(t);
                u = t;
                del(ch[t][1], x);
                pushUp(t);
            } else {
                int t = ch[u][1];
                rotate(t);
                u = t;
                del(ch[t][0], x);
                pushUp(t);
            }
        }
    }

    int rk(int x) {
        int res = 0;
        int p = rt;
        while (p) {
            if (val[p] < x) {
                res += sz[ch[p][0]] + cnt[p];
                p = ch[p][1];
            } else {
                p = ch[p][0];
            }
        }
        return res + 1;
    }

    int kth(int k) {
        int p = rt;
        while (p) {
            if (k <= sz[ch[p][0]]) {
                p = ch[p][0];
            } else {
                k -= sz[ch[p][0]] + cnt[p];
                if (k <= 0) {
                    return val[p];
                }
                p = ch[p][1];
            }
        }
    }

    int pre(int x) {
        int res = x;
        int p = rt;
        while (p) {
            if (val[p] < x) {
                res = res == x ? val[p] : max(res, val[p]);
                p = ch[p][1];
            } else {
                p = ch[p][0];
            }
        }
        return res;
    }

    int nxt(int x) {
        int res = x;
        int p = rt;
        while (p) {
            if (val[p] <= x) {
                p = ch[p][1];
            } else {
                res = res == x ? val[p] : min(res, val[p]);
                p = ch[p][0];
            }
        }
        return res;
    }
}