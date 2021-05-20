/* Robot ARM with 6 Servos - Version 2.0
 * Connected via Sonoff wifi 5v DC 
 * Trigger via Power Platform
 * 
 * Updated: May 20th, 2021
   Author: Waldemir Cambiucci
*/

#include <Servo.h>

Servo myServo[6];
/*
 * define the list of pins for our servos
 */
int pinServo[6] = {3, 5, 6, 9, 10, 11};
/*
 * Capture the trigger
 */
int pinTriggerRun = 2;
int stateARM = 0;
/*
 * States for the ARM
 */
#define ARM_SETUP_SERVO 0
#define ARM_WAIT_TRIGGER 1
#define ARM_PLAY_PRESET 2
#define ARM_STOP_PRESET 3
/*
 * Pre-set of movement
 */
int totPreSet = 17;
int posPreSet[17][2] = {
      {1, 30 },      
      {1, 90 },
      {4, 30 }, 
      {1, 30 },      
      {4, 90 }, 
      {1, 90 },
      {0, 30 },
      {0, 100 },
      {4, 90 },            
      {1, 30 },      
      {2, 30 },
      {0, 30 },
      {0, 100 },
      {1, 90 },
      {2, 90 },    
      {1, 30 },            
      {1, 90 }
  };

/*----------------------------------------------------------------------------
 * the setup function runs once when you press reset or power the board
 */
void setup() {
  
  // State control
  stateARM = ARM_SETUP_SERVO;  
  
  Serial.begin(9600);
  Serial.println("Starting setup() - ARM_SETUP_SERVO");   
   
  // Attaching servos to consecutive pins
  for(int i=0; i<6; i++)
  {
    Serial.println("Attaching myServo[i] : " + String(i) );
    myServo[i].attach(pinServo[i]); 
    delay(100);
    myServo[i].write(90);   
    delay(200);
  }  
  Serial.println("Finishing setup() - ARM_SETUP_SERVO");
  pinMode(pinTriggerRun, INPUT);
  delay(100);  
  
  // State control
  stateARM = ARM_WAIT_TRIGGER;
 }

/*-----------------------------------------------------------------------------
 * the loop function runs over and over again forever
 */
void loop() {
  
  int presetServo = 0;
  int presetAngle = 0;    

  // Reading the pin from sonoff - trigger state
  int readTrigger = 0;

  //------------------------------------------------------------
  if(stateARM == ARM_WAIT_TRIGGER){  
    // Reading trigger from sonoff      
    readTrigger = digitalRead(pinTriggerRun);
    delay(100);
    Serial.println("readTrigger = " + String (readTrigger));
    if( readTrigger == 1 ){
      Serial.println("sonoff off - no trigger!");   
    }
    else{
      Serial.println("sonoff on - trigger received!");    
      stateARM = ARM_PLAY_PRESET;     
    }   
  }
  //------------------------------------------------------------

  /*
   * Swith case for selected PRESET
   */
  switch (stateARM) {
    /*
     * ARM_WAIT_TRIGGER
     */
    case ARM_WAIT_TRIGGER:
      Serial.println("ARM_WAIT_TRIGGER");
      delay(100);      
      break;
      
    /*
     * ARM_PLAY_PRESET
     */
    case ARM_PLAY_PRESET:   
      for(int i=0; i < totPreSet; i++) {
      
        // Running the pre-sets of movements.
        presetServo = posPreSet[i][0];
        presetAngle = posPreSet[i][1];
        
        // Go to mobe the servo.
        Serial.println("presetServo = " + String (presetServo));
        Serial.println("presetAngle = " + String (presetAngle));

        // Moving the servo to the next angle position.
        myServo[presetServo].write(presetAngle);                 
        delay(400); 
      }
      
      // List of preset finished!
      stateARM = ARM_STOP_PRESET; 
      delay(100);      
      break;      
      
    /*
     * ARM_STOP_PRESET
     */
    case ARM_STOP_PRESET:
      Serial.println("ARM_STOP_PRESET");
      stateARM = ARM_SETUP_SERVO;
      delay(100);      
      break;
      
      /*
     * ARM_SETUP_SERVO
     */
    case ARM_SETUP_SERVO:
      Serial.println("Starting ARM_SETUP_SERVO...");
      Serial.println("Finishing ARM_SETUP_SERVO...");    
      stateARM = ARM_WAIT_TRIGGER;
      delay(100);      
      break;
      
    /*
     * default
     */    
    default:
      Serial.println("default");      
      stateARM = ARM_SETUP_SERVO;
      delay(100);      
      break;
   /*
    * Ending switch case
    */
  }
}
