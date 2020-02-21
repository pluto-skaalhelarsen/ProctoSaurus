/*
 *	NumToText.c - attempt to interpret a big GMP number given on the command line as text. If successful, print it to stdout.
 *
 *	D.Merrell - 2/20/20
 *
 *	gcc -m64 -o NumToText NumToText.c -lgmp
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

#define MAX_UNPACKED_SIZE 1000000	// fairly arbitrary max output text size

bool f_chatty = false;	// set to true to enable run-time messages

char UnpackedMsg[MAX_UNPACKED_SIZE];
int UnpackedMsgLen = 0;

mpz_t Rem;	// remainder that is used multiple times for mod calls
mpz_t Tmp;	// generic temp variable to conserve allocations

int EntUnpackText(mpz_t InNum, char *OutText)
{
  int i;
  unsigned long URem;

  mpz_set(Tmp, InNum);
  for(i=0; mpz_sgn(Tmp) != 0; i++)
  {
    mpz_tdiv_qr_ui(Tmp, Rem, Tmp, (unsigned long) 128);
    URem = mpz_get_ui(Rem);
    OutText[i] = (char) URem;
  }
  OutText[i] = 0;

  return(i);
}

int main(int argc, char *argv[])
{
  int i;
  char *TextNumIn;
  mpz_t TextBin;

  TextNumIn = NULL;

  for(i=1; i < argc; i++)
    if((argv[i][0] == '-') && (argv[i][1] == 'p'))
      f_chatty = true;
    else
      TextNumIn = argv[i];

  if(TextNumIn == NULL)
  {
    printf("No number on command line. There is no default value. Sorry...\n");
    return(-1);
  } // if

  if(f_chatty)
    printf("Text string in: %s\n", TextNumIn);

  mpz_init(Rem);
  mpz_init(Tmp);
  mpz_init_set_str(TextBin, TextNumIn, 10);

  if(f_chatty)
    gmp_printf("Initial (unconverted) numeric value: %Zd\n", TextBin);

  UnpackedMsgLen = EntUnpackText(TextBin, UnpackedMsg);
  if(UnpackedMsgLen <= 0)
  {
    printf("Failed to find convertable text!\n");
    return(-1);
  } // if

  if(f_chatty)
    printf("Converted text:\n");

  printf("%s\n", UnpackedMsg);

  return(0);
}

