#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h" 
#include "windows.h"

#define INF 65535


LARGE_INTEGER nFreq;//cpu frequency  
LARGE_INTEGER t1[63];//begin	//time1: t[1...60]  time2: t[61],t[62]
LARGE_INTEGER t2[63];//end
double dt[8]; //time

int arr[5] = {12,24,36,48,60};
int count;

FILE *fp;

enum color
{
	RED,
	BLACK
};


typedef struct
{
	int key;
	int size;
	int color;
}TreeData;

typedef struct OSTNode
{
	TreeData data;
	struct OSTNode *parent;
	struct OSTNode *left;
	struct OSTNode *right; 
}OSTNode,*OSTree;

const OSTree NIL = new OSTNode;


void Random_Num(void)
{
	int i,j,m = 62;
	int num[m];
	srand((unsigned)time(NULL));
	for(i = 0;i < m;i++)
	{
		num[i] = rand()%1000;
		for(j = 0;j < i;j++)
		{
			if(num[j] == num[i])� 
			{
				num[i] = rand()%20;
				j = 0;
			}
		}
	}
		FILE *fp;
		if((fp = fopen("../input/input.txt","w"))== NULL)
		{
			printf("error");
		}
		else
		{
			for(i = 0;i < m;i++)
			{
				fprintf(fp,"%d\n",num[i]);
			}
			printf("successful!\n");
			fclose(fp);
		}
}//随机生成m个正整数 


OSTree TREE_MINIMUN(OSTree T)
{
	while(T->left != NIL)
		T = T->left;
	return T;
}

void LEFT_ROTATE(OSTree &T,OSTree x)
{
	OSTree y;
	y = x->right;
	x->right = y->left;
	if(x->left != NIL)
		y->left->parent = x;
	y->parent = x->parent;
	if(x->parent == NIL)
		T = y;
	else if(x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;
	y->left = x;
	x->parent = y;

	y->data.size = x->data.size;
	x->data.size = x->left->data.size + x->right->data.size + 1;	
}

void RIGHT_ROTATE(OSTree &T,OSTree x)
{
	OSTree y;
	y = x->left;
	x->left = y->right;
	if(y->right != NIL)
		y->right->parent = x;
	y->parent = x->parent;
	if(x->parent == NIL)
		T = y;
	else if(x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;
	y->right = x;
	x->parent = y;
	
	y->data.size = x->data.size;
	x->data.size = x->left->data.size + x->right->data.size + 1;
}

void OS_INSERT_FIXUP(OSTree &T,OSTree z)
{
	OSTree y = new OSTNode;
	while(z->parent->data.color == RED)
	{
		if(z->parent == z->parent->parent->left)
		{
			y = z->parent->parent->right;
			if(y->data.color == RED)
			{
				z->parent->data.color = BLACK;//case1
				y->data.color = BLACK;//case1
				z->parent->parent->data.color = RED;//case1
				z = z->parent->parent;//case1
				if(z->parent == NIL)
				{
					break;
				}
			}			
			else if(z == z->parent->right)
			{
				z = z->parent;//case2
				LEFT_ROTATE(T,z);//case2
			}
			else
			{
			z->parent->data.color = BLACK;//case3
			z->parent->parent->data.color = RED;//case3
			if(z->parent->parent == NIL)
			{
				break;
			}
			RIGHT_ROTATE(T,z->parent->parent);//case3
		}
		}
		else
		{
			y = z->parent->parent->left;
			if(y->data.color == RED)
			{
				z->parent->data.color = BLACK;
				y->data.color = BLACK;
				z->parent->parent->data.color = RED;
				z = z->parent->parent;
				if(z->parent == NIL)
				{
					break;
				}
			}
			else if(z == z->parent->left)
			{
				z = z->parent;
				RIGHT_ROTATE(T,z);
			}
			else
			{
			z->parent->data.color = BLACK;
			z->parent->parent->data.color = RED;
			if(z->parent->parent == NIL)
			{
				break;
			}
			LEFT_ROTATE(T,z->parent->parent);
		}
		}

	}
	T->data.color = BLACK;
}

void OS_INSERT(OSTree &T,TreeData e)
{
	OSTree z = new OSTNode;
	z->data = e; 
	OSTree y = NIL;
	OSTree x = T;
	while(x != NIL)
	{
		y = x;
		if(z->data.key < x->data.key)
		{
			++x->data.size;
			x = x->left;
		}
		else
		{
			++x->data.size;
			x = x->right;
		}
	}
	z->parent = y;
	if(y == NIL)
		T = z;
	else if(z->data.key < y->data.key)
		y->left = z;
	else
		y->right = z;
	z->left = NIL;
	z->right = NIL;
	z->data.color = RED;

	z->data.size = 1;

	OS_INSERT_FIXUP(T,z);
}


void OS_TRANSPLANT(OSTree &T,OSTree u,OSTree v)
{
	if(u->parent == NIL)
		T = v;
	else if(u == u->parent->left)
		u->parent->left = v;
	else
		u->parent->right = v;
	v->parent = u->parent;
}

void OS_DELETE_FIXUP(OSTree &T,OSTree x)
{
	OSTree w = new OSTNode;
	while(x != T && x->data.color == BLACK)
	{
		if(x == x->parent->left)
		{
			w = x->parent->right;
			if(w->data.color == RED)
			{
				w->data.color = BLACK;//case1
				x->parent->data.color = RED;//case1
				LEFT_ROTATE(T,x->parent);//case1
				w = x->parent->right;//case1
			}
			if(w->left->data.color == BLACK && w->right->data.color == BLACK)
			{
				w->data.color = RED;//case2
				x = x->parent;//case2
				if (x == T)
				{
					break;
				}
			}
			else if(w->right->data.color == BLACK)
			{
				w->left->data.color = BLACK;//case3
				w->data.color = RED;//case3
				RIGHT_ROTATE(T,w);//case3
				w = x->parent->right;//case3
			}
			else
			{
				w->data.color = x->parent->data.color;//case4
				x->parent->data.color = BLACK;//case4
				w->right->data.color = BLACK;//case4
				LEFT_ROTATE(T,x->parent);//case4
				x = T;
			}
		}
		else
		{
			w = x->parent->left;
			if(w->data.color == RED)
			{
				w->data.color = BLACK;
				x->parent->data.color = RED;
				RIGHT_ROTATE(T,x->parent);
				w = x->parent->left;
			}
			if(w->right->data.color == BLACK && w->right->data.color == BLACK)
			{
				w->data.color = RED;
				x = x->parent;
				if (x == T)
				{
					break;
				}
			}
			else if(w->left->data.color == BLACK)
			{
				w->right->data.color = BLACK;
				w->data.color = RED;
				LEFT_ROTATE(T,w);
				w = x->parent->left;
			}
			else
			{
				w->data.color = x->parent->data.color;
				x->parent->data.color = BLACK;
				w->left->data.color = BLACK;
				RIGHT_ROTATE(T,x->parent);
				x = T;
			}
		}
	}
	x->data.color = BLACK;
}

void OS_DELETE(OSTree &T,OSTree z)
{
	OSTree x,y,k;
	y = z;
	TreeData *y0 = new TreeData;
	y0->color = y->data.color;
	if(z->left == NIL)
	{
		x = z->right;
		OS_TRANSPLANT(T,z,z->right);
	}
	else if(z->right == NIL)
	{
		x = z->left;
		OS_TRANSPLANT(T,z,z->left);
	}
	else
	{
		y = TREE_MINIMUN(z->right);
		y0->color = y->data.color;
		x = y->right;
		if(y->parent == z)
			x->parent = y;
		else
		{
			OS_TRANSPLANT(T,y,y->right);
			y->right = z->right;
			y->right->parent = y;
		}
		OS_TRANSPLANT(T,z,y);
		y->left = z->left;
		y->left->parent = y;
		y->data.color = z->data.color;
	}
	
	k = y;
	while(k != T)
	{		
		--k->parent->data.size; 
		k = k->parent;
	}
	
	if(y0->color == BLACK)
		OS_DELETE_FIXUP(T,x);
}



void CreatOSTree(OSTree &T,int num[],int n)
{
	TreeData e;
	e.color = RED;
	int i;
	for(i = 1;i <= n;i++)
	{
		e.key = num[i-1];
		QueryPerformanceCounter(&t1[i]);
		OS_INSERT(T,e);
		QueryPerformanceCounter(&t2[i]);
	}
}

void InOrderTraverse(OSTree T)
{
	if(T != NIL)
	{
		InOrderTraverse(T->left);
//		printf("( %d,%d,%d )",T->data.key,T->data.color,T->data.size);
		switch (count)
		{
			case 0:fp = fopen("../output/size12/inorder.txt","a");break;
			case 1:fp = fopen("../output/size24/inorder.txt","a");break;
			case 2:fp = fopen("../output/size36/inorder.txt","a");break;
			case 3:fp = fopen("../output/size48/inorder.txt","a");break;
			case 4:fp = fopen("../output/size60/inorder.txt","a");break;	
		}
		fprintf(fp,"( %d,%d,%d )",T->data.key,T->data.color,T->data.size);				
		fclose(fp); 
		InOrderTraverse(T->right);
	}
}

void PreOrderTraverse(OSTree T)
{
	if(T != NIL)
	{
//		printf("( %d,%d,%d )",T->data.key,T->data.color,T->data.size);
		switch (count)
		{
			case 0:fp = fopen("../output/size12/preorder.txt","a");break;
			case 1:fp = fopen("../output/size24/preorder.txt","a");break;
			case 2:fp = fopen("../output/size36/preorder.txt","a");break;
			case 3:fp = fopen("../output/size48/preorder.txt","a");break;
			case 4:fp = fopen("../output/size60/preorder.txt","a");break;	
		}
		fprintf(fp,"( %d,%d,%d )",T->data.key,T->data.color,T->data.size);				
		fclose(fp);
		PreOrderTraverse(T->left);
		PreOrderTraverse(T->right);
	}
}

void PostOrderTraverse(OSTree T)
{
	if(T != NIL)
	{
		PostOrderTraverse(T->left);
		PostOrderTraverse(T->right);
//		printf("( %d,%d,%d )",T->data.key,T->data.color,T->data.size);
		switch (count)
		{
			case 0:fp = fopen("../output/size12/postorder.txt","a");break;
			case 1:fp = fopen("../output/size24/postorder.txt","a");break;
			case 2:fp = fopen("../output/size36/postorder.txt","a");break;
			case 3:fp = fopen("../output/size48/postorder.txt","a");break;
			case 4:fp = fopen("../output/size60/postorder.txt","a");break;	
		}
		fprintf(fp,"( %d,%d,%d )",T->data.key,T->data.color,T->data.size);				
		fclose(fp);
	}
}

OSTree OS_SELECT(OSTree x,int i)
{
	int r = 1;
	if(x->left != NIL)
		r = x->left->data.size + 1;
	if(i == r)
		return x;
	else if(i < r)
		return OS_SELECT(x->left,i);
	else
		return OS_SELECT(x->right,i - r);
}

void swap(int *a,int *b)
{
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}
int RANDOM(int p,int q)
{
    int i,number;
    srand((unsigned)time(NULL));
    number = rand()%(q-p+1)+p;
    return number;
}
int PARTITION(int a[],int p,int r)
{
    int x = a[r];
    int i = p- 1;
    for(int j = p;j <= r-1;j++)
        {
            if(a[j] <= x)
            {
                i = i + 1;
                swap(&a[j],&a[i]);
            }
        }
    swap(&a[i+1],&a[r]);
    return i+1;
}
int RANDOMIZED_PARTITION(int a[],int p,int r)
{
    int i = RANDOM(p,r);
    swap(&a[r],&a[i]);
    return PARTITION(a,p,r);
}
int RANDOMIZED_SELECT(int a[],int p,int r,int i)
{
    if(p == r)
        return a[p];
    int q = RANDOMIZED_PARTITION(a,p,r);
    int k = q - p + 1;
    if(i == k)
        return a[q];
    else if(i < k)
        return RANDOMIZED_SELECT(a,p,q-1,i);
    else 
        return RANDOMIZED_SELECT(a,q+1,r,i-k);
}


int main()
{
//	Random_Num();
	int i,j,m = 62;
	int num[m];
	memset(num,0,sizeof(num));
	QueryPerformanceFrequency(&nFreq);
	for(count = 0;count < 5;count++)
	{
		NIL->left = NIL->right = NIL->parent = NULL;
		NIL->data.key = INF;
		NIL->data.color = BLACK;
		NIL->data.size = 0;
		OSTree T;
		T = NIL;
		T->parent = NIL;
		memset(num,0,sizeof(num));
		memset(t1,0,sizeof(t1));
		memset(t2,0,sizeof(t2));
		
		m = arr[count];
		if((fp = fopen("../input/input.txt","r"))== NULL)
			printf("error\n");
			else	
			{
				for(i = 0;i < m;i++)
				{
					fscanf(fp,"%d",&num[i]);
				}
				printf("read successful!\n");
				fclose(fp);
			}
	int y =	RANDOMIZED_SELECT(num,0,m,m/3);
	CreatOSTree(T,num,m);
	PreOrderTraverse(T);
	InOrderTraverse(T);
	PostOrderTraverse(T);
	OSTree x;
	x = OS_SELECT(T,m/3);
//	printf("%d\n%d",x->data.key,RANDOMIZED_SELECT(num,0,m,m/3));
	if(x->data.key == y)
		printf("first delete correct!\n");
		else
		printf("first delete wrong!\n");
		
	switch (count)
	{
		case 0:fp = fopen("../output/size12/delete_data.txt","a");break;
		case 1:fp = fopen("../output/size24/delete_data.txt","a");break;
		case 2:fp = fopen("../output/size36/delete_data.txt","a");break;
		case 3:fp = fopen("../output/size48/delete_data.txt","a");break;
		case 4:fp = fopen("../output/size60/delete_data.txt","a");break;	
	}
	fprintf(fp,"%d\n",x->data.key);
	fclose(fp);
	
	QueryPerformanceCounter(&t1[61]);
	OS_DELETE(T,x);
	QueryPerformanceCounter(&t2[61]);
	
	x = OS_SELECT(T,(m-1)/4);
	for(i = 0;i < m;i++)
	{
		if(x->data.key == num[i]) 
		{
			num[i] == INF;
			break;
		}
	}
//	printf("%d\n%d",x->data.key,RANDOMIZED_SELECT(num,0,m,(m-1)/4));
	if(x->data.key == RANDOMIZED_SELECT(num,0,m,(m-1)/4))
		printf("second delete correct!\n");
		else
		printf("second delete wrong!\n");
		printf("\n");
	switch (count)
	{
		case 0:fp = fopen("../output/size12/delete_data.txt","a");break;
		case 1:fp = fopen("../output/size24/delete_data.txt","a");break;
		case 2:fp = fopen("../output/size36/delete_data.txt","a");break;
		case 3:fp = fopen("../output/size48/delete_data.txt","a");break;
		case 4:fp = fopen("../output/size60/delete_data.txt","a");break;	
	}
	fprintf(fp,"%d\n",x->data.key);
	fclose(fp);
	
	QueryPerformanceCounter(&t1[62]);
	OS_DELETE(T,x);
	QueryPerformanceCounter(&t2[62]);
	
//	PreOrderTraverse(T);
//	InOrderTraverse(T);
//	PostOrderTraverse(T);

	for(i = 1;i <= m/10 + 1;i++)
	{
		if(i == m/10+1)
		{
			if(m%10 == 0)
				break;
			else
				dt[i] = (t2[m].QuadPart - t1[i*10-9].QuadPart)/ (double)nFreq.QuadPart *1000000;
		}
		else
			dt[i] = (t2[i*10].QuadPart - t1[i*10-9].QuadPart)/ (double)nFreq.QuadPart *1000000;	
	}
	dt[7] = (t2[m].QuadPart - t1[1].QuadPart)/ (double)nFreq.QuadPart *1000000;	
	switch (count)
	{
		case 0:fp = fopen("../output/size12/time2.txt","a");break;
		case 1:fp = fopen("../output/size24/time2.txt","a");break;
		case 2:fp = fopen("../output/size36/time2.txt","a");break;
		case 3:fp = fopen("../output/size48/time2.txt","a");break;
		case 4:fp = fopen("../output/size60/time2.txt","a");break;	
	}
	fprintf(fp,"%f\n",(t2[61].QuadPart - t1[61].QuadPart)/ (double)nFreq.QuadPart *1000000);
	fprintf(fp,"%f\n",(t2[62].QuadPart - t1[62].QuadPart)/ (double)nFreq.QuadPart *1000000);
	fclose(fp);
	
	switch (count)
	{
		case 0:fp = fopen("../output/size12/time1.txt","a");break;
		case 1:fp = fopen("../output/size24/time1.txt","a");break;
		case 2:fp = fopen("../output/size36/time1.txt","a");break;
		case 3:fp = fopen("../output/size48/time1.txt","a");break;
		case 4:fp = fopen("../output/size60/time1.txt","a");break;	
	}
	for(i = 1;i <= m/10 + 1;i++)
	{
		if(m%10 == 0 && i == m/10+1)
			break;
		else
			fprintf(fp,"%f\n",dt[i]);	
	}
		fprintf(fp,"sum:");
		fprintf(fp,"%f",dt[7]);
		
		fclose(fp);
	}
	return 0;
}
