#include "header.h"


void main1(){
	uint64 a=1,c,b,n,d=0,y;
	n=30;
	do{
		a+=2;
		c=0;
		for(b=1;b<a/2;b+=2){
			if(a%b==0){
				c++;
			}

		}
		if(c==1){
			d++;
			y=a;
			kprintnum(0,15,d);
			kprintnum(0,16,a);
		}
	}while(d != n);
	printf("O primo %d eh %d",n,y);
}
