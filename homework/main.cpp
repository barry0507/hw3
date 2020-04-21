#include "mbed.h"

#include "fsl_port.h"

#include "fsl_gpio.h"
#include <iostream>
#include <bits/stdc++.h>
#include <cmath>
#define UINT14_MAX        16383

// FXOS8700CQ I2C address

#define FXOS8700CQ_SLAVE_ADDR0 (0x1E<<1) // with pins SA0=0, SA1=0

#define FXOS8700CQ_SLAVE_ADDR1 (0x1D<<1) // with pins SA0=1, SA1=0

#define FXOS8700CQ_SLAVE_ADDR2 (0x1C<<1) // with pins SA0=0, SA1=1

#define FXOS8700CQ_SLAVE_ADDR3 (0x1F<<1) // with pins SA0=1, SA1=1

// FXOS8700CQ internal register addresses

#define FXOS8700Q_STATUS 0x00

#define FXOS8700Q_OUT_X_MSB 0x01

#define FXOS8700Q_OUT_Y_MSB 0x03

#define FXOS8700Q_OUT_Z_MSB 0x05

#define FXOS8700Q_M_OUT_X_MSB 0x33

#define FXOS8700Q_M_OUT_Y_MSB 0x35

#define FXOS8700Q_M_OUT_Z_MSB 0x37

#define FXOS8700Q_WHOAMI 0x0D

#define FXOS8700Q_XYZ_DATA_CFG 0x0E

#define FXOS8700Q_CTRL_REG1 0x2A

#define FXOS8700Q_M_CTRL_REG1 0x5B

#define FXOS8700Q_M_CTRL_REG2 0x5C

#define FXOS8700Q_WHOAMI_VAL 0xC7

I2C i2c( PTD9,PTD8);

Serial pc(USBTX, USBRX);

int m_addr = FXOS8700CQ_SLAVE_ADDR1;
DigitalOut led2(LED2);
DigitalIn sw2(SW2);
float g=9.8;
Timeout tout;
char record = '0';
void FXOS8700CQ_readRegs(int addr, uint8_t * data, int len);
void blink();
void FXOS8700CQ_writeRegs(uint8_t * data, int len);
void accerlatelog();
void endlog();
void printlog();
uint8_t who_am_i, data[2], res[6];
int16_t acc16;
float t[3];
int startlog = 0;
int num = 0;
std::string result[200]; //for record x y z and whether over45 degree
int main() {


   pc.baud(115200);




   // Enable the FXOS8700Q


   FXOS8700CQ_readRegs( FXOS8700Q_CTRL_REG1, &data[1], 1);

   data[1] |= 0x01;

   data[0] = FXOS8700Q_CTRL_REG1;

   FXOS8700CQ_writeRegs(data, 2);


   // Get the slave address

   FXOS8700CQ_readRegs(FXOS8700Q_WHOAMI, &who_am_i, 1);


   pc.printf("Here is %x\r\n", who_am_i);
   led2=1;
   EventQueue queue;
   while(1){
      if(sw2==0){
         startlog = 1;
         queue.call_every(100,&accerlatelog);
         queue.call_every(300,&blink);
         queue.call_in(10001,&endlog);
         queue.call_in(10050, &printlog);
         queue.dispatch();
      }
   }


}
   void printlog(){
      for(int j=0;j<100;j++)
         pc.printf("%s",result[j].c_str());
         wait(0.1);
   }
   void endlog(){
      startlog = 0;
      led2=1;
   }
   void accerlatelog() {
      if(startlog==1){

         FXOS8700CQ_readRegs(FXOS8700Q_OUT_X_MSB, res, 6);


         acc16 = (res[0] << 6) | (res[1] >> 2);

         if (acc16 > UINT14_MAX/2)

            acc16 -= UINT14_MAX;

         t[0] = ((float)acc16) / 4096.0f;


         acc16 = (res[2] << 6) | (res[3] >> 2);

         if (acc16 > UINT14_MAX/2)

            acc16 -= UINT14_MAX;

         t[1] = ((float)acc16) / 4096.0f;


         acc16 = (res[4] << 6) | (res[5] >> 2);

         if (acc16 > UINT14_MAX/2)

            acc16 -= UINT14_MAX;

         t[2] = ((float)acc16) / 4096.0f;


      /* printf("FXOS8700Q ACC: X=%1.4f(%x%x) Y=%1.4f(%x%x) Z=%1.4f(%x%x)\r\n",\

               t[0], res[0], res[1],\

               t[1], res[2], res[3],\

               t[2], res[4], res[5]\

         );*/
         if(t[2]<g/10/sqrt(2)){
            record='1';
         }
         else{
            record='0';
         }
         result[num++] =  std::to_string(t[0]) + " " + std::to_string(t[1]) + " " + std::to_string(t[2]) + " " + record + "\r\n";
      
      }
   }
void blink(){
   if(startlog==1)
      led2=!led2;
   else
   {
      led2=1;
   }
   
}
void FXOS8700CQ_readRegs(int addr, uint8_t * data, int len) {

   char t = addr;

   i2c.write(m_addr, &t, 1, true);

   i2c.read(m_addr, (char *)data, len);

}


void FXOS8700CQ_writeRegs(uint8_t * data, int len) {

   i2c.write(m_addr, (char *)data, len);

}