const int DIR = 6;
const int STEP = 7;
const int steps_per_rev = 1000;

void setup()
{
    Serial.begin(9600);
    pinMode(STEP, OUTPUT);
    pinMode(DIR, OUTPUT);
}

void loop()
{
    // Spin clockwise
    digitalWrite(DIR, HIGH);
    Serial.println("Spinning Clockwise...");
    for (int i = 0; i < steps_per_rev; i++)
    {
        digitalWrite(STEP, HIGH);
        delayMicroseconds(2000);
        digitalWrite(STEP, LOW);
        delayMicroseconds(2000);
    }
    delay(1000); // Wait for 1 second

    // Spin anti-clockwise
    digitalWrite(DIR, LOW);
    Serial.println("Spinning Anti-Clockwise...");
    for (int i = 0; i < steps_per_rev; i++)
    {
        digitalWrite(STEP, HIGH);
        delayMicroseconds(2000); // Consistent delay for smoother operation
        digitalWrite(STEP, LOW);
        delayMicroseconds(2000); // Consistent delay for smoother operation
    }
    delay(1000); // Wait for 1 second
}