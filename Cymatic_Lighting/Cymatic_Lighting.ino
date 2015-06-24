#include <Adafruit_NeoPixel.h>
#define PIN 5
#define CNT_LIGHTS 60

Adafruit_NeoPixel strip = Adafruit_NeoPixel(CNT_LIGHTS, PIN, NEO_GRB + NEO_KHZ800); 
//fixed settings 
int analogPinL = 1; // read from multiplexer using analog input 0
int analogPinR = 0; // read from multiplexer using analog input 0
int strobePin = 4; // strobe is attached to digital pin 4
int resetPin = 5; // reset is attached to digital pin 5
int spectrumValueL[6];
int spectrumValueR[6]; // to hold a2d values
int previousSpectrumValueL[6]; // to hold a2d values
int previousSpectrumValueR[6]; // to hold a2d values
int global_brightness = 255; // Sets global brightness, i.e. 64 is 1/4 brightness.
int useColor[2];
int num;
float waveValue;
int prop[CNT_LIGHTS];
int prop_history[CNT_LIGHTS];
int refresh_counter = 0;
int use_refresh = 0;
float use_brightness = 0;
int tmp_refresh_adj = 0;

//lowest reading the MSGEQ7 should recognize 1-1000 range
int minFilter = 50;
//higher number refreshes slow - refreshed every nth interation
int refresh = 20;



 
void setup() 
{
  Serial.begin(9600); // print to serial monitor
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(analogPinL, INPUT);
  pinMode(analogPinR, INPUT);
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  analogReference(DEFAULT);
  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH); 
}
 
void loop() 
{    
   runTime();
}
 
 
void runTime()
{
 
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);
  int changeL = 0;
  int changeR = 0;
  int changePinL = 0;
  int changePinR = 0;
  int k,i,r,g,b;
  float pot_value = 0.0;
  
  int use_le = 0;
  int use_ls = ((CNT_LIGHTS/2)-1);
  int use_rs = ((CNT_LIGHTS/2)+0);
  int use_re = (CNT_LIGHTS-1);
  int spectrum_totalL = 0;
  int spectrum_totalR = 0;
  int spectrum_counts = 0;
  
 
 
  //get readings from chip
  for (i = 0; i < 7; i++)
  {
    spectrum_counts++;
    previousSpectrumValueL[i] = spectrumValueL[i];
    previousSpectrumValueR[i] = spectrumValueR[i];
    digitalWrite(strobePin, LOW);
    delayMicroseconds(30); // to allow the output to settle
    spectrumValueL[i] = analogRead(analogPinL);
    spectrumValueR[i] = analogRead(analogPinR);
    
    spectrum_totalL+= spectrumValueL[i];
    spectrum_totalR+= spectrumValueR[i];
    
    if( spectrumValueL[i] > 400 && i <=1)
          tmp_refresh_adj+= 23;
    if( spectrumValueR[i] > 400 && i <=1)
          tmp_refresh_adj+= 23;
    
    digitalWrite(strobePin, HIGH); 
   
  }//for i
  
  changePinL = (spectrum_totalL);
  changePinR = (spectrum_totalR);
  use_refresh = refresh;
  use_brightness = global_brightness;
  
  use_refresh = 0;
 
  
  

  if(tmp_refresh_adj < 0) { tmp_refresh_adj = 0; }
  if(tmp_refresh_adj > 200) { tmp_refresh_adj = 200; }
 
  refresh_counter++;
  if(refresh_counter>=(use_refresh - round(tmp_refresh_adj * .1)))
  {
    //reset the counter
    refresh_counter = 0;
    tmp_refresh_adj-= 1;
         
    //save the history - RIGHT SIDE
    for (k = use_rs; k <= use_re; k++)
    {
      prop_history[k] = prop[k-1];
    }//for
     
    for (k = use_rs; k <= use_re; k++)
    {
      prop[k] = prop_history[k];
    }//for
    
    
    //save the history - LEFT SIDE
    for (k = use_ls; k >= use_le; k--)
    {
      prop_history[k] = prop[k+1];
    }//for
     
    for (k = use_ls; k >= use_le; k--)
    {
      prop[k] = prop_history[k];
    }//for
    
    //current;
    prop[use_rs] = changePinR;
    prop[use_ls] = changePinL;
            
    for (k = 0; k < CNT_LIGHTS; k++)
    {
      num = prop[k];
      //Serial.println(num);
      if(num>=0)
      {
        get_color();
        strip.setPixelColor( k, useColor[0], useColor[1], useColor[2]);
      }//if
    }//for
         
     
  }//if refresh  

  
  strip.show();
}
 
void get_color()
{
  int r,g,b;
  
  if(num < 500)
  {
    r = 0; g = 0; b = 0;
    
    useColor[0] = g;
    useColor[1] = r;
    useColor[2] = b;
  }
  
  if(num>500)
  {
    getWaveLength();
    getRGB();  
   
    
  }
}

void getWaveLength()
{
  float minVal = 500;
  float maxVal = 4700;
  float minWave = 350;
  float maxWave = 650;
  maxVal = 0;
  minVal = 0;
  if(num>maxVal)
    maxVal = num;
    
  waveValue = ((num - minVal) / (maxVal-minVal) * (maxWave - minWave)) + minWave;
}

void getRGB()
{
  float gamma = .8;
  float rz, gz, bz;
  int intsMax = 255;
  int r,g,b;
  
  if(waveValue >380 && waveValue <=439)
  {
    rz = (waveValue-440)/(440-380);
    gz = 0;
    bz = 1;
  }
  
  if(waveValue >=440 && waveValue <=489)
  {
    rz = 0;
    gz = (waveValue-440)/(490-440);
    bz = 1;
  }
  
  if(waveValue >=490 && waveValue <=509)
  {
    rz = 0;
    gz = 1;
    bz = (waveValue-510)/(510-490);
  }
  
  if(waveValue >=510 && waveValue <=579)
  {
    rz = (waveValue-510)/(580-510);
    gz = 1;
    bz = 0;
  }
  
  if(waveValue >=580 && waveValue <=644)
  {
    rz = 1;
    gz = (waveValue-645)/(645-580);
    bz = 0;
  }
  
  if(waveValue >=645 && waveValue <=780)
  {
    rz = 1;
    gz = 0;
    bz = 0;
  }
  
  r = rz * 255;
  b = bz * 255;
  g = gz * 255;
  
    useColor[0] = g;
    useColor[1] = r;
    useColor[2] = b;
  
}


