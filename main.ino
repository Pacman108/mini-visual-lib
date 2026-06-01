#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "MiniGFX.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_SDA 8
#define OLED_SCL 9
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
MiniGFX gfx(&display);

int chartValues[] = {12, 18, 15, 21, 25, 19, 28, 31, 26, 34, 30, 38};
const int chartCount = sizeof(chartValues) / sizeof(chartValues[0]);

unsigned long lastFrame = 0;
int frame = 0;

int screenMode = 0;
unsigned long lastScreenSwitch = 0;
const unsigned long screenDuration = 3500;

void drawWidgetDashboard() {
  gfx.clear();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("MiniGFX Dashboard");

  gfx.drawBattery(0, 14, 76);
  gfx.drawWiFi(45, 10, 4);

  display.setCursor(75, 15);
  display.print("ESP32");

  gfx.drawProgressBar(0, 32, 120, 10, frame % 101);

  display.setCursor(0, 48);
  display.print("Loading UI ");
  display.print(frame % 101);
  display.print("%");

  gfx.update();
}

void drawChartScreen() {
  gfx.clear();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Sparkline Chart");

  gfx.drawSparkline(chartValues, chartCount, 4, 16, 120, 40);

  display.setCursor(0, 58);
  display.print("Tiny data visualization");

  gfx.update();
}

void drawSpinnerScreen() {
  gfx.clear();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 8);
  display.print("Fetching data...");

  gfx.drawLoadingSpinner(64, 36, 14, frame);

  gfx.update();
}

void drawEmojiScreen() {
  gfx.clear();

  int mood = (frame / 20) % 3;

  gfx.drawEmoji3D(64, 34, 24, mood);

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  if (mood == 0) {
    display.print("3D-style Emoji: Happy");
  } else if (mood == 1) {
    display.print("3D-style Emoji: Sleepy");
  } else {
    display.print("3D-style Emoji: Shocked");
  }

  gfx.update();
}

void drawCubeScreen() {
  gfx.clear();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Wireframe 3D Cube");

  float angle = frame * 0.07;

  gfx.drawRotatingCube(
    64,
    36,
    45,
    angle,
    angle * 0.7,
    angle * 0.4
  );

  gfx.update();
}

void drawCurrentScreen() {
  switch (screenMode) {
    case 0:
      drawWidgetDashboard();
      break;

    case 1:
      drawChartScreen();
      break;

    case 2:
      drawSpinnerScreen();
      break;

    case 3:
      drawEmojiScreen();
      break;

    case 4:
      drawCubeScreen();
      break;
  }
}

void setup() {
  Serial.begin(115200);

  Wire.begin(OLED_SDA, OLED_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 init failed");
    while (true) {
      delay(100);
    }
  }

  display.clearDisplay();
  display.display();

  lastFrame = millis();
  lastScreenSwitch = millis();
}

void loop() {
  if (millis() - lastFrame > 80) {
    lastFrame = millis();

    frame++;

    drawCurrentScreen();
  }

  if (millis() - lastScreenSwitch > screenDuration) {
    lastScreenSwitch = millis();

    screenMode++;

    if (screenMode > 4) {
      screenMode = 0;
    }
  }
}
