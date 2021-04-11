//main.c File 
#include <16F887.h> 
#FUSES XT 
#use delay(crystal=4000000)//Frequencia em 4Mhz

//Nota:Pode conectar o modulo MFRC522 em um PIC 5v sem conversor de tensão.
//Nota:mfrc522 trabalha em SPI slave mode(PIC é o MASTER) 
//Conexões RC522 
#define CS PIN_D3//mfrc 522 chip select pin(SS) to PIC pin3 
//MFRC522 module RST  pin to +3.3v 
//MFRC522 module MOSI pin to PIC16f887 pin 24(RC5/SDO) 
//MFRC522 module MISO pin to PIC16f887 pin 23(RC4/SDI) 
//MFRC522 module SCK  pin to PIC16f887 pin 18(RC3/SCK) 

#include <MFRC522_driver.h>//mfrc 522 Driver 

//spi modes 
#define SPI_MODE_0  (SPI_L_TO_H | SPI_XMIT_L_TO_H) 
#define SPI_MODE_1  (SPI_L_TO_H) 
#define SPI_MODE_2  (SPI_H_TO_L) 
#define SPI_MODE_3  (SPI_H_TO_L | SPI_XMIT_L_TO_H) 

byte FoundTag; // Variavel usada para checar se uma tag qualquer foi encontrada
byte ReadTag; // Variavel usada para guardar valor anti-collision 
byte TagData[16]; // Usado para guardar todo o dado do tag
//Você pode criar ua função para descobrir o código da sua tag e printar em uma tela se necessário
byte GoodTagSerialNumber[4] = {0xa7, 0x42, 0x21, 0xb2}; // Codigo do tag que estamos procurando 
//byte GoodTagSerialNumber[4] = {0xda, 0xcc, 0x8e, 0x19}; 
int i=0; 
byte version; 
int GoodTag=0; // Varivel usada para guardar que A tag correta foi encontrada 


//Lê qualquer tag
void Get_UID() 
{ 
   // Get Tag UID 
   ReadTag = antiCollision(TagData);   
} 

//Procura por tag específica
void Search_for_matching_Tag() { 
 GoodTag=0;
 int i;
 for(i=0; i <= 3; i++) { 
    if (GoodTagSerialNumber[i] != TagData[i]) { 
         GoodTag=0; 
          break; // se não for igual, saia do loop 
       } 
   } 
   
   // Se passamos pelo loop as 4 vezes, então encontramos o codigo correto
   if (TagData[3]==GoodTagSerialNumber[3]) {  
      GoodTag=1; 
   } 
   
   //Aciona porta da lampada
   if (GoodTag == 1){ 
      output_high(pin_d0);
      delay_ms(5000);
      output_low(pin_d0);
   }else { 
     output_low(pin_d0);
     GoodTag=0;      
   } 
} 

void main() { 
   enable_interrupts(GLOBAL);
   enable_interrupts(INT_RDA);
   setup_spi(SPI_MASTER|SPI_MODE_0|SPI_CLK_DIV_64);  
   delay_ms(50); 
   
   MFRC_522_init();
   
   delay_ms(2000);  
   
   while(TRUE){      
     //  Chaca se uma Tag foi encontrada 
     // Se encontrar, FoundTag vai conter "MI_OK"      
    FoundTag = requestTag(MF1_REQIDL, TagData);   
     
    delay_ms(300);
     if (FoundTag == MI_OK){  
        delay_ms(1000); 
        Get_UID(); 
        Search_for_matching_Tag(); 
        delay_ms(1000);  
     } 
      
   } 
}
