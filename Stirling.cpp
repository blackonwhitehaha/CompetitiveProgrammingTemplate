struct Stirling {
    int maxn;
    int mo;
    vector<int> fac;
    vector<int> inv;

    int binpow(int a, int b) {
        int res = 1;
        while (b) {
            if (b & 1) res = 1ll * res * a % mo;
            a = 1ll * a * a % mo;
            b >>= 1;
        }
        return res;
    }
    int getinv(int x) { return binpow(x, mo - 2); }

    void init(int n, int mod) {
        maxn = n;
        mo = mod;
        fac = inv = vector<int>(maxn + 10);
        fac[0] = 1;
        for (int i = 1; i <= maxn; i++) fac[i] = 1ll * i * fac[i - 1] % mo;
        inv[maxn] = getinv(fac[maxn]);
        for (int i = maxn - 1; i >= 0; i--) inv[i] = 1ll * inv[i + 1] * (i + 1) % mo;
    }

    int C(int n, int m) {
        if (m < 0 || n < m) return 0;
        return 1ll * fac[n] * inv[m] % mo * inv[n - m] % mo;
    }
    
    int S2(int n, int k) {//n different balls, k same boxes(not empty)
        int res = 0;
        for (int i = 1; i <= k; i++) {
            i64 op = (k - i) & 1 ? -1 : 1;
            res = (res + op * binpow(i, n) * inv[i] % mo * inv[k - i]) % mo;
            if (res < 0) res += mo;
        }
        return res;
    }
} stirling;