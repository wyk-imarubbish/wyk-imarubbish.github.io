#include <bits/stdc++.h>
using namespace std;

// 矩阵类，用于存储和进行操作
class Matrix {
   private:
    int n, m;  // 矩阵的行数和列数

   public:
    vector<vector<int>> a;
    Matrix() : n(0), m(0) {}  // 构造含参矩阵并初始化
    Matrix(int n, int m) : n(n), m(m) {
        a.resize(n);
        for (auto &i : a) i.resize(m);
    }
    void read(void) {  // 从标准输入读取矩阵元素
        for (auto &i : a) {
            for (auto &j : i) {
                cin >> j;
            }
        }
    }
    void print(void) {  // 打印矩阵元素
        for (auto &i : a) {
            for (auto &j : i) {
                cout << j << " ";
            }
            cout << endl;
        }
    }
    vector<int> &operator[](int i) { return a[i]; };
};
// 编码类，用于处理编码和译码
class Encoding {
   private:
    int m, n;

   public:
    Matrix H, coset_table;
    map<int, int> e, d;
    string toBinary(int num, int len);
    int toDecimal(string num);
    void init(int m, int n, Matrix &H);
    void printEncodingFunction();
    Matrix getLeftCosetTable();
    void printLeftCosetTable();
    int getSyndrome(int x);
    void printSyndromeTable();
    Encoding(int _m, int _n, Matrix &_H) : m(_m), n(_n), H(_H) {
        init(m, n, H);
        coset_table = getLeftCosetTable();
        H.a.resize(n);
        for (int i = 0; i < n - m; i++) {
            vector<int> v;
            for (int j = 0; j < n - m; j++) {
                v.emplace_back(i == j ? 1 : 0);
            }
            H[i + m] = v;
        }
    }
};
// 将整数转化为二进制字符串
string Encoding::toBinary(int num, int len) {
    string binary_str;
    for (int i = 0; i < len; i++) {
        if ((1 << i) & num) {
            binary_str = "1" + binary_str;
        } else {
            binary_str = "0" + binary_str;
        }
    }
    return binary_str;
}
// 将二进制字符串转换为整数
int Encoding::toDecimal(string num) {
    int res = 0;
    for (char c : num) res = (res << 1) + (c == '1' ? 1 : 0);
    return res;
}

//  e和d分别为编码函数和译码函数，类型为map<int,int>
void Encoding::init(int m, int n, Matrix &H) {
    for (int b = 0; b < (1 << m); b++) {  // 遍历所有原码
        int eb = b;
        // 计算编码函数中x1, x2, ... , xr的值
        for (int j = 0; j < n - m; j++) {
            int x = 0;
            for (int i = 0; i < m; i++) {
                x ^= int(bool((1 << i) & b)) * H[m - i - 1][j];
            }
            eb = (eb << 1) + x;  // 每趟循环将eb左移并将计算得到的x的值附在eb后面
        }
        e[b] = eb;  // 存储编码函数
        d[eb] = b;  // 顺便存储部分的译码函数
    }
}
// 打印编码函数
void Encoding::printEncodingFunction() {
    for (auto [b, eb] : e) {
        cout << "e(" << toBinary(b, m) << ")=" << toBinary(eb, n) << endl;
    }
}
// 生成、打印左陪集表
Matrix Encoding::getLeftCosetTable() {
    Matrix table(1 << (n - m), 0);
    vector<bool> vis(1 << n);                                          // vis维护已经处理过的数
    for (int i = 0, j = 0; i < (1 << n) && j < (1 << (n - m)); i++) {  // 遍历所有B^n中的数i；j为译码表的行号
        if (!vis[i]) {                                                 // 如果当前的数未被处理过
            for (auto [b, eb] : e) {                                   // 遍历编码函数（实际是为了遍历译码表第一行）
                table[j].push_back(eb ^ i);                            // 存储左陪集
                vis[eb ^ i] = true;                                    // 记录处理过的数
            }
            // 陪集头为左陪集中权值最小的那个数，使用lambda表达式得到
            int head = *min_element(table[j].begin(), table[j].end(), [](int a, int b) { return __builtin_popcount(a) < __builtin_popcount(b); });
            table[j][0] = head;  // 把陪集头放到第一列
            for (int k = 0; k < (1 << m); k++) {
                table[j][k] = table[j][0] ^ table[0][k];  // 按照陪集头和第一行的异或值整理当前行
                d[table[j][k]] = d[table[0][k]];          // 顺便处理译码函数
            }
            j++;  // 行号加1
        }
    }
    // 按照题目要求对译码表的行进行排序
    sort(table.a.begin(), table.a.end(), [](vector<int> &a, vector<int> &b) {
        if (__builtin_popcount(a[0]) == __builtin_popcount(b[0]))
            return a[0] < b[0];  // 若陪集头权值相同，按照大小排序
        else                     // 否则，按照权值排序
            return __builtin_popcount(a[0]) < __builtin_popcount(b[0]);
    });
    return table;
}

void Encoding::printLeftCosetTable() {
    for (auto coset : coset_table.a) {
        for (int i = 0; i < coset.size(); i++) {
            cout << toBinary(coset[i], n) << " \n"[i == coset.size() - 1];
        }
    }
}

int Encoding::getSyndrome(int x) {
    int syn = 0;                       // 初始化特征值为0
    for (int j = 0; j < n - m; j++) {  // 遍历奇偶校验矩阵每一列
        int y = 0;
        // 计算特征值的当前位y
        for (int i = 0; i < n; i++) {
            y ^= bool((1 << i) & x) & H[n - i - 1][j];
        }
        syn = (syn << 1) + y;  // 更新特征值
    }
    return syn;
}
void Encoding::printSyndromeTable() {
    for (int i = 0; i < (1 << (n - m)); i++) {
        int header = coset_table[i][0];
        cout << toBinary(header, n) << " " << toBinary(getSyndrome(header), n - m) << endl;
    }
}
void test1(void) {  // 测试函数1：读取矩阵H，初始化编码对象，并打印编码函数
    int m, n;
    cin >> m >> n;
    Matrix H(m, n - m);
    H.read();
    Encoding E(m, n, H);
    E.printEncodingFunction();
}
void test2(void) {  // 测试函数2：读取矩阵H，初始化编码对象，并打印左陪集表
    int m, n;
    cin >> m >> n;
    Matrix H(m, n - m);
    H.read();
    Encoding E(m, n, H);
    E.printLeftCosetTable();
}
void test3(void) {  // 测试函数3：读取矩阵H，初始化编码对象，并根据输入的编码值打印解码结果
    int m, n;
    cin >> m >> n;
    Matrix H(m, n - m);
    H.read();
    Encoding E(m, n, H);
    int t;
    cin >> t;
    for (int i = 0; i < t; i++) {
        string eb;
        cin >> eb;
        int todecimaleb = E.toDecimal(eb);
        cout << "d(" << eb << ")=" << E.toBinary(E.d[todecimaleb], m) << endl;
    }
}
void test4() {  // 测试函数4：读取矩阵H，初始化编码对象，并打印综合症表
    int m, n;
    cin >> m >> n;
    Matrix H(m, n - m);
    H.read();
    Encoding E(m, n, H);
    E.printSyndromeTable();
}
void test5() {  // 测试函数5：综合测试，包括编码、解码和综合症计算
    int m, n;
    cin >> m >> n;
    Matrix H(m, n - m);
    H.read();
    Encoding E(m, n, H);
    int t;
    cin >> t;
    for (int i = 0; i < t; i++) {
        string s;
        cin >> s;
        int x = 0, eb = 0;
        for (char c : s) x = (x << 1) + (c == '1' ? 1 : 0);
        cout << "f(" << E.toBinary(x, n) << ")=" << E.toBinary(E.getSyndrome(x), n - m);
        for (int i = 0; i < (1 << (n - m)); i++) {            // 遍历译码表的每一行
            int header = E.coset_table[i][0];                 // 行首的元素为陪集头
            if (E.getSyndrome(header) == E.getSyndrome(x)) {  // 查找特征值相等的陪集头
                cout << " e=" << E.toBinary(header, n);
                eb = header ^ x;  // 计算得到e(b)
                break;
            }
        }
        for (auto [b, _eb] : E.e) {  // 遍历编码函数查找e(b)的原码b
            if (eb == _eb) {
                cout << " d(" << E.toBinary(x, n) << ")=" << E.toBinary(b, m) << endl;
                break;
            }
        }
    }
}
// 主函数
int main() {
    // test1();
    // test2();
    // test3();
     test4();
    // test5();
}
