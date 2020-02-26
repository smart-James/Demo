#include<bits/stdc++.h>
using namespace std;
int main()
{
    int a[2][3][3],ans[3][3]={0}; 
    
    for(int n=0;n<2;n++){
    	for(int i=0;i<3;i++)
    		for(int j=0;j<3;j++)
    			cin>>a[n][i][j];
	}
    
	for(int i=0;i<3;i++){
    		for(int j=0;j<3;j++){
    				for(int t=0;t<3;t++)		
    					ans[i][j]+=a[0][i][t]*a[1][t][j];
    				
				cout<<ans[i][j]<<' ';		
    		}
			cout<<endl;
	}
    return 0;
}


