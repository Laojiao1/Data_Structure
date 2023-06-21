# 普通平衡树（Splay）

## 一、定义

​	平衡树是二叉搜索树和堆合并构成的数据结构，它是一颗空树或它的左右两个子树的高度差的绝对值不超过1，并且左右两个子树都是一颗平衡二叉树

本篇主要说明 **Splay** 算法

## 二、正题

​	平衡树中有许多种类：红黑树，AVL树，伸展树（Splay），Treap等等，Splay算法算是可用性很强的一种了。也就是说比较稳定。

​	在Splay算法中，一个需要处处用到的东西就是旋转，即将当前结点与其前边一个结点一次旋转到目标位置。（后面代码分割部分会具体讲）

## 三、准备工作

```c++
struct Node {
	int s[2]; // 左右儿子 
	int p; // 父节点 
	int v; // 当前节点的值 
	int cnt; // 当前节点出现的数量 
	int size; // 子树大小 
	void init(int p1, int v1) {
		p = p1, v = v1;
		cnt = size = 1;
	}
} t[N];
int root; // 根结点编号 
int idx; // 节点个数 
```

```c++
void pushup(int x) {
	t[x].size = t[t[x].s[0]].size + t[t[x].s[1]].size + t[x].cnt;
}
```

​	push_up操作是用来维护点的儿子数量和自己的儿子数量

​	很好理解：当前子树大小（包括自己） =  左子树大小 + 右子树大小 + 当前根的大小 

## 四、旋转代码 + 核心伸展代码（rotate + Splay）

​	正题中说过：在Splay算法中，一个需要处处用到的东西就是旋转，即将当前结点与其前边一个结点一次旋转到目标位置。

​	由于这个树是一个二叉搜索树，**所以旋转之后要保证性质不变**（左子树 < 根节点 < 右子树 且 左右子树都是一个二叉搜索树）。我们就需要找到当前节点的 **父亲节点** 和 **爷爷结点** ，先将父亲节点与该节点所属关系的另一个子树连起来（即当前节点的儿子所在的子树应该为当前节点相对于父亲节点所在的子树），然后处理一下该节点和父亲节点的关系，最后更新爷爷结点和当前节点的关系。

### 1.旋转代码 （rotate）：

#### 以下图（右旋）为例：

<img src="C:\Users\44174\AppData\Roaming\Typora\typora-user-images\image-20220808163439932.png" alt="image-20220808163439932" style="zoom:33%;" />

以 x 为当前节点

第一步：取出x的父亲节点y，和爷爷结点z

第二步：判断x结点是在y节点的左边还是右边

第三步：把x的右节点b，与x的父节点y链接

第四步：处理x结点和y节点的关系（即当前节点与父亲节点的关系）

第五步：处理x结点和z节点的关系（即当前节点与爷0爷节点的关系）

最后不要忘记 pushup

**rotate 代码：**

```c++
void rotate(int x) { // 旋转, 保序且信息正确 
	int y = t[x].p; // 取出父亲 
	int z = t[y].p; // 取出爷爷
	int k = t[y].s[1] == x;
	
	// 以下代码根据图像理解 
	t[y].s[k] = t[x].s[k^1]; 
	t[t[x].s[k^1]].p = y;
	
	t[x].s[k^1] = y;
	t[y].p = x;
	
	t[z].s[t[z].s[1] == y] = x;
	t[x].p = z;
	
	pushup(y);
	pushup(x);
}
```

### 2. 核心伸展代码（Splay）：

访问一个结点x，并把x旋转到根节点

有以下三种情况：

<img src="C:\Users\44174\AppData\Roaming\Typora\typora-user-images\image-20220808164423026.png" alt="image-20220808164423026" style="zoom:33%;" />

#### 以下图为例：

- k > 0，把 x 转到 k 下面
- k = 0，把 x 转到根

<img src="C:\Users\44174\AppData\Roaming\Typora\typora-user-images\image-20220808164512357.png" alt="image-20220808164512357" style="zoom: 33%;" />

**Splay 代码：**

```c++
void Splay(int x, int k) {
    while(t[x].p != k) {
        int y = t[x].p, z = t[y].p;
        if(z != k) { // 折转底，直转中
            (t[y].s[0] == x) ^ (t[z].s[0] == y) ? rotate(x): rotate(y);
        }
        rotate(x);
    }
    if(k == 0) root = x;
}
```

## 五、一些操作

### 1. 插入 insert

```c++
void insert(int v) {
    int x = root, p = 0; // p记录当前节点的父节点
    while(x && t[x].v != v) {
        p = x;
        x = t[x].s[v > t[x].v];
    }
    if(x) t[x].cnt++;
    else {
        x = ++idx;
        t[p].s[v > t[p].v] = x;
        t[x].init(p, v);
    }
    Splay(x, 0);
}
```

### 2. 查找 find

功能：找到v所在结点，并把该节点转到根

<img src="C:\Users\44174\AppData\Roaming\Typora\typora-user-images\image-20220808170223670.png" alt="image-20220808170223670" style="zoom: 80%;" />

```c++
void find(int v) {
    int x = root;
    while(t[x].s[v > t[x].v] && v != t[x].v) {
        x = t[x].s[v > t[x].v];
    }
    Splay(x, 0);
}
```

### 3. 找前驱 get_pre

首先把v结点转到根节点，判断当前的权值，如果小于目标值的话直接返回；否则往左子树找，直到找到左子树中权值最大的那个点

```c++
int get_pre(int v) {
    find(v);
    int x = root;
    if(t[x].v < v) return x;
    x = t[x].s[0];
    while(t[x].s[1] != 0) {
        x = t[x].s[1];
    }
    return x;
}
```

### 4. 找后继 get_suc

同理找前驱

```c++
int get_suc(int v) {
    find(v);
    int x = root;
    if(t[x].v > v) return x;
    x = t[x].s[1];
    while(t[x].s[0] != 0) {
        x = t[x].s[0];
    }
    return x;
}
```

### 5. 删除 del

功能：删除 v （若有多个相同的数，只删除一个）

步骤：找到当前节点v的前驱结点和后继结点，把前驱结点转到根节点，后继节点移动到前驱节点的右子树，这样的话后继节点的左子树只能是该节点v。

注意要有哨兵（这样的话能保证找到每一个节点的前驱和后继）

```c++
void del(int v) {
    int pre = get_pre(v);
    int suc = get_suc(v);
    Splay(pre, 0), Splay(suc, pre);
    int del = t[suc].s[0];
    if(t[del].cnt > 1) {
        t[del].cnt--;
        Splay(del, 0); // 更新受影响的子树
    } else {
        t[suc].s[0] = 0;
        Splay(suc, 0); // 更新受影响的子树
    }
}
```

### 6. 查排名 get_rank

把当前结点转到根节点然后直接返回左子树大小即可



<img src="C:\Users\44174\AppData\Roaming\Typora\typora-user-images\image-20220808172926261.png" alt="image-20220808172926261" style="zoom:50%;" />

```c++
int get_rank(int v) {
    find(v);
    return t[t[root].s[0]].size; // 不用加1，因为之前插入了哨兵
}
```

### 7. 查排名为k的数值

由于有哨兵的存在，若查找排名为k的数值，应传入参数k+1

首先从根节点开始，分别记录一下当前点的左右节点，然后判断，如果当前点的子节点树加上当前点的值的数量小于查找的排名，直接减去然后走到右儿子，不然就走到左儿子就行了。

```c++
int get_val(int k) {
    int x = root;
    while(1) {
        int y = t[x].s[0];
        if(t[y].size + t[x].cnt < k) {
            k -= t[y].size + t[x].cnt;
            x = t[x].s[1];
        } else {
            if(t[y].size >= k) {
                x = t[x].s[0];
            } else {
                break;
            }
        }
	}
    Splay(x, 0);
    return t[x].v;
}
```

## 六、普通平衡树板子题

https://www.luogu.com.cn/problem/P3369

```c++
#include <bits/stdc++.h>
using namespace std;
const int N = 5e5+5;
struct Node {
	int s[2]; // 左右儿子 
	int p; // 父节点 
	int v; // 当前节点的值 
	int cnt; // 当前节点出现的数量 
	int size; // 子树大小 
	void init(int p1, int v1) {
		p = p1, v = v1;
		cnt = size = 1;
	}
} t[N];
int root; // 根结点编号 
int idx; // 节点个数 
int n;
void pushup(int x) {
	t[x].size = t[t[x].s[0]].size + t[t[x].s[1]].size + t[x].cnt;
} // 左子树大小 + 右子树大小 + 当前根的大小 
void rotate(int x) { // 旋转, 保序且信息正确 
	int y = t[x].p; // 取出父亲 
	int z = t[y].p; // 取出爷爷
	int k = t[y].s[1] == x;
	
	// 以下代码根据图像理解 
	t[y].s[k] = t[x].s[k^1]; 
	t[t[x].s[k^1]].p = y;
	
	t[x].s[k^1] = y;
	t[y].p = x;
	
	t[z].s[t[z].s[1] == y] = x;
	t[x].p = z;
	
	pushup(y);
	pushup(x);
}
void Splay(int x, int k) {
    while(t[x].p != k) {
        int y = t[x].p, z = t[y].p;
        if(z != k) { // 折转底，直转中
            (t[y].s[0] == x) ^ (t[z].s[0] == y) ? rotate(x): rotate(y);
        }
        rotate(x);
    }
    if(k == 0) root = x;
}
void find(int v) {
    int x = root;
    while(t[x].s[v > t[x].v] && v != t[x].v) {
        x = t[x].s[v > t[x].v];
    }
    Splay(x, 0);
}
int get_pre(int v) {
    find(v);
    int x = root;
    if(t[x].v < v) return x;
    x = t[x].s[0];
    while(t[x].s[1] != 0) {
        x = t[x].s[1];
    }
    return x;
}
int get_suc(int v) {
    find(v);
    int x = root;
    if(t[x].v > v) return x;
    x = t[x].s[1];
    while(t[x].s[0] != 0) {
        x = t[x].s[0];
    }
    return x;
}
void del(int v) {
    int pre = get_pre(v);
    int suc = get_suc(v);
    Splay(pre, 0), Splay(suc, pre);
    int del = t[suc].s[0];
    if(t[del].cnt > 1) {
        t[del].cnt--;
        Splay(del, 0); // 更新受影响的子树
    } else {
        t[suc].s[0] = 0;
        Splay(suc, 0); // 更新受影响的子树
    }
}
int get_val(int k) {
    int x = root;
    while(1) {
        int y = t[x].s[0];
        if(t[y].size + t[x].cnt < k) {
            k -= t[y].size + t[x].cnt;
            x = t[x].s[1];
        } else {
            if(t[y].size >= k) {
                x = t[x].s[0];
            } else {
                break;
            }
        }
	}
    Splay(x, 0);
    return t[x].v;
}
int get_rank(int v) {
    find(v);
    return t[t[root].s[0]].size; // 不用加1，因为之前插入了哨兵
}
void insert(int v) {
    int x = root, p = 0; // p记录当前节点的父节点
    while(x && t[x].v != v) {
        p = x;
        x = t[x].s[v > t[x].v];
    }
    if(x) t[x].cnt++;
    else {
        x = ++idx;
        t[p].s[v > t[p].v] = x;
        t[x].init(p, v);
    }
    Splay(x, 0);
}
int main() {
	insert(-2147483647); insert(2147483647);
    scanf("%d", &n);
    while(n--) {
        int op, x;
        scanf("%d %d", &op, &x);
        if(op == 1) insert(x);
        if(op == 2) del(x);
        if(op == 3) printf("%d\n", get_rank(x));
        if(op == 4) printf("%d\n", get_val(x+1));
        if(op == 5) printf("%d\n", t[get_pre(x)].v);
        if(op == 6) printf("%d\n", t[get_suc(x)].v);
    }
	return 0;
}
```





