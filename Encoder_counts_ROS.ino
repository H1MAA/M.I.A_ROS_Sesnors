#include <ros.h>
#include <std_msgs/Int16.h>

#include "Wire.h"

ros::NodeHandle nh;

std_msgs::Int16 count_msg;
ros::Publisher pub_count("Encoder Counts", &count_msg);

#define ENCODERA PA0
#define ENCODERB PA1

long long count = 0;

void setup() {
  // put your setup code here, to run once:
  
  // set pins and  attach interrupts for enocoder feedback
  pinMode(ENCODERA, INPUT_PULLUP);
  pinMode(ENCODERB, INPUT_PULLUP);

  attachInterrupt(ENCODERA, ISR_EncA, CHANGE);
  attachInterrupt(ENCODERB, ISR_EncB, CHANGE);

  Wire.begin();
  Serial.begin(115200);

  nh.initNode();
  nh.advertise(pub_count);
}

long long pub_timer = 0;

void loop() {
  // put your main code here, to run repeatedly:
  
  if (millis() - pub_timer > 500){
    count_msg.data = count;
    nh.spinOnce();
    pub_timer = millis();
  }

}

void ISR_EncA(void){
  if (digitalRead(ENCODERA) != digitalRead(ENCODERB))
    count++;
  else
    count--; 
}

void ISR_EncB(void){
  if (digitalRead(ENCODERA) == digitalRead(ENCODERB))
    count++;
  else
    count--;  
}
