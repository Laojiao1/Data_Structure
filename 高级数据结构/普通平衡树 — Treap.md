# 普通平衡树 — Treap

#### **介绍：**

有旋Treap依靠旋转操作维护树的平衡。

FHQ Treap 又称无旋Treap，由范浩强发明。他抛弃了旋转操作，使用**分裂**和**合并**两个操作来维护树的平衡。

## 一、原理

![image-20220811095218268](C:\Users\44174\AppData\Roaming\Typora\typora-user-images\image-20220811095218268.png)

Treap = Tree + Heap

平衡树上每个节点放两个值：树的权值val，堆的随机值key。对于val值，维护查找树的性质，对于key值，维护堆的性质。

这样，即使插入一个有序序列，也不会退化成一条链。

堆的随机值等价于打乱了有序序列插入的顺序

如图：1 2 3 4 5 6 的插入 等价于 4 2 5 1 3 6

## 二、核心代码

### 1. 准备操作

```c++
// 结点信息
struct Node {
    int val; // 树的权值 
	int key; // 堆的随机值 
	int size; // 子树大小 
	int l, r; // 左右儿子 
} T[N];
```

```c++
// 构造新结点
int newnode(int v) {
    t[++idx].val = v;
    t[v].key = rand();
    t[v].size = 1;
    return idx;
}
```

```c++
// 向上更新
void push_up(int p) {
    t[p].size = t[t[p].l].size + t[t[p].r].size + 1;
}
```

## 2. 分裂（split）

### **1. 按值分裂**

​	根据给的值 v 把一颗树分裂成两棵树，一棵树的值val ≤ v， 另一棵树的值 val > v。

​	如果当前节点p的val ≤ v，说明p以及其左子树都属于分裂后的 左 Treap。p的右子树也可能部分 ≤ v，因此需要继续递归分裂右子树，把 ≤ v 的那部分作为p的右子树。**把 x 指向左 Treap 的根**。

​	同理，如果当前节点p的val > v，说明p以及其右子树都属于分裂后的 右 Treap。p的左子树也可能部分 > v，因此需要继续递归分裂左子树，把 > v 的那部分作为p的左子树。**把 y 指向右 Treap 的根**。

​            <img src="C:\Users\44174\AppData\Roaming\Typora\typora-user-images\image-20220811101735725.png" alt="image-20220811101735725" style="zoom:33%;" />                                              <img src="C:\Users\44174\AppData\Roaming\Typora\typora-user-images\image-20220811101806678.png" alt="image-20220811101806678" style="zoom:33%;" /> 

```c++
void split(int p, int v, int& x, int& y) {
    if(!p) {
        x = y = 0;
        return;
    }
    if(t[p].val <= v) {
        x = p;
        split(t[x].r, v, t[x].r, y);
    } else {
        y = p;
        split(t[y].l, v, x, t[y].l);
    }
    push_up(p);
}
```

### **2. 合并 （merge）**

​	合并函数两个参数：左Treap的根指针x，右Treap的根指针y。必须满足x中所有结点的val值小于等于y中所有结点的val值。

​	因为两个Treap已经有序，所以在合并的之后只需要考虑把哪个树放在上面，把哪个树放在下面。也就是需要判断将哪一个树作为子树。根据小根堆的性质，我们把key值小的放在上面。

<img src="C:\Users\44174\AppData\Roaming\Typora\typora-user-images\image-20220811102736767.png" alt="image-20220811102736767" style="zoom:33%;" />            <img src="C:\Users\44174\AppData\Roaming\Typora\typora-user-images\image-20220811102750524.png" alt="image-20220811102750524" style="zoom:33%;" />

```c++
int merge(int x, int y) {
    if(!x || !y) return x + y;
    if(t[x].key < t[y].key) {
        t[x].r = merge(t[x].r, y);
        push_up(x);
        return x;
    } else {
        t[y].l = merge(x, t[y].l);
        push_up(y);
        return y;
    }
}
```

## 三、操作部分

### 1. 插入（insert）

​	首先按值分裂，其次建立新节点指向临时变量z，最后进行合并，先合并分裂后的左子树和z，保证搜索树性质，其次再与分裂后的右子树合并

**例如：insert (3)**

<img src="C:\Users\44174\AppData\Roaming\Typora\typora-user-images\image-20220811104210028.png" alt="image-20220811104210028" style="zoom:33%;" />

```c++
void insert(int v) {
    int x, y, z;
    split(root, v, x, y);
    z = newnode(v);
    root = merge(merge(x, z), y);
}
```

### 2. 删除（del）

​	首先对根节点root按值val分裂，分裂出x, z；然后再对x按val-1进行分裂，分裂出x，y；其次合并y的左右子树；最后合并x, y, z

**例如：del (3)**

<img src="C:\Users\44174\AppData\Roaming\Typora\typora-user-images\image-20220811105229404.png" alt="image-20220811105229404" style="zoom:33%;" />

```c++
void del(int v) {
    int x, y, z;
    split(root, v, x, z);
    split(x, v-1, x, y);
    y = merge(t[y].l, t[y].r);
    root = merge(merge(x, y), z);
}
```

###  3. 返回排名第k个结点的编号（get_k）

```c++
int get_k(int p, int k) {
    if(k <= t[t[p].l].size) {
        return get_k(t[p].l, k);
    }
    if(k == t[t[p].l].size + 1) {
        return p;
    }
    return get_k(t[p].r, k - t[t[p].l].size - 1);
}
```

### 4. 找前驱（get_pre）

​	首先对root按val-1进行分裂，然后向左子树找到最大的编号，输出该编号对应的权值即可，最后别忘了合并起来。

```c++
void get_pre(int v) {
    int x, y;
    split(root, v-1, x, y);
    int p = get_k(x, t[x].size);
    cout << t[p].val << endl;
    root = merge(x, y);
}
```

### 5. 找后继（get_suc）

​	首先对root按val进行分裂，然后向右子树找到第一个编号，输出该编号对应的权值即可，最后别忘了合并起来。

```c++
void get_suc(int v) {
    int x, y;
    split(root, v, x, y);
    int p = get_k(y, 1);
    cout << t[p].val << endl;
    root = merge(x, y);
}
```

### 6. 按值查找排名 （get_rank）

```c++
void get_rank(int v) {
    int x, y;
    split(root, v-1, x, y);
    cout << t[x].size+1 << endl;
    root = merge(x, y);
}
```

### 7. 按排名查找值（get_val)

```c++
void get_val(int k) {
    int p = get_k(root, k);
    cout << t[p].val << endl;
}
```

## 四、整体代码

https://www.luogu.com.cn/problem/P3369

```c++
#include <bits/stdc++.h>
using namespace std;
const int N = 5e5+5; 
struct Node {
	int val; // 树的权值 
	int key; // 堆的随机值 
	int size; // 子树大小 
	int l, r; // 左右儿子 
} t[N];
int n, idx, root;
void push_up(int p) {
    t[p].size = t[t[p].l].size + t[t[p].r].size + 1;
}
int newnode(int v) {
    t[++idx].val = v;
    t[idx].key = rand();
    t[idx].size = 1;
    return idx;
}
void split(int p, int v, int& x, int& y) {
    if(!p) {
        x = y = 0;
        return;
    }
    if(t[p].val <= v) {
        x = p;
        split(t[x].r, v, t[x].r, y);
    } else {
        y = p;
        split(t[y].l, v, x, t[y].l);
    }
    push_up(p);
}
int merge(int x, int y) {
    if(!x || !y) return x + y;
    if(t[x].key < t[y].key) {
        t[x].r = merge(t[x].r, y);
        push_up(x);
        return x;
    } else {
        t[y].l = merge(x, t[y].l);
        push_up(y);
        return y;
    }
}
void insert(int v) {
    int x, y, z;
    split(root, v, x, y);
    z = newnode(v);
    root = merge(merge(x, z), y);
}
void del(int v) {
    int x, y, z;
    split(root, v, x, z);
    split(x, v-1, x, y);
    y = merge(t[y].l, t[y].r);
    root = merge(merge(x, y), z);
}
int get_k(int p, int k) { // 返回第k个结点
    if(k <= t[t[p].l].size) {
        return get_k(t[p].l, k);
    }
    if(k == t[t[p].l].size + 1) {
        return p;
    }
    return get_k(t[p].r, k - t[t[p].l].size - 1);
}
void get_pre(int v) {
    int x, y;
    split(root, v-1, x, y);
    int p = get_k(x, t[x].size);
    cout << t[p].val << endl;
    root = merge(x, y);
}
void get_suc(int v) {
    int x, y;
    split(root, v, x, y);
    int p = get_k(y, 1);
    cout << t[p].val << endl;
    root = merge(x, y);
}
void get_rank(int v) {
    int x, y;
    split(root, v-1, x, y);
    cout << t[x].size+1 << endl;
    root = merge(x, y);
}
void get_val(int k) {
    int p = get_k(root, k);
    cout << t[p].val << endl;
}
int main() {
	scanf("%d", &n);
    while(n--) {
        int op, x;
        scanf("%d %d", &op, &x);
        if(op == 1) insert(x);
        if(op == 2) del(x);
        if(op == 3) get_rank(x);
        if(op == 4) get_val(x);
        if(op == 5) get_pre(x);
        if(op == 6) get_suc(x);
    }
	return 0;
} 
```

## 拓展：可持久化平衡树 (动态开点)

https://www.luogu.com.cn/problem/P3835

```c++
#include <bits/stdc++.h>
using namespace std;
const int N = 500005;
struct Node {
	int l, r;
	int val;
	int rnd;
	int size;
} t[N*50]; 
int root[N], idx;
void newnode(int& x, int v) {
	x = ++idx;
	t[idx].val = v;
	t[idx].rnd = rand();
	t[idx].size = 1;
}
void push_up(int p) {
	t[p].size = t[t[p].l].size + t[t[p].r].size + 1;
}
void split(int p, int v, int& x, int& y) {
	if(!p) {
		x = y = 0;
		return;
	}
	if(t[p].val <= v) {
		x = ++idx;
		t[x] = t[p];
		split(t[x].r, v, t[x].r, y);
		push_up(x);
	} else {
		y = ++idx;
		t[y] = t[p];
		split(t[y].l, v, x, t[y].l);
		push_up(y);
	}
}
int merge(int x, int y) {
	if(!x || !y) return x + y;
	if(t[x].rnd < t[y].rnd) {
		t[x].r = merge(t[x].r, y);
		push_up(x);
		return x;
	} else {
		t[y].l = merge(x, t[y].l);
		push_up(y);
		return y;
	}
}
void insert(int& root, int v) {
	int x, y, z;
	split(root, v, x, y);
	newnode(z, v);
	root = merge(merge(x, z), y);
}
void del(int& root, int v) {
	int x, y, z;
	split(root, v, x, z);
	split(x, v-1, x, y);
	y = merge(t[y].l, t[y].r);
	root = merge(merge(x, y), z);
}
int get_rank(int& root, int v) {
	int x, y;
	split(root, v-1, x, y);
	int ans = t[x].size + 1;
	root = merge(x, y);
	return ans;
}
int get_val(int root, int k) {
	if(k == t[t[root].l].size + 1) return t[root].val;
	else if(k <= t[t[root].l].size) {
		return get_val(t[root].l, k);
	} else {
		return get_val(t[root].r, k - t[t[root].l].size - 1);
	}
}
int get_pre(int &root, int v){
	int x, y, s, ans;
	split(root, v-1, x, y);
	if(!x) return -2147483647;
	s = t[x].size;
	ans = get_val(x, s);
	root = merge(x, y);
	return ans;
}
int get_suc(int& root, int v) {
	int x, y, ans;
	split(root, v, x, y);
	if(!y) return 2147483647;
	ans = get_val(y, 1);
	root = merge(x, y);
	return ans;
}
int main() {
  	int n, ver, op, v;
  	scanf("%d", &n);
  	for(int i = 1; i <= n; ++i) {
    	scanf("%d %d %d", &ver, &op, &v);
    	root[i] = root[ver];
    	if(op == 1) insert(root[i], v);
    	else if(op == 2) del(root[i], v);
    	else if(op == 3) printf("%d\n", get_rank(root[i], v));
    	else if(op == 4) printf("%d\n", get_val(root[i], v));
    	else if(op == 5) printf("%d\n", get_pre(root[i], v));
    	else printf("%d\n", get_suc(root[i], v));
  	}
  	return 0;
}
```

