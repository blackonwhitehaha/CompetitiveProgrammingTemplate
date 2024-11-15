struct EBCC {
    int n;// [0, n)
    int tote;
    vector<vector<pair<int, int> > > adj;// {v, id_of_edge}
    int tot;
    vector<int> dfn, low, stk;
    int cntebcc;
    vector<int> ebcc;
    
    EBCC() {}
    EBCC(int n) { init(n); }

    void init(int n) {
        this->n = n;
        tote = tot = cntebcc = 0;
        adj.assign(n, {});
        dfn.assign(n, -1);
        low.assign(n, -1);
        stk.clear();
        ebcc.resize(n);
    }
    
    void addEdge(int u, int v) {
        adj[u].push_back({v, tote});
        adj[v].push_back({u, tote++});
    }

    void tarjan(int u, int from) {
        dfn[u] = low[u] = tot++;
        stk.push_back(u);
        for (auto [v, id] : adj[u]) {
            if (id == from) continue;
            if (dfn[v] == -1) {
                tarjan(v, id);
                low[u] = min(low[u], low[v]);
            } else {
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (dfn[u] == low[u]) {
            int v;
            do {
                v = stk.back();
                ebcc[v] = cntebcc;
                stk.pop_back();
            } while (v != u);
            cntebcc++;
        }
    }

    vector<int> work() {
        for (int i = 0; i < n; i++) {
            if (dfn[i] == -1) {
                tarjan(i, -1);
            }
        }
        return ebcc;
    }

    struct Graph {
        int n;
        vector<pair<int, int> > edges;
        vector<int> sz;
        vector<int> cnte;
    };

    Graph compress() {
        Graph g;
        g.n = cntebcc;
        g.sz.resize(cntebcc);
        g.cnte.resize(cntebcc);

        for (int u = 0; u < n; u++) {
            g.sz[ebcc[u]]++;
            for (auto [v, id] : adj[u]) {
                if (v > u) continue;// be careful of self-loop
                if (ebcc[u] == ebcc[v]) {
                    g.cnte[ebcc[u]]++;
                } else {
                    g.edges.push_back({ebcc[u], ebcc[v]});
                }
            }
        }
        return g;
    }
};