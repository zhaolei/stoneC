#include "1602.h"


void lcd_init()
{
        LCD_RS_DDR |= LCD_RS;
        LCD_RS_PORT |= LCD_RS;

        LCD_RW_DDR |= LCD_RW; 
        LCD_RW_PORT |= LCD_RW;

        LCD_E_DDR |= LCD_E; 
        LCD_E_PORT |= LCD_E;

        LCD_DATA_PORT = 0xff;
        LCD_DATA_DDR = 0xff;

        lcd_write_command(0x38);

        lcd_write_command(0x0C);

        lcd_write_command(0x06);
 
        lcd_write_command(0x01);
}

void lcd_clear()
{
        //lcd_write_command(0x01);
        lcd_write_command(0x38);

        lcd_write_command(0x0C);

        lcd_write_command(0x06);
 
        lcd_write_command(0x01);
}

void lcd_write_command(int cmd)
{
        //PORTC = 0x00;
        LCD_RS_PORT &= ~LCD_RS;
        LCD_RW_PORT &= ~LCD_RW;

        LCD_DATA_PORT = cmd;
        lcd_run();
}

void lcd_write_data(int cmd)
{
        //PORTC = 0x00;
        LCD_RS_PORT |= LCD_RS;
        LCD_RW_PORT &= ~LCD_RW;

        LCD_DATA_PORT = cmd;
        lcd_run();
}

void lcd_run()
{
        LCD_E_PORT |= LCD_E;
        _delay_ms(10);
        LCD_E_PORT ^= LCD_E ;
}

void lcd_write_string(char *c, unsigned char x, unsigned char y)
{
        while(*c)
        {
                lcd_write_char(*c, x, y);
                c++;
                y++;
        }
}

void lcd_write_char(unsigned char c, unsigned char x, unsigned char y)
{
        int cmd = _BV(7);
        cmd += y;
        if(x > 1) {
                cmd +=_BV(6);
        }

        lcd_write_command(cmd);

        lcd_write_data(c);
}
