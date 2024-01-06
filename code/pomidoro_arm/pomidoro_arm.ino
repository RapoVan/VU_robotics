#include <LiquidCrystal.h>
#include <Servo.h>

// Create a Servo object
Servo myservo;

// Create a LiquidCrystal object for the LCD
LiquidCrystal lcd(12, 11, 4, 5, 6, 7);

// Define pin assignments for components
const int led1 = 8;
const int led2 = 9;
const int button1 = 2;
const int button2 = 3;
const int servo = 10;

// Variables for the timer
unsigned long startTime = 0;
unsigned long elapsedTime = 0;
volatile bool buttonPressed = false;

unsigned long time_start = 0;
unsigned long time = 0;

// Define time intervals for study and breaks
const int study_minutes = 1;
const int repeats = 2;              // Number of study-break cycles
const int short_break_minutes = 1;
const int long_break_minutes = 2;

int count = 0;                      // Counter for study-break cycles
int break_duration;                 // Duration of breaks
long Timestamp_Button_Pressed;      // Timestamp when the button is pressed

void setup() {
  // Initialize the LCD
  lcd.begin(16, 2);

  // Set pin modes for components
  pinMode(button1, INPUT_PULLUP);
  myservo.attach(servo);

  // Display initial message on the LCD
  lcd.print("Press the button");
  lcd.setCursor(0, 1);
  lcd.print("to start");

  // Attach an interrupt to the button1 pin that triggers on a falling edge
  attachInterrupt(digitalPinToInterrupt(button1), buttonInterrupt, FALLING);
}

// Interrupt service routine for the button press
void buttonInterrupt() {
  buttonPressed = true;
  time_start = millis();  // Record the start time of the button press
}

// Function to update the LCD display with elapsed time
void updateDisplay(unsigned long ms) {
  unsigned int minutes = ms / 60000;  // Convert milliseconds to minutes
  unsigned int seconds = (ms / 1000) % 60;   // Extract remaining seconds

  // Update the display with elapsed time
  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  lcd.print(minutes);
  lcd.print("m ");
  lcd.print(seconds);
  lcd.print("s ");
}

// Function to convert minutes to milliseconds
unsigned long minute_to_millis(const int minute) {
return minute * 60000;
}

void loop() {
  if (buttonPressed) { 
    lcd.clear();  // Clear the LCD screen
    count = 0;    // Reset the study-break cycle counter

    while(count < repeats){

      myservo.write(23);  // Set servo position for study time (adjust depending on servo position)

      while(time < minute_to_millis(study_minutes)) {
        time = millis() - time_start;   // Calculate elapsed time
        lcd.setCursor(0, 0);
        lcd.print("Study time!");
        updateDisplay(time);
        digitalWrite(led1, HIGH);       // Turn on study LED
        digitalWrite(led2, LOW);        // Turn off break LED
      }

      time_start = millis();            // Record the start time for the break
      time = millis() - time_start;
      myservo.write(150);               // Set servo position for break (adjust depending on servo position)

      if(count==(repeats-1)){
        break_duration = long_break_minutes;
        lcd.setCursor(0, 0);
        lcd.print("Long break");
      }else{
        break_duration = short_break_minutes;
        lcd.setCursor(0, 0);
        lcd.print("Short break");
      }

      while(time < minute_to_millis(break_duration)) {
        time = millis() - time_start;
        updateDisplay(time);
        digitalWrite(led1, LOW);        // Turn off study LED
        digitalWrite(led2, HIGH);       // Turn on break LED
      }

      time_start = millis();            // Record the start time for the next study session
      time = millis() - time_start;
      count++;                          // Increment the study-break cycle counter
    }
  }
}