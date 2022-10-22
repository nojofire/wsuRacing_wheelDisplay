#include <AsyncTimer.h>
#include <MAX72XX.h>
#include <SPI.h>
#include <MegaCAN.h>
#include <mcp2515.h>

/////CHANGE THESE VALUES TO CHANGE WHAT LIMITER YOU WANT AND THE THRESHOLD//limiter values
int increment = 0;
const int limit = 10000; //absolute limit for displayed value
const int flipper = 400;  //counter value used for flipping modes
int mode = 1;
const double threshold = 0.85; //used to calculate threshold to start blinking

int blinker = 0;
const int blink_limit = 1000; //counter limit to control blinking mode of indicator light
int setting =1;

const extern uint32_t baseID=1512;
const int SS_MAX_DISP = PIN_PD7;
const int MOSI_Pin = PIN_PB5;
const int SCK_Pin = PIN_PB7;

//Segmented Display
MAX72XX SegmentedDisp(SS_MAX_DISP,1);     //defines one max chip
MAX72XXDisplay rpmDisp(&SegmentedDisp,0,8); //defines the chip has 8 digits attatched to it


//CAN set up
const int SS_CAN = PIN_PC0;

//Define frames and Squirt Data
MCP2515 mcp2515(SS_CAN);
MegaCAN megasquirt_can;
MegaCAN_broadcast_message_t mega_Dash;

//Define delay timer
void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_PB0,OUTPUT);
  pinMode(SS_MAX_DISP,OUTPUT); 
  Serial.begin(115200);  
 
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS,MCP_20MHZ);
  mcp2515.setNormalMode();
  digitalWrite(PIN_PB0,HIGH);
  
}


void loop() {
  can_frame frame;
  
 
//  if(mcp2515.readMessage(&frame) == MCP2515::ERROR_OK){
//      megasquirt_can.getBCastData(frame.can_id,frame.data, mega_Dash);
//        }
//      //if the value is above set values begins blinking mode
//      if(mega_Dash.tps >= int(limit*threshold)){ 
//        if(increment >= flipper){        
//            mode ^=1;
//            increment = 0;
//          }
//          else{
//            increment++;
//          }
//      }
//      else if(mega_Dash.rpm <int(limit*threshold)){
//        increment = 0;
//        mode = 1;
//      }
//     shiftOn(mega_Dash.rpm,mode);
//
      //blinking light control
      blinker++;
      
      if(blinker >= blink_limit){
        setting ^= 1;
        blinker =0;
      }
      if(setting == 1){
          digitalWrite(PIN_PB0,HIGH);
          rpmDisp.writeNumber(37,MAX72XX::Character::ZERO);
          rpmDisp.update();
              
       }
       else if(setting == 0){
          digitalWrite(PIN_PB0,LOW);
          rpmDisp.writeNumber(456,MAX72XX::Character::ZERO);
          rpmDisp.update();
    
       }
  }

void shiftOn(int value, int mode){
    if(mode){ //mode one: Displays numbers to 7seg display
    rpmDisp.writeNumber(mega_Dash.rpm,MAX72XX::Character::ZERO);
    rpmDisp.update();
    }  
    else{ //mode two: Displays all blanks to 7zeg display
      rpmDisp.fill(MAX72XX::Character::BLANK);
      rpmDisp.update();
    }
   }
