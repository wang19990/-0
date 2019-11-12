#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct customer 
{
	int c_custkey;
	char c_mkgsegment[20];
}cus;
typedef struct ord
{
	int o_orderkey;  
	int o_custkey;
	char c_orderdate[10]; 
}orders;
typedef struct item
{
	int l_orderkey;
	double l_extendedprice;
	char l_shipdate[10];
}lineitem;
typedef struct
{
	int l_orderkey;
	char o_orderdate[10];
	double l_extendedprice;
}result;
cus * read_customer()
{
	FILE *fp;
	cus *p = NULL;
	p = (cus *)malloc(100 * sizeof(cus));
	char ch;
	int i=0;
	fp = fopen("customer.txt", "r");
	if (fp== NULL)
	{
		printf("ERROR\n");
		return NULL;
	}
	while (!feof(fp))
		fscanf(fp, "%d%c%s", &p[i].c_custkey, &ch, p[i++].c_mkgsegment);
	fclose(fp);
	return p;
}
orders * read_order() 
{
	FILE *fp=NULL; 
	orders * a=NULL;
	a = (orders * )malloc(4000*sizeof(orders));
	int i=0;
	char b,c;
	long long d;
	fp = fopen("orders.txt","r");
	if(fp == NULL)
	{
		printf("ERROR\n");
		return NULL;
	}
	while(!feof(fp))
	{	
		fscanf(fp,"%d%c%lld%c%s",&a[i].o_orderkey,&b,&d,&c,a[i].c_orderdate);
		a[i].o_custkey=d%100;
		i++;
	}
	fclose(fp);
	return a;
}
lineitem * read_lineitem()
{
	FILE * fp;
	lineitem * l=NULL;
	l = (lineitem *)malloc(1000*sizeof(lineitem));
	int i=0;
	char b,c;
	fp = fopen("lineitem.txt","r");
	if(fp==NULL)
	{
		printf("ERROR\n");
		return NULL;
	}
	while(!feof(fp))
		fscanf(fp,"%d%c%lf%c%s",&l[i].l_orderkey,&c,&l[i].l_extendedprice,&b,l[i++].l_shipdate);
	fclose(fp);
	return l; 
}
result * operate(cus * cus,orders * ord,lineitem * item,char *c_mkgsegment, char * order_date,char * ship_date)
{
	int i,j,k,l=0,m=0;
	result * r1=NULL;
	result * r2=NULL;
	result temp;
	r1 = (result *)malloc(1000*sizeof(result));
	r2 = (result *)malloc(1000*sizeof(result));	
	for(i=0;i<100;i++)
	{
		if(strcmp(cus[i].c_mkgsegment,c_mkgsegment)==0)
		{
			for(j=0;j<4000;j++)
			{
				for(k=0;k<1000;k++)
				if(cus[i].c_custkey==ord[j].o_custkey&&ord[j].o_orderkey==item[k].l_orderkey&&(strcmp(ord[j].c_orderdate,order_date)<0)&&(strcmp(item[k].l_shipdate,ship_date)>0))
				{
					r1[l].l_orderkey=item[k].l_orderkey;
					strcpy(r1[l].o_orderdate,ord[j].c_orderdate);
					r1[l].l_extendedprice=item[k].l_extendedprice;
					l++;
				}
			}
		}
	}
	r2[0].l_orderkey = r1[0].l_orderkey;
	strcpy(r2[0].o_orderdate,r1[0].o_orderdate);
	r2[0].l_extendedprice = r1[0].l_extendedprice;
	for(i=1;i<l;i++)
	{
		if(r2[m].l_orderkey==r1[i].l_orderkey)
		{
			r2[m].l_extendedprice += r1[i].l_extendedprice;
		}
		else
		{
			m++;
			r2[m].l_orderkey = r1[i].l_orderkey;
			strcpy(r2[m].o_orderdate,r1[i].o_orderdate);
			r2[m].l_extendedprice = r1[i].l_extendedprice;
		}
	}
	for(i=0;i<m-1;i++)
	{
		for(j=0;j<m-i;j++)
		{
			if(r2[j].l_extendedprice<r2[j+1].l_extendedprice)
			{
				temp.l_extendedprice=r2[j].l_extendedprice;
				temp.l_orderkey=r2[j].l_orderkey;
				strcpy(temp.o_orderdate,r2[j].o_orderdate);
				r2[j].l_extendedprice=r2[j+1].l_extendedprice;
				r2[j].l_orderkey=r2[j+1].l_orderkey;
				strcpy(r2[j].o_orderdate,r2[j+1].o_orderdate);
				r2[j+1].l_extendedprice=temp.l_extendedprice;
				r2[j+1].l_orderkey=temp.l_orderkey;
				strcpy(r2[j+1].o_orderdate,temp.o_orderdate);
			}
		}
	}
	return r2;
}
int output(char *a,char *b,char *c,int limit)
{
	cus *cust=NULL;
	orders *ord=NULL;
	lineitem *item=NULL;
	cust=read_customer();
	ord=read_order();
	item=read_lineitem();
	result *r=NULL;
	r=operate(cust,ord,item,a,b,c);
	printf("l_orderkey|o_orderdate|revenue\n");
	int f=0;
	for(int i=0;i<limit;i++)
	{
		if(r[i].l_orderkey>0)
		{
			printf("%-10d|%-11s|%-20.2lf\n",r[i].l_orderkey,r[i].o_orderdate,r[i].l_extendedprice);
			f++;
		}
	}
	if(f==0)
		printf("Ã»ÓÐ¼ÇÂ¼\n");
	return 0;
 } 
int main(int argc,char **argv)
{
	int i,fun;
	unsigned int n=atoi(argv[4]);
	for(i=1;i<=n;i++)
	{
		unsigned int limit=atoi(argv[4*i+4]);
		fun=output(argv[4*i+1],argv[4*i+2],argv[4*i+3],limit);
	}
	return 0;
}
