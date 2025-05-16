#include <LPC21xx.H>
#include "rtc.h"
#include "lcd.h"
#include "defines.h"
#include "keypad.h"
#include "delay.h"
#define sw1  19
#define sw2  20 
#define buzzer 6
u32 hour=5,min=50,sec=0,day,date,month,year;
u8 Mhour=5,Mmin=50,Mday,Mdate;
s32 KeyC,KeyC2,KeyC3;
unsigned char week1[][4] = {"SUN","MON","TUE","WED","THU","FRI","SAT"};
int main()
{
	IODIR0|=1<<buzzer;
	RTC_Init();
	InitLCD();
	Keypad_Init();
	CmdLCD(0x0C);
	RTCSetTime(hour,min,0);
	RTCSetDate(date,2,2025);
	RTCSetDay(day);
	while(1)
	{
		//CmdLCD(0x01);
		ip1:	RTCGetTime(&hour,&min,&sec);		
		DisplayRTCTime(hour,min,sec);
	  RTCGetDate(&date,&month,&year);
		DisplayRTCDate(date,month,year);
		RTCGetDay(&day);
    DisplayRTCDay(day);
		if((hour==Mhour)&&(min==Mmin)&&(sec==3)){
			IOSET0=1<<buzzer;
			while(READBIT(IOPIN0,sw2)==0){
			RTCGetTime(&hour,&min,&sec);		
		  DisplayRTCTime(hour,min,sec);
			}
			IOCLR0=1<<buzzer;
		}
		if(READBIT(IOPIN0,sw1)){
		CmdLCD(0x01);	
		ip2:CmdLCD(0x80);
		StrLCD("1.EDIT RTC");
		CmdLCD(0xC0);
		StrLCD("2.EDIT MT 3.BACK");
			KeyC=KeyDetect();
			if(KeyC>'2'||KeyC<'1')
			{
				if(KeyC=='3')
				{
					CmdLCD(0x01);
					goto ip1;
				}
				goto ip2;	
	    }
			
			ip3:CmdLCD(0x01);
			StrLCD("1.hr2.mi.3.day");
		  CmdLCD(0xC0);
		  StrLCD("4.date 5.back");
			KeyC2=KeyDetect();
			if(KeyC2>'4'||KeyC2<'1')
			{
				if(KeyC2=='5')
				{
					CmdLCD(0x01);
					goto ip2;
				}
				goto ip3;	
	    }
			CmdLCD(0x01);
			if(KeyC=='1'){
					RTCGetTime(&hour,&min,&sec);		
		      DisplayRTCTime(hour,min,sec);
				  CmdLCD(0xc0);
				if(KeyC2=='1')
				{
					StrLCD("EDIT HOUR:-");
					KeyC3=ReadNum();
					if(KeyC3>23||KeyC3<0)
					{			goto ip3;
					}	
					RTCSetTime(KeyC3,min,0);
				}
				else if(KeyC2=='2')
				{
					StrLCD("EDIT MIN:-");
						KeyC3=ReadNum();
					if(KeyC3>59||KeyC3<0)
					{			goto ip3;
					}
					RTCSetTime(hour,KeyC3,0);
        }
				else if(KeyC2=='3')
				{
					StrLCD("EDIT DAY:-");
						KeyC3=ReadNum();
					if(KeyC3>6||KeyC3<0)
					{			goto ip3;
					}
				  RTCSetDay(KeyC3);
}
				else if(KeyC2=='4')
				{
					StrLCD("EDIT DATE:-");
					KeyC3=ReadNum();
					if(KeyC3>30||KeyC3<0)
					{			goto ip3;
					}
					RTCSetDate(KeyC3,2,2025);
}
				}
			else if(KeyC=='2'){
				DisplayMRTCTime(Mhour,Mmin,sec);
				  CmdLCD(0xc0);
				if(KeyC2=='1')
				{
					
					StrLCD("EDIT HOUR:-");
					KeyC3=ReadNum();
					if(KeyC3>23||KeyC3<0)
					{			goto ip3;
					}
					Mhour=KeyC3;
        }
				else if(KeyC2=='2')
				{
					StrLCD("EDIT MIN:-");
					KeyC3=ReadNum();
					if(KeyC3>59||KeyC3<0)
					{			goto ip3;
					}
					Mmin=KeyC3;
}
				else if(KeyC2=='3')
				{
					StrLCD("EDIT DAY:-");
					KeyC3=ReadNum();
					if(KeyC3>6||KeyC3<0)
					{			goto ip3;
					}
					Mday=KeyC3;
}
				else if(KeyC2=='4')
				{
					StrLCD("EDIT DATE:-");
					KeyC3=ReadNum();
					if(KeyC3>30||KeyC3<0)
					{			goto ip3;
					}
					Mdate=KeyC3;
         }
			}	

		goto ip3;	
		}
					

    }
}
