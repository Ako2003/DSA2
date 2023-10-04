#include <stdio.h>
#include <stdlib.h>

typedef struct Blast{
  int nPossition;
  int nScore;
  struct Blast *pNext;
}SBlast;

int Up(SBlast **ppBlast);
int Down(SBlast **ppBlast);
int maxScore(SBlast **ppBlast);
int minScore(SBlast **ppBlast);
int bestScore(SBlast **ppBlast);
int localPosition(SBlast **ppBlast);

int Up(SBlast **ppBlast){
  SBlast *pTemp=*ppBlast;
  if(*ppBlast==NULL){
    *ppBlast=(SBlast*)malloc(sizeof(SBlast));
    (*ppBlast)->nScore=1;
    (*ppBlast)->nPossition=1;
    (*ppBlast)->pNext=NULL;
    return 0;
  }
  while(pTemp->pNext)pTemp=pTemp->pNext;
  pTemp->pNext=(SBlast*)malloc(sizeof(SBlast));
  pTemp->pNext->nScore=pTemp->nScore+1;
  pTemp->pNext->nPossition=pTemp->nPossition+1;
  pTemp->pNext->pNext=NULL;
  return 0;
}

int Down(SBlast **ppBlast){
  SBlast *pTemp=*ppBlast;
  if(*ppBlast==NULL){
    *ppBlast=(SBlast*)malloc(sizeof(SBlast));
    (*ppBlast)->nScore=-1;
    (*ppBlast)->nPossition=1;
    (*ppBlast)->pNext=NULL;
    return 0;
  }
  while(pTemp->pNext)pTemp=pTemp->pNext;
  pTemp->pNext=(SBlast*)malloc(sizeof(SBlast));
  pTemp->pNext->nScore=pTemp->nScore-1;
  pTemp->pNext->nPossition=pTemp->nPossition+1;
  pTemp->pNext->pNext=NULL;
  return 0;
}

int maxScore(SBlast **ppBlast){
  SBlast *pTemp=*ppBlast;
  int maxScore=0;
  while(pTemp){
    if(pTemp->nScore>maxScore)maxScore=pTemp->nScore;
    pTemp=pTemp->pNext;
  }
  return maxScore;
}

int minScore(SBlast **ppBlast){
  SBlast *pTemp=*ppBlast;
  int minScore=0;
  while(pTemp){
    if(pTemp->nScore<minScore)minScore=pTemp->nScore;
    pTemp=pTemp->pNext;
  }
  return minScore;
}

int bestScore(SBlast **ppBlast){
  SBlast *pTemp=*ppBlast;
  int minScore=pTemp->nScore;
  int bestScore=0;
  while(pTemp){
    if(pTemp->nScore-minScore>bestScore)bestScore=pTemp->nScore-minScore;
    if(pTemp->nScore<minScore)minScore=pTemp->nScore;
    pTemp=pTemp->pNext;
  }
  return bestScore;
}

int localPosition(SBlast **ppBlast){
    SBlast *pTemp=*ppBlast;
    int minScore=pTemp->nScore;
    int minScorepossition=pTemp->nPossition;
    int bestScore=0;
    int bestScorepossition=0;
    while(pTemp){
        if(pTemp->nScore-minScore>bestScore){
            bestScore=pTemp->nScore-minScore;
            bestScorepossition=minScorepossition;
        }
        if(pTemp->nScore<minScore){
            minScore=pTemp->nScore;
            minScorepossition=pTemp->nPossition;
        }
        pTemp=pTemp->pNext;
    }
    return bestScorepossition;
}


int main(){
  SBlast *pBlast=NULL;
  char match[] = "01111000000110111100000011";
  for(int i=0;i<sizeof(match)-1;i++){
    if(match[i]=='0') Down(&pBlast);
    else Up(&pBlast);
  }
  printf("The final max score is: %d\n",maxScore(&pBlast));
  printf("The final min score is: %d\n",minScore(&pBlast));
  printf("The best cumulative score is: %d\n",bestScore(&pBlast));
  printf("The current possition is: %d\n",localPosition(&pBlast));
}