struct SCC {
    int n;
    vector<vector<int> > adj;
    int tot;// [0, tot)
    vector<int> dfn, low;
    vector<int> stk;
    vector<bool> in_stk;
    int cntscc;// [0, cntscc)
    vector<int> scc_id;

    SCC() {};
    SCC(int n) { init(n); };

    void init(int n) {
        this->n = n;
        tot = cntscc = 0;
        adj.assign(n, {});
        dfn.assign(n, -1);
        low.assign(n, -1);
        stk.clear();
        in_stk.assign(n, 0);
        scc_id.assign(n, -1);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }

    void tarjan(int u) {
        stk.push_back(u);
        in_stk[u] = true;
        dfn[u] = low[u] = tot++;
        for (int v : adj[u]) {
            if (dfn[v] == -1) {
                tarjan(v);
                low[u] = min(low[u], low[v]);
            } else if (in_stk[v]) low[u] = min(low[u], dfn[v]);
        }

        if (dfn[u] == low[u]) {
            int cur;
            do {
                cur = stk.back();
                stk.pop_back();
                //operation
                in_stk[cur] = false;
                scc_id[cur] = cntscc;
            } while (cur != u);
            cntscc++;
        }
    }

    vector<int> work() {
        for (int i = 0; i < n; i++) {
            if (dfn[i] == -1) {
                tarjan(i);
            }
        }
        return scc_id;
    }
};