#ifndef screen_h
#define screen_h

extern id_t oldRenderMap[13][7];

class Screen { //A set of functions, not a proper object
    byte* Screen::idToBitmap (id_t id); //Generates a bitmap from an ID.
  public:
    void renderBitmap(const byte bitmap[], uint8_t rows, uint8_t columns, byte xPixel, byte yPixel);
    void renderBlock (xcoord_t x, ycoord_t y, int8_t xPixelOffset, int8_t yPixelOffset);
    void renderBlock (CoordPair coords, int8_t xPixelOffset = 0, int8_t yPixelOffset = 0);
    void renderWorld ();
};

extern Screen screen;

#endif
