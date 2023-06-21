//将一系列给定数字插入一个初始为空的最小堆H[]。随后对
//任意给定的下标i，打印从H[]到根结点的路径
//输入样例：
//5 3
//46 23 26 24 10
//5 4 3
//输出样例:
//24 23 10
//46 23 10
//26 10 

#include <stdio.h>
#define MAXH 1001
#define MINH -10001
int H[MAXH], size;

void Create() {
	size = 0;
	H[0] = MINH; //哨兵
	 
}

void Insert(int X) {
	int i;
	for(i = ++size; H[i/2] > X; i /= 2) {
		H[i] = H[i/2];
	}
	H[i] = X;
}


int main() {
	int n, m, x, i, j;
	scanf("%d %d", &n, &m);
	Create();
	for(i = 0; i < n; i++) {
		scanf("%d", &x);
		Insert(x);
	}
	for(i = 0; i < m; i++) {
		scanf("%d", &j);
		printf("%d ", H[j]);
		while(j > 1) {
			j /= 2;
			printf("%d ", H[j]);
		}
		printf("\n");
	}
	return 0;
} 
