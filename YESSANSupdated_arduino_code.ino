#include "Arduino_GigaDisplay_GFX.h"
#include "Arduino_GigaDisplayTouch.h"
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>

GigaDisplay_GFX display;
Arduino_GigaDisplayTouch touchDetector;

// Page definitions
enum Page {
  HOME,
  TEMPERATURE,
  VOLUME1,
  VOLUME2,
  CONCENTRATION,
  SPEED,
  PRINT_CONFIRM,
  PRINT,
  ERROR_PAGE
};

Page currentPage = HOME;

// Parameter storage
float selectedTemp = -1;
float selectedVol1 = -1;
float selectedVol2 = -1;
float selectedConc = -1;
float selectedSpeed = -1;

// Temporary selection (before confirmation)
float tempSelection = -1;

// Temperature options
int tempOptions[] = {32, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80};
int numTempOptions = 11;

// Concentration options
int concOptions[] = {0, 25, 50, 75, 100};
int numConcOptions = 5;

// Volume options (1-10)
int volOptions[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int numVolOptions = 10;

// Speed options (1-5)
int speedOptions[] = {1, 2, 3, 4, 5};
int numSpeedOptions = 5;

// Colors
#define BG_COLOR 0x0010        // Darker blue
#define BUTTON_COLOR 0xFFFF    // White
#define BUTTON_SELECTED_COLOR 0xC618  // Light grey
#define TEXT_COLOR 0xFFFF      // White
#define BUTTON_TEXT_COLOR 0x0010  // Dark blue (for text inside buttons)
#define CONFIRM_COLOR 0xA9CF   // Soft ballet pink
#define CANCEL_COLOR 0xF800    // Red
#define CLEAR_COLOR 0xFD20     // Orange

// Touch handling
bool lastTouchState = false;

void setup() {
  Serial.begin(9600);
  display.begin();
  touchDetector.begin();
  
  // Set to portrait mode: 480 wide x 800 tall
  display.setRotation(0);
  
  display.fillScreen(BG_COLOR);
  drawHomePage();
  
  // Seed random number generator
  randomSeed(analogRead(0));
}

void loop() {
  uint8_t contacts;
  GDTpoint_t points[5];
  
  contacts = touchDetector.getTouchPoints(points);
  
  if (contacts > 0 && !lastTouchState) {
    int x = points[0].x;
    int y = points[0].y;
    
    Serial.print("Touch at X: ");
    Serial.print(x);
    Serial.print(" Y: ");
    Serial.println(y);
    
    handleTouch(x, y);
    lastTouchState = true;
    delay(250);
  } else if (contacts == 0) {
    lastTouchState = false;
  }
  
  delay(10);
}

void handleTouch(int x, int y) {
  switch (currentPage) {
    case HOME:
      handleHomeTouch(x, y);
      break;
    case TEMPERATURE:
      handleParameterTouch(x, y, tempOptions, numTempOptions, &selectedTemp);
      break;
    case VOLUME1:
      handleParameterTouch(x, y, volOptions, numVolOptions, &selectedVol1);
      break;
    case VOLUME2:
      handleParameterTouch(x, y, volOptions, numVolOptions, &selectedVol2);
      break;
    case CONCENTRATION:
      handleParameterTouch(x, y, concOptions, numConcOptions, &selectedConc);
      break;
    case SPEED:
      handleParameterTouch(x, y, speedOptions, numSpeedOptions, &selectedSpeed);
      break;
    case PRINT_CONFIRM:
      handlePrintConfirmTouch(x, y);
      break;
    case PRINT:
      handlePrintTouch(x, y);
      break;
    case ERROR_PAGE:
      handleErrorTouch(x, y);
      break;
  }
}

void drawHomePage() {
  display.fillScreen(BG_COLOR);
  display.setTextColor(TEXT_COLOR);
  display.setFont(&FreeSansBold12pt7b);
  display.setCursor(35, 50);
  display.print("PARAMETER CONTROL");
  
  // Draw parameter buttons vertically stacked - portrait 480x800
  drawHomeButton(40, 120, "Temperature", selectedTemp, "C");
  drawHomeButton(40, 230, "Volume 1", selectedVol1, "mL");
  drawHomeButton(40, 340, "Volume 2", selectedVol2, "mL");
  drawHomeButton(40, 450, "Concentration", selectedConc, "%");
  drawHomeButton(40, 560, "Speed", selectedSpeed, "mL/min");
  
  // Draw PRINT button at bottom
  display.fillRoundRect(140, 690, 200, 80, 8, CONFIRM_COLOR);
  display.setFont(&FreeSansBold18pt7b);
  display.setTextColor(TEXT_COLOR);
  display.setCursor(185, 740);
  display.print("PRINT");
}

void drawHomeButton(int x, int y, const char* label, float value, const char* unit) {
  // Draw button
  display.fillRoundRect(x, y, 400, 90, 8, BUTTON_COLOR);
  
  // Label with bold font
  display.setFont(&FreeSansBold12pt7b);
  display.setTextColor(BUTTON_TEXT_COLOR);
  display.setCursor(x + 15, y + 30);
  display.print(label);
  
  // Status with regular font
  display.setFont(&FreeSans9pt7b);
  display.setCursor(x + 15, y + 65);
  if (value < 0) {
    display.print("Not Selected");
  } else {
    display.print("Selected: ");
    
    // Special formatting for concentration
    if (unit[0] == '%') {
      float inverse = 100 - value;
      display.print(value, 1);
      display.print(":");
      display.print(inverse, 1);
      display.print("%");
    } else {
      display.print(value, 1);
      display.print(" ");
      if (unit[0] == 'C') {
        display.print((char)247); // Degree symbol
      }
      display.print(unit);
    }
  }
}

void handleHomeTouch(int x, int y) {
  if (x >= 40 && x <= 440) {
    if (y >= 120 && y <= 210) {
      currentPage = TEMPERATURE;
      tempSelection = selectedTemp;
      drawParameterPage("Temperature", tempOptions, numTempOptions, "C");
    } else if (y >= 230 && y <= 320) {
      currentPage = VOLUME1;
      tempSelection = selectedVol1;
      drawParameterPage("Volume 1", volOptions, numVolOptions, "mL");
    } else if (y >= 340 && y <= 430) {
      currentPage = VOLUME2;
      tempSelection = selectedVol2;
      drawParameterPage("Volume 2", volOptions, numVolOptions, "mL");
    } else if (y >= 450 && y <= 540) {
      currentPage = CONCENTRATION;
      tempSelection = selectedConc;
      drawParameterPage("Concentration", concOptions, numConcOptions, "%");
    } else if (y >= 560 && y <= 650) {
      currentPage = SPEED;
      tempSelection = selectedSpeed;
      drawParameterPage("Speed", speedOptions, numSpeedOptions, "mL/min");
    }
  }
  
  // Check PRINT button
  if (x >= 140 && x <= 340 && y >= 690 && y <= 770) {
    // Check if all parameters are selected
    if (selectedTemp < 0 || selectedVol1 < 0 || selectedVol2 < 0 || 
        selectedConc < 0 || selectedSpeed < 0) {
      Serial.println("Error: Not all parameters selected");
      currentPage = ERROR_PAGE;
      drawErrorPage();
    } else {
      Serial.println("All parameters selected, proceeding to confirm");
      
      // 50% chance to show meow easter egg
      if (random(100) < 50) {
        Serial.println("MEOW!");
        showMeowEasterEgg();
        delay(500); // Show for half a second
      }
      
      currentPage = PRINT_CONFIRM;
      drawPrintConfirmPage();
    }
  }
}

void showMeowEasterEgg() {
  display.fillScreen(BG_COLOR);
  display.setTextColor(TEXT_COLOR);
  display.setFont(&FreeSansBold24pt7b);
  display.setCursor(160, 400);
  display.print("MEOW");
}

void drawParameterPage(const char* title, int* options, int numOptions, const char* unit) {
  display.fillScreen(BG_COLOR);
  display.setTextColor(TEXT_COLOR);
  display.setFont(&FreeSansBold12pt7b);
  display.setCursor(50, 40);
  display.print("Select ");
  display.print(title);
  display.print(":");
  
  // Draw white display box showing current selection with +/- buttons
  int boxX = 40;
  int boxY = 80;
  int boxWidth = 240;
  int boxHeight = 70;
  int buttonSize = 70;
  int buttonSpacing = 10;
  
  // Draw - button (left)
  display.fillRoundRect(boxX, boxY, buttonSize, boxHeight, 8, CONFIRM_COLOR);
  display.setFont(&FreeSansBold24pt7b);
  display.setTextColor(TEXT_COLOR);
  display.setCursor(boxX + 20, boxY + 50);
  display.print("-");
  
  // Draw display box (center)
  display.fillRoundRect(boxX + buttonSize + buttonSpacing, boxY, boxWidth, boxHeight, 8, BUTTON_COLOR);
  display.setFont(&FreeSansBold12pt7b);
  display.setTextColor(BUTTON_TEXT_COLOR);
  
  // Display current selection in the box
  if (tempSelection < 0) {
    display.setCursor(boxX + buttonSize + buttonSpacing + 40, boxY + 45);
    display.print("Not Set");
  } else {
    String valueStr = "";
    
    // Special formatting for concentration
    if (unit[0] == '%') {
      float inverse = 100 - tempSelection;
      valueStr = String(tempSelection, 1) + ":" + String(inverse, 1) + "%";
    } else {
      valueStr = String(tempSelection, 1) + " ";
      if (unit[0] == 'C') {
        valueStr += (char)247; // Degree symbol
      }
      valueStr += unit;
    }
    
    // Center the text in the box
    int charCount = valueStr.length();
    int textWidth = charCount * 14;
    int textX = boxX + buttonSize + buttonSpacing + (boxWidth / 2) - (textWidth / 2);
    display.setCursor(textX, boxY + 45);
    display.print(valueStr);
  }
  
  // Draw + button (right)
  display.fillRoundRect(boxX + buttonSize + buttonSpacing + boxWidth + buttonSpacing, boxY, buttonSize, boxHeight, 8, CONFIRM_COLOR);
  display.setFont(&FreeSansBold24pt7b);
  display.setTextColor(TEXT_COLOR);
  display.setCursor(boxX + buttonSize + buttonSpacing + boxWidth + buttonSpacing + 17, boxY + 50);
  display.print("+");
  
  // Draw option buttons - 3 columns for portrait
  int cols = 3;
  int buttonWidth = 120;
  int buttonHeight = 80;
  int spacingX = 20;
  int spacingY = 20;
  int startX = 40;
  int startY = 180;
  
  for (int i = 0; i < numOptions; i++) {
    int row = i / cols;
    int col = i % cols;
    int bx = startX + col * (buttonWidth + spacingX);
    int by = startY + row * (buttonHeight + spacingY);
    
    bool isSelected = (abs(options[i] - tempSelection) < 0.01);
    uint16_t bgColor = isSelected ? BUTTON_SELECTED_COLOR : BUTTON_COLOR;
    
    display.fillRoundRect(bx, by, buttonWidth, buttonHeight, 8, bgColor);
    display.setFont(&FreeSansBold18pt7b);
    display.setTextColor(BUTTON_TEXT_COLOR);
    
    // Center text in button
    String valueStr = String(options[i]);
    int charCount = valueStr.length();
    int textWidth = charCount * 20;
    int textX = bx + (buttonWidth - textWidth) / 2;
    int textY = by + 50;
    
    display.setCursor(textX, textY);
    display.print(options[i]);
  }
  
  // Draw clear button above confirm button
  display.fillRoundRect(170, 630, 140, 50, 8, CLEAR_COLOR);
  display.setFont(&FreeSansBold12pt7b);
  display.setTextColor(TEXT_COLOR);
  display.setCursor(195, 663);
  display.print("CLEAR");
  
  // Draw confirm button at bottom center
  display.fillRoundRect(140, 700, 200, 70, 8, CONFIRM_COLOR);
  display.setFont(&FreeSansBold12pt7b);
  display.setTextColor(TEXT_COLOR);
  display.setCursor(170, 745);
  display.print("CONFIRM");
}

void drawPrintConfirmPage() {
  display.fillScreen(BG_COLOR);
  display.setTextColor(TEXT_COLOR);
  display.setFont(&FreeSansBold18pt7b);
  display.setCursor(80, 50);
  display.print("Begin Loading?");
  
  // Draw parameter summary
  int yPos = 120;
  int lineHeight = 80;
  
  // Temperature
  drawParameterSummary(40, yPos, "Temperature:", selectedTemp, "C");
  yPos += lineHeight;
  
  // Volume 1
  drawParameterSummary(40, yPos, "Volume 1:", selectedVol1, "mL");
  yPos += lineHeight;
  
  // Volume 2
  drawParameterSummary(40, yPos, "Volume 2:", selectedVol2, "mL");
  yPos += lineHeight;
  
  // Concentration
  drawParameterSummary(40, yPos, "Concentration:", selectedConc, "%");
  yPos += lineHeight;
  
  // Speed
  drawParameterSummary(40, yPos, "Speed:", selectedSpeed, "mL/min");
  
  // Draw CONFIRM button
  display.fillRoundRect(60, 650, 160, 80, 8, CONFIRM_COLOR);
  display.setFont(&FreeSansBold12pt7b);
  display.setTextColor(TEXT_COLOR);
  display.setCursor(85, 700);
  display.print("CONFIRM");
  
  // Draw CANCEL button
  display.fillRoundRect(260, 650, 160, 80, 8, CANCEL_COLOR);
  display.setFont(&FreeSansBold12pt7b);
  display.setTextColor(TEXT_COLOR);
  display.setCursor(295, 700);
  display.print("CANCEL");
}

void drawParameterSummary(int x, int y, const char* label, float value, const char* unit) {
  display.setFont(&FreeSansBold12pt7b);
  display.setTextColor(TEXT_COLOR);
  display.setCursor(x, y);
  display.print(label);
  
  display.setFont(&FreeSans9pt7b);
  display.setCursor(x, y + 30);
  if (value < 0) {
    display.print("Not Selected");
  } else {
    // Special formatting for concentration
    if (unit[0] == '%') {
      float inverse = 100 - value;
      display.print(value, 1);
      display.print(":");
      display.print(inverse, 1);
      display.print("%");
    } else {
      display.print(value, 1);
      display.print(" ");
      if (unit[0] == 'C') {
        display.print((char)247); // Degree symbol
      }
      display.print(unit);
    }
  }
}

void drawErrorPage() {
  display.fillScreen(BG_COLOR);
  
  // Draw error box
  display.fillRoundRect(40, 250, 400, 150, 8, CANCEL_COLOR);
  display.setTextColor(TEXT_COLOR);
  display.setFont(&FreeSansBold12pt7b);
  
  // Center the error message
  display.setCursor(60, 300);
  display.print("Error, please specify");
  display.setCursor(95, 335);
  display.print("all parameters");
  
  // Draw OK button
  display.fillRoundRect(165, 450, 150, 70, 8, BUTTON_COLOR);
  display.setFont(&FreeSansBold18pt7b);
  display.setTextColor(BUTTON_TEXT_COLOR);
  display.setCursor(210, 495);
  display.print("OK");
}

void handleErrorTouch(int x, int y) {
  // Check OK button
  if (x >= 165 && x <= 315 && y >= 450 && y <= 520) {
    Serial.println("OK button pressed on error page");
    currentPage = HOME;
    drawHomePage();
  }
}

void handlePrintConfirmTouch(int x, int y) {
  // Check CONFIRM button
  if (x >= 60 && x <= 220 && y >= 650 && y <= 730) {
    Serial.println("Confirm button pressed - starting print");
    currentPage = PRINT;
    drawPrintPage();
  }
  
  // Check CANCEL button
  if (x >= 260 && x <= 420 && y >= 650 && y <= 730) {
    Serial.println("Cancel button pressed");
    currentPage = HOME;
    drawHomePage();
  }
}

void drawPrintPage() {
  display.fillScreen(0xFFFF); // White background: 0xFFFF
  
  // Rainbow colored "Loading..." text
  const char* text = "Loading...";
  int textLen = strlen(text);
  uint16_t colors[] = {0xA9CF, 0xA9CF, 0xA9CF, 0xA9CF, 0xA9CF, 0xA9CF};
  
  display.setFont(&FreeSansBold24pt7b);
  int startX = 85;
  int startY = 370;
  
  for (int i = 0; i < textLen; i++) {
    display.setTextColor(colors[i % 6]);
    display.setCursor(startX + (i * 28), startY);
    display.print(text[i]);
  }
  
  // Draw HOME button at bottom
  display.fillRoundRect(140, 680, 200, 80, 8, CONFIRM_COLOR);
  display.setFont(&FreeSansBold18pt7b);
  display.setTextColor(TEXT_COLOR);
  display.setCursor(195, 730);
  display.print("HOME");
}

void handlePrintTouch(int x, int y) {
  // Check HOME button
  if (x >= 140 && x <= 340 && y >= 680 && y <= 760) {
    currentPage = HOME;
    drawHomePage();
  }
}

void handleParameterTouch(int x, int y, int* options, int numOptions, float* selectedParam) {
  int boxX = 40;
  int boxY = 80;
  int boxHeight = 70;
  int buttonSize = 70;
  int buttonSpacing = 10;
  
  // Determine increment amount based on current page
  float increment = 0.1;
  if (currentPage == CONCENTRATION) {
    increment = 0.5; // Use 0.5 for concentration
  }
  
  // Check - button
  if (x >= boxX && x <= (boxX + buttonSize) && y >= boxY && y <= (boxY + boxHeight)) {
    Serial.println("Minus button pressed");
    // If nothing selected, start from first option, otherwise decrease
    if (tempSelection < 0) {
      tempSelection = options[0];
    } else {
      tempSelection -= increment;
      if (tempSelection < 0) tempSelection = 0;
    }
    Serial.print("New value: ");
    Serial.println(tempSelection);
    
    // Redraw page
    const char* title = "";
    const char* unit = "";
    getCurrentPageInfo(&title, &unit);
    drawParameterPage(title, options, numOptions, unit);
    return;
  }
  
  // Check + button
  int plusX = boxX + buttonSize + buttonSpacing + 240 + buttonSpacing;
  if (x >= plusX && x <= (plusX + buttonSize) && y >= boxY && y <= (boxY + boxHeight)) {
    Serial.println("Plus button pressed");
    // If nothing selected, start from first option, otherwise increase
    if (tempSelection < 0) {
      tempSelection = options[0];
    } else {
      tempSelection += increment;
    }
    Serial.print("New value: ");
    Serial.println(tempSelection);
    
    // Redraw page
    const char* title = "";
    const char* unit = "";
    getCurrentPageInfo(&title, &unit);
    drawParameterPage(title, options, numOptions, unit);
    return;
  }
  
  // Check clear button
  if (x >= 170 && x <= 310 && y >= 630 && y <= 680) {
    Serial.println("Clear button pressed");
    tempSelection = -1;
    
    // Redraw page
    const char* title = "";
    const char* unit = "";
    getCurrentPageInfo(&title, &unit);
    drawParameterPage(title, options, numOptions, unit);
    return;
  }
  
  // Check confirm button
  if (x >= 140 && x <= 340 && y >= 700 && y <= 770) {
    Serial.println("Confirm button pressed");
    if (tempSelection >= 0) {
      *selectedParam = tempSelection;
      Serial.print("Parameter saved: ");
      Serial.println(*selectedParam);
    }
    currentPage = HOME;
    drawHomePage();
    return;
  }
  
  // Check option buttons
  int cols = 3;
  int buttonWidth = 120;
  int buttonHeight = 80;
  int spacingX = 20;
  int spacingY = 20;
  int startX = 40;
  int startY = 180;
  
  for (int i = 0; i < numOptions; i++) {
    int row = i / cols;
    int col = i % cols;
    int bx = startX + col * (buttonWidth + spacingX);
    int by = startY + row * (buttonHeight + spacingY);
    
    if (x >= bx && x <= (bx + buttonWidth) && y >= by && y <= (by + buttonHeight)) {
      tempSelection = options[i];
      Serial.print("Option button pressed: ");
      Serial.println(tempSelection);
      
      // Redraw to show selection
      const char* title = "";
      const char* unit = "";
      getCurrentPageInfo(&title, &unit);
      drawParameterPage(title, options, numOptions, unit);
      break;
    }
  }
}

void getCurrentPageInfo(const char** title, const char** unit) {
  if (currentPage == TEMPERATURE) {
    *title = "Temperature";
    *unit = "C";
  } else if (currentPage == VOLUME1) {
    *title = "Volume 1";
    *unit = "mL";
  } else if (currentPage == VOLUME2) {
    *title = "Volume 2";
    *unit = "mL";
  } else if (currentPage == CONCENTRATION) {
    *title = "Concentration";
    *unit = "%";
  } else if (currentPage == SPEED) {
    *title = "Speed";
    *unit = "mL/min";
  }
}
