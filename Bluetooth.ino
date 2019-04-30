#include <Clock.h>
#include <Event.h>
#include <Galaxia.h>
#include <HWI.h>
#include <Mailbox.h>
#include <Semaphore.h>
#include <SWI.h>
#include <SWItrigger.h>
#include <Task.h>
#include <Timer.h>

#include "utils.h"

boolean stringComplete; //set when the user presses return
uint8_t len; //length of read string
String str; //string read from UART input
int constant; //designates the PID constant to be modified by the next input (0=none, 1=kP, 2=kD, 3 = kI)

/*
 * Setup for UART communication with the Bluetooth card, initializing globals
 * 
 * @param none
 * @return void
 */
void setupBluetooth() {
  Serial1.begin(115200);
  stringComplete = 0;
  len = 0;
  str = "";
  constant = 0;
}

/*
 * Main Bluetooth "thread" that gets run periodically to read commands from the UART and process them
 * 
 * @param none
 * @return void
 */
void loopBluetooth() {
  char input;
  int temp;
  //union str {
  //}
  //while (1) {
  if (stringComplete == 0 && Serial1.available()) {
    input = Serial1.read();              // read input
    if (input == '\b' && len > 0) { // if input is backspace, move string position back 1 (-2 + 1) to allow it to overwrite the last character inputted
      Serial1.print("\b \b");         // put a space in place of the last character so it actually goes away then backspace again so the cursor is in the right spot
      if(str.length() > 0) str.remove(str.length() - 1);
      len--;                      // move backward in string
    } else if (input == '\b') {   // keep user from backspacing into the prompt and keep i from going below 0 (string[-1] == death or something)
      len = 0;
    } else if (input == '\r') {
      //str += '\0';
      if(len != 0)
        stringComplete = 1;
    } else {                      // if input isn't a backspace, write character to the string
      Serial1.write(input);               // print input (uart doesn't do that by default)
      len++;
      str += input;
      if (len == 10) {
        str = str;
        stringComplete = 1;
      }
    }
  } else if (stringComplete == 1) {
    Serial1.print("\n\r");
    temp = runCommand(str);
    if (temp != -1) {
      
    } else {
      if(str.equals("kp")) {
        constant = 1;
      } else if(str.equals("kd")) {
        constant = 2;
      } else if(str.equals("ki")) {
        constant = 3;
      } else if(constant != 0) {
        temp = str.toInt();
        switch(constant) {
         case 1:
          kp = temp;
          Serial1.println(kp);
          break;
         case 2:
          kd = temp * 10;
          Serial1.println(kd);
          break;
         case 3:
          ki = temp;
          Serial1.println(ki);
          break;
         default:
          break;
        }
      } else {
        Serial1.print("Unknown Command");
        Serial1.print("\n\r");
      }
    }
    stringComplete = 0;
    len = 0;
    str = "";
  } else {
    // do nothing
  }
  //temp = resolveCommand(in);             // resolve command string to command number
  //if (temp != -1) {                      // if it could be resolved, put it in command queue
  //i = fifo_put(commandQueue, &temp);
  //runCommands();
  //if(i == -1)
  //    Serial1.print("FIFO full");   // prints command if the fifo is full
  //} else {
  //    Serial1.print("\n\r");            // if command couldn't be resolved, go to new line
  //    Serial1.print("Unknown command"); // then output command unknown
  //}
  //}
}
