/*
 *	CombineNum.c - Convert two number strings from the command line and combine them into one numeric value by XOR. Print the combined value to stdout.
 *
 *	D.Merrell - 2/20/20
 *
 *	gcc -m64 -o CombineNum CombineNum.c -lgmp
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <alloca.h>
#include <math.h>
#include <time.h>

#include <stdbool.h>

#include <gmp.h>

bool f_chatty = false;	// set to true to enable run-time messages

int main(int argc, char *argv[])
{
  int i;
  char *Text0NumIn = NULL;
  mpz_t Text0Bin;
  char *Text1NumIn = NULL;
  mpz_t Text1Bin;
  mpz_t CombinedBin;

  for(i=1; i < argc; i++)
    if((argv[i][0] == '-') && (argv[i][1] == 'p'))
      f_chatty = true;
    else if(Text0NumIn == NULL)
      Text0NumIn = argv[i];
    else
      Text1NumIn = argv[i];

  if((Text0NumIn == NULL) || (Text1NumIn == NULL))
  {
    printf("No number on command line. There is no default value. Sorry...\n");
    return(-1);
  } // if

  if(f_chatty)
    printf("Text strings in:\n  %s\n  %s\n", Text0NumIn, Text1NumIn);

  mpz_init_set_str(Text0Bin, Text0NumIn, 10);
  mpz_init_set_str(Text1Bin, Text1NumIn, 10);

  if(f_chatty)
    gmp_printf("Initial (unconverted) numeric values:\n  %Zd\n  %Zd\n", Text0Bin, Text1Bin);

  mpz_init(CombinedBin);
  mpz_xor(CombinedBin, Text0Bin, Text1Bin);

  if(f_chatty)
    printf("Combined number:\n");

  gmp_printf("%Zd\n", CombinedBin);

  return(0);
}

