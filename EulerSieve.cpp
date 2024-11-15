struct Euler {
    int maxn = 1e5;
    vector<bool> isp;
    int tot;
    vector<int> p; 
    vector<int> fp;//first prime factor -> smallest prime factor
    vector<int> phi;
    void init(int n) {
        maxn = n;
        isp.resize(maxn + 10);
        p.resize(maxn + 10);
        fp.resize(maxn + 10);
        phi.resize(maxn + 10);

        phi[1] = 1;
        for (int i = 2; i <= maxn; i++) isp[i] = 1;
        for (int i = 1; i <= maxn; i++) fp[i] = i;
        for (int i = 2; i <= maxn; i++) {
            if (isp[i]) {
                p[++tot] = i;
                phi[i] = i - 1;
            }
            for (int j = 1; j <= tot && 1ll * i * p[j] <= maxn; j++) {
                isp[i * p[j]] = false;
                fp[i * p[j]] = p[j];
                if (i % p[j] == 0) {
                    phi[i * p[j]] = phi[i] * p[j];
                    break;
                } else {
                    phi[i * p[j]] = phi[i] * (p[j] - 1);
                }
            }
        }
    }
} euler;