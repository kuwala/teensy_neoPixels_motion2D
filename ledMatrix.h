#ifndef LEDMATRIX_H
#define LEDMATRIX_H
  //includes here
  #include <Adafruit_NeoPixel.h>

// 4 x 4 grid
#define NUMLEDS 16
#define STRIP_BRIGHTNESS 64
#define WIDTH 4
#define HEIGHT

class LedMatrix {
  public:
    int w;
    int h;
    int x;
    int y;
    int r;
    int g;
    int b;
    int index;
    int numLeds;
    int cellBrightness[NUMLEDS];

    // cells stores the strip led index;
    int cells[4][4];
    Adafruit_NeoPixel strip;
    int xyToIndex(int x, int y) {
      Serial.print("y * w + x: " );
      Serial.print(y * w + x);
      int index = y * w + x;
      if(y > 0) {
        index -=1;
      }
      return y * w + x;
      Serial.print("y * w + x: " );
    }
    int indexToY(int i) {
      int y = floor(i / h);
      return y;
    }
    int indexToX(int i ) {
      int x = i % h;
      return x;
    }

  LedMatrix(int _w, int _h, int _pin) {
    w = _w;
    h = _h;
    x = 0;
    y = 0;
    r = 0;
    g = 0;
    b = 0;
    index = 0;
    numLeds = NUMLEDS;
//    initCells();
    for(int i = 0; i < sizeof(cellBrightness)/sizeof(int); i ++ ) {
      cellBrightness[i] = 0;
    }
    strip = Adafruit_NeoPixel(NUMLEDS, _pin, NEO_GRB + NEO_KHZ800);
    strip.begin();
    strip.setBrightness(STRIP_BRIGHTNESS);
    allOff();
  }
//
  void initCells() {
    int count = 0;
    int width = 4;
    for(int row = 0; row < 4; row++ ) {
      for(int col = 0; col < 4; col++) {
        //zigzag the connections
        // { {0,  1,  2,  3},
        //   {7,  6,  5,  4}, // on rows count backwords
        //   {8,  9,  10, 11},
        //   {15, 14, 13, 12} }; // backwards
        if(row % 2) {
          // odd rows : 1, 3 ...
          cells[row][col] = count + width - col * 2;
        } else {
          //even rows : 0 , 2 ...
          cells[row][col] = count;
        }
        count++;
        Serial.print("Row: ");
        Serial.println(row);
        Serial.print("Col: ");
        Serial.println(col);
        Serial.print("Count: ");
        Serial.println(count);
        Serial.print("Cell: ");
        Serial.println(cells[col][row]);
      }
    }
  }
  
//  int mapToIndex(int num) {
//    int index = num % numLeds;
//    if(index < 0 ) {
//      index = numLeds - abs(index);
//    }
//    return index;
//  }

  void draw() {
    drawGrid();
    strip.show();
  }

  void update() {
    // change index
    //allOff();
    
    //setDotAtXY(random(4),random(4));
    //drawDotAtXY();
    diminishBrightness();
    
  }
  void diminishBrightness() {
    for(int i = 0; i < sizeof(cellBrightness)/sizeof(int); i ++ ) {
      if (cellBrightness[i] > 0 ) {
        cellBrightness[i] = cellBrightness[i] - 50;
      } else {
        cellBrightness[i] = 0;
      }
    }
  }
  void setXY (int _x, int _y) {
    x = _x;
    y = _y;
    int i = xyToIndex(x,y);
    cellBrightness[i] = 255;
  }
  void drawDotAtXY () {
    int index = xyToIndex(x, y);
    strip.setPixelColor(index, strip.Color(cellBrightness[index],0,0));
  }
  void drawGrid() {
    for(int i = 0; i < numLeds; i ++ ) {
      strip.setPixelColor(i, strip.Color(cellBrightness[i],0,0));
    }
  }
  void allOff() {
    for(int i = 0; i < numLeds; i ++ ) {
      strip.setPixelColor(i, strip.Color(0,0,0));
    }
  }
    
};
#endif




