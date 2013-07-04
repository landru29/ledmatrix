#ifndef __ARIAL8_H
#define __ARIAL8_H
#include "../config.h"

char arial8_mapping[] =
	"!\"#$\%&'()*+'-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[ ]^_`abcdefghijklmnopqrstuvwxyz{|}~";

unsigned char arial8_font[] = {
    // @0 '!' (1 pixels wide)
    0b11111101,

    // @8 '"' (4 pixels wide)
    0b11100000,
    0b00000000,
    0b00000000,
    0b11100000,

    // @16 '#' (9 pixels wide)
    0b00100100,
    0b00100111,
    0b00111100,
    0b11100100,
    0b00100100,
    0b00100111,
    0b00111100,
    0b11100100,
    0b00100100,

    // @32 '$' (7 pixels wide)
    0b00110100,
    0b01001010,
    0b01001010,
    0b11111111,
    0b01001010,
    0b01001010,
    0b00100100,

    // @40 '%' (12 pixels wide)
    0b01100000,
    0b10010000,
    0b10010000,
    0b10010001,
    0b01100010,
    0b00001100,
    0b00110000,
    0b01000110,
    0b10001001,
    0b00001001,
    0b00001001,
    0b00000110,

    // @56 '&' (9 pixels wide)
    0b00000100,
    0b01001010,
    0b10110001,
    0b10010001,
    0b10101001,
    0b01000110,
    0b00000000,
    0b00001010,
    0b00000001,

    // @72 ''' (1 pixels wide)
    0b11100000,

    // @80 '(' (3 pixels wide)
    0b00011000,
    0b01100110,
    0b10000001,

    // @88 ')' (3 pixels wide)
    0b10000001,
    0b01100110,
    0b00011000,

    // @96 '*' (5 pixels wide)
    0b00100000,
    0b00101100,
    0b01110000,
    0b00101100,
    0b00100000,

    // @104 '+' (7 pixels wide)
    0b00001000,
    0b00001000,
    0b00001000,
    0b00111110,
    0b00001000,
    0b00001000,
    0b00001000,

    // @112 ',' (3 pixels wide)
    0b00000010,
    0b00000100,
    0b00001000,

    // @120 '-' (4 pixels wide)
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,

    // @128 '.' (1 pixels wide)
    0b00000010,

    // @136 '/' (4 pixels wide)
    0b00000011,
    0b00001100,
    0b00110000,
    0b11000000,

    // @144 '0' (7 pixels wide)
    0b00111100,
    0b01000010,
    0b10000001,
    0b10000001,
    0b10000001,
    0b01000010,
    0b00111100,

    // @152 '1' (4 pixels wide)
    0b00010000,
    0b00100000,
    0b01000000,
    0b11111111,

    // @160 '2' (6 pixels wide)
    0b01000001,
    0b10000011,
    0b10000101,
    0b10001001,
    0b01010001,
    0b00100001,

    // @168 '3' (7 pixels wide)
    0b00000110,
    0b01000010,
    0b10010001,
    0b10010001,
    0b10110001,
    0b01101010,
    0b00000100,

    // @176 '4' (8 pixels wide)
    0b00000100,
    0b00001100,
    0b00010100,
    0b00100100,
    0b01000100,
    0b10000100,
    0b11111111,
    0b00000100,

    // @184 '5' (7 pixels wide)
    0b11110100,
    0b10010010,
    0b10010001,
    0b10010001,
    0b10010001,
    0b10001010,
    0b00000100,

    // @192 '6' (7 pixels wide)
    0b00111100,
    0b01000010,
    0b10010001,
    0b10010001,
    0b10010001,
    0b01010010,
    0b00001100,

    // @200 '7' (6 pixels wide)
    0b10000000,
    0b10000000,
    0b10000011,
    0b10001100,
    0b10010000,
    0b11100000,

    // @208 '8' (7 pixels wide)
    0b00000100,
    0b01101010,
    0b10010001,
    0b10010001,
    0b10010001,
    0b01101010,
    0b00000100,

    // @216 '9' (7 pixels wide)
    0b00100000,
    0b01010010,
    0b10001001,
    0b10001001,
    0b10001001,
    0b01010010,
    0b00111100,

    // @224 ':' (1 pixels wide)
    0b00010010,

    // @232 ';' (3 pixels wide)
    0b00000010,
    0b00000100,
    0b00101000,

    // @240 '<' (7 pixels wide)
    0x00, //        
    0x06, //      OO
    0x18, //    OO  
    0x60, //  OO    
    0x80, // O      
    0x60, //  OO    
    0x18, //    OO
    0x06, //      OO  

    // @248 '=' (7 pixels wide)
    0x00, //        
    0x00, //        
    0xFE, // OOOOOOO
    0x00, //        
    0x00, //        
    0xFE, // OOOOOOO
    0x00, //        
    0x00, //        

    // @256 '>' (7 pixels wide)
    0x00, //        
    0xD0, // OO
    0x60, //   OO
    0x0D, //     OO
    0x02, //       O
    0x0D, //     OO
    0x60, //   OO
    0xD0, // OO

    // @264 '?' (7 pixels wide)
    0x38, //   OOO  
    0x44, //  O   O 
    0x82, // O     O
    0x04, //      O 
    0x10, //    O   
    0x10, //    O   
    0x00, //        
    0x10, //    O   

    // @272 '@' (15 pixels wide)
    0x07, 0xE0, //      OOOOOO    
    0x18, 0x18, //    O        O  
    0x44, 0x62, //  O  OO O O    O
    0x90, 0x22, // O  O      O   O
    0x88, 0xC8, // O   O   OO  O  
    0x47, 0x70, //  O   OOO OOO   
    0x18, 0x0C, //    O         O 
    0x07, 0xF0, //      OOOOOOO   

    // @288 'A' (9 pixels wide)
    0x08, 0x00, //     O    
    0x14, 0x00, //    O O   
    0x22, 0x00, //   O   O  
    0x22, 0x00, //   O   O  
    0x7F, 0x00, //  OOOOOOO 
    0x41, 0x00, //  O     O 
    0x80, 0x80, // O       O
    0x80, 0x80, // O       O

    // @304 'B' (9 pixels wide)
    0xFF, 0x00, // OOOOOOOO 
    0x80, 0x80, // O       O
    0x81, 0x00, // O      O 
    0xFE, 0x00, // OOOOOOO  
    0x81, 0x00, // O      O 
    0x80, 0x80, // O       O
    0x81, 0x00, // O      O 
    0xFE, 0x00, // OOOOOOO  

    // @320 'C' (10 pixels wide)
    0x1F, 0x00, //    OOOOO  
    0x20, 0x80, //   O     O 
    0x40, 0x40, //  O       O
    0x80, 0x00, // O         
    0x80, 0x00, // O         
    0x40, 0x40, //  O       O
    0x20, 0x80, //   O     O 
    0x1F, 0x00, //    OOOOO  

    // @336 'D' (10 pixels wide)
    0xFE, 0x00, // OOOOOOO   
    0x81, 0x00, // O      O  
    0x80, 0x80, // O       O 
    0x80, 0x40, // O        O
    0x80, 0x40, // O        O
    0x80, 0x80, // O       O 
    0x81, 0x00, // O      O  
    0xFE, 0x00, // OOOOOOO   

    // @352 'E' (9 pixels wide)
    0xFF, 0x80, // OOOOOOOOO
    0x80, 0x00, // O        
    0x80, 0x00, // O        
    0xFF, 0x00, // OOOOOOOO 
    0x80, 0x00, // O        
    0x80, 0x00, // O        
    0x80, 0x00, // O        
    0xFF, 0x80, // OOOOOOOOO

    // @368 'F' (8 pixels wide)
    0xFF, // OOOOOOOO
    0x80, // O       
    0x80, // O       
    0xFE, // OOOOOOO 
    0x80, // O       
    0x80, // O       
    0x80, // O       
    0x80, // O       

    // @376 'G' (10 pixels wide)
    0x1E, 0x00, //    OOOO   
    0x21, 0x00, //   O    O  
    0x40, 0x80, //  O      O 
    0x80, 0x00, // O         
    0x87, 0xC0, // O    OOOOO
    0x80, 0x40, // O        O
    0x21, 0x00, //   O    O  
    0x1E, 0x00, //    OOOO   

    // @392 'H' (9 pixels wide)
    0x80, 0x80, // O       O
    0x80, 0x80, // O       O
    0x80, 0x80, // O       O
    0xFF, 0x80, // OOOOOOOOO
    0x80, 0x80, // O       O
    0x80, 0x80, // O       O
    0x80, 0x80, // O       O
    0x80, 0x80, // O       O

    // @408 'I' (1 pixels wide)
    0xF8, // OOOOO
    0x20, //   O
    0x20, //   O
    0x20, //   O
    0x20, //   O
    0x20, //   O
    0x20, //   O
    0xF8, // OOOOO

    // @416 'J' (7 pixels wide)
    0x3E, //   OOOOO
    0x08, //     O
    0x08, //     O
    0x08, //     O
    0x08, //     O
    0x88, // O   O
    0x88, // O   O
    0x70, //  OOO 

    // @424 'K' (9 pixels wide)
    0x80, 0x80, // O       O
    0x81, 0x00, // O      O 
    0x84, 0x00, // O    O   
    0x98, 0x00, // O  OO    
    0xA8, 0x00, // O O O    
    0x82, 0x00, // O     O  
    0x81, 0x00, // O      O 
    0x80, 0x80, // O       O

    // @440 'L' (7 pixels wide)
    0x80, // O      
    0x80, // O      
    0x80, // O      
    0x80, // O      
    0x80, // O      
    0x80, // O      
    0x80, // O      
    0xFE, // OOOOOOO

    // @448 'M' (11 pixels wide)
    0x80, 0x20, // O         O
    0xC0, 0x60, // OO       OO
    0xC0, 0x60, // OO       OO
    0xA0, 0xA0, // O O     O O
    0x91, 0x20, // O  O   O  O
    0x8A, 0x20, // O   O O   O
    0x84, 0x20, // O    O    O
    0x8 , 0x20, // O         O

    // @464 'N' (9 pixels wide)
    0x80, 0x80, // O       O
    0xC0, 0x80, // OO      O
    0xA0, 0x80, // O O     O
    0x90, 0x80, // O  O    O
    0x84, 0x80, // O    O  O
    0x82, 0x80, // O     O O
    0x81, 0x80, // O      OO
    0x80, 0x80, // O       O

    // @480 'O' (10 pixels wide)
    0x1E, 0x00, //    OOOO   
    0x40, 0x80, //  O      O 
    0x80, 0x40, // O        O
    0x80, 0x40, // O        O
    0x40, 0x80, //  O      O 
    0x21, 0x00, //   O    O  
    0x1E, 0x00, //    OOOO   

    // @496 'P' (9 pixels wide)
    0xFE, 0x00, // OOOOOOO  
    0x81, 0x00, // O      O 
    0x80, 0x80, // O       O
    0x81, 0x00, // O      O 
    0xFE, 0x00, // OOOOOOO  
    0x80, 0x00, // O        
    0x80, 0x00, // O        
    0x80, 0x00, // O        

    // @512 'Q' (10 pixels wide)
    0x1E, 0x00, //    OOOO   
    0x21, 0x00, //   O    O  
    0x40, 0x80, //  O      O 
    0x80, 0x40, // O        O
    0x80, 0x40, // O        O
    0x46, 0xC0, //  O   OO OO
    0x21, 0x80, //   O    OO 
    0x1E, 0xC0, //    OOOO OO

    // @528 'R' (9 pixels wide)
    0xFE, 0x00, // OOOOOOO  
    0x81, 0x00, // O      O 
    0x80, 0x80, // O       O
    0x81, 0x00, // O      O 
    0xFE, 0x00, // OOOOOOO  
    0x84, 0x00, // O    O   
    0x81, 0x00, // O      O 
    0x80, 0x80, // O       O

    // @544 'S' (9 pixels wide)
    0x3E, 0x00, //   OOOOO  
    0x41, 0x00, //  O     O 
    0x40, 0x00, //  O       
    0x38, 0x00, //   OOO    
    0x07, 0x00, //      OOO 
    0x80, 0x80, // O       O
    0x41, 0x00, //  O     O 
    0x3E, 0x00, //   OOOOO  

    // @560 'T' (7 pixels wide)
    0xFD, // OOOOOOO
    0x01, //    O    
    0x01, //    O    
    0x01, //    O    
    0x01, //    O    
    0x01, //    O    
    0x01, //    O    
    0x01, //    O    

    // @576 'U' (9 pixels wide)
    0x80, 0x80, // O       O
    0x80, 0x80, // O       O
    0x80, 0x80, // O       O
    0x80, 0x80, // O       O
    0x80, 0x80, // O       O
    0x80, 0x80, // O       O
    0x41, 0x00, //  O     O 
    0x3E, 0x00, //   OOOOO  

    // @592 'V' (9 pixels wide)
    0x80, 0x80, // O       O
    0x41, 0x00, //  O     O 
    0x41, 0x00, //  O     O 
    0x22, 0x00, //   O   O  
    0x22, 0x00, //   O   O  
    0x14, 0x00, //    O O   
    0x08, 0x00, //     O    
    0x08, 0x00, //     O    

    // @608 'W' (15 pixels wide)
    0x81, 0x02, // O      O      O
    0x82, 0x82, // O     O O     O
    0x42, 0x84, //  O    O O    O 
    0x44, 0x44, //  O   O   O   O 
    0x24, 0x48, //   O  O   O  O  
    0x28, 0x28, //   O O     O O  
    0x10, 0x10, //    O       O   
    0x10, 0x10, //    O       O   

    // @624 'X' (11 pixels wide)
    0x40, 0x40, //  O       O 
    0x20, 0x80, //   O     O  
    0x0A, 0x00, //     O O    
    0x04, 0x00, //      O     
    0x0A, 0x00, //     O O    
    0x11, 0x00, //    O   O   
    0x40, 0x40, //  O       O 
    0x80, 0x20, // O         O

    // @640 'Y' (9 pixels wide)
    0x80, 0x80, // O       O
    0x41, 0x00, //  O     O 
    0x22, 0x00, //   O   O  
    0x14, 0x00, //    O O   
    0x08, 0x00, //     O    
    0x08, 0x00, //     O    
    0x08, 0x00, //     O    
    0x08, 0x00, //     O    

    // @656 'Z' (9 pixels wide)
    0x7F, 0x80, //  OOOOOOOO
    0x01, 0x00, //        O 
    0x02, 0x00, //       O  
    0x04, 0x00, //      O   
    0x08, 0x00, //     O    
    0x10, 0x00, //    O     
    0x20, 0x00, //   O      
    0xFF, 0x80, // OOOOOOOOO

    // @672 '[' (3 pixels wide)
    0xE0, // OOO
    0x80, // O  
    0x80, // O  
    0x80, // O  
    0x80, // O  
    0x80, // O  
    0x80, // O  
    0xE0, // OOO

    // @680 '\' (4 pixels wide)
    0x80, // O   
    0x80, // O   
    0x40, //  O  
    0x40, //  O  
    0x20, //   O 
    0x20, //   O 
    0x10, //    O
    0x10, //    O

    // @688 ']' (3 pixels wide)
    0xE0, // OOO
    0x20, //   O
    0x20, //   O
    0x20, //   O
    0x20, //   O
    0x20, //   O
    0x20, //   O
    0xE0, // OOO

    // @696 '^' (7 pixels wide)
    0x10, //    O   
    0x28, //   O O  
    0x44, //  O   O 
    0x82, // O     O
    0x00, //        
    0x00, //        
    0x00, //        
    0x00, //        

    // @704 '_' (9 pixels wide)
    0x00, 0x00, //          
    0x00, 0x00, //          
    0x00, 0x00, //          
    0x00, 0x00, //          
    0x00, 0x00, //          
    0x00, 0x00, //          
    0x00, 0x00, //          
    0xFF, 0x80, // OOOOOOOOO

    // @720 '`' (2 pixels wide)
    0x00, //   
    0x80, // O 
    0x40, //  O
    0x00, //   
    0x00, //   
    0x00, //   
    0x00, //   
    0x00, //   

    // @728 'a' (7 pixels wide)
    0x00, //        
    0x00, //        
    0x3C, //   OOOO 
    0x42, //  O    O
    0x0E, //     OOO
    0x72, //  OOO  O
    0x86, // O    OO
    0x7A, //  OOOO O

    // @736 'b' (6 pixels wide)
    0x00, //        
    0x00, //        
    0x80, // O      
    0x80, // O      
    0xB8, // O OOO  
    0xC4, // OO   O 
    0xC4, // OO   O 
    0xB8, // O OOO  

    // @744 'c' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x38, //   OOO 
    0x44, //  O   O
    0x80, // O     
    0x80, // O     
    0x44, //  O   O
    0x38, //   OOO 

    // @752 'd' (7 pixels wide)
    0x00, //        
    0x00, //        
    0x02, //       O
    0x02, //       O
    0x3A, //   OOO O
    0x46, //  O   OO
    0x46, //  O   OO
    0x3A, //   OOO O

    // @760 'e' (7 pixels wide)
    0x00, //        
    0x00, //        
    0x38, //   OOO  
    0x44, //  O   O 
    0xFE, // OOOOOOO
    0x82, // O      
    0x44, //  O   O 
    0x38, //   OOO  

    // @768 'f' (4 pixels wide)
    0x00, //     
    0x00, //     
    0x30, //   OO
    0x40, //  O  
    0xF0, // OOOO
    0x40, //  O  
    0x40, //  O  
    0x40, //  O  

    // @776 'g' (7 pixels wide)
    0x00, //        
    0x00, //        
    0x3A, //   OOO O
    0x46, //  O   OO
    0x82, // O     O
    0x3A, //   OOO O
    0x84, // O    O 
    0x78, //  OOOO  

    // @784 'h' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x80, // O     
    0x80, // O     
    0xB8, // O OOO 
    0xC4, // OO   O
    0x84, // O    O
    0x84, // O    O

    // @792 'i' (1 pixels wide)
    0x00, //  
    0x00, //  
    0x80, // O
    0x00, //  
    0x80, // O
    0x80, // O
    0x80, // O
    0x80, // O

    // @800 'j' (3 pixels wide)
    0x00, //    
    0x00, //    
    0x20, //   O
    0x00, //    
    0x20, //   O
    0x20, //   O
    0x20, //   O
    0xC0, // OO 

    // @808 'k' (7 pixels wide)
    0x00, //        
    0x00, //        
    0x80, // O      
    0x88, // O   O  
    0x90, // O  O   
    0xB0, // O OO   
    0x84, // O    O 
    0x82, // O     O

    // @816 'l' (2 pixels wide)
    0x00, //  
    0x00, //  
    0x80, // O
    0x80, // O
    0x80, // O
    0x80, // O
    0x80, // O
    0x80, //  O

    // @824 'm' (11 pixels wide)
    0x00, 0x00, //            
    0x00, 0x00, //            
    0xB9, 0xC0, // O OOO  OOO 
    0xC6, 0x20, // OO   OO   O
    0x84, 0x20, // O    O    O
    0x84, 0x20, // O    O    O
    0x84, 0x20, // O    O    O
    0x84, 0x20, // O    O    O

    // @840 'n' (6 pixels wide)
    0x00, //       
    0x00, //       
    0xB8, // O OOO 
    0xC4, // OO   O
    0x84, // O    O
    0x84, // O    O
    0x84, // O    O
    0x84, // O    O

    // @848 'o' (7 pixels wide)
    0x00, //        
    0x00, //        
    0x38, //   OOO  
    0x44, //  O   O 
    0x82, // O     O
    0x82, // O     O
    0x44, //  O   O 
    0x38, //   OOO  

    // @856 'p' (6 pixels wide)
    0x00, //        
    0x00, //        
    0xB8, // O OOO  
    0xC4, // OO   O 
    0xC4, // OO   O 
    0xB8, // O OOO  
    0x80, // O      
    0x80, // O      

    // @864 'q' (7 pixels wide)
    0x00, //        
    0x00, //        
    0x3A, //   OOO O
    0x46, //  O   OO
    0x46, //  O   OO
    0x3A, //   OOO O
    0x02, //       O
    0x02, //       O

    // @872 'r' (4 pixels wide)
    0x00, //     
    0x00, //     
    0xB0, // O OO
    0xC0, // OO  
    0x80, // O   
    0x80, // O   
    0x80, // O   
    0x80, // O   

    // @880 's' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x78, //  OOOO 
    0x84, // O    O
    0x80, // O     
    0x78, //  OOOO 
    0x84, // O    O
    0x78, //  OOOO 

    // @888 't' (4 pixels wide)
    0x00, //     
    0x00, //     
    0x40, //  O  
    0x40, //  O  
    0xF0, // OOOO
    0x40, //  O  
    0x40, //  O  
    0x70, //  OOO

    // @896 'u' (6 pixels wide)
    0x00, //       
    0x00, //       
    0x84, // O    O
    0x84, // O    O
    0x84, // O    O
    0x84, // O    O
    0x8C, // O   OO
    0x74, //  OOO O

    // @904 'v' (7 pixels wide)
    0x00, //        
    0x00, //        
    0x82, // O     O
    0x82, // O     O
    0x44, //  O   O 
    0x44, //  O   O 
    0x28, //   O O  
    0x10, //    O   

    // @912 'w' (11 pixels wide)
    0x00, 0x00, //            
    0x00, 0x00, //            
    0x84, 0x20, // O    O    O
    0x84, 0x20, // O    O    O
    0x4A, 0x40, //  O  O O  O 
    0x51, 0x40, //  O O   O O 
    0x20, 0x80, //   O     O  
    0x20, 0x80, //   O     O  

    // @928 'x' (7 pixels wide)
    0x00, //        
    0x00, //        
    0x82, // O     O
    0x44, //  O   O 
    0x28, //   O O  
    0x10, //    O   
    0x44, //  O   O 
    0x82, // O     O

    // @936 'y' (7 pixels wide)
    0x00, //        
    0x00, //        
    0x82, // O     O
    0x44, //  O   O 
    0x28, //   O O  
    0x28, //   O O  
    0x10, //    O   
    0x60, //  OO    

    // @944 'z' (7 pixels wide)
    0x00, //        
    0x00, //        
    0xFE, // OOOOOOO
    0x04, //      O 
    0x08, //     O  
    0x10, //    O   
    0x20, //   O    
    0xFE, // OOOOOOO

    // @952 '{' (5 pixels wide)
    0x18, //    OO
    0x20, //   O  
    0x20, //   O  
    0x20, //   O  
    0xC0, // OO   
    0x20, //   O  
    0x20, //   O  
    0x18, //    OO

    // @960 '|' (1 pixels wide)
    0x80, // O
    0x80, // O
    0x80, // O
    0x80, // O
    0x80, // O
    0x80, // O
    0x80, // O
    0x80, // O

    // @968 '}' (5 pixels wide)
    0xC0, // OO   
    0x20, //   O  
    0x20, //   O  
    0x20, //   O  
    0x18, //    OO
    0x20, //   O  
    0x20, //   O  
    0xC0, // OO   

    // @976 '~' (8 pixels wide)
    0x00, //         
    0x00, //         
    0x00, //         
    0x71, //  OOO   O
    0x8E, // O   OOO 
    0x00, //         
    0x00, //         
    0x00, //         
};

unsigned int arial8_info[] = {
    1,// !
    4,// "
    9,// #
    7,// $
    12,// %
    9,// &
    1,// '
    3,// (
    3,// )
    5,// *
    7,// +
    3,// ,
    4,// -
    1,// .
    4,// /
    7,// 0
    4,// 1
    6,// 2
    7,// 3
    8,// 4
    7,// 5
    7,// 6
    6,// 7
    7,// 8
    7,// 9
    1,// :
    3,// ;
    7,// <
    7,// =
    7,// >
    7,// ?
    15,// @
    9,// A
    9,// B
    10,// C
    10,// D
    9,// E
    8,// F
    10,// G
    9,// H
    5,// I
    7,// J
    9,// K
    7,// L
    11,// M
    9,// N
    10,// O
    9,// P
    10,// Q
    9,// R
    9,// S
    7,// T
    9,// U
    9,// V
    15,// W
    11,// X
    9,// Y
    9,// Z
    3,// [
    4,// \
    3,// ]
    7,// ^
    9,// _
    2,// `
    7,// a
    6,// b
    6,// c
    7,// d
    7,// e
    4,// f
    7,// g
    6,// h
    1,// i
    3,// j
    7,// k
    2,// l
    11,// m
    6,// n
    7,// o
    6,// p
    7,// q
    4,// r
    6,// s
    4,// t
    6,// u
    7,// v
    11,// w
    7,// x
    7,// y
    7,// z
    5,// {
    1,// |
    5,// }
    8 // ~
};

#endif
