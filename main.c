#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#define MrLCDsCrib 				PORTB
#define DataDir_MrLCDsCrib		DDRB
#define MrLCDsControl 			PORTD
#define DataDir_MrLCDsControl	DDRD
#define LightSwitch 			5
#define ReadWrite				7
#define BiPolarMood				2
#define LED						7
#define Button					6

void Check_If_MrLCD_IsBuisy(void);
void Peek_A_Boo(void);
void Send_A_Command(unsigned char command);
void Send_A_Character(unsigned char character);
void Send_A_String(char *StringofCharacters);
int prosent = 0;


void main(void){

	int Pressed = 0;
	int Pressed_Confidence_Level = 0;
	int Released_Confidence_Level = 0;
	
	DDRC |=1<<LED | 0<<Button;
	PORTC |=1<<LED | 1<<Button;
	DataDir_MrLCDsControl |= 1<<LightSwitch | 1<<ReadWrite |1<<BiPolarMood;
	_delay_ms(15);
	
	Send_A_Command(0x01); //Clears screen 0x01 == 00000001
	_delay_ms(2);
	Send_A_Command(0x38);
	_delay_us(50);
	Send_A_Command(0b00001110);
	_delay_us(50);
	
	/*
	Send_A_Character(0x4d); //
	Send_A_Character(0x65); //
	Send_A_Character(0x74); //
	Send_A_Character(0x72); //
	Send_A_Character(0x61); //
	Send_A_Character(0x66); //
	Send_A_Character(0x6f); //
	Send_A_Character(0x6e); //
	Send_A_Character(0x69); //
	Send_A_Character(0x63); //
	Send_A_Character(0x20); //
	Send_A_Character(0x4d); //
	Send_A_Character(0x65); //
	Send_A_Character(0x64); //
	Send_A_Character(0x69); //
	Send_A_Character(0x61); //
	*/

		Send_A_Command(0x01); //Clears screen 0x01 == 00000001
		_delay_ms(2);
		Send_A_String("Booting");
		_delay_ms(50);
		Send_A_String(".");
		_delay_ms(50);
		Send_A_String(".");
		_delay_ms(50);
		Send_A_String(".");
		_delay_ms(200);
		
		
		prosent=0;
		
		Send_A_Command(0x01); //Clears screen 0x01 == 00000001
		_delay_ms(2);
		Send_A_String("  MATRIX-OS v8");
		_delay_ms(300);
		Send_A_Command(0x01); //Clears screen 0x01 == 00000001
		_delay_ms(2);
		
		TCCR1B |= 1<<CS10;
		int repeatCount = 0;
		int timesPressed=0;
		TCNT1 = 0;
		char *repeatString[10];
		Send_A_Command(0x01); //Clears screen 0x01 == 00000001
		_delay_ms(2);
		Send_A_String("LED ON?    Yes");
		while (repeatCount<500){
			
			
			
			if (bit_is_clear(PINC, Button)){
				Pressed_Confidence_Level++;
				if (Pressed_Confidence_Level > 400){
					if (Pressed==0){
						timesPressed++;
						PORTC ^= 1 << LED;
						Pressed = 1;
						Send_A_Command(0x80); //Clears screen 0x01 == 00000001
						_delay_ms(2);
						if (timesPressed %2 ==1){
							Send_A_String("LED ON?    No ");
						}else{
							Send_A_String("LED ON?    Yes");
						}
						
					}
					Pressed_Confidence_Level = 0;
				}
			}else{
				Released_Confidence_Level++;
				if (Released_Confidence_Level > 400){
					Pressed = 0;
					Released_Confidence_Level = 0;
				}
			}
			if (TCNT1 > 7000){
				repeatCount++;
				TCNT1 = 0;
			}
			if (repeatCount %100 ==0){
				Send_A_Command(0x80+40);
				itoa((500-repeatCount)/100, repeatString, 10);
				Send_A_String(repeatString);
				_delay_ms(4);
				Send_A_Command(0x80);
			}
		}
		
	
	Send_A_Command(0x01); //Clears screen 0x01 == 00000001
	_delay_ms(2);
		
	char *positionString[4];
	
	while(1){
	
	//LCDen er på 0 og 40 
	
		for(int x = 0;x<55;x++){
			Send_A_Command(0x80 + x);//1. set the current location
			Send_A_String("MATRIX OS");	//2. put an x at that location
			itoa(x, positionString, 10);
			Send_A_Command(0x80+57); //54 bra 
			Send_A_String(positionString);//3. Display the location as a number
			_delay_ms(50);//4. Add a delay to slow it down
			Send_A_Command(0x80 + x);//5. go back to the location
			Send_A_String("         ");//6. Put a space.
		}
	}
}

void Check_If_MrLCD_IsBuisy(){
	DataDir_MrLCDsCrib = 0; //input mode
	MrLCDsControl |= 1<<ReadWrite;
	MrLCDsControl &= ~1<<BiPolarMood;
	
	while(MrLCDsCrib >= 0x80){ //
		Peek_A_Boo();
	}
	
	DataDir_MrLCDsCrib = 0xFF; //0xFF means 0b11111111
 }

void Peek_A_Boo(){
	MrLCDsControl |=1<<LightSwitch;
	asm volatile("nop");
	asm volatile("nop");
	MrLCDsControl &= ~1<<LightSwitch;
}
void Send_A_Command(unsigned char command){
	Check_If_MrLCD_IsBuisy();
	MrLCDsCrib = command;
	MrLCDsControl &= ~ (1<<ReadWrite|1<<BiPolarMood);
	Peek_A_Boo();
	MrLCDsCrib = 0;
}
void Send_A_Character(unsigned char character){
	
	Check_If_MrLCD_IsBuisy();
	MrLCDsCrib = character;
	MrLCDsControl &= ~ (1<<ReadWrite);
	MrLCDsControl |= 1<<BiPolarMood;
	Peek_A_Boo();
	MrLCDsCrib = 0;
}

void Send_A_String(char *StringofCharacters){

	while(*StringofCharacters>0){
		Send_A_Character(*StringofCharacters++);
	}
}

