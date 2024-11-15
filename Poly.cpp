using i64 = long long;
using poly = vector<int>;
const int maxn = 1e6;
const int maxlen = 1 << int(__lg(2 * maxn - 1 - 1) + 1);
const int mo = 998244353;

namespace Cipolla {// 模意义开根
    struct num {
        i64 x;// 实部
        i64 y;// 虚部(即虚数单位√w的系数)
    };
    i64 w;
    num mul(num a, num b, i64 p) {// 复数乘法 
        num res;
        res.x = ((a.x * b.x % p + a.y * b.y % p * w % p) % p + p) % p;
        res.y = ((a.x * b.y % p + a.y * b.x % p) % p + p) % p;
        return res;
    }
    i64 qpow_r(i64 a, i64 b, i64 p) {
        i64 res = 1;
        while(b) {
            if(b & 1) res = res * a % p;
            a = a * a % p;
            b >>= 1;
        }
        return res;
    }
    i64 qpow_i(num a, i64 b, i64 p) {
        num res = {1, 0};
        while(b) {
            if(b & 1) res = mul(res, a, p);
            a = mul(a, a, p);
            b >>= 1;
        }
        return res.x % p;
    }
    i64 cipolla(i64 n, i64 p) {//p必须是奇素数；如果找到一个解x,那么p-x也是解（0的“另一个”解是0）
        srand(time(0));
        n %= p;
        if (n == 0) return 0;
        if(qpow_r(n, (p - 1) / 2, p) == -1 + p) return -1;// 据欧拉准则判定是否有解 
        
        i64 a;
        while(1) {// 找出一个符合条件的a，平均随机2次
            a = rand() % p;
            w = (((a * a) % p - n) % p + p) % p;// w = a^2 - n，虚数单位的平方
            if(qpow_r(w, (p - 1) / 2, p) == -1 + p) break;
        }
        num x = {a, 1};
        i64 res = qpow_i(x, (p + 1) / 2, p);
        res = min(res, mo - res);
        return res;
    }
}
int binpow(int a, int b) {
    int res = 1;
    for (; b; a = 1ll * a * a % mo, b >>= 1) { if (b & 1) res = 1ll * res * a % mo; }
    return res;
}
int getinv(int x) { return binpow(x, mo - 2); }
namespace NTT {
    const int g = 3;
    poly Omega(int len) {
        int wn = binpow(g, mo / len);
        poly w(len);
        w[len >> 1] = 1;
        for (int i = len / 2 + 1; i < len; i++) w[i] = i64(w[i - 1]) * wn % mo;
        for (int i = len / 2 - 1; i >= 1; i--) w[i] = w[i << 1];
        return w;
    }
    auto W = Omega(maxlen);// !!n <= len!!
    void FFT(int* a, int n) {
        for (int k = n >> 1; k; k >>= 1)
            for (int i = 0, y; i < n; i += k << 1)
                for (int j = 0; j < k; ++j)
                    y = a[i + j + k], a[i + j + k] = i64(a[i + j] - y + mo) * W[k + j] % mo, a[i + j] = (a[i + j] + y) % mo;
    }
    void IFFT(int* a, int n) {
        for (int k = 1; k < n; k <<= 1)
            for (int i = 0, x, y; i < n; i += k << 1)
                for (int j = 0; j < k; ++j)
                    x = a[i + j], y = i64(a[i + j + k]) * W[k + j] % mo, a[i + j + k] = x < y ? x - y + mo : x - y, a[i + j] = (a[i + j] + y) % mo;
        int Inv = mo - (mo - 1) / n;
        for (int i = 0; i < n; i++) a[i] = (i64)a[i] * Inv % mo;
        reverse(a + 1, a + n);
    }
}
namespace Poly {
    int norm(int n) { return 1 << (__lg(n - 1) + 1); }
    void norm(poly &a) { 
        if (a.size()) a.resize(norm(a.size()), 0); 
        else a = {0};
    }
    void DFT(poly &a) { return NTT::FFT(a.data(), a.size()); }
    void IDFT(poly &a) { return NTT::IFFT(a.data(), a.size()); }
    void dot(poly &a, poly &b) { for (int i = 0; i < a.size(); i++) a[i] = i64(a[i]) * b[i] % mo; }

    poly& operator *= (poly &a, int b) {
        for (auto &x : a) x = i64(x) * b % mo;
        return a;
    } 
    poly operator * (poly a, int b) { return a *= b; }
    poly operator * (int a, poly b) { return b * a; }
    //Poly mul O(nlogn)
    poly operator * (poly a, poly b) {
        int n = a.size() + b.size() - 1, len = norm(n);
        if (a.size() <= 8 || b.size() <= 8) {
            poly c(n);
            for (int i = 0, lim_i = a.size(); i < lim_i; i++) {
                for (int j = 0, lim_j = b.size(); j < lim_j; j++) {
                    c[i + j] = (c[i + j] + (i64)a[i] * b[j]) % mo;
                }
            }
            return c;
        }
        a.resize(len), b.resize(len);
        DFT(a), DFT(b), dot(a, b), IDFT(a);
        return a.resize(n), a;
    }

    poly& operator /= (poly &a, int b) { return a *= binpow(b, mo - 2); }
    poly operator / (poly a, int b) { return a /= b; }

    poly& operator += (poly &a, poly b) {
        a.resize(max(a.size(), b.size()));
        for (int i = 0; i < a.size(); i++) a[i] = (a[i] + b[i]) % mo;
        return a;
    }
    poly operator + (poly a, poly b) { return a += b; }

    poly& operator -= (poly &a, poly b) {
        a.resize(max(a.size(), b.size()));
        for (int i = 0; i < a.size(); i++) a[i] = (a[i] - b[i] + mo) % mo;
        return a;
    }
    poly operator - (poly a, poly b) { return a -= b; }

    // Poly inv, O(nlogn)
    poly Inv2k(poly a) { // |a| = 2 ^ k
        int n = a.size(), m = n >> 1;
        if (n == 1) return {getinv(a[0])};
        poly b = Inv2k(poly(a.begin(), a.begin() + m)), c = b;
        b.resize(n), DFT(a), DFT(b), dot(a, b), IDFT(a);
        for (int i = 0; i < n; i++) a[i] = i < m ? 0 : mo - a[i];
        DFT(a), dot(a, b), IDFT(a);
        return move(c.begin(), c.end(), a.begin()), a;
    }
    poly Inv(poly a) {// mod x^n
        int n = a.size();
        norm(a), a = Inv2k(a);
        return a.resize(n), a;
    }
    
    // Poly div
    poly operator / (poly a, poly b) {// a = res * b + remainder
        int k = a.size() - b.size() + 1;
        if (k < 0) return {0};
        reverse(a.begin(), a.end());
        reverse(b.begin(), b.end());
        b.resize(k), a = a * Inv(b);
        a.resize(k), reverse(a.begin(), a.end());
        return a;
    }
    pair<poly, poly> operator % (poly a, const poly& b) {
        poly c = a / b;
        a -= b * c, a.resize(b.size() - 1);
        return {c, a};
    }

    // Poly calculus
    poly deriv(poly a) {
        for (int i = 1; i < a.size(); i++) a[i - 1] = i64(i) * a[i] % mo;
        return a.pop_back(), a;
    }
    poly inv = {0, 1}; 
    void calcinv(int x) {
        for (int i = int(inv.size()); i < x + 1; i++) {
            inv.push_back(i64(mo - (mo / i)) * inv[mo % i] % mo);
        }
    }
    poly integ(poly a) {
        a.push_back(0);
        calcinv((int)a.size() - 1);
        for (int i = (int)a.size() - 1; i >= 1; i--) a[i] = (i64)inv[i] * a[i - 1] % mo;
        return a[0] = 0, a;
    }

    //Poly ln O(nlogn)
    poly Ln(poly a) {// assert(a[0] == 1)
        int n = a.size();
        a = deriv(a) * Inv(a);
        return a.resize(n - 1), integ(a);
    }

    // Poly exp O(nlogn)
    poly Exp(poly a) {// assert(a[0] == 0)
        int n = a.size(), k = norm(n);
        poly b = {1}, c, d; a.resize(k);
        for (int L = 2; L <= k; L <<= 1) {
            d = b, b.resize(L), c = Ln(b), c.resize(L);
            for (int i = 0; i < L; i++) c[i] = a[i] - c[i] + (a[i] < c[i] ? mo : 0);
            c[0] = (c[0] + 1) % mo, DFT(b), DFT(c), dot(b, c), IDFT(b);
            move(d.begin(), d.end(), b.begin());
        } 
        return b.resize(n), b;
    }

    // Poly sqrt O(nlogn)
    poly Sqrt(poly a) {
        int n = a.size(), k = norm(n); a.resize(k);
        poly b = {(int)Cipolla::cipolla(a[0], mo), 0}, c;
        for (int L = 2; L <= k; L <<= 1) {
            b.resize(L), c = poly(a.begin(), a.begin() + L) * Inv2k(b);
            for (int i = L / 2; i < L; i++) b[i] = i64(c[i]) * (mo + 1) / 2 % mo; 
        }
        return b.resize(n), b;
    }

    poly Pow(poly &a, int b) { return Exp(Ln(a) * b); }// a[0] = 1
    // b1 = b % mo, b2 = b % phi(mo), mod x^n
    // a[0] != 0 iff b >= n
    poly Pow(poly a, int b1, int b2) {
        int n = a.size(), d = 0, k;
        while (d < n && !a[d]) ++d;
        if ((i64)d * b1 >= n) return poly(n);
        a.erase(a.begin(), a.begin() + d);
        k = getinv(a[0]), norm(a *= k);
        a = Pow(a, b1) * binpow(k, mo - 1 - b2);
        a.resize(n), d *= b1;
        for (int i = n - 1; i >= 0; i--) a[i] = i >= d ? a[i - d] : 0;
        return a;
    }
}
using namespace Poly;