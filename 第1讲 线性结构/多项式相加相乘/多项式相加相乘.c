#include <stdio.h>
#include <stdlib.h>
typedef struct PolyNode *Polynomial;
struct PolyNode {
	int coef;
	int expon;
	Polynomial link;

};

void Attach(int c, int e, Polynomial* pRear);
Polynomial ReadPoly();
Polynomial Add(Polynomial P1, Polynomial P2);
int Compare(int e1, int e2);
Polynomial Mult(Polynomial P1, Polynomial P2);
void  PrintPoly(Polynomial P);

void Attach(int c, int e, Polynomial *pRear) {
	Polynomial P;
	P = (Polynomial)malloc(sizeof(struct PolyNode));
	P->coef = c; //对新节点进行赋值
	P->expon = e;
	P->link = NULL;

	(*pRear)->link = P;
	*pRear = P;
}

Polynomial ReadPoly() {
	Polynomial Rear;//用Rear指针指向当前结果表达式的尾项
	Polynomial P, t;
	int c, e, N;
	scanf_s("%d", &N);
	P = (Polynomial)malloc(sizeof(struct PolyNode));
	P->link = NULL;
	Rear = P;
	while (N--) {
		scanf_s("%d %d", &c, &e);
		Attach(c, e, &Rear);
	}
	//删除临时生成的头节点
	t = P; 
	P = P->link;
	free(t);
	return P;
}

//多项式相加函数
Polynomial Add(Polynomial P1, Polynomial P2) {
	Polynomial front, rear, temp;
	int sum;
	rear = (Polynomial)malloc(sizeof(struct PolyNode));
	front = rear;
	while (P1 && P2) {
		switch (Compare(P1->expon, P2->expon)) { //Compare实现对两个项系数的比较，第一个大返回1，第二个大返回-1，相等返回0
		case 1:
			Attach(P1->coef, P1->expon, &rear);
			P1 = P1->link;
			break;
		case -1:
			Attach(P2->coef, P2->expon, &rear);
			P2 = P2->link;
			break;
		case 0:
			sum = P1->coef + P2->coef;
			if (sum) { //sum不为0的情况下
				Attach(sum, P1->expon, &rear);
			}
			P1 = P1->link;
			P2 = P2->link;
			break;
		}
	}

	//将为处理完的另一个多项式的所有结点一次复制到结果多项式去
	while (P1) {
		Attach(P1->coef, P1->expon, &rear);
		P1 = P1->link;
	}
	while (P2) {
		Attach(P2->coef, P2->expon, &rear);
		P2 = P2->link;
	}

	rear->link = NULL;
	//删除临时空头节点
	temp = front;
	front = front->link;
	free(temp);
	//返回头节点
	return front;
}

int Compare(int e1, int e2) {
	int flag;
	if (e1 > e2) {
		flag = 1;
	}
	else if (e1 < e2) {
		flag = -1;
	}
	else {
		flag = 0;
	}
	return flag;
}

Polynomial Mult(Polynomial P1,Polynomial P2) {
	Polynomial Rear, P, t1, t2, t;
	int c, e;
	if (!P1 || !P2) {
		return NULL;
	}
	t1 = P1; t2 = P2;
	P = (Polynomial)malloc(sizeof(struct PolyNode));
	Rear = P;
	while (t2) {
		Attach(t1->coef * t2->coef, t1->expon + t2->expon, &Rear);
		t2 = t2->link;
	}

	t1 = t1->link;
	while (t1) {
		t2 = P2;
		Rear = P;
		while (t2) {
			e = t1->expon + t2->expon;
			c = t1->coef * t2->coef;
			while (Rear->link && Rear->link->expon > e) {
				Rear = Rear->link;
				if (Rear->link && Rear->link->expon == e) {
					if(Rear->link->coef + c) {
						Rear->link->coef += c;
					}
					else {
						//删除
						t = Rear->link;
						Rear->link = t->link;
						free(t);
					}
				}
				else {
					t = (Polynomial)malloc(sizeof(struct PolyNode));
					t->coef = c;
					t->expon = e;
					t->link = Rear->link;
					Rear->link = t;
					Rear = Rear->link;
				}
			}
		}
		t1 = t1->link;
	}

	t2 = P;
	P = P->link;
	free(t2);

	return P;
}

void  PrintPoly(Polynomial P) {
	int flag = 0;
	if (!P) {
		printf("0 0\n");
		return;
	}
	while (P) {
		if (flag)
			flag = 1;
		else 
			printf(" ");
		printf("%d %d", P->coef, P->expon);
		P = P->link;
	}
	printf("\n");
}

int main() {
	Polynomial P1, P2, PP, PS;
	//读入多项式
	P1 = ReadPoly();
	P2 = ReadPoly();
	//多项式相乘
	PP = Mult(P1, P2);
	//输出多项式
	PrintPoly(PP);
	//多项式相加
	PS = Add(P1, P2);
	//输出多项式
	PrintPoly(PS);
	return 0;
}
