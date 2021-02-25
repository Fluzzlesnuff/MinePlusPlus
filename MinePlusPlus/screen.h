#ifndef screen_h
#define screen_h

class Screen { //A set of functions, not a proper object
    const byte* idToBitmap (id_t id, byte version = 0, FunctionCallContext context = FunctionCallContext::Generic); //Generates a bitmap from an ID.
    void forceUpdateAnimations (); //Internal function to update animation frame counters. Always updates.
  public:
    void renderBitmap(const byte bitmap[], uint8_t rows, uint8_t columns, byte xPixel, byte yPixel);
    void renderBlock (xcoord_t x, ycoord_t y, int8_t xPixelOffset, int8_t yPixelOffset);
    void renderBlock (const CoordPair& coords, int8_t xPixelOffset = 0, int8_t yPixelOffset = 0);
    void renderWorld (bool reRenderAnimatedBlocks = false);
    void forceRenderWorld ();
    void renderWorldOverview ();
    void updateAnimations (); //Public function to update animations. Only runs if the last update was more than a certain period  ago.
};

extern Screen screen;

#endif