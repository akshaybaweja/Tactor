#include <Servo.h>
#include <Tactor.h>

#define TACTOR_COUNT 7

const int ledPin[] = {A2, A1, A0, A4, 13, 12, 11, 10};
const int servoPin[] = {2, 3, 4, 5, 6, 7, 8, 9};

int servoFrameMillis = 20; //minimum time between servo updates
float startPos = 0;

Servo servos[8];
Tactor tactors[8];

//Serial Port
const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];

int tactorFromP;
int rangeFromP;
int velFFromP;
int velTFromP;
int durationGFromP;
int durationFromP;
int randomnessFromP;
int randomnessGFromP;
int minimumFromP;
int maximumFromP;

boolean newData = false;

int countF = 0; //count fromward
int countT = 0; //count toward

int range = 100;

int minimum_angle;
int maximum_angle;

int randomness = 0;
int randomnessG = 0;

int velF = 60;  //velocity fromward
int velT = 60;  //velocity toward

int delayF = 2; //control the speed
int delayT = 2;

int speed_ = 1; //speed_ always equals to 1;
bool from = false;

int duration = 10;
int period = (duration / 1.5) * 100;
int period_;

int durationG = 300;

unsigned long time_now = 0;

bool flag = false;

void setup() {

  Serial.begin(9600);

  for (int i = 0; i < TACTOR_COUNT; i++) {
    servos[i].attach(servoPin[i]);
    tactors[i].begin(servos[i], servoFrameMillis, startPos, ledPin[i]);
  }

  //wait 1 sec after initialization
  delay(1000);
}

void loop() {
  // Read Data from Host
  recvWithStartEndMarkers();

  if (newData == true) {
    strcpy(tempChars, receivedChars);
    parseData();
    //showParsedData();
    newData = false;
  }

  range = rangeFromP;
  velF = velFFromP;
  velT = velTFromP;
  durationG = durationGFromP; //gap duration
  duration = durationFromP;
  randomness = randomnessFromP;
  randomnessG = randomnessGFromP;
  minimum_angle = minimumFromP;
  maximum_angle = maximumFromP;

   // tactor funtions
  if (tactorFromP == 1 && !flag) {
    testTactor(0);
    flag = true;
  }

  updateTactors();

  if(tactors[0].hasArrived())
    tactors[0].deactivate();
}

void updateTactors() {
  for (int i = 0; i < TACTOR_COUNT; i++)
    tactors[i].update();
}

void testTactor(int i) {
  tactors[i].activate();
  tactors[i].moveTo(maximum_angle, ((maximum_angle - minimum_angle) / velT) * 1000);
  tactors[i].start();
}


void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      }
      else {
        receivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

void parseData() {      // split the data into its parts

  char * strtokIndx; // this is used by strtok() as an index

  //tactor
  strtokIndx = strtok(tempChars, ",");     // get the first part - the string
  tactorFromP = atoi(strtokIndx); // copy it to messageFromPC
  //toward_vel
  strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
  velTFromP = atoi(strtokIndx);     // convert this part to an integer
  //fromward_vel
  strtokIndx = strtok(NULL, ",");
  velFFromP = atoi(strtokIndx);     // convert this part to a float
  //range
  strtokIndx = strtok(NULL, ",");
  rangeFromP = atoi(strtokIndx);
  //minimum_angle
  strtokIndx = strtok(NULL, ",");
  minimumFromP = atoi(strtokIndx);
  //maximum_angle
  strtokIndx = strtok(NULL, ",");
  maximumFromP = atoi(strtokIndx);
  //motion_randomness
  strtokIndx = strtok(NULL, ",");
  randomnessFromP = atoi(strtokIndx);
  //gap randomness
  strtokIndx = strtok(NULL, ",");
  randomnessGFromP = atoi(strtokIndx);
  //duration
  strtokIndx = strtok(NULL, ",");
  durationFromP = atoi(strtokIndx);
  //gap duration
  strtokIndx = strtok(NULL, ",");
  durationGFromP = atoi(strtokIndx);
}

