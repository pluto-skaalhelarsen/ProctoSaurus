/*
 *	TextToNum.c - convert a text string given on the command line to a GMP big number and print that value back to stdout as a decimal string
 *
 *	D.Merrell - 2/20/20
 *
 *	gcc -m64 -o TextToNum TextToNum.c -lgmp
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

#define DFLT_TEXT "What would you do if we let you go home and the plastic was melted, and so was the chrome?";

bool f_chatty = false;	// set to true to enable run-time messages

int EntPackText(mpz_t OutNum, char *InText, int InLen)
{
  int i;

  for(i=InLen-1; i >= 0; i--)
  {
    mpz_mul_ui(OutNum, OutNum, (unsigned long) 128);
    mpz_add_ui(OutNum, OutNum, (unsigned long) (InText[i] & 0x7f));
  }

  return(0);
}

char *TextIn = DFLT_TEXT;

int main(int argc, char *argv[])
{
  int i;
  mpz_t TextBin;

  for(i=1; i < argc; i++)
    if((argv[i][0] == '-') && (argv[i][1] == 'p'))
      f_chatty = true;
    else
      TextIn = argv[i];

  if(f_chatty)
    printf("Text in: %s\n", TextIn);

  mpz_init(TextBin);

  EntPackText(TextBin, TextIn, strlen(TextIn));

  if(f_chatty)
    printf("Numeric representation:\n");
  gmp_printf("%Zd\n", TextBin);

  if(f_chatty)
    printf("  Number of decimal digits is %d\n", (int) mpz_sizeinbase(TextBin, 10));

  return(0);
}

