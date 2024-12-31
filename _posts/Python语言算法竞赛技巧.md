---
title: Python语言算法竞赛技巧
---

## 输入

```python
t = int(input()) # 读入单个数
n, m, k = [int i for i in input().split()] # 解包一行有限个数据
a = list(map(int, input().split())) # 读入一行数组
```

## 开数组

- **务必减少append方法的使用，尽量用定长数组，array库测下来基本无用**

```python
arr = [0] * n # 这是测试下来最快的方法
arr = [[0] * n for _ in range(m)] # 二维数组
```

## 自定义排序

- 在`sort`方法中，key的含义是按照key进行排序，接受一个单输入单输出的函数。

```python
arr = [-1, -2, 3, 4, 2, 2, 1]
arr.sort(reverse = True) # 由大到小排序
arr.sort(key = abs) # 按照绝对值排序
arr.sort(key = lambda x : x % 3) # 按照对3的余数排序
```

- 使用 `functools.cmp_to_key` 模拟 C++ 风格的比较函数
- 与C++不同的是，返回值必须是1，0，-1

```python
from functools import cmp_to_key
def cmp(x, y):
    return x > y
arr.sort(key = cmp_to_key(cmp))
```

## 二分

- lower_bound和upper_bound的平替

```python
import bisect # 在标准库里，放心使用
arr = [1, 3, 3, 3, 5, 7, 9] # 有序数组
target = 3 # 要查找的值
lb = bisect.bisect_left(arr, target) # lower_bound,但是返回的是数组下标
rb = bisect.bisect_right(arr, target) # upper_bound,同样返回的是数组下标
cnt = rb - lb
```

## 快速幂

- 系统自带的函数，复杂度为 $O(\log{n})$

```python
a, n, mod = 2, 10, 10
pow(a, n, mod) # 2 ** 10 % 10 = 4
```

## 集合

