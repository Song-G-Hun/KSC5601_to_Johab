// johab_test.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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
      �ѱ� font ó���κ�
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
	
 
	first  = HanTable1[HAN1];	//1		�ش� �ʼ��� N��° ��
	mid = HanTable2[HAN2];	//1		�ش� �߼��� N��° ��
	last = HanTable3[HAN3];	//0		�ش� ������ N��° ��

  /*------------------------------
    ��,��,���� �ش� ��Ʈ Ÿ��(��)�� �����Ѵ�.
  ------------------------------*/

  /*
   �ʼ� 19��:��������������������������������������
   �߼� 21��:�������¤äĤŤƤǤȤɤʤˤ̤ͤΤϤФѤҤ�
   ���� 27��:����������������������������������������������������

   �ʼ�
      �ʼ� 1�� : ��ħ���� '�������¤äĤŤƤ�' �� ����
      �ʼ� 2�� : ��ħ���� '�Ǥˤ�'
      �ʼ� 3�� : ��ħ���� '�̤�'
      �ʼ� 4�� : ��ħ���� '�Ȥɤʤ�'
      �ʼ� 5�� : ��ħ���� '�ͤΤ�'
      �ʼ� 6�� : ��ħ�ִ� '�������¤äĤŤƤ�' �� ����
      �ʼ� 7�� : ��ħ�ִ� '�Ǥˤ̤Ф�'
      �ʼ� 8�� : ��ħ�ִ� '�ȤɤʤҤͤΤ�'

   �߼�
      �߼� 1�� : ��ħ���� '����' �� ����
      �߼� 2�� : ��ħ���� '����' �̿��� ����
      �߼� 3�� : ��ħ�ִ� '����' �� ����
      �߼� 4�� : ��ħ�ִ� '����' �̿��� ����

   ����
      ���� 1�� : �߼� '������' �� ����
      ���� 2�� : �߼� '�äŤʤͤϤҤ�'
      ���� 3�� : �߼� '���¤ĤƤɤ�'
      ���� 4�� : �߼� '�Ǥˤ̤Ф�'

  */
  if(!last)  //��ħ ���� ���
  {
    firstType = cho[mid];
    if(first == 1 || first == 24) midType = 0;
    else midType = 1;
  }
  else       //��ħ �ִ� ���
  {
    firstType = cho2[mid];
    if(first == 1 || first == 24) midType = 2;
    else midType = 3;
    lastType = jong[mid];
  }

  //�ʼ�
  pB = HANFontImage;
  pF = (unsigned char*)johab844_1824 + (firstType * 20 + first) * 72;
  i = 72;
  while(i--) *pB++ = *pF++;

  //�߼�
  pB = HANFontImage;
  pF = (unsigned char*)johab844_1824 + (8 * 20 + midType * 22 + mid) * 72;
  i = 72;
  while(i--) *pB++ |= *pF++;

  //����
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

	cho  = HanTable1[cho];	//1		�ش� �ʼ��� N��° ��
	jung = HanTable2[jung];	//1		�ش� �߼��� N��° ��
	jong = HanTable3[jong];	//0		�ش� ������ N��° ��

	type1 = jong ? Type1Table2[jung] : Type1Table1[jung];	// ���� ���¿� ���� �ʼ�Ÿ�� ����.
	type2 = ( cho == 1 || cho == 16 ) ? 0: 1;				// �ʼ��� '��'�̳� '��'�϶� 0, �ƴϸ� 1
	if ( jong ) type2 += 2;									// ������ ������ �߼� Ÿ�԰� +2 ����
	type3 = Type3Table[jung];	// �߼��� ���� ���� Ÿ�� ����.

	if ( cho ){
		cho--;
		pp = (unsigned int)(COUNT_CHO*type1) + cho;	// (19*0) + 0
 	    
		pB = HANFontImage;
		pF = (unsigned char*)johab844_1824 + (pp* 72);
		i = 72;
		while(i--) *pB++ = *pF++;		
	}
	if ( jung ){
		pp = 151 + (unsigned int)(COUNT_JUNG*type2) + jung;	// 151(�ʼ� 19�� * �ʼ����� 8��)		
		pB = HANFontImage;
		pF = (unsigned char*)johab844_1824 + (pp* 72);
		i = 72;
		while(i--) *pB++ |= *pF++;
	}

	if ( jong ){
		pp = 235+ (unsigned int)(COUNT_JONG*type3) + jong;	// 235(151 + �߼� 21�� * �߼����� 4��)		
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
	
	//tmp = 0xd3b7; //��
	//tmp = 0xBDF6    ; //�� 
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

