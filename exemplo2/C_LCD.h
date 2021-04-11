//C_LCD.h file 
#define _lcd_h_ 
#include <math.h> 
void lcd_init(void);              
byte lcd_read_byte(void);          
byte lcd_read_nibble(void);        
void lcd_send_byte(byte address, byte n);  
void lcd_send_nibble(byte n);    
void lcd_gotoxy(byte x, byte y); 
char lcd_getc(byte x, byte y);    
void lcd_putc(char c);            
//Define LCD connection Pins 
#define LCD_RS_PIN         PIN_A1    
#define LCD_RW_PIN         PIN_A2 
#define LCD_ENABLE_PIN     PIN_A3 
#define LCD_DATA4          PIN_A4 
#define LCD_DATA5          PIN_A5 
#define LCD_DATA6          PIN_E0 
#define LCD_DATA7          PIN_E1 

#define lcd_output_enable(x)   output_bit(LCD_ENABLE_PIN, x)  
#define lcd_enable_tris()   output_drive(LCD_ENABLE_PIN) 

#define lcd_output_rs(x)   output_bit(LCD_RS_PIN, x)  
#define lcd_rs_tris()      output_drive(LCD_RS_PIN) 

#define lcd_output_rw(x)   output_bit(LCD_RW_PIN, x) 
#define lcd_rw_tris()      output_drive(LCD_RW_PIN) 

#define lcd_line_one   0x00    
#define lcd_line_two   0x40    
#define lcd_line_three   0x14 
#define lcd_line_four   0x54 
#define LCD_TYPE 0x02          


byte const LCD_INIT_STRING[4] = {0x28 | (LCD_TYPE << 2), 0x0C, 0x01, 0x06}; 
void lcd_init(void)    
{ 

byte i; 

output_drive(LCD_DATA4);  
output_drive(LCD_DATA5); 
output_drive(LCD_DATA6); 
output_drive(LCD_DATA7); 

lcd_enable_tris(); 
lcd_rs_tris(); 
lcd_rw_tris(); 

lcd_output_rs(0);      
lcd_output_rw(0);        
lcd_output_enable(0);    

delay_ms(15); 

for(i=1;i<=3;i++)        
   {                
   lcd_send_nibble(0x03);    
   delay_ms(5);        
   }                
                    

lcd_send_nibble(0x02);      
for(i=0;i<=3;i++) 
   lcd_send_byte(0,LCD_INIT_STRING[i]); 
}                            

// *************************************************** 
byte lcd_read_byte(void)    
{ 
byte low,high; 
output_float(LCD_DATA4);  
output_float(LCD_DATA5); 
output_float(LCD_DATA6); 
output_float(LCD_DATA7); 

lcd_output_rw(1);        
delay_cycles(1);        
lcd_output_enable(1);      
delay_cycles(1);          
high = lcd_read_nibble();  
lcd_output_enable(0);      
                
delay_cycles(1);        
lcd_output_enable(1);      
delay_cycles(1);          
low = lcd_read_nibble();    
lcd_output_enable(0);      

output_drive(LCD_DATA4);    
output_drive(LCD_DATA5); 
output_drive(LCD_DATA6); 
output_drive(LCD_DATA7); 

return((high<<4) | low);    
} 

// *************************************************** 
byte lcd_read_nibble(void)    
{ 

byte n = 0x00;  

n |= input(LCD_DATA4);        
n |= input(LCD_DATA5) << 1; 
n |= input(LCD_DATA6) << 2; 
n |= input(LCD_DATA7) << 3; 

return(n);    
} 

// *************************************************** 
void lcd_send_byte(byte address, byte n)    
{                                

lcd_output_rs(0);      
while(bit_test(lcd_read_byte(),7));  
lcd_output_rs(address);  
delay_cycles(1);    
lcd_output_rw(0);    
delay_cycles(1);      
lcd_output_enable(0);    
lcd_send_nibble(n >> 4);  
lcd_send_nibble(n & 0x0F);  
} 

// *************************************************** 
void lcd_send_nibble(byte n)  
{ 

output_bit(LCD_DATA4, bit_test(n, 0));  
output_bit(LCD_DATA5, bit_test(n, 1)); 
output_bit(LCD_DATA6, bit_test(n, 2)); 
output_bit(LCD_DATA7, bit_test(n, 3)); 
    
delay_cycles(1);      
lcd_output_enable(1);  
delay_us(2);          
lcd_output_enable(0);    
} 

// *************************************************** 
void lcd_gotoxy(byte x, byte y)  
{ 

byte address; 

if(y==1)              
   address=lcd_line_one;      
else if(y==2)      
   address=lcd_line_two;      
else if(y==3) 
   address=lcd_line_three;      
else if(y==4) 
   address=lcd_line_four;    

address+=x-1;              
lcd_send_byte(0,0x80|address);  
}                      

// *************************************************** 
char lcd_getc(byte x, byte y) 
{ 

char value; 

lcd_gotoxy(x,y);            
while(bit_test(lcd_read_byte(),7));  
lcd_output_rs(1);        
value = lcd_read_byte();  
lcd_output_rs(0);        

return(value);    
} 

// *************************************************** 
void lcd_putc(char c)  
{ 

switch (c) 
   { 
   case '\f':    
      lcd_send_byte(0,0x01);  
      delay_ms(2);      
      break; 
   case '\n':      
      lcd_gotoxy(1,0x02);      
      break; 
    case '\b':      
      lcd_send_byte(0,0x10); 
      break;            
   default:        
      lcd_send_byte(1,c);    
      break; 
   } 
}      
// *************************************************** 
