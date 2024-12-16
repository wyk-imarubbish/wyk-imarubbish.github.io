---
title: 2024BUPT网络赛题解(A~D)
tag: 题解
---
## Problem A. 板烧鸡腿堡 

### 题意简述

给定一个2024/9/8到2025/9/7之间的日期，判断是否是周一。

### 思路

维护月份前缀和 $days_i$，则 $m$ 月 $d$ 日在一年中的总天数为 $days_{m-1}+d$，求与某个周一的天数差值是否能被 $7$ 整除即可。

### 代码

```cpp
#include <bits/stdc++.h>
using namespace std;
int mon[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int days[20] = {0};
int main() {
    for (int i = 1; i <= 12; i++) {
        days[i] = days[i - 1] + mon[i];
    }
    int base = days[9] + 7;
    int T;
    cin >> T;
    while (T--) {
        int y, m, d;
        scanf("%d-%d-%d", &y, &m, &d);
        if (y == 2024 && abs(base - days[m - 1] - d) % 7 == 0) {
            cout << "Grilled Chicken Sandwich!!!\n";
        } else if (y == 2025 && (days[m - 1] + d + 365 - base) % 7 == 0) {
            cout << "Grilled Chicken Sandwich!!!\n";
        } else {
            cout << "Canteen...\n";
        }
    }
}
```



## Problem B. 计算综测分

### 题意简述

给定综测分的计算方法，输入姓名、各项分数、班委任职和评级以及志愿时长，要求排序后输出。

### 思路

按照题意模拟即可。注意判断浮点数相等需要用 `fabs(a - b) < eps`，以及志愿时长能加的最高分是 $8$ 分。

### 代码

```cpp
#include <bits/stdc++.h>
using namespace std;
constexpr int MAXN = 1E5 + 5;
constexpr double eps = 1E-8;
struct Student {
    string name;
    int a, time;
    double score;
} stu[MAXN];
bool cmp(Student &x, Student &y) {
    if (fabs(x.score - y.score) < eps && x.time == y.time && x.a == y.a)
        return x.name < y.name;
    else if (fabs(x.score - y.score) < eps && x.time == y.time)
        return x.a > y.a;
    else if (fabs(x.score - y.score) < eps)
        return x.time > y.time;
    else
        return x.score > y.score;
}
int main() {
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        int s1, s2, s3;
        cin >> stu[i].name >> s1 >> s2 >> s3;
        stu[i].score = 0.1 * s1 + 0.7 * s2 + 0.2 * s3;
        string c;
        cin >> stu[i].a;
        if (stu[i].a > 0) {
            cin >> c;
            if (c == "A+") {
                stu[i].score += 12 * 0.2;
            } else if (c == "A") {
                stu[i].score += 11 * 0.2;
            } else if (c == "B") {
                stu[i].score += 10 * 0.2;
            } else if (c == "C") {
                stu[i].score += 8 * 0.2;
            }
        }
        int m;
        cin >> m;
        stu[i].time = 0;
        int tmp = 0;
        for (int j = 0; j < m; j++) {
            int opt, h;
            cin >> opt >> h;
            if (opt == 1) {
                stu[i].time += h;
                tmp += 2;
            } else {
                stu[i].time += h;
                tmp += 1;
            }
        }
        stu[i].score += min(tmp, 16) * 0.1;
    }
    sort(stu, stu + n, cmp);
    for (int i = 0; i < n; i++) {
        cout << stu[i].name;
        printf(" %.1f\n", stu[i].score);
    }
}
```



## Problem C. k8s

### 题意简述

把文章中每个长度大于2的单词转换为“首字母-中间字符数-尾字母”的形式。

### 思路

一直 `getchar()` 读到单词时压缩单词再输出，否则直接输出。

### 代码

```cpp
#include <bits/stdc++.h>
using namespace std;
int main() {
    char ch = getchar();
    while (ch != EOF) {
        if (!isalpha(ch)) {
            cout << ch;
            ch = getchar();
        } else {
            string s;
            s.push_back(ch);
            ch = getchar();
            while (ch != EOF && isalpha(ch)) {
                s.push_back(ch);
                ch = getchar();
            }
            if (s.size() > 2U) {
                cout << s[0] << s.size() - 2 << s[s.size() - 1];
            } else {
                cout << s;
            }
        }
    }
    return 0;
}
```



## Problem D. random

### 题意简述

给定长度为 $n \leq 5$ 的正整数序列 $\{A_n\}$，进行 $k \leq 5$ 次操作，每次随机选择两个下标（可以相等）进行交换。

求 $\sum_{i=1}^{n-1}(A_i\bigoplus A_{i+1})+(A_n\bigoplus A_1)$ 的期望。

### 思路

注意到数据范围很小，考虑暴力求解，至多 $(5 \cdot 5)^5=9765625$​ 种排列。dfs出每一种排列然后暴力计算，最后取平均数即可。

每次暴力计算带一个小的常数，我们赛时估计最坏情况大概需要 5E7 次运算。于是给出了两个优化：

1. 当 $n \leq k$ 时，所有排列都可以取到，并且似乎是等可能的，那么对所有排列进行计算然后取平均即可。
2. 对于更一般的情况，还是考虑dfs，但是可以先对所有排列进行预处理，然后用康托展开建立映射关系，dfs到一个排列直接把预处理好的值加到总和里即可。
3. （赛后的想法）要是卡时间也就是 $n=k=5$​ 时会比较极限，直接本地跑完然后打表就行了。

### 代码

*赛时看到很多人在一个小时多点就过了D，于是直接写了一发纯暴力交上去*

```cpp
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;
using lb = long double;
constexpr ll MOD = 998244353;
ll a[5], n, k, res, cnt;

ll ksc(ull x, ull y, ll p) { return (x * y - (ull)((lb)x / p * y) * p + p) % p; }
ll fastPow(ll x, ll y, ll p) {
    ll res = 1;
    while (y) {
        if (y & 1) {
            res = ksc(res, x, p);
        }
        x = ksc(x, x, p);
        y >>= 1;
    }
    return res;
}

ll count() {
    ll ans = 0;
    for (int i = 0; i < n; i++) {
        ans = (ans + (a[i] ^ a[(i + 1) % n])) % MOD;
    }
    return ans;
}

void dfs(int dep) {
    if (dep == k) {
        res = (res + count()) % MOD;
        cnt++;
        return;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            swap(a[i], a[j]);
            dfs(dep + 1);
            swap(a[i], a[j]);
        }
    }
}

int main() {
    cin >> n >> k;
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    dfs(0);
    cout << res * fastPow(cnt, MOD - 2, MOD) % MOD << "\n";
}
```