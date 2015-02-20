#include <avr/io.h>
#include <util/delay.h>

#define MrLCDsCrib 				PORTB
#define DataDir_MrLCDsCrib		DDRB
#define MrLCDsControl 			PORTD
#define DataDir_MrLCDsControl	DDRD
#define LightSwitch 			5
#define ReadWrite				7
#define BiPolarMood				2

void Check_If_MrLCD_IsBuisy(void);
void Peek_A_Boo(void);
void Send_A_Command(unsigned char command);
void Send_A_Character(unsigned char character);
void Send_A_String(char *StringofCharacters);
int prosent = 0;

void main(void){

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
	while(1){
		Send_A_Command(0x01); //Clears screen 0x01 == 00000001
		_delay_ms(2);
		Send_A_String("Booting");
		_delay_ms(50);
		Send_A_String(".");
		_delay_ms(50);
		Send_A_String(".");
		_delay_ms(50);
		Send_A_String(".");
		_delay_ms(100);
		
		Send_A_Command(0x01); //Clears screen 0x01 == 00000001
		_delay_ms(2);
		Send_A_String("Loading Kernel");
		_delay_ms(50);
		Send_A_String(".");
		_delay_ms(50);
		Send_A_String(".");
		_delay_ms(50);
		Send_A_String(".");
		_delay_ms(200);
		
		while(prosent<101){
			Send_A_Command(0x01); //Clears screen 0x01 == 00000001
			_delay_ms(2);
			Send_A_String(" " + prosent);
			_delay_ms(15);
			prosent++;
		}
		prosent=0;
		
		Send_A_Command(0x01); //Clears screen 0x01 == 00000001
		_delay_ms(2);
		Send_A_String("      MATRIX-OS v10.4.5");
		_delay_ms(300);
		
		Send_A_Command(0x01); //Clears screen 0x01 == 00000001
		_delay_ms(300);
	}
		
	
	
	while(1){
	
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

