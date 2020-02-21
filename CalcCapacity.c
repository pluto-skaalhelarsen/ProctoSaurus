/*
 *	CalcCapacity.c - Calculate the capacity of the chain named on the command line and print it to stdout
 *
 *	D.Merrell - 2/20/20
 *
 *	gcc -m64 -o CalcCapacity CalcCapacity.c -lgmp
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <alloca.h>
#include <math.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdbool.h>

#include <gmp.h>

#define FILE_NAME_SIZE 512
#define DEFAULT_CHAIN_FILE_NAME "ProctoDemo.template.chain.txt"

#include "ProctoSaurus.h"

bool f_chatty = false;

char ChainFileName[FILE_NAME_SIZE] = DEFAULT_CHAIN_FILE_NAME;

char *ChainIn;
int ChainInSize = 0;	// size of chain text buffer
int CurChainIn = 0;	// index of the current element in use

bool LoadChainInput(char *InName)
{
  struct stat ChainFileStat;	// file statistics including file size
  int ChainFile;

  if(stat(InName, &ChainFileStat) < 0)
  {
    printf("Cannot stat chain input file %s\n", InName);
    return(false);
  } // if

  ChainInSize = (int) ChainFileStat.st_size;
  if(ChainInSize < 0)
  {
    printf("Bad size of chain input file %s\n", InName);
    return(false);
  } // if

  if((ChainIn = (char *) malloc(ChainInSize +1)) == NULL)
  {
    printf("Cannot allocate memory for input chain\n");
    return(false);
  } // if

  if((ChainFile = open(InName, O_RDONLY)) < 0)
  {
    printf("Cannot open chain file %s\n", InName);
    return(false);
  } // if
  if(read(ChainFile, ChainIn, ChainInSize) < ChainInSize)
  {
    printf("Error reading chain file\n");
    return(false);
  } // if
  close(ChainFile);

  ChainIn[ChainInSize] = 0;	// make sure final string is terminated
  CurChainIn = 0;	// point to beginning

  return(true);
}

int GetChainSize(char *InBuf)
{
  char *wptr;
  int SemiCnt;

  for(SemiCnt=0, wptr=InBuf; wptr = strchr(wptr, ';'); wptr++, SemiCnt++);
  return(SemiCnt);
}

	// get the next chain selection after verifying match with ent
bool XlatNextChainIn(pEntSeq OutItem)
{
  int Sel, Ent;
  int Start, End;

  if(CurChainIn >= ChainInSize)		// unexpected end of chain
    return(-1);

  for(Start=CurChainIn; (Start < CurChainIn+10) && (!isdigit(ChainIn[Start])); Start++);	// find the start of a number
  for(End=Start; (End < CurChainIn+10) && (ChainIn[End] != ','); End++);	// find the end of a number
  ChainIn[End] = 0;
  Sel = atoi(&ChainIn[Start]);

  for(Start=++End; (Start < CurChainIn+20) && (!isdigit(ChainIn[Start])); Start++);	// find the start of a number
  for(End=Start; (End < CurChainIn+20) && (ChainIn[End] != ';'); End++);	// find the end of a number
  ChainIn[End++] = 0;
  Ent = atoi(&ChainIn[Start]);

  CurChainIn = End;

  OutItem->Ent = Ent;
  OutItem->Sel = Sel;

  return(true);
}

int main(int argc, char *argv[])
{
  int i;
  pEntSeq EChain;
  int EChainLen;
  mpz_t ChainCap;

  for(i=1; i < argc; i++)
    if((argv[i][0] == '-') && (argv[i][1] == 'p'))
      f_chatty = true;
    else
      strcpy(ChainFileName, argv[i]);

  if(f_chatty)
    printf("Loading chain file %s\n", ChainFileName);

  if(!LoadChainInput(ChainFileName))
  {
    printf("Error loading input file %s\n", ChainFileName);
    return(-1);
  } // if

  EChainLen = GetChainSize(ChainIn);

  if(f_chatty)
    printf("ChainLen %d\n", EChainLen);

  if((EChain = (pEntSeq) calloc(EChainLen, sizeof(EntSeq))) == NULL)
  {
    printf("Cannot allocate memory for bin chain ln %d\n", EChainLen);
    return(-1);
  } // if

  for(i=0; i < EChainLen; i++)
    XlatNextChainIn(&EChain[i]);
  free(ChainIn);

  mpz_init_set_ui(ChainCap, EChain[0].Ent);
  for(i=1; i < EChainLen; i++)
    mpz_mul_ui(ChainCap, ChainCap, EChain[i].Ent);

  if(f_chatty)
    printf("Chain capacity:\n");
  gmp_printf("%Zd\n", ChainCap);
  if(f_chatty)
    printf("    (%d bits)\n", (int) mpz_sizeinbase(ChainCap, 2));

  return(0);
}

