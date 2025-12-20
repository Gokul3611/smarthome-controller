/**
 * Simple LED Control Example
 * ==========================
 * This is a minimal example for testing the mini controller
 * Use this if you want to quickly test LED control without all the features
 */

#define LED_PIN 2  // Change if your board uses a different pin

// PWM Configuration
#define PWM_CHANNEL 0
#define PWM_FREQUENCY 5000
#define PWM_RESOLUTION 8

void setup() {
  Serial.begin(115200);
  Serial.println("\n=== Simple LED Test ===");
  
  // Initialize LED with PWM
  ledcSetup(PWM_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttachPin(LED_PIN, PWM_CHANNEL);
  
  Serial.printf("LED Pin: %d\n", LED_PIN);
  Serial.println("Test starting...\n");
}

void loop() {
  // Test 1: Full brightness
  Serial.println("Test 1: Full brightness (100%)");
  ledcWrite(PWM_CHANNEL, 255);
  delay(2000);
  
  // Test 2: Half brightness
  Serial.println("Test 2: Half brightness (50%)");
  ledcWrite(PWM_CHANNEL, 127);
  delay(2000);
  
  // Test 3: Low brightness
  Serial.println("Test 3: Low brightness (10%)");
  ledcWrite(PWM_CHANNEL, 25);
  delay(2000);
  
  // Test 4: Off
  Serial.println("Test 4: Off");
  ledcWrite(PWM_CHANNEL, 0);
  delay(2000);
  
  // Test 5: Fade in
  Serial.println("Test 5: Fade in");
  for (int i = 0; i <= 255; i += 5) {
    ledcWrite(PWM_CHANNEL, i);
    delay(50);
  }
  delay(1000);
  
  // Test 6: Fade out
  Serial.println("Test 6: Fade out");
  for (int i = 255; i >= 0; i -= 5) {
    ledcWrite(PWM_CHANNEL, i);
    delay(50);
  }
  delay(1000);
  
  // Test 7: Blink
  Serial.println("Test 7: Blink (5 times)");
  for (int i = 0; i < 5; i++) {
    ledcWrite(PWM_CHANNEL, 255);
    delay(300);
    ledcWrite(PWM_CHANNEL, 0);
    delay(300);
  }
  delay(1000);
  
  Serial.println("\n=== All tests complete. Repeating... ===\n");
  delay(2000);
}
