#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class MiniGFX {
public:
  MiniGFX(Adafruit_SSD1306* display);

  void clear();
  void update();

  // Widgets
  void drawBattery(int x, int y, int percent);
  void drawWiFi(int x, int y, int strength);
  void drawProgressBar(int x, int y, int w, int h, int percent);
  void drawSparkline(const int values[], int count, int x, int y, int w, int h);
  void drawLoadingSpinner(int cx, int cy, int radius, int frame);

  // Character / emoji
  void drawEmoji3D(int cx, int cy, int r, int mood);
  void drawHappy3D(int cx, int cy, int r);
  void drawSleepy3D(int cx, int cy, int r);
  void drawSurprised3D(int cx, int cy, int r);

  // 3D wireframe
  void drawRotatingCube(int cx, int cy, int size, float angleX, float angleY, float angleZ);

private:
  Adafruit_SSD1306* oled;

  void drawFaceBase(int cx, int cy, int r);
  void drawFaceHighlight(int cx, int cy, int r);
  void drawFaceShading(int cx, int cy, int r);

  void drawSmile(int cx, int cy);
  void drawFlatMouth(int cx, int cy);
  void drawOpenMouth(int cx, int cy);

  void drawEye(int x, int y);
  void drawClosedEye(int x, int y);

  void project3D(
    float x, float y, float z,
    int cx, int cy,
    int scale,
    float& outX,
    float& outY
  );

  void rotatePoint(
    float x, float y, float z,
    float angleX, float angleY, float angleZ,
    float& outX, float& outY, float& outZ
  );
};
