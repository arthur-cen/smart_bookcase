// This #include statement was automatically added by the Particle IDE.
#include <LiquidCrystal_I2C_Spark.h>

//Setup led pin
int yellowLed = A2; //yellow
int blueLed = A0; //blue
int greenLed = A1; //green
//Setup tilt sensor Pin
int blueSwitch = D4;
int greenSwitch = D5;
int yellowSwitch = D6;
/**
 * Setup tilt value
 */
int blueState = LOW;
int greenState = LOW;
int yellowState = LOW;

LiquidCrystal_I2C *lcd;

int lastSecond = 0;
int off = 1;
/**
 * For now hard coded the code for demostration purpose
 **/
char yellowQuote[] = "Whether or not it draws on new scientific research, technology is a branch of moral philosophy, not of science.";
char blueQuote[] = "The Power of Thinking Without Thinking.";
char greenQuote[] = "Going off the script to get more go faster and shortcut your way in success.";

/**
 * Variables to hold characters in the LCD screens, the size of char array is 17 
 * because there is always a additional new line character appended at each end of string
 */
char first_line[17];
char second_line[17];

bool receivingQuote = false;
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

bool screenOff = true;

void setup() {
    Serial.begin(9600);
    pinMode(yellowLed, OUTPUT);
    pinMode(greenLed, OUTPUT);
    pinMode(blueLed, OUTPUT);
    
    pinMode(blueSwitch, INPUT);
    pinMode(greenSwitch, INPUT);
    pinMode(yellowSwitch, INPUT);
    lcd = new LiquidCrystal_I2C(0x27, 16, 2);
    lcd->init();
    lcd->backlight();
    lcd->clear();
    /**
     * register a cloud function that can be called when a new quote is coming
     */
    Particle.function("receiveQuote", receiveQuote);
    Serial.begin(9600);
}

void loop() {
    blueState = digitalRead(blueSwitch);
    greenState = digitalRead(greenSwitch);
    yellowState = digitalRead(yellowSwitch);
    
    digitalWrite(blueLed, blueState);
    digitalWrite(greenLed, greenState);
    digitalWrite(yellowLed, yellowState);
    /**
     * if receiving quote, not displaying current quote 
     */
    if (receivingQuote) {
        currentMillis = millis();
        if (currentMillis - previousMillis >= 15000) {
            receivingQuote = false;
            currentMillis = 0;
            previousMillis = 0;
        }
    /**
     * if not receiving quote, check the book states. If the state is high, the book is picked up, vice versa.
     */
    } else if (blueState || greenState || yellowState) {
        if (previousMillis == 0) {
            previousMillis = millis();
            if(blueState) {
                displayText(blueQuote);
            } else if (greenState) {
                displayText(greenQuote);
            } else {
                displayText(yellowQuote);
            }
        }
    } else {
        // Only shut off the screen when screen is on.
        if (!screenOff) {
            clearScreen();
            previousMillis = 0;
        }
    }
}
/**
 * A helper function that takes in a char array and diplay the text on the lcd screen 
 * */
void displayText(const char quote[]) {
    screenOff = false;
    lcd->clear();
    lcd->display();
    lcd->backlight();
    for (int i=0; i < strlen(quote); i+=32) {
        memset(first_line, 0, sizeof(first_line));
        memset(second_line, 0, sizeof(second_line));
        lcd->clear();
        for (int j=0; j < 16; j++) {
            first_line[j] = quote[j + i];
        }
        lcd->setCursor(0, 0);
        lcd->print(first_line);
        for (int k=0; k < 16; k++) {
            second_line[k] = quote[i + k + 16];
        }
        lcd->setCursor(0, 1);
        lcd->print(second_line);
        delay(3000);
    }
    Serial.println(quote);
}

/**
 * A cloud function that receives quote and updates the screen
 */
int receiveQuote(String quoteOverride) {
    previousMillis = millis();
    receivingQuote = true;
    displayText(quoteOverride);
    return 1;
}
/**
 * A helper function that cleans the screen
 */
void clearScreen() {
    screenOff = true;
    lcd->clear();
    lcd->noDisplay();
    lcd->noBacklight();
}





















