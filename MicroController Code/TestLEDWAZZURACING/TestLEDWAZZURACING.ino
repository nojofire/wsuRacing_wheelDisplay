// #include <LedControl.h>

// Defining pins
#define MOSI_Din 11 // Master Out Slave In -to-> Data in
#define SS_LOAD 19 // 19 = A5 on nano // Slave Select -to-> Load
#define SCK_CLK 13 // Serial Clock -to-> Clock in
#define ANALOG_IN 14 // Signal from variable potv

// the program will take an integer and send the binary representation of it of it to the MAX7219.
// We are concerned only with 8 digits at a time as the max 7219 will latch an 8 bit value and display that to each digit.
// the digits are time multiplexed where each digit gets pulled to ground alternating (for our purposes) between digit 1 (most significant)
// and digit 0 (least significant). Each "digit" can be can be set to display or not (reg_scanLimit), and for each digit there are 7 wires
// (segment DP-G) that can be pulled high depending on what you code. I'm going to display a bar graph showing how much relative RPM the engine 
// is running at. To decode the RPM into a number of LEDs we'll use a function that will take in a percentage (as an integer between 
// 1 and 100) and display a set number of LEDs based on percentage breakpoints. I chose 15 LEDs because that's the number I saw on a few
// F1 wheels, but this number can change depending on how much room I have. The MAX7219 can accomidate 8 sets of 8 leds, or 8 individual
// digits on a seven segment display (w/ DP).



// This array will be used to show a set number of LEDs based on the rpmPercentage.
// 128 will be 1000 0000 which according to how we set up our circuit will light up the most significant LED, then
// 192 will light up the 2 most significant LEDs as it's binary representation is 1100 0000. up to 1111 1110 1111 1111 or 15 leds0
int barGraph_decimal_to_LED[8] = { 128, 192, 224, 240, 248, 252, 254, 255}; // [0] = 1 LED (leftmost), [1] = 2 LEDs, ... [7] all eight LEDs on

// defining variables
int rpmPercent = 0;
int rpmIn = 0;
int maxRpm_temp = 0;



// defining max7219 command registers
byte reg_noOp = 0x00;
byte reg_digit0 = 0x01;
byte reg_digit1 = 0x02;
byte reg_digit2 = 0x03;
byte reg_digit3 = 0x04;
byte reg_digit4 = 0x05;
byte reg_digit5 = 0x06;
byte reg_digit6 = 0x07;
byte reg_digit7 = 0x08;
byte reg_decodeMode = 0x09;
byte reg_intensity = 0x0a;
byte reg_scanLimit = 0x0b;
byte reg_shutdown = 0x0c;
byte reg_displayTest = 0x0f;

void makePercentage(int rpmIn, int *rpmPercent_ptr) // converting the rpm number we get to a ratio ( rpm_in / MAX_RPM )
{
  *rpmPercent_ptr = ((double)rpmIn / (double)maxRpm_temp) * 100;
}

void ledOut (int rpmPercentage)
{
  if (rpmPercent == 0) {
  output(reg_digit1, 0x00); // no LEDs on
  output(reg_digit0, 0x00);
  }
  else if (rpmPercentage > 0 && rpmPercentage < 13)
  {
     output(reg_digit1, barGraph_decimal_to_LED[0]); // 1 LED; 1000 0000 0000 0000
     output(reg_digit0, 0x00);
  }
  else if (rpmPercentage >= 13 && rpmPercentage < 20)
  {
    output(reg_digit1, barGraph_decimal_to_LED[1]); // 2 LEDs; 1100 0000 0000 0000
    output(reg_digit0, 0x00);
  }
  else if (rpmPercentage >= 20 && rpmPercentage < 27)
  {
    output(reg_digit1, barGraph_decimal_to_LED[2]); // 3 LEDs; 1110 0000 0000 0000
    output(reg_digit0, 0x00);
  }
  else if (rpmPercentage >= 27 && rpmPercentage < 33) 
  {
    output(reg_digit1, barGraph_decimal_to_LED[3]); // 4 LEDs; 1111 0000 0000 0000
    output(reg_digit0, 0x00);
  }
  else if (rpmPercentage >= 33 && rpmPercentage < 40)
  {
    output(reg_digit1, barGraph_decimal_to_LED[4]); // 5 LEDs; 1111 1000 0000 0000
    output(reg_digit0, 0x00);
  }
  else if (rpmPercentage >= 40 && rpmPercentage < 47)
  {
    output(reg_digit1, barGraph_decimal_to_LED[5]); // 6 LEDs; 1111 1100 0000 0000
    output(reg_digit0, 0x00);
  }
  else if (rpmPercentage >= 47 && rpmPercentage < 53)
  {
    output(reg_digit1, barGraph_decimal_to_LED[6]); // 7 LEDs; 1111 1110 0000 0000;
    output(reg_digit0, 0x00);
  } 
  else if (rpmPercentage >= 53 && rpmPercentage < 60)
  {
    output(reg_digit1, barGraph_decimal_to_LED[7]); // 8 LEDs; 1111 1111 0000 0000;
    output(reg_digit0, 0x00);
  
  } // digit1 will now stay constant for higher percentages
  else if (rpmPercentage >= 60 && rpmPercentage < 67)
  {
    output(reg_digit1, barGraph_decimal_to_LED[7]);
    output(reg_digit0, barGraph_decimal_to_LED[0]); // 9 LEDs; 1111 1111 1000 0000;
  }
  else if (rpmPercentage >= 67 && rpmPercentage < 73)
  {
    output(reg_digit1, barGraph_decimal_to_LED[7]);
    output(reg_digit0, barGraph_decimal_to_LED[1]); // 10 LEDs; 1111 1111 1100 0000;
  }
  else if (rpmPercentage >= 73 && rpmPercentage < 80)
  {
    output(reg_digit1, barGraph_decimal_to_LED[7]);
    output(reg_digit0, barGraph_decimal_to_LED[2]); // 11 LEDs; 1111 1111 1110 0000;
  }
  else if (rpmPercentage >= 80 && rpmPercentage < 87)
  {
    output(reg_digit1, barGraph_decimal_to_LED[7]);
    output(reg_digit0, barGraph_decimal_to_LED[3]); // 12 LEDs; 1111 1111 1111 0000;
  }
  else if (rpmPercentage >= 87 && rpmPercentage < 93)
  {
    output(reg_digit1, barGraph_decimal_to_LED[7]);
    output(reg_digit0, barGraph_decimal_to_LED[4]); // 13 LEDs; 1111 1111 1111 1000;
  }
  else if (rpmPercentage >= 93 && rpmPercentage < 100)
  {
    output(reg_digit1, barGraph_decimal_to_LED[7]);
    output(reg_digit0, barGraph_decimal_to_LED[5]); // 14 LEDs; 1111 1111 1111 1100;
  }
  else // == 100
  {
    output(reg_digit1, barGraph_decimal_to_LED[7]);
    output(reg_digit0, barGraph_decimal_to_LED[6]); // 15 LEDs; 1111 1111 1111 1110;
  }
}

void ledStartup(void)
{
  rpmPercent = 0;
  int loaded = 0; // this is not meaningful right now, my intention is to have this startup last as long as some external timing. For instance if some process of the car takes a long time (relatively).
  while (!loaded)
  {
    ledOut(rpmPercent++);
    if(rpmPercent == 101) 
    {
      delay(100);
      for (int i = 0; i < 4; i++) // this will flash an alternating pattern
      {
        delay(75);
        output(reg_digit1, 170); //decimal 42 goes -to-> 1010 1010;
        output(reg_digit0, 170);
        delay(75);
        output(reg_digit1, 85); // decimal 85 goes -to-> 0101 0101
        output(reg_digit0, 85);
      }
      output(reg_digit1, 0x00); // no LEDs on
      output(reg_digit0, 0x00);
      delay(100);
      rpmPercent = 0; // reseting value
      loaded = 1;
    }
    delay(25);
  }
}

void initialize(void)
{
  pinMode(MOSI_Din, OUTPUT);
  pinMode(SS_LOAD, OUTPUT);
  pinMode(SCK_CLK, OUTPUT);
  digitalWrite(SS_LOAD, HIGH);
}

void output(byte address, byte data) // data gets shifted into registers over 16 clock cycles, but don't get latched unless SS_LOAD is set high on, or soon after the 16th rising edge of the clock.
{
  digitalWrite(SS_LOAD, LOW); // setting low to start data out
  shiftOut(MOSI_Din, SCK_CLK, MSBFIRST, address); // 8 bit address first
  shiftOut(MOSI_Din, SCK_CLK, MSBFIRST, data); // 8 bit data
  digitalWrite(SS_LOAD, HIGH); // latching the data into registers
}

void setup() {
  // put your setup code here, to run once:
initialize();
output(reg_displayTest, 0x00);  //display test register - test mode off = 0x00; on = 0x01;                                       
output(reg_shutdown, 0x01);  //shutdown register - normal operation = 0x01; shutdown = 0x00
output(reg_scanLimit, 0x01);  //scan limit register - display digits 0 through 7 = 0x07; 0 - 6 = 0x06; 0-5 = 0x05, etc all the way to display only digit 0 = 0x00
output(reg_intensity, 0x0f);  //intensity register - max brightness = 0xXf; min = 0xX0; X = dont care
output(reg_decodeMode, 0x00);  //decode mode register - CodeB decode all digits in BCD  = 0x01; no BCD = 0x00

output(reg_digit1, 0x00);
output(reg_digit0, 0x00); //digit 0 (rightmost digit) data

rpmPercent = 0;
maxRpm_temp = 1023; // for 5 volts. the ADCs on the arduino convert input voltages between 0 and 5 volts (the operating voltage of the nano), and converts them to integers between 0 and 1023
ledStartup();

Serial.begin(9600);
}

// random number
int randNum = random(2);

void loop() { // this will just be a kind of simulation of some a drifting value randomly  up and down
    // put your main code here, to run repeatedly:
   delay (50);
  
    if (randNum == 0)
    {
      // decrement rpmPercent by 1-5%;
      rpmPercent -= random(1,6);
    }
    else
    {
      // increment by 1-5%;
      rpmPercent += random(1,6);
    
    }
    ledOut(rpmPercent);
    if (rpmPercent < 5) // if decrementing will put us negative we force an increment
    {
      rpmPercent += 5;
    }
    else if(rpmPercent > 95) // preventing us from going over 
    {
      rpmPercent -= 5;
      }
    randNum = random(2);
    delay(10);
}
