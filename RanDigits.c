#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int NumDigits = 335;
int SeedSource;

int main(int argc, char *argv[])
{
  int i;

  if((SeedSource = (int) clock()) < 0)
    SeedSource = -SeedSource;

  if(argc > 1)
    NumDigits = atoi(argv[1]);

  if(argc > 2)
    SeedSource = atoi(argv[2]);

  srand(SeedSource);

  for(i=0; i < NumDigits; i++)
    printf("%d", (rand() >> 3) % 10);
  printf("\n");

  return(0);
}

