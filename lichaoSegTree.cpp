#include <bits/stdc++.h>
using namespace std;

const int maxn = 4e4;
int tot;
struct Line {
    int x0, y0, x1, y1;//x0 <= x1
    double gety(int x) {
        if (x0 == x1) return max(y0, y1);
        return y0 + 1.0 * (y1 - y0) * (x - x0) / (x1 - x0);
    }
} line[100000 + 10];
int d[4 * maxn + 10];

void upd(int o, int l, int r, int k) {
    if (r < line[k].x0 || l > line[k].x1) return ;
    if (l == r) {
        if (!d[o] || line[d[o]].gety(l) < line[k].gety(l)) d[o] = k;
        return ;
    }
    int mid = l + r >> 1;
    if (line[k].x0 <= l && r <= line[k].x1) {
        if (!d[o]) {
            d[o] = k;
            return ;
        }
        int u = d[o], v = k;
        if (line[u].gety(mid) < line[v].gety(mid)) swap(u, v);
        if (line[u].gety(l) >= line[v].gety(l)) {
            if (line[u].gety(r) >= line[v].gety(r)) {
                d[o] = u;
                return ;
            } else {
                d[o] = u;
                upd(o << 1 | 1, mid + 1, r, v);
            }
        } else {
            d[o] = u;
            upd(o << 1, l, mid, v);
        }
        return ;
    }
    upd(o << 1, l, mid, k);
    upd(o << 1 | 1, mid + 1, r, k);
}

int qry(int o, int l, int r, int p) {
    int res = d[o];
    if (l == r) return res;
    int mid = l + r >> 1;
    int tmp;
    if (p <= mid) {
        tmp = qry(o << 1, l, mid, p);
    } else {
        tmp = qry(o << 1 | 1, mid + 1, r, p);
    }
    if (!res) res = tmp;
    else if (tmp) {
        if (line[res].gety(p) < line[tmp].gety(p)) res = tmp;
    }
    return res;
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    int n, lastans = 0;
    cin >> n;
    while (n--) {
        int op;
        cin >> op;
        if (op == 0) {
            int k;
            cin >> k;
            k = (k + lastans - 1) % 39989 + 1;
            lastans = qry(1, 1, maxn, k);
            cout << lastans << '\n';
        } else {
            int x0, y0, x1, y1;
            cin >> x0 >> y0 >> x1 >> y1;
            x0 = (x0 + lastans - 1) % 39989 + 1;
            x1 = (x1 + lastans - 1) % 39989 + 1;
            y0 = (y0 + lastans - 1) % 1000000000 + 1;
            y1 = (y1 + lastans - 1) % 1000000000 + 1;
            if (x0 > x1) swap(x0, x1), swap(y0, y1);
            line[++tot] = {x0, y0, x1, y1};
            upd(1, 1, maxn, tot);
        }
    }
    return 0;
}
