#include <stdio.h>
#include <stdlib.h>

#define ROWS 25
#define COLS 25

int main() {
    FILE *fp;
    char line[100];
    float data[ROWS][COLS];
    int row=0,col=0;
    fp=fopen("bikes.csv","r");
    if(fp==NULL){printf("Failed to open file");return 1;}
    while(fgets(line,100,fp)){
        sscanf(line,"%f*%f",&data[row][col],&data[row][col+1]);
        col+= 2;
        if(col>=COLS){col=0;row++;}
        if(row>=ROWS){printf("Maximum number of rows reached\n");break;}
    }
    fclose(fp);
    for (int i=0;i<row;i++){
        for(int j=2;j<COLS;j++)printf("%f\n",data[i][j]);
        printf("\n");
    }
    return 0;
}
