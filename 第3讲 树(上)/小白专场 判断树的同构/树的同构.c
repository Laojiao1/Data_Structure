//架构
//int main() {
//	建立二叉树1;
//	建立二叉树2;
//	判断是否同构并输出;
//	
//	return 0;
//}

#include <stdio.h>
#define MaxTree 10
#define ElementType char
#define Tree int
#define Null -1

struct TreeNode {
	ElementType Element;
	Tree Left;
	Tree Right;

}T1[MaxTree], T2[MaxTree];

Tree BuildTree(struct TreeNode T[]) {
	int N;
	int i;
	int Root;
	ElementType cl, cr;
	scanf("%d", &N); //输入树的节点个数
	if (N) {
		int Check[8];
		for (i = 0; i < 8; i++)
			Check[i] = 0;
		for (i = 0; i < 8; i++) {
			scanf("%c %c %c", &T->Element, &cl, &cr);
			//对cl(左子树)进行处理
			if (cl != '-') {
				T[i].Left = cl - '0';
				Check[T[i].Left] = 1;
			}
			else {
				T[i].Left = Null;
			}
			//对cr(右子树)进行处理
			if (cr != '-') {
				T[i].Right = T[i].Right - '0';
				Check[T[i].Right] = 1;
			}
			else {
				Check[T[i].Right] = Null;
			}
		}
		for (i = 0; i < N; i++) {	
			if (!Check[i]) break;
		}
		Root = i;
	}
	return Root;
}

int Isomophic(Tree R1, Tree R2) {
	if ((R1 == Null) && (R2 == Null))
		return 1;
	if((R1 == Null)&&(R2 != Null) || (R1 != Null) && (R2 == Null))
		return 0;
	if(T1[R1].Element != T2[R2].Element)
		return 0;
	if ((T1[R1].Left == Null) && (T2[R2].Left == Null))
		return Isomophic(T1[R1].Right, T2[R2].Right);
	if (((T1[R1].Left != Null) && (T2[R2].Left != Null)) && ((T1[T1[R1].Left].Element) == (T2[T2[R2].Left].Element)))
		return (Isomophic(T1[R1].Left, T2[R2].Left) && Isomophic(T1[R1].Right, T2[R2].Right));
	else
		return (Isomophic(T1[R1].Left, T2[R2].Right) && Isomophic(T1[R1].Right, T2[R2].Left));
}

int main() {
	Tree R1, R2;
	R1 = BuildTree(T1);
	R2 = BuildTree(T2);
	if (Isomophic(R1, R2)) {
		printf("Yes!");
	}
	else {
		printf("No!");
	}
	return 0;
}
