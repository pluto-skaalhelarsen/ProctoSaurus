/*
 *	NumToChain - Convert a Thesaurus of the Gods chain file and a numric string to a chain with the given numeric value and print that chain to stdout.
 *
 *	D.Merrell - 2/19/20
 *
 *	gcc -m64 -o NumToChain NumToChain.c -lgmp
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
#define DEFAULT_NUM_FILE_NAME "ProctoDemo.hidden.txt"

#include "ProctoSaurus.h"

bool f_chatty = false;
bool f_chain_file_assigned = false;

char ChainFileName[FILE_NAME_SIZE] = DEFAULT_CHAIN_FILE_NAME;
char NumFileName[FILE_NAME_SIZE] = DEFAULT_NUM_FILE_NAME;

char *ChainIn;
int ChainInSize = 0;	// size of chain text buffer
int CurChainIn = 0;	// index of the current element in use

char *NumIn;
int NumInSize = 0;

bool LoadTextInput(char *InName, char **InBuf, int *InBufSize)
{
  struct stat InFileStat;	// file statistics including file size
  int InFile;

  if(stat(InName, &InFileStat) < 0)
  {
    printf("Cannot stat text input file %s\n", InName);
    return(false);
  } // if

  *InBufSize = (int) InFileStat.st_size;
  if(InBufSize < 0)
  {
    printf("Bad size of text input file %s\n", InName);
    return(false);
  } // if

  if((*InBuf = (char *) malloc(*InBufSize +1)) == NULL)
  {
    printf("Cannot allocate memory for input text\n");
    return(false);
  } // if

  if((InFile = open(InName, O_RDONLY)) < 0)
  {
    printf("Cannot open input file %s\n", InName);
    return(false);
  } // if
  if(read(InFile, *InBuf, *InBufSize) < *InBufSize)
  {
    printf("Error reading text file\n");
    return(false);
  } // if
  close(InFile);

  (*InBuf)[*InBufSize] = 0;	// make sure final string is terminated

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
  mpz_t ChainBin;
  mpz_t NumBin;
  mpz_t Rem;	// remainder that is used multiple times for mod calls
  mpz_t Tmp;	// generic temp variable to conserve allocations

  for(i=1; i < argc; i++)
    if((argv[i][0] == '-') && (argv[i][1] == 'p'))
    {
      f_chatty = true;
    } // if
    else
    {
      if(f_chain_file_assigned)
      {
        strcpy(NumFileName, argv[i]);
      } // if
      else
      {
        strcpy(ChainFileName, argv[i]);
        f_chain_file_assigned = true;
      } // else
    } // else

  if(f_chatty)
    printf("Loading chain file %s\n", ChainFileName);

  if(!LoadTextInput(ChainFileName, &ChainIn, &ChainInSize))
  {
    printf("Error loading input file %s\n", ChainFileName);
    return(-1);
  } // if
  CurChainIn = 0;	// point to beginning

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
  {
    gmp_printf("Chain capacity:\n%Zd\n", ChainCap);
    printf("    (%d bits)\n", (int) mpz_sizeinbase(ChainCap, 2));
  } // if

	// notice that we implicitly make the zeroth chain element the high-order digit of the numeric value
  mpz_init_set_ui(ChainBin, EChain[0].Sel);
  for(i=1; i < EChainLen; i++)
  {
    mpz_mul_ui(ChainBin, ChainBin, EChain[i].Ent);
    mpz_add_ui(ChainBin, ChainBin, EChain[i].Sel);
  } // for

  if(f_chatty)
  {
    printf("Chain original content:\n");
    gmp_printf("%Zd\n", ChainBin);
    printf("    (%d bits)\n", (int) mpz_sizeinbase(ChainBin, 2));
  } // if

	// .... start converting the input num .....
  if(!LoadTextInput(NumFileName, &NumIn, &NumInSize))
  {
    printf("Error loading input file %s\n", NumFileName);
    return(-1);
  } // if
  if(NumIn[NumInSize-1] <= ' ')		// trailing new line?
    NumIn[NumInSize-1] = 0;

	// the chain was arbitrarily given orientation with EChain[0] as the high-order end of the numeric value
  mpz_init_set_str(NumBin, NumIn, 10);	// convert the numric value in string form to a GMP integer
  mpz_init_set(Tmp, NumBin);	// preserve the number and decompose the tmp value
  mpz_init(Rem);	// prepare these variables for use
  for(i=EChainLen-1; i >= 0; i--)	// start with the low-order end of the chain and the number
  {
    mpz_fdiv_qr_ui(Tmp, Rem, Tmp, (unsigned long) EChain[i].Ent);
    EChain[i].Sel = (int) mpz_get_ui(Rem);
  } // for

  if(f_chatty)
    if(mpz_sgn(Tmp) != 0)		// was the numeric value consumed?
      printf("Input number exceeds the capacity of the chain\n");

  for(i=0; i < EChainLen; i++)
    printf("%d,%d;", EChain[i].Sel, EChain[i].Ent);
  printf("\n\n");

  return(0);
}


