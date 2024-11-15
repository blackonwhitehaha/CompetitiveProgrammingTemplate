namespace LCT {
    const int maxn = 1e6;
    int tot;
    int fa[maxn + 10], ch[maxn + 10][2], val[maxn + 10], tag[maxn + 10], d[maxn + 10];
    void pushUp(int x) {
        d[x] = d[ch[x][0]] ^ d[ch[x][1]] ^ val[x];
    }
    void pushDown(int x) {
        if (tag[x]) {
            for (int i = 0; i < 2; i++) if (ch[x][i]) {
                int C = ch[x][i];
                swap(ch[C][0], ch[C][1]);
                tag[C] ^= 1;
            }
            tag[x] = 0;
        }
    }
    bool get(int x) { return x == ch[fa[x]][1]; }
    bool isRoot(int x) { return ch[fa[x]][0] != x && ch[fa[x]][1] != x; }
    void rotate(int x) {
        int y = fa[x], z = fa[y], chk = get(x);
        if (!isRoot(y)) ch[z][get(y)] = x;
        ch[y][chk] = ch[x][chk ^ 1];
        if (ch[x][chk ^ 1]) fa[ch[x][chk ^ 1]] = y;
        ch[x][chk ^ 1] = y;
        fa[y] = x;
        fa[x] = z;
        pushUp(y);
        pushUp(x);
    }
    void splay(int x) {
        if (isRoot(x)) pushUp(x);
        for (int f; f = fa[x], !isRoot(x); rotate(x)) {
            if (!isRoot(f)) rotate(get(x) == get(f) ? f : x);
        }
    }
    void update(int x) {
        if (!isRoot(x)) update(fa[x]);
        pushDown(x);
    }
    void access(int x) {
        int tmp = x;
        for (int p = 0; x; p = x, x = fa[x]) {
            update(x);
            splay(x);
            ch[x][1] = p;
        }
        splay(tmp);
    }
    void makeRoot(int x) {
        access(x);
        swap(ch[x][0], ch[x][1]);
        tag[x] ^= 1;
    }
    int find(int x) {//get x's root
        access(x);
        pushDown(x);
        while (ch[x][0]) {
            x = ch[x][0];
            pushDown(x);
        }        
        splay(x);
        return x;
    }
    void link(int x, int y) {
        if (find(x) == find(y)) return ;
        makeRoot(y);
        fa[y] = x;
    }
    void cut(int x, int y) {
        if (find(x) != find(y)) return ;
        makeRoot(x);
        access(y);
        if (ch[y][0] != x || ch[x][0] || ch[x][1]) return ;
        ch[y][0] = 0;
        fa[x] = 0;
    }
}