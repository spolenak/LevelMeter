/**
 * @example CompDualStateButton.ino
 * 
 * @par How to Use
 * This example shows that when the dual state button component on the Nextion screen is released,
 * the text of Text component will change one every time.      
 *
 * @author  huang xianming (email:<xianming.huang@itead.cc>)
 * @date    2015/11/11
 * @updated 2016/12/25 bring HMI up to v0.32 to avoid too old issues
 * @convert by Patrick Martin, no other changes made
 * @copyright 
 * Copyright (C) 2014-2015 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */

#include "Nextion.h"
#include "toneAC.h"

/*
 * Declare a dual state button object [page id:0,component id:1, component name: "bt0"]. 
 */
NexDSButton bt0 = NexDSButton(0, 1, "bt0");
NexDSButton bt1 = NexDSButton(0, 2, "bt1");
NexProgressBar j0 = NexProgressBar(0, 3, "j0");
NexProgressBar j1 = NexProgressBar(0, 5, "j1");
NexNumber n0 = NexNumber(0, 10, "n0");
NexNumber n1 = NexNumber(0, 11, "n1");

int ledPin = 4;

boolean sensor1State = false;
boolean sensor2State = false;

boolean playSound = false;
uint32_t timeoutLoop = 0;

/*
 * Register a dual state button object to the touch event list.  
 */
NexTouch *nex_listen_list[] = 
{
    &bt0,
    &bt1,
    NULL
};
    
void bt0PopCallback(void *ptr)
{
      uint32_t dual_state;
      NexDSButton *btn = (NexDSButton *)ptr;

      if (btn->getValue(&dual_state) == false) {
      return;
    }

      uint32_t bt1State = 0;
      bt1.getValue(&bt1State);

      if (bt1State == 0)
        bt1.setValue(1);
      else
        bt1.setValue(0);
    

        
    //digitalWrite(ledPin, dual_state == 1 ? HIGH : LOW);

    playSound = true;
}

void bt1PopCallback(void *ptr)
{
    uint32_t dual_state;
    NexDSButton *btn = (NexDSButton *)ptr;

    if (btn->getValue(&dual_state) == false) {
      return;
    }

    digitalWrite(ledPin, dual_state == 1 ? HIGH : LOW);
    playSound = true;
}

void setup(void)
{    
    pinMode(ledPin, OUTPUT);      // sets the digital pin as output

     /* Set the baudrate which is for debug and communicate with Nextion screen. */
    nexInit();

    /* Register the pop event callback function of the dual state button component. */
    bt0.attachPop(bt0PopCallback, &bt0);
    bt1.attachPop(bt1PopCallback, &bt1);
}

uint16_t pipserState = 1;
boolean playPipser = true;
uint32_t timerOnPipser = 100000;
uint32_t timerOffPipser = 0;

void loop(void)
{   
    /*
     * When a pop or push event occured every time,
     * the corresponding component[right page id and component id] in touch event list will be asked.
     */

    
    if (playPipser) {
        if (pipserState == 1 && timerOffPipser == 0 ) {
          pipserState = 0;
          timerOnPipser=100000;
          digitalWrite(ledPin, LOW);
        } else if (pipserState == 0 && timerOnPipser == 0) {
          pipserState = 1;
          timerOffPipser = 100000;
          digitalWrite(ledPin,  HIGH);
        } else {
          if (timerOnPipser > 0) timerOnPipser--;
          if (timerOffPipser > 0) timerOffPipser--;
        }
    }

    if(playSound) {
        toneAC(2000,10,1000);
        playSound = false;
        timeoutLoop = 300000;
    } else if (timeoutLoop == 0) {
      nexLoop(nex_listen_list);
      refreshDisplay();
    } else {
      if (timeoutLoop > 0) {
        timeoutLoop--;
      }
    }



}

uint32_t cntRefresh = 0;
uint8_t val1 = 0;
uint8_t val2 = 0;
    
void refreshDisplay()
{
   cntRefresh ++;
   if (cntRefresh < 200000) {
      return;
   }
    cntRefresh = 0;

    j0.setValue(val1);
    n0.setValue(val1++);
    j1.setValue(val2);
    n1.setValue(val2++);

    val1 %= 100;
    val2 %= 100;
}
 












