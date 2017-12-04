#include<stdio.h>  
#include<stdlib.h>  
#include<string.h>  
#include<iostream>
using namespace std;
typedef struct Node  
{  
    unsigned char ch; 
    int times;   
    struct Node *left; 
    struct Node *right;  
};  
typedef struct  
{  
    unsigned char ch;  
    char code[100];    
    int times;   
    int codelen;   
}Huffman;  
struct Node *root;   
int count(char filename[],Huffman *node)  
{  
    FILE *fp = fopen(filename,"r");    
    int num = 0;  
    unsigned char temp = fgetc(fp);   
    while(!feof(fp)) {  
        int i;  
        for (i = 0; i < num; i++)  {  
            if (temp == node[i].ch)  {  
                node[i].times++;  
                break;  
            }   
        }   
        if (i==num) {  
            node[i].ch = temp;  
            node[i].times=1;  
            num++;  
        }  
        temp = fgetc(fp); 
    }  
    fclose(fp);  
    return num;  
}  
struct Node* buildtree(Huffman *node, int n)  
{  
    struct Node **pp,*temp; 
    pp = (struct Node**)malloc(n*sizeof(struct Node));   
	for (int i = 0; i < n; i++)  {  
        pp[i] = (struct Node*)malloc(sizeof(struct Node));  
        pp[i]->ch = node[i].ch;  
        pp[i]->times = node[i].times;  
        pp[i]->left = pp[i]->right = NULL;  
    }  
    for (int i = 0; i < n-1; i++) {  
        int m1=-1, m2;  
        for (int j = 0; j < n; j++)   {  
            if (m1 == -1 && pp[j] != NULL) {  
                m1 = j;  continue;  
            }  
            if (pp[j] != NULL) {  
            	m2 = j;   break;  
            }  
        }  
        for (int j = m2; j < n; j++)  {  
            if (pp[j]!= NULL && pp[j]->times < pp[m1]->times) {  
                m2 = m1;  m1 = j;  
            }  
            else if (pp[j] != NULL && pp[j]->times < pp[m2]->times)  m2 = j;  
        }  
        temp = (struct Node*)malloc(sizeof(struct Node));  
        temp->left = pp[m1];  temp->right = pp[m2];  
        temp->times = pp[m1]->times + pp[m2]->times;  temp->ch = NULL;  
        pp[m1] = temp;  pp[m2] = NULL;  
    }  
    free(pp);  
    return temp; 
}  
void codeHuffman(struct Node *node,int len,Huffman *pp,int num)   
{  
    static char code[100];  
 	if (node->left != NULL)  {  
        code[len] = '0';  code[len + 1] = '\0';  
        codeHuffman(node->left, len+1,pp,num);
    }  
    if (node->right != NULL)  {  
        code[len] = '1';   code[len + 1] = '\0';  
        codeHuffman(node->right, len + 1, pp, num); 
    }  
    else {  
        for (int i = 0; i < num; i++)  {  
            if (pp[i].ch == node->ch)  {  
                strcpy(pp[i].code, code);  
                pp[i].codelen = 0;  
                for (int j = 0; pp[i].code[j] != '\0'; j++)  pp[i].codelen++;  
               return;  
            }  
        }  
    }  
}  
void output1(char sourceFile[],Huffman *pp,int n,char targetFile[])  
{  
    FILE *fp1, *fp2; 
    unsigned char temp, chh = 0;  
    int len = 0,total = 0, e;    
    for (int i = 0; i < n; i++)  total = total + pp[i].codelen*pp[i].times;  
    e = total % 8;    
    fp1 = fopen(sourceFile, "r");  
    fp2 = fopen(targetFile, "w");   
    fputc(n,fp2);  fputc(e,fp2);   
    for (int i = 0; i < n; i++) {  
    	fputc(pp[i].ch, fp2);  
        fputc(pp[i].times,fp2);  
    }  
	temp = fgetc(fp1);       
    while (!feof(fp1)) {  
        for (int i = 0; 1; i++) {  
            if (temp == pp[i].ch) {  
               for(int j = 0;j < pp[i].codelen;j++)  {   
                    if(len!=0)  chh = chh<<1;    
                    chh = chh|(pp[i].code[j]-'0');  
                    len++;  
                    if(len == 8) {  
                        fputc(chh,fp2);  
                        chh = 0;  len = 0;  
                    }  
               }  
                break;  
            }  
        }  
        temp = fgetc(fp1);  
    }  
    if(len!=0)  
        fputc(chh,fp2);  
    fclose(fp1);  fclose(fp2);  
}  
void output2(char sourceFile[], Huffman *cc, char targetFile[])  
{  
    FILE *fp1, *fp2;   
    int n, e = 0;   
    fp1 = fopen(sourceFile, "r");  
    n = fgetc(fp1); e = fgetc(fp1);   
    for (int i = 0; i < n; i++) {  
        cc[i].ch = fgetc(fp1);  
        cc[i].times = fgetc(fp1); 
    }   
    struct Node *root = buildtree(cc, n); 
    codeHuffman(root, 0, cc, n);  
    struct Node *pp = root;   
    unsigned char temp; 
    fp2 = fopen(targetFile, "w");   
    temp = fgetc(fp1);  
    while (!feof(fp1)) {  
        unsigned char flag = fgetc(fp1);  
        int i;  
       if (feof(fp1))   i = e-1;  
        else   i = 7;  
        for (; i >= 0; i--) {  
          if ((temp>>i & 1)==0)  
                pp = pp->left;  
            else  
                pp = pp->right;  
            if (pp->left == NULL && pp->right == NULL) {  
                fputc(pp->ch,fp2);  
                pp = root;  
            }  
        }  
       temp = flag;  
    }  
    fclose(fp1);  fclose(fp2);  
}   
int main()  
{  
    while (1)  
    {  
        Huffman node[2000];  
        int num, a;  
        char s1[100] = ""; 
        char s2[100] = "";  
		cout<<endl;   
        cout<<"1.Huffman compress."<<endl;  
    	cout<<"2.Huffman decompress."<<endl;  
        cout<<"3.Exit."<<endl;  
        cout<<"Please select:";
        cin>>a; 
        fflush(stdin);  
        if (a == 1)  
        {  
            cout<<"Please input source file name(size less than 4GB):";
            scanf("%s", s1);  
            fflush(stdin);  
            FILE *fp = fopen(s1, "r");  
            if (fp == NULL)   exit(1);
            else   fclose(fp);     
            cout<<"Please input code file name:";
            scanf("%s", s2);  
            fflush(stdin);  
            num = count(s1, node); 
            root = buildtree(node, num);            
            codeHuffman(root, 0, node, num);
            cout<<"Processing..."<<endl;   
            output1(s1, node, num, s2);  
            cout<<"Process end."<<endl;  
            cout<<endl;
        }  
        else if (a == 2)  
        {       
            cout<<"Please input code file name:";
            scanf("%s", s1);  
            fflush(stdin);  
            FILE *fp = fopen(s1, "r");  
            if (fp == NULL)  exit(1); 
            else  fclose(fp);  
           	cout<<"Please input target file name:"; 
            scanf("%s", s2);  
            fflush(stdin);  
            cout<<"Processing..."<<endl;   
            output2(s1, node, s2); 
            cout<<"Process end."<<endl;
			cout<<endl;  
        }  
        else if (a == 3)  return 0;  
        else  continue;  
    }
	return 0;  
}  

