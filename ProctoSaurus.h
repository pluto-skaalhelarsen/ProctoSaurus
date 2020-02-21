/*
 *	ProctoSaurus.h - ProctoSaurus definitions for Thesaurus of the Gods chain file processing
 *
 *
 *	D.Merrell 2/19/20
 *
 */
#ifndef _PROCTOSAURUS_H_
#define _PROCTOSAURUS_H_

	// binary chain data type
typedef struct _EntSeq
{
  int Sel;		// selection component
  int Ent;		// entropy component
} EntSeq, *pEntSeq;

#endif // _PROCTOSAURUS_H_


