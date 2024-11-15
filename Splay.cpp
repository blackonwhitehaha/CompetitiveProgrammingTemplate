namespace Splay {
    const int maxn = 3e5;
    const int inf = 0x3f3f3f3f;
    int rt = 0, tot = 0;
    int fa[maxn + 10], ch[maxn + 10][2], val[maxn + 10], cnt[maxn + 10], sz[maxn + 10];
    void maintain(int x) { 
        sz[x] = sz[ch[x][0]] + sz[ch[x][1]] + cnt[x];
        //assert(node 0 isn't used)
    }
    void clear(int x) { 
        ch[x][0] = ch[x][1] = fa[x] = val[x] = sz[x] = cnt[x] = 0;
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
        maintain(y);
        maintain(x);
    }
    void splay(int x) {
        if (!fa[x]) maintain(x);
        for (int f = fa[x]; f = fa[x], f; rotate(x)) {
            if (fa[f]) rotate(get(x) == get(f) ? f : x);  
        }
        rt = x;
    }

    void ins(int k) {
        if (!rt) {
            val[++tot] = k;
            cnt[tot] = 1;
            rt = tot;
            maintain(rt);
            return;
        }
        int cur = rt, f = 0;
        while (1) {
            if (val[cur] == k) {
                cnt[cur]++;
                splay(cur);
                break;
            }
            f = cur;
            cur = ch[cur][val[cur] < k];
            if (!cur) {
                val[++tot] = k;
                cnt[tot] = 1;
                fa[tot] = f;
                ch[f][val[f] < k] = tot;
                splay(tot);
                break;
            }
        }
    }

    int rk(int k) {//get k's rank
        int res = 0, cur = rt;
        int f = 0;
        while (cur) {
            f = cur;
            if (val[cur] == k) {
                res += sz[ch[cur][0]];
                break;
            } else if (val[cur] < k) {
                res += sz[ch[cur][0]] + cnt[cur];
                cur = ch[cur][1];
            } else {
                cur = ch[cur][0];
            }
        }
        if (f) splay(f);
        return res + 1;
    }

    int kth(int k) {//get kth number
        if (!rt || k > sz[rt]) return inf;
        int cur = rt;
        while (1) {
            if (ch[cur][0] && k <= sz[ch[cur][0]]) {
                cur = ch[cur][0];
            } else {
                k -= cnt[cur] + sz[ch[cur][0]];
                if (k <= 0) {
                    splay(cur);
                    return val[cur];
                }
                cur = ch[cur][1];
            }
        }
    }

    int pre(int k) {
        int res = -inf;
        int p = rt, f = 0;
        while (p) {
            f = p;
            if (val[p] < k) {
                res = max(res, val[p]);
                p = ch[p][1];
            } else {
                p = ch[p][0];
            }
        }
        if (f) splay(f);
        return res;
    }

    int nxt(int k) {
        int res = inf;
        int p = rt, f = 0;
        while (p) {
            f = p;
            if (val[p] <= k) {
                p = ch[p][1];
            } else {
                res = min(res, val[p]);
                p = ch[p][0];
            }
        }
        if (f) splay(f);
        return res;      
    }

    void del(int k) {
        rk(k);
        if (!rt || val[rt] != k) return ;
        if (cnt[rt] > 1) {
            cnt[rt]--;
            maintain(rt);
            return;
        }
        if (!ch[rt][0] && !ch[rt][1]) {
            clear(rt);
            rt = 0;
            return;
        }
        if (!ch[rt][0]) {
            int cur = rt;
            rt = ch[rt][1];
            fa[rt] = 0;
            clear(cur);
            return;
        }
        if (!ch[rt][1]) {
            int cur = rt;
            rt = ch[rt][0];
            fa[rt] = 0;
            clear(cur);
            return;
        }
        int cur = rt, x = ch[cur][0];
        while (ch[x][1]) x = ch[x][1];
        fa[ch[cur][1]] = x;
        ch[x][1] = ch[cur][1];
        rt = ch[cur][0];
        fa[rt] = 0;
        clear(cur);
        splay(x);
    }
}