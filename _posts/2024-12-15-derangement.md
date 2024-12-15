---
title: 生日问题
tags: 概率论
article_header:
  type: cover
  image:
    src: /screenshot.jpg
---
# 用概率论求N阶行列式展开式中含主对角线元素的个数

## 问题建模

要用概率论的思想计算 $N$ 阶行列式展开式中包含主对角线元素的项的个数，可以将问题转化为一个排列组合问题并借助概率论的思路分析。

- $N$ 阶行列式的展开式基于矩阵中所有 $N!$ 个排列（即 $\text{S}_N$，对 $\{1, 2, \dots, N\}$的全排列）确定。每个排列对应展开式中的一项。

​	展开式中的每一项具有如下形式：
$$
\operatorname{sign}(\sigma)\cdot a_{1,\sigma(1)}a_{2,\sigma(2)}\cdots a_{N,\sigma(N)},
$$
​	其中 $\sigma\in S_{N}$ 是一个排列。

- 主对角线元素是 $\{a_{1,1},a_{2,2},\ldots,a_{N,N}\}$。问题可以表述为：从所有可能的排列 $\sigma$ 中，找到那些包含 $a_{i,i} (1 \leq i \leq N)$ 的排列。

## 求解过程

利用概率论中的事件来解决这个问题。

随机取一个排列 $\sigma$ 。 设事件 $A_i$ 表示 $a_{i, i}$ 在排列 $\sigma$ 中的概率。

由容斥原理，我们有：
$$
P(\text{至少一个 }A_i\text{ 发生})=\sum_{i=1}^NP(A_i)-\sum_{1\leq i<j\leq N}P(A_i\cap A_j)+\cdots+(-1)^{k+1}\sum_{1\leq i_1<i_2<\cdots<i_k\leq N}P(A_{i_1}\cap\cdots\cap A_{i_k}),
$$
其中，$\sum_{1\leq i_1<\cdots<i_k\leq N}P(A_{i_1}\cap\cdots\cap A_{i_k})=C_{N}^k\frac{(N-k)!}{N!}$

因此，
$$
P(\text{至少一个 }A_i\text{ 发生})=\sum_{k=1}^N(-1)^{k+1}\cdot C_{N}^k\cdot \frac{(N-k)!}{N!}
=\sum_{k=1}^N\frac{(-1)^k}{k!}
$$
共有 $N!$ 个排列，因此 $N$ 阶行列式展开式中含主对角线元素的个数为：
$$
N!\cdot \sum_{k=1}^N\frac{(-1)^{k+1}}{k!}
$$

## 结论

 $N$ 阶行列式展开式中含主对角线元素的个数为$N!\cdot \sum_{k=1}^N\frac{(-1)^{k+1}}{k!}$。

## 备注

- 这其实就是错排问题的反面。 

- $N$ 个数的错排数记作 $!N$，本问题结论可改写为 $N! - !N$ 。

  其中，$!N$ 有简化公式 $!N = \left\lfloor\frac{N!}{e}+0.5\right\rfloor$，即 $\frac{N!}{e}$ 四舍五入得到的正整数。