#ifndef screen_h
#define screen_h

class Screen { //A set of functions, not a proper object
    const byte* idToBitmap (const id_t id, const byte version = 0, FunctionCallContext context = FunctionCallContext::Generic); //Generates a bitmap from an ID.
    void forceUpdateAnimations (); //Internal function to update animation frame counters. Always updates.
  public:
    void renderBitmap(const byte bitmap[], const uint8_t rows, const uint8_t columns, const byte xPixel, const byte yPixel);
    void renderBlock (const xcoord_t x, const ycoord_t y, const int8_t xPixelOffset, const int8_t yPixelOffset);
    void renderBlock (const CoordPair coords, const int8_t xPixelOffset = 0, const int8_t yPixelOffset = 0);
    void renderWorld (const bool reRenderAnimatedBlocks = false);
    void forceRenderWorld ();
    void renderWorldOverview ();
    void updateAnimations (); //Public function to update animations. Only runs if the last update was more than a certain period  ago.
};

extern Screen screen;

#endif
