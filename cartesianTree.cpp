namespace cartesian {//笛卡尔树
    int rt, tot;
    int fa[maxn + 10], ch[maxn + 10][2], val[maxn + 10], rnk[maxn + 10];
    void build(vector<array<int, 2> > a) {//val, rnk
        sort(a.begin(), a.end());// 按二叉搜索树的性质将val从小到大插入
        int p;
        for (int i = 0; i < a.size(); i++) {
            val[++tot] = a[i][0];
            rnk[tot] = a[i][1];
            p = tot - 1;
            while (p && rnk[p] > rnk[tot]) p = fa[p];// 小根堆
            ch[tot][0] = p ? ch[p][1] : rt;
            if (ch[p][1]) fa[ch[p][1]] = tot;            
            fa[tot] = p;
            if (p) ch[p][1] = tot;
            else rt = tot;
        }
    }
}