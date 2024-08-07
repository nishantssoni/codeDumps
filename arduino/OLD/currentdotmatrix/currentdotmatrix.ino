
/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to LOAD(CS)
 pin 10 is connected to the CLK 
 We have only a single MAX72XX.
 */
#include <LedControl.h>
int DIN = 12;
int CS =  11;
int CLK = 10;

LedControl lc=LedControl(DIN,CLK,CS,0);

void setup(){
 lc.shutdown(0,false);       
 lc.setIntensity(0,10);      
 lc.clearDisplay(0);    
}

void loop(){ 
    
// //     //Facial Expression
//     byte smile[8]=   {0x3C,0x42,0xA5,0x81,0xA5,0x99,0x42,0x3C};
//     byte neutral[8]= {0x3C,0x42,0xA5,0x81,0xBD,0x81,0x42,0x3C};
//     byte sad[8]=   {0x3C,0x42,0xA5,0x81,0x99,0xA5,0x42,0x3C};
   
//     //Arrow
//     byte arrow_up[8]= {0x18,0x3C,0x7E,0xFF,0x18,0x18,0x18,0x18};
//     byte arrow_down[8]= {0x18,0x18,0x18,0x18,0xFF,0x7E,0x3C,0x18};
   
    
//     //Alternate Pattern
//     byte d1[8]= {0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55};
//     byte d2[8]= {0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA};
    
//     //Moving car

    

//     byte b1[8]= {0x00,0x00,0x00,0x00,0x18,0x3C,0x18,0x3C};
//     byte b2[8]= {0x00,0x00,0x00,0x18,0x3C,0x18,0x3C,0x00};
//     byte b3[8]= {0x00,0x00,0x18,0x3C,0x18,0x3C,0x00,0x00};
//     byte b4[8]= {0x00,0x18,0x3C,0x18,0x3C,0x00,0x00,0x00};
//     byte b5[8]= {0x18,0x3C,0x18,0x3C,0x00,0x00,0x00,0x00};
//     byte b6[8]= {0x3C,0x18,0x3C,0x00,0x00,0x00,0x00,0x18};
//     byte b7[8]= {0x18,0x3C,0x00,0x00,0x00,0x00,0x18,0x3C};
//     byte b8[8]= {0x3C,0x00,0x00,0x00,0x00,0x18,0x3C,0x18};

// for(int i = 0; i<10; i++){
// //Moving car
//     printByte(b1);
//     delay(50);
//     printByte(b2);
//     delay(50);
//     printByte(b3);
//     delay(50);
//     printByte(b4);
//     delay(50);
//     printByte(b5);
//     delay(50);
//     printByte(b6);
//     delay(50);
//     printByte(b7);
//     delay(50);
//     printByte(b8);
//     delay(50);
//     delay(100);
// }

// //alternate pattern
//     printByte(d1);
//     delay(100);

//     printByte(d2);
//     delay(100);

// //Arrow
//     printByte(arrow_up);
//     delay(2000);

//     printByte(arrow_down);
//     delay(2000);

   
// //Facial Expression   
//     printByte(smile);
     
//     delay(1000);

//     printByte(neutral);
    
//     delay(1000);

//     printByte(sad);    

//     delay(1000);

// for(int i=0;i<8;i++){
//   for(int j=0;j<8; j++){
//     lc.setLed(0,i,j,true); 
//     delay(100);
//     lc.clearDisplay(0);  
//     lc.setIntensity(0,(i*j)%16); 
//   }
// }

// byte smile[8]=   {0b01010100,0b00110011,0b00110011,0b00110011,0b00110011,0b00110011,0b00110011,0b00110011};
// printByte(smile);

byte fan[2][8] = {{
  0b00010000,
  0b00010000,
  0b00010000,
  0b00010000,
  0b11111110,
  0b00010000,
  0b00010000,
  0b00010000
},
{
  0b10000001,
  0b01000010,
  0b00100100,
  0b00011000,
  0b00011000,
  0b00100100,
  0b01000010,
  0b10000001
}
};

for(int i = 0;i<10;i++){
printByte(fan[i%2]);
delay(500);
}

   

    // for(int i=0;i<8;i++){
    //   for(int j=0;j<8; j++){
    //     if((i==j)){
    //       lc.setLed(0,i,j,true); 
    //       delay(100);
    //       // lc.clearDisplay(0);  
    //     }
    //   }
    // }

    // for(int i=7;i>=0;i--){
    //       lc.setLed(0,i,7,true); 
    //       delay(100);
    //       // lc.clearDisplay(0);  
    //   }
  

    // for(int i=0;i<8;i++){
    //   for(int j=0;j<8; j++){
    //     if((i+j==7)){
    //       lc.setLed(0,i,j,true); 
    //       delay(100);
    //       // lc.clearDisplay(0);  
    //     }
    //   }
    // }

    //     for(int i=7;i>=0;i--){
    //       lc.setLed(0,i,0,true); 
    //       delay(100);
    //       // lc.clearDisplay(0);  
    //   }
    //   delay(500);
    //   lc.clearDisplay(0); 
 
}

void printByte(byte character [])
{
  int i = 0;
  for(i=0;i<8;i++)
  {
    lc.setRow(0,i,character[i]);
  }
}