/****************************************************
  Company:   Hiwonder
  Author:Shenzhen Hiwonder Technology Co., Ltd
  store:lobot-zone.taobao.com
*****************************************************
  Sensor：Hiwonder Series, MP3 module
  Communication mode：iic
  Working voltage：5v
*****************************************************/
/*
 * Support up to 32G TF Card
 * Support FAT16,FAT32 file system
 * Support MP3、 WAV、 WMA format songs
 * In TF card to build a name of the folder "MP3"
 * Then the songs thta you want play in it. The songs named format is as follow:
 * 0001+song name，for example,the song Little Apple could be named:0001 Little Apple
 * It can also be 0001 without the song name, and so on, 0010, 0100, 1000...
 */
#include <Wire.h>

#define I2C_ADDR		0x7B //iic address

#define MP3_PLAY_NUM_ADDR         1//play specify the track, 0~3000, low bit front,high bit behind
#define MP3_PLAY_ADDR             5//play
#define MP3_PAUSE_ADDR            6//stop
#define MP3_PREV_ADDR             8//prev track
#define MP3_NEXT_ADDR             9//next track
#define MP3_VOL_VALUE_ADDR        12//specify volume value 0~30
#define MP3_SINGLE_LOOP_ON_ADDR   13//turn on single loop, it will be effective when it turned on in play
#define MP3_SINGLE_LOOP_OFF_ADDR  14//turn off single loop

bool WireWriteByte(uint8_t val)
{
    Wire.beginTransmission(I2C_ADDR);
    Wire.write(val);
    if( Wire.endTransmission() != 0 ) {
        return false;
    }
    return true;
}

bool WireWriteDataArray(  uint8_t reg,uint8_t *val,unsigned int len){
    unsigned int i;

    Wire.beginTransmission(I2C_ADDR);
    Wire.write(reg);
    for(i = 0; i < len; i++) {
        Wire.write(val[i]);
    }
    if( Wire.endTransmission() != 0 ) {
        return false;
    }
    return true;
}

int WireReadDataArray(   uint8_t reg, uint8_t *val, unsigned int len){
    unsigned char i = 0;
    
    /* Indicate which register we want to read from */
    if (!WireWriteByte(reg)) {
        return -1;
    }
    
    /* Read block data */
    Wire.requestFrom(I2C_ADDR, len);
    while (Wire.available()) {
        if (i >= len) {
            return -1;
        }
        val[i] = Wire.read();
        i++;
    }   
    return i;
}

void setup(){
  uint16_t MP3Num = 16; //specify play the track number
  uint8_t MP3Vol = 30; //volume value
  Wire.begin();
  Serial.begin(115200);

  delay(200);//wait for the MP3 module nitialization to complete
  Serial.println("Start");
  
  WireWriteDataArray(MP3_VOL_VALUE_ADDR, &MP3Vol, 1); //set volume value
  delay(15);//there must be a small delay between the two commands
  WireWriteDataArray(MP3_PLAY_NUM_ADDR,(uint8_t*)&MP3Num, 2); //play special track
  delay(3000);
  WireWriteDataArray(MP3_PREV_ADDR,NULL,0); //prev track
  delay(3000);
  WireWriteDataArray(MP3_PAUSE_ADDR,NULL,0); //stop
  delay(3000);
  WireWriteDataArray(MP3_PLAY_ADDR,NULL,0); //paly
}

void loop()
{
}
