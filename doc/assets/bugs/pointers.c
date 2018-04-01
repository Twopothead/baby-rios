#include <stdio.h>
int main()
{
	int a[100]={0};
	for(int i=0;i<100;i++)a[i]=i;
	int *p=(int *)&a;
	int *q=(int *)&a;
	printf("X  &p:%d,&q:%d\n",(int)&p,(int)&q);
	printf("OK p:%d,q:%d\n",(int)p,(int)q);
	printf("p[13]%d,q[13]:%d",p[13],q[13]);
	return 0;
}
