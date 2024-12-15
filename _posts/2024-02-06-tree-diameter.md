---
title: 树的直径
tag: 算法笔记
---
## 定义：

树上最远的两点之间的距离(**可以有多条**)

## 求法一：树形dp

枚举所有的节点，求以该节点为起点的最长距离和次长距离之和，最大值即为树的直径。代码如下：

```cpp
long long diameter, dp[MAXN];
void getDiameter1(int x, int fa)
{
    for (int i = head[x]; i; i = e[i].next)
    {
        int y = e[i].to;
        if (y == fa) continue;
        getDiameter1(y, x);
        diameter = max(diameter, dp[x] + dp[y] + e[i].dis);
        /*
         * 有两种情况：
         * 1. 直径不含x，则在递归y时已经求出了直径
         * 2. 直径含x，则直径为x出发的最长距离+y出发不经过x的最长距离+dis(x, y)
         *    由于还没有用y的结果更新dp[x]所以x出发的最长距离一定不经过y
         */
        dp[x] = max(dp[x], dp[y] + e[i].dis);
    }
}
```
160ms [提交记录](https://www.luogu.com.cn/record/147001757)

## 求法二： 两次dfs搜索
定理：对于一棵边权非负的树，从树上任意一点出发能到达的最远的点必为直径的端点（证明略）。因此通过两次dfs可以找到一条直径并且可以找到这条直径的两个端点。代码如下：
```cpp
int ep, eq; // ep, eq为直径的两个端点
int dfs(int x, int fa, ll dis)
{
    // 起始节点, 父节点, 当前距离
    if (dis > diameter)
    {
        diameter = dis;
        ep = x;
    }
    for (int i = head[x]; i; i = e[i].next)
    {
        int y = e[i].to;
        if (y == fa) continue;
        dfs(y, x, dis + e[i].dis);
    }
    return ep;
}
void getDiameter2()
{
    eq = dfs(1, 1, 0);
    ep = dfs(eq, eq, 0);
}
```
170ms [提交记录](https://www.luogu.com.cn/record/147005629)