#include <stdio.h>
#include <stdlib.h>

#define n 25
#define m 25
int main() {
    FILE *fp;
    char line[100];
    float pareto[100];
    float data[n][m];
    int pareto_counter = 0;
    int row=0,col=0;
    fp=fopen("bikes.csv","r");
    if(fp==NULL){printf("Failed to open file");return 1;}
    while (fgets(line,100,fp)){sscanf(line, "%f*%f", &data[row][col], &data[row][col+1]);row++;}
    fclose(fp);
    int nPareto=0;
    for(int i=1;i<row-1;i++){
        int dominant=0;
        for(int j=0;j<row-1;j++){
            if((data[j][0]<=data[i][0]&&data[j][1]<=data[i][1])&&(data[j][0]<data[i][0]||data[j][1]<data[i][1])){
                dominant=1;
                break;
            }
        }
    }
    printf("PARETO VALUES\n");
    for(int i=0;i<pareto_counter;i++)printf("Pareto values:%.2f\n\n",pareto[i]);
    return 0;
}
