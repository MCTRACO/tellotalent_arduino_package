/*
 * Copyright (C) 2020 DJI.
 *
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-25     robomaster   first version
 */

#include "RMTT_Libs.h"
#include "RMTT_Protocol.h"
#include "ArduinoJson.h"

static uint16_t sdk_time = 0;

RMTT_Protocol::RMTT_Protocol()
{
    initStatus();
}

RMTT_Protocol::RMTT_Protocol(uint16_t time)
{
    sdk_time = time;
}

RMTT_Protocol::~RMTT_Protocol()
{
}

void RMTT_Protocol::initStatus()
{
	root["mid"] = -2;
    root["x"] = -200;
    root["y"] = -200;
    root["z"] = -200;
	root["mpryP"] = 0;
	root["mpryY"] = 0;
	root["mpryR"] = 0;
    root["pitch"] = 0;
    root["roll"] = 0;
    root["yaw"] = 0;
    root["vgx"] = 0;
    root["vgy"] = 0;
    root["vgz"] = 0;
    root["templ"] = 0;
    root["temph"] = 0;
    root["tof"] = 0;
    root["h"] = 0;
    root["bat"] = 0;
    root["baro"] = 0;
    root["time"] = 0;
    root["agx"] = 0;
    root["agy"] = 0;
    root["agz"] = 0;
}

void RMTT_Protocol::Split(String& body, String data[], int len, char separator)
{
    int head = 0, tail = 0;
    for(int i=0; i<len; i++){
        for(int j=head; j<body.length(); j++){
            if(j==body.length()-1){
                tail = j+1;
                data[i] = body.substring(head, tail);
            } else if (body[j] == separator) {
                tail = j;
                data[i] = body.substring(head, tail);
                head = j+1;
                break;
            }
            yield();
        }
        yield();
    }
}

void RMTT_Protocol::SendCMD(char *cmd)
{
    Serial1.printf("[TELLO] %s", cmd);
    if(sdk_time)
    {
        delay(sdk_time);
    }
}

void RMTT_Protocol::SDKOn()
{
    SendCMD((char*)"command");
}

void RMTT_Protocol::SDKOff()
{
}

void RMTT_Protocol::TakeOff()
{
    SendCMD((char*)"takeoff");
}

void RMTT_Protocol::Land()
{
    SendCMD((char*)"land");
}

void RMTT_Protocol::Emergency()
{
    SendCMD((char*)"emergency");
}

void RMTT_Protocol::Up(int16_t x)
{
    char s[20] = {0};
    snprintf(s, sizeof(s), "up %d", x);
    SendCMD((char*)s);
}

void RMTT_Protocol::Down(int16_t x)
{
    char s[20] = {0};
    snprintf(s, sizeof(s), "down %d", x);
    SendCMD((char*)s);
}

void RMTT_Protocol::Left(int16_t x)
{
    char s[20] = {0};
    snprintf(s, sizeof(s), "left %d", x);
    SendCMD((char*)s);
}

void RMTT_Protocol::Right(int16_t x)
{
    char s[20] = {0};
    snprintf(s, sizeof(s), "right %d", x);
    SendCMD((char*)s);
}

void RMTT_Protocol::Forward(int16_t x)
{
    char s[20] = {0};
    snprintf(s, sizeof(s), "forward %d", x);
    SendCMD((char*)s);
}

void RMTT_Protocol::Back(int16_t x)
{
    char s[20] = {0};
    snprintf(s, sizeof(s), "back %d", x);
    SendCMD((char*)s);
}

void RMTT_Protocol::CW(uint16_t x)
{
    char s[20] = {0};
    snprintf(s, sizeof(s), "cw %d", x);
    SendCMD((char*)s);
}

void RMTT_Protocol::CCW(uint16_t x)
{
    char s[20] = {0};
    snprintf(s, sizeof(s), "ccw %d", x);
    SendCMD((char*)s);
}

void RMTT_Protocol::Flip(char x)
{
    char s[20] = {0};
    snprintf(s, sizeof(s), "flip %c", x);
    SendCMD((char*)s);
}

void RMTT_Protocol::Go(int16_t x, int16_t y, int16_t z, uint16_t speed)
{
    char s[40] = {0};
    snprintf(s, sizeof(s), "go %d %d %d %d", x, y, z, speed);
    SendCMD((char*)s);
}

void RMTT_Protocol::Go(int16_t x, int16_t y, int16_t z, uint16_t speed, char *mid)
{
    char s[40] = {0};
    snprintf(s, sizeof(s), "go %d %d %d %d %s", x, y, z, speed, mid);
    SendCMD((char*)s);
}

void RMTT_Protocol::Stop()
{
    SendCMD((char*)"stop");
}

void RMTT_Protocol::Curve(int16_t x1, int16_t y1, int16_t z1, int16_t x2, int16_t y2, int16_t z2, uint16_t speed)
{
    char s[60] = {0};
    snprintf(s, sizeof(s), "curve %d %d %d %d %d %d %d", x1, y1, z1, x2, y2, z2, speed);
    SendCMD((char*)s);
}

void RMTT_Protocol::Curve(int16_t x1, int16_t y1, int16_t z1, int16_t x2, int16_t y2, int16_t z2, uint16_t speed, char *mid)
{
    char s[60] = {0};
    snprintf(s, sizeof(s), "curve %d %d %d %d %d %d %d %s", x1, y1, z1, x2, y2, z2, speed, mid);
    SendCMD((char*)s);
}

void RMTT_Protocol::Jump(int16_t x, int16_t y, int16_t z, uint16_t speed, int16_t yaw, char *mid1, char *mid2)
{
    char s[60] = {0};
    snprintf(s, sizeof(s), "jump %d %d %d %d %d %s %s", x, y, z, speed, yaw, mid1, mid2);
    SendCMD((char*)s);
}

void RMTT_Protocol::SetSpeed(int16_t x)
{
    char s[20] = {0};
    snprintf(s, sizeof(s), "speed %d", x);
    SendCMD((char*)s);
}

void RMTT_Protocol::SetRC(int16_t a, int16_t b, int16_t c, int16_t d)
{
    char s[40] = {0};
    snprintf(s, sizeof(s), "rc %d %d %d %d", a, b, c, d);
    SendCMD((char*)s);
}

void RMTT_Protocol::SetMon()
{
    SendCMD((char*)"mon");
}

void RMTT_Protocol::SetMoff()
{
    SendCMD((char*)"moff");
}

void RMTT_Protocol::SetMdirection(uint8_t x)
{
    char s[20] = {0};
    snprintf(s, sizeof(s), "mdirection %d", x);
    SendCMD((char*)s);
}

void RMTT_Protocol::ReadSpeed()
{
    SendCMD((char*)"speed?");
}

void RMTT_Protocol::ReadBattery()
{
    SendCMD((char*)"battery?");
}

void RMTT_Protocol::ReadTime()
{
    SendCMD((char*)"time?");
}

void RMTT_Protocol::ReadSN()
{
    SendCMD((char*)"sn?");
}

void RMTT_Protocol::ReadSDKVersion()
{
    SendCMD((char*)"sdk?");
}

void RMTT_Protocol::startUntilControl(){
  pinMode(34, INPUT_PULLUP);
  RMTT_RGB::Init();
  while(!(getTelloMsgString("[TELLO] command",1000)==String("ETT ok"))){}
  RMTT_RGB::SetRGB(0,255,0);
  while (!((digitalRead(34))==0)) {}
  RMTT_RGB::SetRGB(0,0,0);
  delay(1000);
  RMTT_RGB::SetRGB(0,255,0);
  delay(1000);
  RMTT_RGB::SetRGB(0,0,0);
}

// 静态函数
String RMTT_Protocol::getTelloMsgString(char* cmd, uint32_t timeout){
  while(Serial1.available()){
    Serial1.read();
  }
  String back;
  Serial1.printf(cmd);
  long oldtime = millis();
  while(!Serial1.available()){
    long newtime = millis();
    if((newtime-oldtime)>timeout){
      back = "timeout";
      return back;
    }
  }
  while(Serial1.available()){
    back += String(char(Serial1.read()));
  }
  if(back.endsWith("\r\n")){
    back = back.substring(0,back.indexOf("\r\n"));
  }
  return back;
}

int RMTT_Protocol::getTelloMsgInt(char* cmd, uint32_t timeout){
  while(Serial1.available()){
    Serial1.read();
  }
  Serial1.printf(cmd);
  long oldtime = millis();
  while(!Serial1.available()){
    long newtime = millis();
    if((newtime-oldtime)>timeout){
      return -1;
    }
  }
  String back;
  while(Serial1.available()){
    back += String(char(Serial1.read()));
  }
  if(back.endsWith("\r\n")){
    back = back.substring(0,back.indexOf("\r\n"));
  }
  return back.substring(back.indexOf(' ') + 1, back.length()).toInt();
}

String RMTT_Protocol::getTelloResponseString(uint32_t timeout){
  String back;
  long oldtime = millis();
  while(!Serial1.available()){
    long newtime = millis();
    if((newtime-oldtime)>timeout){
      back = "timeout";
      return back;
    }
  }
  while(Serial1.available()){
    back += String(char(Serial1.read()));
  }
  if(back.endsWith("\r\n")){
    back = back.substring(0,back.indexOf("\r\n"));
  }
  return back;
}

int RMTT_Protocol::getTelloResponseInt(uint32_t timeout){
  String back;
  long oldtime = millis();
  while(!Serial1.available()){
    long newtime = millis();
    if((newtime-oldtime)>timeout){
      return -1;
    }
  }
  while(Serial1.available()){
    back += String(char(Serial1.read()));
  }
  if(back.endsWith("\r\n")){
    back = back.substring(0,back.indexOf("\r\n"));
  }
  return back.substring(back.indexOf(' ') + 1, back.length()).toInt();
}

int RMTT_Protocol::getTelloStatusWithNameInt(char* name)
{
	return root[name];
}

float RMTT_Protocol::getTelloStatusWithNameFloat(char* name)
{
	return root[name];
}

void RMTT_Protocol::getTelloStatus(uint32_t timeout){
	String back = getTelloMsgString("[TELLO] status?", timeout);
	back.replace("ETT ","");
	if(back != "timeout"){
		String data[21];
		Split(back, data, 21, ';');
		for(int i=0; i<21; i++){
			String name = data[i].substring(0,data[i].indexOf(":"));
			String value = data[i].substring(data[i].indexOf(":")+1, data[i].length());
			if(name == "baro" || name == "agx" || name == "agy" || name == "agz"){
				root[name] = value.toFloat();
			}else if(name == "mpry"){
				String mpry[3];
				Split(value, mpry, 3, ',');
				root["mpryP"] = mpry[0].toInt();
				root["mpryY"] = mpry[1].toInt();
				root["mpryR"] = mpry[2].toInt();
			}else{
				root[name] = value.toInt();
			}
		}
	}else{
		initStatus();
	}
}

uint8_t re_tag = 0;
uint8_t re_cnt = 0;

int RMTT_Protocol::sendTelloCtrlMsg(char *cmd_str)
{
  re_cnt = 0;
  while(true)
  {
    while(Serial1.available()){
      Serial1.read();
    }
    Serial1.printf("[TELLO] Re%02x%02x %s", re_tag, re_cnt++, cmd_str);
    // Serial.printf("[TELLO] Re%02x%02x %s", re_tag, re_cnt++, cmd_str);
    long oldtime = millis();
    while(!Serial1.available()){
      long newtime = millis();
      if((newtime-oldtime)>1000){
        Serial1.printf("[TELLO] Re%02x%02x %s", re_tag, re_cnt++, cmd_str);
        // Serial.printf("[TELLO] Re%02x%02x %s", re_tag, re_cnt++, cmd_str);
        oldtime = newtime;
      }
    }

    String back;
    while(Serial1.available()){
      back += String(char(Serial1.read()));
    }

    // ETT Re[tag][id] ok/error
    // ETT Rexxxx ok/error
    // Serial.printf(back.c_str());
    if (back.length() >= 12)
    {
      if ((back.c_str()[11] == 'o') && ((back.c_str()[12] == 'k')))
      {
        break;
      }
    }
    else
    {
      delay(100);
    }
  }
  re_tag++;
  return 0;
}
