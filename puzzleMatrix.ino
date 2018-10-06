#include "fonts.h"
const uint8_t data = 13, clk = 12, dffclock = 11, triggerOut = 10, triggerIn = 3, Q = 2;
uint8_t twiceTemp = 0, enlargeTemp1 = 0, enlargeTemp2 = 0, enlargeTemp3 = 0, unlostCounter = 0, buffer1[8] = {0, 0, 0, 0, 0, 0, 0, 0}, buffer2[8] = {0, 0, 0, 0, 0, 0, 0, 0}, buffer3[8] = {0, 0, 0, 0, 0, 0, 0, 0}, buffer4[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint16_t i = 0, j = 0, k = 0, x = 0, y = 0, z = 0, state = 1;
const unsigned long column[17] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536};
unsigned long lt = 0;
void setup() {
  pinMode(data, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(dffclock, OUTPUT);
  pinMode(triggerOut, OUTPUT);
  digitalWrite(triggerOut, LOW);
  pinMode(triggerIn, INPUT);
  pinMode(Q, INPUT);
  Serial.begin(9600);
  lt = millis();
}

int zoomInLeftTop() {
  if (k == 0) {
    for (i = 0; i < 8; i++) {
      buffer3[i] = fonts['K' + j][i];
      buffer1[i] = 0; buffer2[i] = 0; buffer4[i] = 0;
    }
  }
  if (k == 1) {
    for (i = 0; i < 4; i++) {//1.2
      if (buffer3[i] >= 128) {
        buffer1[i] = 1;
        if (i == 4)buffer2[i + 1] = 1;
      }
    }
    for (i = 4; i < 8; i++) {
      if (buffer3[i] >= 128) {
        if (i < 7)buffer1[i + 1] = 1;
        if (i == 7)buffer2[0] = 1;
      }
    }
    buffer4[0] = buffer3[7];        //4
    for ( i = 0; i < 8; i++) {
      for (uint8_t l = 0; l < 4; l++)   if (buffer4[i] % column[8 - l] >= column[7 - l])  buffer4[i] += column[7 - l];
      if (buffer4[i] % 16 >= 8)buffer4[i] += 16;
    }
    for (i = 0; i < 8; i++) {            //3
      for (uint8_t l = 0; l < 4; l++)   if (buffer3[i] % column[8 - l] >= column[7 - l])  buffer3[i] += column[7 - l];
      if (buffer3[i] % 16 >= 8)buffer3[i] += 16;
    }
    for (i = 7; i > 3; i--)buffer3[i] = buffer3[i - 1];
  }
  if (k == 2) {
    for (i = 7; i >= 0; i--) {     //2
      if (i > 0)buffer2[i] = buffer2[i - 1];
      if (i == 0)buffer2[0] = buffer1[7];
      for (uint8_t l = 0; l < 7; l++)   if (buffer2[i] % column[8 - l] >= column[7 - l])  buffer2[i] += column[7 - l];
      if (buffer2[i] % 2 >= 1)buffer2[i] += 2;
      if (i > 0 && buffer4[i - 1] >= 128)buffer2[i]++;
      if (i == 0 && buffer3[7] >= 128)buffer2[0]++;
    }
    for (i = 7; i >= 0; i--) {       //1
      if (i > 4)buffer1[i] = buffer1[i - 1];
      for (uint8_t l = 0; l < 7; l++)   if (buffer1[i] % column[8 - l] >= column[7 - l])  buffer1[i] += column[7 - l];
      if (buffer1[i] % 2 >= 1)buffer1[i] += 2;
      if (i > 4 && buffer3[i - 1] >= 128)buffer1[i]++;
      if (i <= 4 && buffer3[i] >= 128)buffer1[i]++;
    }
    for (i = 7; i >= 0; i--) {        //4
      if (i > 0)buffer4[i] = buffer4[i - 1];
      if (i == 0)buffer4[0] = buffer3[7];
      for (uint8_t l = 0; l < 2; l++)   if (buffer4[i] % column[8 - l] >= column[7 - l])  buffer4[i] += column[7 - l];
      if (buffer4[i] % 64 >= 32)buffer4[i] += 64;
    }
    for (i = 7; i >= 0; i--) {                   //3
      for (uint8_t l = 0; l < 2; l++)   if (buffer3[i] % column[8 - l] >= column[7 - l])  buffer3[i] += column[7 - l];
      if (buffer3[i] % 64 >= 32)buffer3[i] += 64;
      if (i > 4 && i < 7)buffer3[i + 1] = buffer3[i];
    }
  }
  if (k == 3) {
    for (i = 7; i >= 0; i--) {       //2
      if (i > 0)buffer2[i] = buffer2[i - 1];
      if (i == 0)buffer2[0] = buffer1[7];
      buffer2[i] = buffer2[i] << 1;
      if (buffer4[i - 1] >= 128 && i > 0)buffer2[i]++;
      if (buffer3[7] >= 128 && i == 0)buffer2[0]++;
    }
    for (i = 7; i >= 0; i--) {       //1
      if (i > 2) {
        buffer1[i] = buffer1[i - 1];
        buffer1[i] = buffer1[i] << 1;
        if (buffer3[i - 1] >= 128)buffer1[i]++;
      }
      else {
        buffer1[i] = buffer1[i] << 1;
        if (buffer3[i] >= 128)buffer1[i]++;
      }
    }
    for (i = 7; i >= 0; i--) {//4
      if (i > 0)buffer4[i] = buffer4[i - 1];
      else if (i == 0)buffer4[0] = buffer3[7];
    }
    for (i = 7; i >= 2; i--)buffer3[i] = buffer3[i - 1];    //3
  }
  if (k == 4) {
    for (i = 7; i >= 0; i--) {       //2
      if (i > 0)buffer2[i] = buffer2[i - 1];
      if (i == 0)buffer2[0] = buffer1[7];
      buffer2[i] = buffer2[i] << 1;
      if (i > 0 && buffer4[i - 1] >= 128)buffer2[i]++;
      if (i == 0 && buffer3[7] >= 128)buffer2[0]++;
    }
    for (i = 7; i >= 0; i--) {       //1
      buffer1[i] = buffer1[i] << 1;
      if (buffer3[i] >= 128)buffer1[i]++;
    }
    for (i = 7; i >= 0; i--) {   //4
      if (i > 0)buffer4[i] = buffer4[i - 1];
      for (uint8_t l = 0; l < 5; l++)   if (buffer4[i] % column[8 - l] >= column[7 - l])  buffer4[i] += column[7 - l];
      if (buffer4[i] % 8 >= 4)buffer4[i] += 8;////////***************************************5/10 11.59
    }
    for (i = 7; i >= 0; i--) {   //3
      if (buffer4[i] >= 128)buffer4[i] -= 128;
      if (buffer4[i] % 128 >= 64)buffer4[i] += 64;
      if (buffer4[i] % 64 >= 32)buffer4[i] += 32;
      if (buffer4[i] % 32 >= 16)buffer4[i] += 16;
      if (buffer4[i] % 16 >= 8)buffer4[i] += 8;
      if (buffer4[i] % 8 >= 4)buffer4[i] += 8;
    }
  }
  if (k == 5) {
    for (i = 0; i < 8; i++) {       //1
      if (i < 5)buffer1[i] = buffer1[i + 1];
      if (buffer1[i] >= 128)buffer1[i] -= 128;
      if (buffer1[i] % 128 >= 64)buffer1[i] += 64;
      if (buffer1[i] % 64 >= 32)buffer1[i] += 32;
      if (buffer1[i] % 32 >= 16)buffer1[i] += 16;
      if (buffer1[i] % 16 >= 8)buffer1[i] += 8;
      if (buffer1[i] % 8 >= 4)buffer1[i] += 8;
    }
    for (i = 0; i < 8; i++) {       //2
      if (buffer2[i] >= 128)buffer2[i] -= 128;
      if (buffer2[i] % 128 >= 64)buffer2[i] += 64;
      if (buffer2[i] % 64 >= 32)buffer2[i] += 32;
      if (buffer2[i] % 32 >= 16)buffer2[i] += 16;
      if (buffer2[i] % 16 >= 8)buffer2[i] += 8;
      if (buffer2[i] % 8 >= 4)buffer2[i] += 8;
    }
    for (i = 0; i < 5; i++) {   //3
      buffer3[i] = buffer3[i + 1];
    }
    for (i = 0; i < 8; i++) {   //4
    }
  }
  if (k == 6) {
    for (i = 0; i < 8; i++) {       //1
      if (i < 7)buffer1[i] = buffer1[i + 1];
      if (i == 7)buffer1[i] = buffer2[0];
      buffer1[i] = buffer1[i] << 1;
      if (i < 7 && buffer3[i + 1] >= 128)buffer1[i]++;
      if (i == 7 && buffer4[0] >= 128)buffer1[i]++;
    }
    for (i = 0; i < 8; i++) {       //2
      if (i < 6)buffer2[i] = buffer2[i + 1];
      buffer2[i] = buffer2[i] << 1;
      if (i < 6 && buffer4[i + 1] >= 128)buffer2[i]++;
      if (i >= 6 && buffer4[i] >= 128)buffer2[i]++;
    }
    for (i = 0; i < 8; i++) {   //3
      if (i < 7)buffer3[i] = buffer3[i + 1];
      if (i == 7)buffer3[7] = buffer4[0];
      if (buffer3[i] >= 128)buffer3[i] -= 128;
      if (buffer3[i] % 128 >= 64)buffer3[i] += 64;
      if (buffer3[i] % 64 >= 32)buffer3[i] += 32;
      if (buffer3[i] % 32 >= 16)buffer3[i] += 16;
      if (buffer3[i] % 16 >= 8)buffer3[i] += 8;
      if (buffer3[i] % 8 >= 4)buffer3[i] += 4;
      if (buffer3[i] % 4 >= 2)buffer3[i] += 4;
    }
    for (i = 0; i < 8; i++) {   //4
      if (i < 6)buffer4[i] = buffer4[i + 1];
      if (buffer4[i] >= 128)buffer4[i] -= 128;
      if (buffer4[i] % 128 >= 64)buffer4[i] += 64;
      if (buffer4[i] % 64 >= 32)buffer4[i] += 32;
      if (buffer4[i] % 32 >= 16)buffer4[i] += 16;
      if (buffer4[i] % 16 >= 8)buffer4[i] += 8;
      if (buffer4[i] % 8 >= 4)buffer4[i] += 4;
      if (buffer4[i] % 4 >= 2)buffer4[i] += 4;
    }
  }
  if (k == 7) {
    for (i = 0; i < 8; i++) {       //1
      if (i < 2)buffer1[i] = buffer1[i + 1];
      buffer1[i] = buffer1[i] << 1;
      if (i < 2 && buffer3[i + 1] >= 128)buffer1[i]++;
      if (i >= 2 && buffer3[i] >= 128)buffer1[i]++;
    }
    for (i = 0; i < 8; i++) {       //2
      buffer2[i] = buffer2[i] << 1;
      if (buffer4[i] >= 128)buffer2[i]++;
    }
    for (i = 0; i < 8; i++) {   //3
      if (i < 2)buffer3[i] = buffer3[i + 1];
      if (buffer3[i] >= 128)buffer3[i] -= 128;
      if (buffer3[i] % 128 >= 64)buffer3[i] += 64;
      if (buffer3[i] % 64 >= 32)buffer3[i] += 32;
      if (buffer3[i] % 32 >= 16)buffer3[i] += 16;
      if (buffer3[i] % 16 >= 8)buffer3[i] += 8;
      if (buffer3[i] % 8 >= 4)buffer3[i] += 4;
      if (buffer3[i] % 4 >= 2)buffer3[i] += 2;
      if (buffer3[i] % 2 >= 1)buffer3[i] += 2;
    }
    for (i = 0; i < 8; i++) {   //4
      if (buffer4[i] >= 128)buffer4[i] -= 128;
      if (buffer4[i] % 128 >= 64)buffer4[i] += 64;
      if (buffer4[i] % 64 >= 32)buffer4[i] += 32;
      if (buffer4[i] % 32 >= 16)buffer4[i] += 16;
      if (buffer4[i] % 16 >= 8)buffer4[i] += 8;
      if (buffer4[i] % 8 >= 4)buffer4[i] += 4;
      if (buffer4[i] % 4 >= 2)buffer4[i] += 2;
      if (buffer4[i] % 2 >= 1)buffer4[i] += 2;
    }
  }
  if (k == 8) {
    for (i = 0; i < 8; i++) {       //1
      if (i < 7)buffer1[i] = buffer1[i + 1];
      if (i == 7)buffer1[7] = buffer2[0];
      if (buffer1[i] % 128 >= 64)buffer1[i] += 128;
    }
    for (i = 0; i < 8; i++) {       //2
      if (i < 7)buffer2[i] = buffer2[i + 1];
      if (i == 6)buffer2[6] = buffer2[7];
      if (buffer2[i] % 128 >= 64)buffer2[i] += 128;
    }
    for (i = 0; i < 8; i++) {   //3
      if (i < 7)buffer3[i] = buffer3[i + 1];
      if (i == 7)buffer3[7] = buffer4[0];
    }
    for (i = 0; i < 8; i++) {   //4
      if (i < 7)buffer4[i] = buffer4[i + 1];
    }
  }
  k++;
  if (k == 4) {
    k = 10;
    j++;
    if (j == 'Z' - 'A' + 1)j = 0;
  }
  //***********************************************************************************
}
int zoomInRightTop() {
  if (k == 0) {
    for (i = 0; i < 8; i++) {
      buffer4[i] = fonts['A' + j][i];
      buffer1[i] = 0; buffer2[i] = 0; buffer3[i] = 0;
    }
  }
  if (k == 1) {
    for (i = 4; i < 8; i++) {
      if (buffer4[i] >= 128) {
        buffer2[i] = 1;
        if (i == 4)buffer2[i - 1] = 1;
      }
    }
    for (i = 0; i < 4; i++) {
      if (buffer4[i] >= 128) {
        if (i > 0)buffer2[i - 1] = 1;
        if (i == 0)buffer1[7] = 1;
      }
    }
    buffer3[7] = buffer4[0] % 16;        //3
    if (buffer4[0] % 16 >= 8)buffer3[7] += 16;
    if (buffer4[0] % 32 >= 16)buffer3[7] += 32;
    if (buffer4[0] % 64 >= 32)buffer3[7] += 64;
    if (buffer4[0] % 128 >= 64)buffer3[7] += 128;
    for (i = 0; i < 8; i++) {            //4
      if (buffer4[i] >= 128)buffer4[i] -= 128;
      if (buffer4[i] % 128 >= 64)buffer4[i] += 64;
      if (buffer4[i] % 64 >= 32)buffer4[i] += 32;
      if (buffer4[i] % 32 >= 16)buffer4[i] += 16;
      if (buffer4[i] % 16 >= 8)buffer4[i] += 16;
    }
    for (i = 0; i < 4; i++)buffer4[i] = buffer4[i + 1];
  }
  if (k == 2) {
    for (i = 0; i < 8; i++) {       //1
      if (i < 7)buffer1[i] = buffer1[i + 1];
      if (i == 7)buffer1[7] = buffer2[0];
      if (buffer1[i] >= 128)buffer1[i] -= 128;
      if (buffer1[i] % 128 >= 64)buffer1[i] += 64;
      if (buffer1[i] % 64 >= 32)buffer1[i] += 32;
      if (buffer1[i] % 32 >= 16)buffer1[i] += 16;
      if (buffer1[i] % 16 >= 8)buffer1[i] += 8;
      if (buffer1[i] % 8 >= 4)buffer1[i] += 4;
      if (buffer1[i] % 4 >= 2)buffer1[i] += 2;
      if (buffer1[i] % 2 >= 1)buffer1[i] += 2;
      if (i < 7 && buffer3[i + 1] >= 128)buffer1[i]++;
      if (i == 7 && buffer4[0] >= 128)buffer1[7]++;
    }
    for (i = 0; i < 8; i++) {       //2
      if (i < 3)buffer2[i] = buffer2[i + 1];
      if (buffer2[i] >= 128)buffer2[i] -= 128;
      if (buffer2[i] % 128 >= 64)buffer2[i] += 64;
      if (buffer2[i] % 64 >= 32)buffer2[i] += 32;
      if (buffer2[i] % 32 >= 16)buffer2[i] += 16;
      if (buffer2[i] % 16 >= 8)buffer2[i] += 8;
      if (buffer2[i] % 8 >= 4)buffer2[i] += 4;
      if (buffer2[i] % 4 >= 2)buffer2[i] += 2;
      if (buffer2[i] % 2 >= 1)buffer2[i] += 2;
      if (i < 2 && buffer4[i + 1] >= 128)buffer2[i]++;
      if (i >= 2 && buffer4[i] >= 128)buffer2[i]++;
    }
    for (i = 0; i < 8; i++) {        //3
      if (i < 7)buffer3[i] = buffer3[i + 1];
      if (i == 7)buffer3[7] = buffer4[0];
      if (buffer3[i] >= 128)buffer3[i] -= 128;
      if (buffer3[i] % 128 >= 64)buffer3[i] += 64;
      if (buffer3[i] % 64 >= 32)buffer3[i] += 64;
    }
    for (i = 0; i < 8; i++) {                   //4
      if (buffer4[i] >= 128)buffer4[i] -= 128;
      if (buffer4[i] % 128 >= 64)buffer4[i] += 64;
      if (buffer4[i] % 64 >= 32)buffer4[i] += 64;
    }
    for (i = 0; i < 2; i++)buffer4[i] = buffer4[i + 1];
  }
  if (k == 3) {
    for (i = 0; i < 8; i++) {       //1
      if (i < 7)buffer1[i] = buffer1[i + 1];
      if (i == 6)buffer1[7] = buffer2[0];
      buffer1[i] = buffer1[i] << 1;
    }
    for (i = 0; i < 5; i++) {       //2
      buffer2[i] = buffer2[i + 1];
      buffer2[i] = buffer2[i] << 1;
      if (buffer4[i] >= 128) {
        if (i > 0)buffer2[i - 1]++;
        if (i == 0)buffer1[7]++;
      }
    }
    for (i = 5; i < 8; i++) {
      buffer2[i] = buffer2[i] << 1;
      buffer2[4] = buffer2[5];
      if (buffer4[i] >= 128) {
        buffer2[i]++;
        if (i == 5)buffer2[i - 1]++;
      }
    }
    for (i = 1; i < 8; i++)if (buffer3[i] >= 128)buffer1[i - 1]++;
    for (i = 0; i < 7; i++)buffer3[i] = buffer3[i + 1];    //3
    buffer3[7] = buffer4[0];
    for (i = 0; i < 5; i++)buffer4[i] = buffer4[i + 1];    //4
  }
  if (k == 4) {
    for (i = 0; i < 8; i++) {       //1
      if (i < 7)buffer1[i] = buffer1[i + 1];
      buffer1[i] = buffer1[i] << 1;
      if (i < 7 && buffer3[i + 1] >= 128)buffer1[i]++;
      if (i == 7 && buffer3[7] >= 128)buffer1[7]++;
    }
    for (i = 0; i < 8; i++) {       //2
      buffer2[i] = buffer2[i] << 1;
      if (buffer4[i] >= 128)buffer2[i]++;
    }
    for (i = 0; i < 8; i++) {   //3
      if (i < 7)buffer3[i] = buffer3[i + 1];
      if (buffer3[i] >= 128)buffer3[i] -= 128;
      if (buffer3[i] % 128 >= 64)buffer3[i] += 64;
      if (buffer3[i] % 64 >= 32)buffer3[i] += 32;
      if (buffer3[i] % 32 >= 16)buffer3[i] += 16;
      if (buffer3[i] % 16 >= 8)buffer3[i] += 8;
      if (buffer3[i] % 8 >= 4)buffer3[i] += 8;
    }
    for (i = 0; i < 8; i++) {   //4
      if (buffer4[i] >= 128)buffer4[i] -= 128;
      if (buffer4[i] % 128 >= 64)buffer4[i] += 64;
      if (buffer4[i] % 64 >= 32)buffer4[i] += 32;
      if (buffer4[i] % 32 >= 16)buffer4[i] += 16;
      if (buffer4[i] % 16 >= 8)buffer4[i] += 8;
      if (buffer4[i] % 8 >= 4)buffer4[i] += 8;
    }
  }
  if (k == 5) {
    for (i = 0; i < 8; i++) {       //1
      if (i < 5)buffer1[i] = buffer1[i + 1];
      if (buffer1[i] >= 128)buffer1[i] -= 128;
      if (buffer1[i] % 128 >= 64)buffer1[i] += 64;
      if (buffer1[i] % 64 >= 32)buffer1[i] += 32;
      if (buffer1[i] % 32 >= 16)buffer1[i] += 16;
      if (buffer1[i] % 16 >= 8)buffer1[i] += 8;
      if (buffer1[i] % 8 >= 4)buffer1[i] += 8;
    }
    for (i = 0; i < 8; i++) {       //2
      if (buffer2[i] >= 128)buffer2[i] -= 128;
      if (buffer2[i] % 128 >= 64)buffer2[i] += 64;
      if (buffer2[i] % 64 >= 32)buffer2[i] += 32;
      if (buffer2[i] % 32 >= 16)buffer2[i] += 16;
      if (buffer2[i] % 16 >= 8)buffer2[i] += 8;
      if (buffer2[i] % 8 >= 4)buffer2[i] += 8;
    }
    for (i = 0; i < 5; i++) {   //3
      buffer3[i] = buffer3[i + 1];
    }
    for (i = 0; i < 8; i++) {   //4
    }
  }
  if (k == 6) {
    for (i = 0; i < 8; i++) {       //1
      if (i < 7)buffer1[i] = buffer1[i + 1];
      if (i == 7)buffer1[i] = buffer2[0];
      buffer1[i] = buffer1[i] << 1;
      if (i < 7 && buffer3[i + 1] >= 128)buffer1[i]++;
      if (i == 7 && buffer4[0] >= 128)buffer1[i]++;
    }
    for (i = 0; i < 8; i++) {       //2
      if (i < 6)buffer2[i] = buffer2[i + 1];
      buffer2[i] = buffer2[i] << 1;
      if (i < 6 && buffer4[i + 1] >= 128)buffer2[i]++;
      if (i >= 6 && buffer4[i] >= 128)buffer2[i]++;
    }
    for (i = 0; i < 8; i++) {   //3
      if (i < 7)buffer3[i] = buffer3[i + 1];
      if (i == 7)buffer3[7] = buffer4[0];
      if (buffer3[i] >= 128)buffer3[i] -= 128;
      if (buffer3[i] % 128 >= 64)buffer3[i] += 64;
      if (buffer3[i] % 64 >= 32)buffer3[i] += 32;
      if (buffer3[i] % 32 >= 16)buffer3[i] += 16;
      if (buffer3[i] % 16 >= 8)buffer3[i] += 8;
      if (buffer3[i] % 8 >= 4)buffer3[i] += 4;
      if (buffer3[i] % 4 >= 2)buffer3[i] += 4;
    }
    for (i = 0; i < 8; i++) {   //4
      if (i < 6)buffer4[i] = buffer4[i + 1];
      if (buffer4[i] >= 128)buffer4[i] -= 128;
      if (buffer4[i] % 128 >= 64)buffer4[i] += 64;
      if (buffer4[i] % 64 >= 32)buffer4[i] += 32;
      if (buffer4[i] % 32 >= 16)buffer4[i] += 16;
      if (buffer4[i] % 16 >= 8)buffer4[i] += 8;
      if (buffer4[i] % 8 >= 4)buffer4[i] += 4;
      if (buffer4[i] % 4 >= 2)buffer4[i] += 4;
    }
  }
  if (k == 7) {
    for (i = 0; i < 8; i++) {       //1
      if (i < 2)buffer1[i] = buffer1[i + 1];
      buffer1[i] = buffer1[i] << 1;
      if (i < 2 && buffer3[i + 1] >= 128)buffer1[i]++;
      if (i >= 2 && buffer3[i] >= 128)buffer1[i]++;
    }
    for (i = 0; i < 8; i++) {       //2
      buffer2[i] = buffer2[i] << 1;
      if (buffer4[i] >= 128)buffer2[i]++;
    }
    for (i = 0; i < 8; i++) {   //3
      if (i < 2)buffer3[i] = buffer3[i + 1];
      if (buffer3[i] >= 128)buffer3[i] -= 128;
      if (buffer3[i] % 128 >= 64)buffer3[i] += 64;
      if (buffer3[i] % 64 >= 32)buffer3[i] += 32;
      if (buffer3[i] % 32 >= 16)buffer3[i] += 16;
      if (buffer3[i] % 16 >= 8)buffer3[i] += 8;
      if (buffer3[i] % 8 >= 4)buffer3[i] += 4;
      if (buffer3[i] % 4 >= 2)buffer3[i] += 2;
      if (buffer3[i] % 2 >= 1)buffer3[i] += 2;
    }
    for (i = 0; i < 8; i++) {   //4
      if (buffer4[i] >= 128)buffer4[i] -= 128;
      if (buffer4[i] % 128 >= 64)buffer4[i] += 64;
      if (buffer4[i] % 64 >= 32)buffer4[i] += 32;
      if (buffer4[i] % 32 >= 16)buffer4[i] += 16;
      if (buffer4[i] % 16 >= 8)buffer4[i] += 8;
      if (buffer4[i] % 8 >= 4)buffer4[i] += 4;
      if (buffer4[i] % 4 >= 2)buffer4[i] += 2;
      if (buffer4[i] % 2 >= 1)buffer4[i] += 2;
    }
  }
  if (k == 8) {
    for (i = 0; i < 8; i++) {       //1
      if (i < 7)buffer1[i] = buffer1[i + 1];
      if (i == 7)buffer1[7] = buffer2[0];
      if (buffer1[i] % 128 >= 64)buffer1[i] += 128;
    }
    for (i = 0; i < 8; i++) {       //2
      if (i < 7)buffer2[i] = buffer2[i + 1];
      if (i == 6)buffer2[6] = buffer2[7];
      if (buffer2[i] % 128 >= 64)buffer2[i] += 128;
    }
    for (i = 0; i < 8; i++) {   //3
      if (i < 7)buffer3[i] = buffer3[i + 1];
      if (i == 7)buffer3[7] = buffer4[0];
    }
    for (i = 0; i < 8; i++) {   //4
      if (i < 7)buffer4[i] = buffer4[i + 1];
    }
  }
  if (k >= 8) {
    for (i = 0; i < 8; i++) {       //1
      if (i < 7)buffer1[i] = buffer1[i + 1];
      if (i == 7)buffer1[7] = buffer2[0];
      if (buffer1[i] % 128 >= 64)buffer1[i] += 128;
    }
    for (i = 0; i < 8; i++) {       //2
      if (i < 7)buffer2[i] = buffer2[i + 1];
      if (i == 6)buffer2[6] = buffer2[7];
      if (buffer2[i] % 128 >= 64)buffer2[i] += 128;
    }
    for (i = 0; i < 8; i++) {   //3
      if (i < 7)buffer3[i] = buffer3[i + 1];
      if (i == 7)buffer3[7] = buffer4[0];
    }
    for (i = 0; i < 8; i++) {   //4
      if (i < 7)buffer4[i] = buffer4[i + 1];
    }
  }
  k++;
  if (k == 22) {
    k = 0;
    j++;
    if (j == 'Z' - 'A' + 1)j = 0;
  }
}
int trans1(int enlargeTemp, int level) { //上下放大的運算法
  int enlargeTemp2 = 0;
  if (enlargeTemp / 128 == 1) {
    enlargeTemp2 += (128 + 64);
    enlargeTemp -= 128;
  }
  if (enlargeTemp / 64 == 1) {
    enlargeTemp2 += (32 + 16);
    enlargeTemp -= 64;
  }
  if (enlargeTemp / 32 == 1) {
    enlargeTemp2 += (8 + 4);
    enlargeTemp -= 32;
  }
  if (enlargeTemp / 16 == 1) {
    enlargeTemp2 += (2 + 1);
    enlargeTemp -= 16;
  }
  if (level == 1)enlargeTemp1 = enlargeTemp;
  else enlargeTemp3 = enlargeTemp;
  return (enlargeTemp2);
}
int trans2(int enlargeTemp, int level) {
  int enlargeTemp2 = 0;
  if (enlargeTemp / 8 == 1) {
    enlargeTemp2 += (128 + 64);
    enlargeTemp -= 8;
  }
  if (enlargeTemp / 4 == 1) {
    enlargeTemp2 += (32 + 16);
    enlargeTemp -= 4;
  }
  if (enlargeTemp / 2 == 1) {
    enlargeTemp2 += (8 + 4);
    enlargeTemp -= 2;
  }
  if (enlargeTemp % 2 == 1) {
    enlargeTemp2 += (2 + 1);
  }
  if (level == 1)enlargeTemp1 = enlargeTemp;
  else enlargeTemp3 = enlargeTemp;
  return (enlargeTemp2);
}
int twiceBigMatrix() {
  for (i = 0; i < 7; i++) {
    buffer1[i] = buffer1[i + 1];
    buffer2[i] = buffer2[i + 1];
    buffer3[i] = buffer3[i + 1];
    buffer4[i] = buffer4[i + 1];
  }
  enlargeTemp1 = fonts['A' + k][j]; /////////////////////left矩陣
  if (j < 4)enlargeTemp3 = fonts['A' + k][j + 4]; //左右放大斷層修正
  else enlargeTemp3 = fonts['A' + k + 1][j - 4];
  buffer1[7] = trans1(enlargeTemp1, 1); //讀第8行的運算法
  buffer3[7] = trans2(enlargeTemp1, 1);
  buffer2[7] = trans1(enlargeTemp3, 2); /////////////////right矩陣
  buffer4[7] = trans2(enlargeTemp3, 2);
  unlostCounter++;
  if (unlostCounter == 2) {
    j++;
    unlostCounter = 0;
  }
  if (j > 7) {
    j = 0;
    k++;
    if (k == 'Z' - 'A')k = 0;
  }
}
void matrix() {
  for (i = 0; i < 7; i++)buffer1[i] = buffer1[i + 1];
  if (state == 1)buffer1[7] = fonts['A' + k][j];
  else if (state == 2)buffer1[7] = 0;
  j++;
  if (j > 7) {
    j = 0;
    if (k == 'Z' - 'A'+1)k = 0;
    state++;
    if (state == 2) {
      digitalWrite(triggerOut, HIGH);
      delay(5);
      digitalWrite(triggerOut, LOW);
      k++;
    }
  }
}
void shift(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val)
{
  uint8_t i;
  for (i = 0; i < 8; i++)  {
    if (bitOrder == LSBFIRST)
      digitalWrite(dataPin, !!(val & (1 << i)));
    else
      digitalWrite(dataPin, !!(val & (1 << (7 - i))));
    digitalWrite(clockPin, HIGH);
    digitalWrite(clockPin, LOW);
  }
}
void dffReset() {
  while (digitalRead(Q) == 0) {
    digitalWrite(dffclock, LOW);
    digitalWrite(dffclock, HIGH);
  }
  while (digitalRead(Q) == 1) {
    digitalWrite(dffclock, LOW);
    digitalWrite(dffclock, HIGH);
  }
}
void loop() {
  if (digitalRead(triggerIn) == HIGH)state = 1;
  if (state < 3) {
    if (y > 170) {//y is speed control
      matrix();
      y = 0;
    } y++;
    for (x = 0; x < 8; x++) {
      digitalWrite(dffclock, LOW);
      shiftOut(data, clk, LSBFIRST, 255 - buffer1[x]);
      shiftOut(data, clk, LSBFIRST, column[x]);
      digitalWrite(dffclock, HIGH);
    }
  }
}
