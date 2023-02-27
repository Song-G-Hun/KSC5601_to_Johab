// johab_test.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "HFont.h"
#include "ksc2kssm.h"
#ifndef _BV
#define _BV(bit) (1<<(bit))
#endif

#define COUNT_CHO       19
#define COUNT_JUNG      21
#define COUNT_JONG      27

unsigned char HANFontImage[72] = {0, };
/*=============================================================================
      한글 font 처리부분
  =============================================================================*/
extern unsigned short KSSMCODE[25][94];
unsigned char johab844_1824[];

unsigned short KSSMcode(unsigned char c, unsigned char d)
{
	unsigned int i=c-0xB0, j=d-0xA1;

	if (i < 0 || i > 24 || j < 0 || j > 93) return 0;
	else return KSSMCODE[i][j];
}

unsigned char* getHAN_font(unsigned char HAN1, unsigned char HAN2, unsigned char HAN3)
{
  unsigned int utf16;
  unsigned char first, mid, last;
  unsigned char i;
  unsigned char firstType, midType, lastType = 0;
  unsigned char *pB, *pF;

  unsigned char cho[]  = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 3, 3, 1, 2, 4, 4, 4, 2, 1, 3, 0 };
  unsigned char cho2[] = { 0, 5, 5, 5, 5, 5, 5, 5, 5, 6, 7, 7, 7, 6, 6, 7, 7, 7, 6, 6, 7, 5 };
  unsigned char jong[] = { 0, 0, 2, 0, 2, 1, 2, 1, 2, 3, 0, 2, 1, 3, 3, 1, 2, 1, 3, 3, 1, 1 };
	
 
	first  = HanTable1[HAN1];	//1		해당 초성의 N번째 수
	mid = HanTable2[HAN2];	//1		해당 중성의 N번째 수
	last = HanTable3[HAN3];	//0		해당 종성의 N번째 수

  /*------------------------------
    초,중,종성 해당 폰트 타입(벌)을 결정한다.
  ------------------------------*/

  /*
   초성 19자:ㄱㄲㄴㄷㄸㄹㅁㅂㅃㅅㅆㅇㅈㅉㅊㅋㅌㅍㅎ
   중성 21자:ㅏㅐㅑㅒㅓㅔㅕㅖㅗㅘㅙㅚㅛㅜㅝㅞㅟㅠㅡㅢㅣ
   종성 27자:ㄱㄲㄳㄴㄵㄶㄷㄹㄺㄻㄼㄽㄾㄿㅀㅁㅂㅄㅆㅇㅈㅊㅋㅌㅍㅎ

   초성
      초성 1벌 : 받침없는 'ㅏㅐㅑㅒㅓㅔㅕㅖㅣ' 와 결합
      초성 2벌 : 받침없는 'ㅗㅛㅡ'
      초성 3벌 : 받침없는 'ㅜㅠ'
      초성 4벌 : 받침없는 'ㅘㅙㅚㅢ'
      초성 5벌 : 받침없는 'ㅝㅞㅟ'
      초성 6벌 : 받침있는 'ㅏㅐㅑㅒㅓㅔㅕㅖㅣ' 와 결합
      초성 7벌 : 받침있는 'ㅗㅛㅜㅠㅡ'
      초성 8벌 : 받침있는 'ㅘㅙㅚㅢㅝㅞㅟ'

   중성
      중성 1벌 : 받침없는 'ㄱㅋ' 와 결합
      중성 2벌 : 받침없는 'ㄱㅋ' 이외의 자음
      중성 3벌 : 받침있는 'ㄱㅋ' 와 결합
      중성 4벌 : 받침있는 'ㄱㅋ' 이외의 자음

   종성
      종성 1벌 : 중성 'ㅏㅑㅘ' 와 결합
      종성 2벌 : 중성 'ㅓㅕㅚㅝㅟㅢㅣ'
      종성 3벌 : 중성 'ㅐㅒㅔㅖㅙㅞ'
      종성 4벌 : 중성 'ㅗㅛㅜㅠㅡ'

  */
  if(!last)  //받침 없는 경우
  {
    firstType = cho[mid];
    if(first == 1 || first == 24) midType = 0;
    else midType = 1;
  }
  else       //받침 있는 경우
  {
    firstType = cho2[mid];
    if(first == 1 || first == 24) midType = 2;
    else midType = 3;
    lastType = jong[mid];
  }

  //초성
  pB = HANFontImage;
  pF = (unsigned char*)johab844_1824 + (firstType * 20 + first) * 72;
  i = 72;
  while(i--) *pB++ = *pF++;

  //중성
  pB = HANFontImage;
  pF = (unsigned char*)johab844_1824 + (8 * 20 + midType * 22 + mid) * 72;
  i = 72;
  while(i--) *pB++ |= *pF++;

  //종성
  if(last)
  {
    pB = HANFontImage;
    pF = (unsigned char *)johab844_1824 + (8 * 20 + 4 * 22 + lastType * 28 + last) * 72;
    i = 72;
    while(i--) *pB++ |= *pF++;
  }

  return HANFontImage;
}


#define CHOSONG(i)	(unsigned char) ((i >> 10) & 0x1f)
			/* get 5-bits of Chosong from the    */
			/* high byte of the 2-byte syllable. */

#define JUNGSONG(i)	(unsigned char) ((i >> 5) & 0x1f)
			/* get 5-bits of Jungsong from the   */
			/* 2-byte Korean syllable.           */
#define JONGSONG(i)	(unsigned char) (i & 0x001f)
			/* get 5-bits of Jongsong from the  */
			/* low byte of the 2-byte syllable. */


void han_lcd(unsigned char value1, unsigned char value2, unsigned char posi)
{
	unsigned char cho, jung, jong;
	unsigned char  type1, type2, type3;
	unsigned int  i, pp;
unsigned char *pB, *pF;

	cho  = (value1 & 0x7c) >> 2;					//	2
	jung = ((value1 & 0x03) << 3) | (value2 >> 5);	//	3
	jong = value2 & 0x1f;							//	1

	cho  = HanTable1[cho];	//1		해당 초성의 N번째 수
	jung = HanTable2[jung];	//1		해당 중성의 N번째 수
	jong = HanTable3[jong];	//0		해당 종성의 N번째 수

	type1 = jong ? Type1Table2[jung] : Type1Table1[jung];	// 종성 유뮤에 따른 초성타입 선택.
	type2 = ( cho == 1 || cho == 16 ) ? 0: 1;				// 초성이 'ㄱ'이나 'ㅋ'일때 0, 아니면 1
	if ( jong ) type2 += 2;									// 종성이 있을때 중성 타입값 +2 증가
	type3 = Type3Table[jung];	// 중성에 따른 종성 타입 선택.

	if ( cho ){
		cho--;
		pp = (unsigned int)(COUNT_CHO*type1) + cho;	// (19*0) + 0
 	    
		pB = HANFontImage;
		pF = (unsigned char*)johab844_1824 + (pp* 72);
		i = 72;
		while(i--) *pB++ = *pF++;		
	}
	if ( jung ){
		pp = 151 + (unsigned int)(COUNT_JUNG*type2) + jung;	// 151(초성 19개 * 초성벌수 8개)		
		pB = HANFontImage;
		pF = (unsigned char*)johab844_1824 + (pp* 72);
		i = 72;
		while(i--) *pB++ |= *pF++;
	}

	if ( jong ){
		pp = 235+ (unsigned int)(COUNT_JONG*type3) + jong;	// 235(151 + 중성 21개 * 중성벌수 4개)		
		pB = HANFontImage;
		pF = (unsigned char*)johab844_1824 + (pp* 72);
		i = 72;
		while(i--) *pB++ |= *pF++;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	unsigned int han = 0xb0a1,tmp,i,j,mask;
	unsigned char cho,jung,jong;
	unsigned char *ptr ;
	
	//tmp = 0xd3b7; //힝
	//tmp = 0xBDF6    ; //쏭 
	tmp = KSSMcode(0xbd,0xf6);
	//tmp = 0xb0a1;
	printf("%x \n",tmp);

  cho = CHOSONG(tmp);
  jung = JUNGSONG(tmp);
  jong = JONGSONG(tmp);

  printf("cho %x jung %x jong %x \n",cho,jung,jong);

  ptr=getHAN_font(cho,jung,jong);

   for(i=0;i<72;i+=3)
  {
		mask = 0x80;
		for(j=0;j<8;j++)
		{
			if(ptr[i] & mask)	printf("*");
			else				printf(" ");
			mask >>=1;
		}
		mask = 0x80;
		for(j=0;j<8;j++)
		{
			if(ptr[i+1] & mask)	printf("*");
			else				printf(" ");
			mask >>=1;
		}
		mask = 0x80;
		for(j=0;j<8;j++)
		{
			if(ptr[i+2] & mask)	printf("*");
			else				printf(" ");
			mask >>=1;
		}
		printf("\n");
  }
	return 0;
}

