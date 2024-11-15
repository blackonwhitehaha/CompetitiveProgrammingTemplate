struct BLT {// Binary Lifting on Tree
    int n;// [0, n)
    vector<vector<int> > adj;
    vector<int> dep;
    struct Info {   
        int top;
        Info operator + (Info &b) {
            Info res;
            res.top = b.top;// violates the commutative property
            return res;
        }
        void operator += (Info &b) {
            top = b.top;
        }
    };
    vector<vector<Info> > dp;

    BLT() {}
    BLT(int n) { init(n); }
    void init(int n) {
        this->n = n;
        adj.assign(n, {});
        dp.assign(20, vector<Info>(n));
        dep.resize(n, -1);
    }

    void addEdge(int u, int v) {
        adj[u].emplace_back(v);
        adj[v].emplace_back(u);
    }

    void dfs0(int u, int fa) {
        dp[0][u] = {fa};
        dep[u] = dep[fa] + 1;
        for (int j = 1; j <= 19; j++) {
            dp[j][u] = dp[j - 1][u] + dp[j - 1][ dp[j - 1][u].top ];
        }
        for (auto v : adj[u]) {
            if (v == fa) continue;
            dfs0(v, u);
        }
    }

    Info lca(int x, int y) {
        if (dep[x] < dep[y]) swap(x, y);
        Info resx, resy, res;
        resx.top = x;
        resy.top = y;
        int tmp = dep[x] - dep[y];
        for (int j = 0; j <= 19; j++) if (tmp & (1 << j)) {
            resx += dp[j][x];
            x = dp[j][x].top;
        }
        if (x == y) return resx;
        for (int j = 19; j >= 0; j--) if (dp[j][x].top != dp[j][y].top) {
            resx += dp[j][x];
            x = dp[j][x].top; 
            resy += dp[j][y];
            y = dp[j][y].top;
        }
        resx += dp[0][x];
        resy += dp[0][y];
        res = resx + resy;
        return res;
    }
};