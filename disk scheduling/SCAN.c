#include<stdio.h>
#include<stdlib.h>
void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}
void bubblesort(int *arr, int size){
    for(int i=0;i<size-1;i++){
        for(int j=0;j<size-i-1;j++){
            if(arr[j]>arr[j+1]){
                swap(arr+j, arr+j+1);
            }
        }
    }
}
void SCAN(int *req, int reqsize, int tracks, int start){
    bubblesort(req, reqsize);
    int i=0;
    for(i=0;i<reqsize;i++) if(req[i]>start)break;
    int j=i-1;
    int sum = req[i]-start;
    int seq[100];
    seq[0]=req[i];
    int k=1;
    printf("request | seektime\n");
    printf("%d      | %d\n",req[i],sum);
    for(i=i+1;i<reqsize;i++){
        seq[k++] = req[i];
        sum+=abs(seq[k-1]-seq[k-2]);
        printf("%d     | %d\n", req[i], sum);
    }
    seq[k++] = tracks-1;
    sum+=abs(seq[k-1]-seq[k-2]);
    for(j;j>=0;j--){
        seq[k++]=req[j];
        sum+=abs(seq[k-1]-seq[k-2]);
        printf("%d     | %d\n", req[j], sum);
    }
    printf("\nTotal seek time :- %d", sum);
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
    printf("Enter total number of tracks : ");
    scanf("%d", &tracks);
    printf("Enter starting track : ");
    scanf("%d", &start);
    SCAN(req, reqsize, tracks, start);
}