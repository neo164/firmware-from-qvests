int potpin=0; 
int potpin2=0; 
int potpin3=0; 
int led=13;
int m = 6;
int x=700;//500
int y=155;//240
const int sensorPin = A1;
int sensorValue = 0;         // the sensor value
int sensorMin = 1023;        // minimum sensor value
int sensorMax = 0;           // maximum sensor value

void setup() 
{ 
  Serial.begin(9600);
  pinMode(6,OUTPUT);
  pinMode(9,OUTPUT); 
  pinMode(10,OUTPUT); 
  pinMode(11,OUTPUT); 
  pinMode(13,OUTPUT); 
  pinMode(14,INPUT); 
  digitalWrite(led,1);
  delay(200);
  digitalWrite(led,0);

} 

void loop () 
{ label:
  sensorValue = analogRead(sensorPin);

  // apply the calibration to the sensor reading
  sensorValue = map(sensorValue, sensorMin, sensorMax, 0, 255);
  // in case the sensor value is outside the range seen during calibration
  sensorValue = constrain(sensorValue, 0, 255);

  potpin=analogRead(14); 

  Serial.println(potpin,DEC); 
  Serial.print("intens:"); 
  Serial.println(sensorValue,DEC);

  if (potpin>=525){
    analogWrite(11,255); 
    analogWrite(9,255); 
    analogWrite(10,255); 
    digitalWrite(led,1);
    delay(10);
    digitalWrite(led,0);
    delay(500);
  }
  else{

if(potpin <= 50){
analogWrite(9,255);
analogWrite(10,0);
analogWrite(11,0);
  goto label;
}
    potpin=(potpin*2)-(potpin/2); 
    //potpin+=50;
    if(potpin >= 50){
      for(int t=0;t<=sensorValue;t++){
        analogWrite(m,t);
        delay(1);
       
       
      }
    } 
    delay(10);
    analogWrite(m,0);
    delay(potpin);



    if(potpin<=x) 
    { 


      if(potpin>=0 && potpin<=y) 
      { 

        analogWrite(11,0); 
        analogWrite(9,y); 
        analogWrite(10,potpin); 


      } 
      else 
      { 

        analogWrite(11,0); 
        analogWrite(9,(x-potpin)); 
        analogWrite(10,y); 




      } 

    } 
    //*******************************************************// 

    potpin2=(potpin-x); 

    if(potpin2<=0) 
    { 
      potpin2=0; 
    }    
    else 
    {    
      if(potpin2<=x) 
      { 
        if(potpin2>0 && potpin2<=y) 
        { 
          analogWrite(9,0); 
          analogWrite(10,y); 
          analogWrite(11,potpin2); 



        } 
        else 
        { 
          analogWrite(9,0); 
          analogWrite(10,(x-potpin2)); 
          analogWrite(11,y); 

        } 

      }    
    } 

    //***************************************************// 

    potpin3=(potpin-1023); 
    if(potpin3<=0) 
    { 
      potpin3=0; 
    } 
    else 
    { 
      if(potpin3<=x) 
      { 
        if(potpin3>0 && potpin3<=y) 
        { 
          analogWrite(10,0); 
          analogWrite(11,y); 
          analogWrite(9,potpin3);


        } 
        else 
        { 
          analogWrite(10,0); 
          analogWrite(11,(x-potpin3)); 
          analogWrite(9,y); 

        } 
      } 
    } 
  }
} 

potpin = map(potpin, 0, 1024, 0, 255);







