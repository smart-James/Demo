#include<bits/stdc++.h>
using namespace std;

bool flag[8]={1,1,1,1,1,1,1,1};
bool d1[15]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
bool d2[15]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

int main(){
	
	int x=0;
	char c;
	for(int i=0;i<8;i++){
		
		c=getchar();
		if(c!='*'){
			int n=c-'1';
			flag[n]=false;
			d1[i-n+7]=false;
			d2[n+i]=false;
			}else x=i;
		}
		
		for(int i=0;i<8;i++)
			if(flag[i]&&d1[x-i+7]&&d2[x+i]){
				cout<<(i+1);
				return 0;
				}
	cout<<"No Answer";

	return 0;
}







