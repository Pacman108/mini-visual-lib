#include "MiniGFX.h"
#include <math.h>

MiniGFX::MiniGFX(Adafruit_SSD1306* display) {
  oled = display;
}

void MiniGFX::clear() {
  oled->clearDisplay();
}

void MiniGFX::update() {
  oled->display();
}

// ----------------------------------------------------
// Battery icon
// ----------------------------------------------------

void MiniGFX::drawBattery(int x, int y, int percent) {
  if (percent < 0) percent = 0;
  if (percent > 100) percent = 100;

  int bodyW = 24;
  int bodyH = 10;
  int terminalW = 3;
  int terminalH = 4;

  oled->drawRect(x, y, bodyW, bodyH, SSD1306_WHITE);
  oled->fillRect(x + bodyW, y + 3, terminalW, terminalH, SSD1306_WHITE);

  int fillW = map(percent, 0, 100, 0, bodyW - 4);

  oled->fillRect(x + 2, y + 2, fillW, bodyH - 4, SSD1306_WHITE);

  if (percent <= 20) {
    oled->drawLine(x + 5, y + 2, x + 10, y + 7, SSD1306_BLACK);
    oled->drawLine(x + 10, y + 2, x + 5, y + 7, SSD1306_BLACK);
  }
}

// ----------------------------------------------------
// Wi-Fi icon
// strength: 0 to 4
// ----------------------------------------------------

void MiniGFX::drawWiFi(int x, int y, int strength) {
  if (strength < 0) strength = 0;
  if (strength > 4) strength = 4;

  // Center dot
  if (strength >= 1) {
    oled->fillCircle(x, y + 14, 2, SSD1306_WHITE);
  } else {
    oled->drawCircle(x, y + 14, 2, SSD1306_WHITE);
  }

  // Small arc approximation
  if (strength >= 2) {
    oled->drawLine(x - 5, y + 10, x - 2, y + 7, SSD1306_WHITE);
    oled->drawLine(x + 2, y + 7, x + 5, y + 10, SSD1306_WHITE);
  }

  // Medium arc approximation
  if (strength >= 3) {
    oled->drawLine(x - 9, y + 7, x - 4, y + 3, SSD1306_WHITE);
    oled->drawLine(x + 4, y + 3, x + 9, y + 7, SSD1306_WHITE);
  }

  // Large arc approximation
  if (strength >= 4) {
    oled->drawLine(x - 13, y + 4, x - 7, y, SSD1306_WHITE);
    oled->drawLine(x + 7, y, x + 13, y + 4, SSD1306_WHITE);
  }
}

// ----------------------------------------------------
// Progress bar
// ----------------------------------------------------

void MiniGFX::drawProgressBar(int x, int y, int w, int h, int percent) {
  if (percent < 0) percent = 0;
  if (percent > 100) percent = 100;

  oled->drawRoundRect(x, y, w, h, 3, SSD1306_WHITE);

  int fillW = map(percent, 0, 100, 0, w - 4);

  if (fillW > 0) {
    oled->fillRoundRect(x + 2, y + 2, fillW, h - 4, 2, SSD1306_WHITE);
  }
}

// ----------------------------------------------------
// Sparkline chart
// ----------------------------------------------------

void MiniGFX::drawSparkline(const int values[], int count, int x, int y, int w, int h) {
  if (count < 2) return;

  int minVal = values[0];
  int maxVal = values[0];

  for (int i = 1; i < count; i++) {
    if (values[i] < minVal) minVal = values[i];
    if (values[i] > maxVal) maxVal = values[i];
  }

  if (maxVal == minVal) {
    maxVal = minVal + 1;
  }

  oled->drawRect(x, y, w, h, SSD1306_WHITE);

  int prevX = x + 1;
  int prevY = y + h - 2 - map(values[0], minVal, maxVal, 0, h - 3);

  for (int i = 1; i < count; i++) {
    int px = x + 1 + ((w - 2) * i) / (count - 1);
    int py = y + h - 2 - map(values[i], minVal, maxVal, 0, h - 3);

    oled->drawLine(prevX, prevY, px, py, SSD1306_WHITE);

    prevX = px;
    prevY = py;
  }
}

// ----------------------------------------------------
// Loading spinner
// ----------------------------------------------------

void MiniGFX::drawLoadingSpinner(int cx, int cy, int radius, int frame) {
  const int points = 8;

  for (int i = 0; i < points; i++) {
    float angle = (2.0 * PI * i) / points;
    int x = cx + cos(angle) * radius;
    int y = cy + sin(angle) * radius;

    int active = frame % points;

    if (i == active) {
      oled->fillCircle(x, y, 3, SSD1306_WHITE);
    } else if ((i + 1) % points == active || (i + 2) % points == active) {
      oled->fillCircle(x, y, 2, SSD1306_WHITE);
    } else {
      oled->drawPixel(x, y, SSD1306_WHITE);
    }
  }
}

// ----------------------------------------------------
// 3D-style emoji base
// ----------------------------------------------------

void MiniGFX::drawFaceBase(int cx, int cy, int r) {
  // Drop shadow / offset contour
  oled->drawCircle(cx + 2, cy + 2, r, SSD1306_WHITE);
  oled->drawCircle(cx + 3, cy + 3, r - 1, SSD1306_WHITE);

  // Main face
  oled->drawCircle(cx, cy, r, SSD1306_WHITE);
  oled->drawCircle(cx, cy, r - 1, SSD1306_WHITE);

  // Inner contour
  oled->drawCircle(cx, cy + 1, r - 4, SSD1306_WHITE);
}

void MiniGFX::drawFaceHighlight(int cx, int cy, int r) {
  // Top-left highlight
  oled->drawLine(cx - r / 2, cy - r / 2, cx - r / 4, cy - r / 2 - 2, SSD1306_WHITE);
  oled->drawPixel(cx - r / 2 - 1, cy - r / 2 + 2, SSD1306_WHITE);
  oled->drawPixel(cx - r / 2, cy - r / 2 + 3, SSD1306_WHITE);
}

void MiniGFX::drawFaceShading(int cx, int cy, int r) {
  // Dithered lower-right shading
  for (int yy = cy; yy < cy + r - 3; yy += 3) {
    for (int xx = cx; xx < cx + r - 3; xx += 4) {
      int dx = xx - cx;
      int dy = yy - cy;

      if (dx * dx + dy * dy < (r - 4) * (r - 4)) {
        oled->drawPixel(xx, yy, SSD1306_WHITE);
      }
    }
  }

  oled->drawLine(cx - r / 2, cy + r / 2 + 2, cx + r / 2, cy + r / 2 + 2, SSD1306_WHITE);
}

void MiniGFX::drawEye(int x, int y) {
  oled->fillCircle(x, y, 3, SSD1306_WHITE);
  oled->drawPixel(x - 1, y - 1, SSD1306_BLACK);
}

void MiniGFX::drawClosedEye(int x, int y) {
  oled->drawLine(x - 5, y, x + 5, y, SSD1306_WHITE);
  oled->drawPixel(x - 6, y - 1, SSD1306_WHITE);
  oled->drawPixel(x + 6, y - 1, SSD1306_WHITE);
}

void MiniGFX::drawSmile(int cx, int cy) {
  oled->drawLine(cx - 10, cy, cx - 6, cy + 4, SSD1306_WHITE);
  oled->drawLine(cx - 6, cy + 4, cx, cy + 6, SSD1306_WHITE);
  oled->drawLine(cx, cy + 6, cx + 6, cy + 4, SSD1306_WHITE);
  oled->drawLine(cx + 6, cy + 4, cx + 10, cy, SSD1306_WHITE);
}

void MiniGFX::drawFlatMouth(int cx, int cy) {
  oled->drawLine(cx - 10, cy, cx + 10, cy, SSD1306_WHITE);
  oled->drawLine(cx - 8, cy + 1, cx + 8, cy + 1, SSD1306_WHITE);
}

void MiniGFX::drawOpenMouth(int cx, int cy) {
  oled->drawCircle(cx, cy + 3, 5, SSD1306_WHITE);
  oled->drawCircle(cx, cy + 3, 4, SSD1306_WHITE);
}

// mood:
// 0 = happy
// 1 = sleepy
// 2 = surprised
void MiniGFX::drawEmoji3D(int cx, int cy, int r, int mood) {
  if (mood == 0) drawHappy3D(cx, cy, r);
  else if (mood == 1) drawSleepy3D(cx, cy, r);
  else if (mood == 2) drawSurprised3D(cx, cy, r);
}

void MiniGFX::drawHappy3D(int cx, int cy, int r) {
  drawFaceBase(cx, cy, r);
  drawFaceHighlight(cx, cy, r);
  drawFaceShading(cx, cy, r);

  drawEye(cx - 9, cy - 6);
  drawEye(cx + 9, cy - 6);
  drawSmile(cx, cy + 8);
}

void MiniGFX::drawSleepy3D(int cx, int cy, int r) {
  drawFaceBase(cx, cy, r);
  drawFaceHighlight(cx, cy, r);
  drawFaceShading(cx, cy, r);

  drawClosedEye(cx - 9, cy - 6);
  drawClosedEye(cx + 9, cy - 6);
  drawFlatMouth(cx, cy + 10);

  oled->setTextSize(1);
  oled->setTextColor(SSD1306_WHITE);
  oled->setCursor(cx + r - 4, cy - r + 2);
  oled->print("Z");
  oled->setCursor(cx + r + 3, cy - r - 5);
  oled->print("z");
}

void MiniGFX::drawSurprised3D(int cx, int cy, int r) {
  drawFaceBase(cx, cy, r);
  drawFaceHighlight(cx, cy, r);
  drawFaceShading(cx, cy, r);

  oled->drawCircle(cx - 9, cy - 6, 4, SSD1306_WHITE);
  oled->drawCircle(cx + 9, cy - 6, 4, SSD1306_WHITE);
  drawOpenMouth(cx, cy + 8);
}

// ----------------------------------------------------
// 3D cube rendering
// ----------------------------------------------------

void MiniGFX::rotatePoint(
  float x, float y, float z,
  float angleX, float angleY, float angleZ,
  float& outX, float& outY, float& outZ
) {
  // Rotate around X axis
  float cosX = cos(angleX);
  float sinX = sin(angleX);

  float y1 = y * cosX - z * sinX;
  float z1 = y * sinX + z * cosX;
  float x1 = x;

  // Rotate around Y axis
  float cosY = cos(angleY);
  float sinY = sin(angleY);

  float x2 = x1 * cosY + z1 * sinY;
  float z2 = -x1 * sinY + z1 * cosY;
  float y2 = y1;

  // Rotate around Z axis
  float cosZ = cos(angleZ);
  float sinZ = sin(angleZ);

  outX = x2 * cosZ - y2 * sinZ;
  outY = x2 * sinZ + y2 * cosZ;
  outZ = z2;
}

void MiniGFX::project3D(
  float x, float y, float z,
  int cx, int cy,
  int scale,
  float& outX,
  float& outY
) {
  float distance = 4.0;
  float perspective = scale / (z + distance);

  outX = cx + x * perspective;
  outY = cy + y * perspective;
}

void MiniGFX::drawRotatingCube(int cx, int cy, int size, float angleX, float angleY, float angleZ) {
  float vertices[8][3] = {
    {-1, -1, -1},
    { 1, -1, -1},
    { 1,  1, -1},
    {-1,  1, -1},
    {-1, -1,  1},
    { 1, -1,  1},
    { 1,  1,  1},
    {-1,  1,  1}
  };

  int edges[12][2] = {
    {0, 1}, {1, 2}, {2, 3}, {3, 0},
    {4, 5}, {5, 6}, {6, 7}, {7, 4},
    {0, 4}, {1, 5}, {2, 6}, {3, 7}
  };

  float projected[8][2];

  for (int i = 0; i < 8; i++) {
    float rx, ry, rz;

    rotatePoint(
      vertices[i][0],
      vertices[i][1],
      vertices[i][2],
      angleX,
      angleY,
      angleZ,
      rx,
      ry,
      rz
    );

    project3D(rx, ry, rz, cx, cy, size, projected[i][0], projected[i][1]);
  }

  for (int i = 0; i < 12; i++) {
    int a = edges[i][0];
    int b = edges[i][1];

    oled->drawLine(
      projected[a][0],
      projected[a][1],
      projected[b][0],
      projected[b][1],
      SSD1306_WHITE
    );
  }
}
