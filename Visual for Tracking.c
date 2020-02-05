#include "derivative.h" /* include peripheral declarations */
#include "lcd.h"

static void ENPulse();

// Delay routines
// Microsecond (at 4 MHz bus speed)
//
void _delay_us(unsigned long delay)
{
	unsigned long	i, j;
	
	for (i = 0; i < delay; i++)
	{
		for (j = 0; j < 1; j++)
		{
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
		}
	}
}

// Millisecond (calls _delay_us 1000 times)
//
void _delay_ms(unsigned long delay)
{
	unsigned long  i;
  
	for(i = 0; i < delay; i++){
		_delay_us(1000);
  }
}
void InitGPIO(void)
{
      SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK; /* Enable clock gate for ports to enable pin routing */
     
      PORTB_PCR0 = PORT_PCR_MUX(1);
      PORTB_PCR1 = PORT_PCR_MUX(1);
      PORTB_PCR2 = PORT_PCR_MUX(1);
      PORTB_PCR3 = PORT_PCR_MUX(1);
      PORTC_PCR1 = PORT_PCR_MUX(1);
      PORTC_PCR2 = PORT_PCR_MUX(1);
     
      GPIOB_PCOR = 0x0f;
      GPIOB_PDDR = 0x0f;
 
      GPIOC_PCOR = 0x06;
      GPIOC_PDDR = 0x06;
}
// Pulses the EN line
//
static void ENPulse()
{
	GPIOC_PSOR = 0x04;
	_delay_us(1);
	GPIOC_PCOR = 0x04;
}

// Initializes the LCD
//
void Lcd_Init()
{
	InitGPIO();
	
	GPIOC_PCOR = 0x06;
	
	GPIOB_PCOR = 0x0f;
    _delay_ms(40);

    GPIOB_PSOR = 0x03;
    ENPulse();
    _delay_ms(6);

    ENPulse();
    _delay_ms(6);

    ENPulse();
    _delay_ms(6);

    Lcd_Cmd(0x28);
    _delay_us(50);

    Lcd_Cmd(0x28);
    _delay_us(50);

    Lcd_Cmd(0x0C);
    _delay_us(50);

    Lcd_Cmd(0x01);
    _delay_ms(4);
}

// writes a command, such as clearing the screen
// or positioning the cursor
//
void Lcd_Cmd(byte cmd)
{
    GPIOC_PCOR = 0x06;
    
    GPIOB_PCOR = 0x0f;
    GPIOB_PSOR = cmd >> 4;
    ENPulse();

    GPIOB_PCOR = 0x0f;
    GPIOB_PSOR = cmd & 0x0f;
    ENPulse();
    
    _delay_us(50);
}

// writes a data byte (a character to display)
//
void Lcd_Data(byte data)
{
    GPIOC_PCOR = 0x06;
	GPIOC_PSOR = 0x02;

	GPIOB_PCOR = 0x0f;
    GPIOB_PSOR = data >> 4;
    ENPulse();

    GPIOB_PCOR = 0x0f;
    GPIOB_PSOR = data & 0x0F;
    ENPulse();
    
    _delay_us(50);
}

// writes a single character
// if row and col are < 0, the cursor is not set i.e. character will be 
// written where the cursor last ended up
//
// if row and col are >= 0, then the cursor is positioned at that
// row and column before writing. NOTE: row and col are 0-based i.e. upper-left
// corner is 0,0 not 1,1
//
void Lcd_PutCh(int row, int col, char ch)
{
    int		pos;

    if (row >= 0 && col >= 0)
    {
		switch (row)
		{
			case 0:
				break;
			case 1:
				row = 0x40;
				break;
			case 2:
				row = 0x14;
				break;
			case 3:
				row = 0x54;
				break;
			default:
				row = 0;
				break;
		}
        if (col > 19)	// too long
            col = 0;

        pos = (row + col) | 0x80;		// 0x80 is cmd for positioning cursor
        Lcd_Cmd(pos);
    }

	Lcd_Data(ch);
}

// writes a string of characters. same as Lcd_PutCh for positioning
// cursor
//
void Lcd_PutStr(int row, int col, char* str)
{
    int		pos;

    if (row >= 0 && col >= 0)
    {
		switch (row)
		{
			case 0:
				break;
			
			case 1:
				row = 0x40;
				break;

			default:
				row = 0;
				break;
		}
		
        if (col > 15)	// too long
            col = 0;

        pos = (row + col) | 0x80;		// 0x80 is cmd for positioning cursor
        Lcd_Cmd(pos);
    }

    while (*str)
    {
        Lcd_Data(*str);
        str++;
    }
}
int main(void)
{
	for(;;){
	}

	return 0;
}
