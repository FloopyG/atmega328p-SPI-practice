#include <avr/io.h>
#include <util/delay.h>


// Initalizing array with the bit-representation of the sentece that will be displayed on MAX7219
uint8_t text[26] = {0b00111111,
                  0b00001000,
                  0b00001000,
                  0b00111111,
                  0b00000000,
                  0b00111111,
                  0b00101001,
                  0b00101001,
                  0b00111001,
                  0b00000000,
                  0b00111111,
                  0b00000001,
                  0b00000001,
                  0b00000000,
                  0b00111111,
                  0b00000001,
                  0b00000001,
                  0b00000000,
                  0b00111111,
                  0b00100001,
                  0b00100001,
                  0b00111111,
                  0b00000000,
                  0b00000000,
                  0b00000000,
                  0b00000000,
                  0b00000000
                  };
uint8_t buffer[8];

void initSPI(void) 
{
  DDRB |= (1 << PB2);     // Set SS output 
  PORTB |= (1 << PB2);      // Begin high (unselected)

  DDRB |= (1 << PB3);       // Output on MOSI 
  DDRB |= (1 << PB5);       // Output on SCK 

  SPCR |= (1 << MSTR);      // Clockmaster 
  SPCR |= (1 << SPE);       // Enable SPI
}

void sendDataMAX7219(uint8_t addr, uint8_t data)
{
  // CS = 0
  PORTB &= ~(1 << PB2);
  // Start addres transmit
  SPDR = addr;
  // Wait untill transmition end
  while(!(SPSR & (1<<SPIF)));
  // Start data transmit
  SPDR = data;
  // Wait untill transmition end
  while(!(SPSR & (1<<SPIF)));
  // CS = 1
  PORTB |= (1 << PB2);
}

void initMAX7219(void)
{
  DDRB |= 1 << PB2; // CS - out
  sendDataMAX7219(0x0F, 0); // normal mode
  sendDataMAX7219(0x09, 0); // no decode
  sendDataMAX7219(0x0B, 7); // set 8x8 row/col
  sendDataMAX7219(0x0A, 5); // set intensity 5
  sendDataMAX7219(0x0C, 1); // power on  
// очистка дисплея
  for (uint8_t i = 0; i < 8; i++)
  {
    sendDataMAX7219(i+1, 0);
  }
}

void printBuffer(void)
{
  for(uint8_t i = 0; i < 8; i++)
  {
    sendDataMAX7219(8 - i, buffer[i]);
  }
}


void pushBuffer(uint8_t x) 
{
  for (uint8_t i = 0; i < 7; i++)
  {
    buffer[i] = buffer[i + 1];
  }
  buffer[7] = x;
}

void pushChar (uint8_t c[])
{
  for (uint8_t i = 0; i < 26; i++)
  {
    pushBuffer(c[i]);
    printBuffer();
    _delay_ms(100);
  }
}


int main(void)
{
  initSPI();
  initMAX7219();
 
  while (1)
  {
    pushChar(text);
  }
  
  
  

return 0;
}
