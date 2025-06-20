#include <LPC21xx.H>

#include "types.h"

/* Keypad pin connections */
//rows are connected to P1.16 to P1.19
#define ROW0 16
#define ROW1 17
#define ROW2 18
#define ROW3 19

//cols are connected to P1.20 to P1.23
#define COL0 20
#define COL1 21
#define COL2 22
#define COL3 23

//keypad lookup table
/*
unsigned char keypad_lut[4][4] = {1,2,3,4,
																	5,6,7,8,
																	9,10,11,12,
																	13,14,15,16};
																	*/

unsigned char keypad_lut[4][4] = {'7','8','9','/',
																	'4','5','6','*',
																	'1','2','3','-',
																	'C','0','=','+'};

void Keypad_Init(void)																	
{
	//configuring the rows port pins as output port pins	
	IODIR1 |= ((1<<ROW0)|(1<<ROW1)|(1<<ROW2)|(1<<ROW3));
	//initially making all rows to zero
	//IOCLR1 = ((1<<ROW0)|(1<<ROW1)|(1<<ROW2)|(1<<ROW3));
	//initially making all columns to one
	//IOSET1 |= ((1<<COL0)|(1<<COL1)|(1<<COL2)|(1<<COL3));
}


u32 ColScan(void)
{
	u32 c0,c1,c2,c3;
	
	c0 = ((IOPIN1>>COL0)&1);
	c1 = ((IOPIN1>>COL1)&1);
	c2 = ((IOPIN1>>COL2)&1);
	c3 = ((IOPIN1>>COL3)&1);
	
	if((c0&c1&c2&c3) == 0)
		return 0;
	else
		return 1;	
}

u32 RowCheck(void)
{
	u32 rNo;

	//row0 identification
	
	while(1)
	{					
		//row0 making low
		IOCLR1 = (1<<ROW0);
		//remining rows making high
		IOSET1 = ((1<<ROW1)|(1<<ROW2)|(1<<ROW3));
		//if any one of the column value is 0 then row value is 0 then no need to check other rows
		if(ColScan() == 0)
		{
				rNo = 0;
				break;
		}
		
		//row1 identification

		//row1 making low
		IOCLR1 = (1<<ROW1);
		//remining rows making high
		IOSET1 = ((1<<ROW0)|(1<<ROW2)|(1<<ROW3));
		//if any column value is 0 then row value is 1 then no need to check other rows
		if(ColScan() == 0)
		{
				rNo = 1;
				break;
		}
		
		//row2 identification

		//row2 making low
		IOCLR1 = (1<<ROW2);
		//remining rows making high
		IOSET1 = ((1<<ROW0)|(1<<ROW1)|(1<<ROW3));
		//if any column value is 0 then row value is 2 then no need to check other rows
		if(ColScan() == 0)
		{
				rNo = 2;
				break;
		}
		
		//row3 identification

		//row3 making low
		IOCLR1 = (1<<ROW3);
		//remining rows making high
		IOSET1 = ((1<<ROW0)|(1<<ROW1)|(1<<ROW2));
		//if any column value is 0 then row value is 3
		if(ColScan() == 0)
		{
				rNo = 3;
				break;
		}
	}
	return rNo;
}

u32 ColCheck(void)
{
	u32 cNo;
	//columns identification process
	if(((IOPIN1>>COL0)&1) == 0)
		cNo = 0;
	else if(((IOPIN1>>COL1)&1) == 0)
		cNo = 1;
	else if(((IOPIN1>>COL2)&1) == 0)
		cNo = 2;
	else if(((IOPIN1>>COL3)&1) == 0)
		cNo = 3;
	
	return cNo;
}

u32 KeyDetect(void)
{
	u32 rowval,colval;
	
	IOCLR1 = ((1<<ROW0)|(1<<ROW1)|(1<<ROW2)|(1<<ROW3));
	
	//waiting for switch press
	while(ColScan());
	//row identification process
	rowval = RowCheck();
	//columns identification process
	colval = ColCheck();
	//waiting for switch release
	while(!ColScan());
	
	return keypad_lut[rowval][colval];	
}
