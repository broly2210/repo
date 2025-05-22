#include<stdio.h>
#include<stdlib.h>
void SSTF(int *req, int reqsize, int curr){
    int seq[100], k=0;
    int visited[1000]={0}, sum = 0, completed=0;
    printf("request | seektime\n");
    while(completed<reqsize){
        int min = 9999, index=-1;
        for(int i=0;i<reqsize;i++){
            if(visited[req[i]]==0 && abs(req[i]-curr)<min){
                min = abs(req[i]-curr);
                index = i;
            }
        }
        seq[k++]=req[index];
        visited[req[index]]=1;
        if(sum==0) sum+=abs(seq[k-1] - curr);
        else sum+=abs(seq[k-1]-seq[k-2]);
        curr=req[index];
        printf("%d     | %d\n", req[index],sum);
        completed++;
    }
}
int main(){
    int reqsize;
    printf("Enter total number of requests : ");
    scanf("%d", &reqsize);
    int req[50];
    for(int i=0;i<reqsize;i++){
        scanf("%d", &req[i]);
    }
    int tracks, start;
    printf("Enter starting track : ");
    scanf("%d", &start);
    SSTF(req, reqsize, start);
}