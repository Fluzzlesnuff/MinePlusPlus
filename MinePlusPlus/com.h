#ifndef com_h
#define com_h

class IOStream {
  public:
    void init (long bitrate = 1000000);
};
class IStream : public IOStream {
    friend IStream& operator>> (IStream& in, String& var);
};
class OStream : public IOStream {
    uint8_t format = DEC;
    friend OStream& operator<< (OStream& out, int input);
    friend OStream& operator<< (OStream& out, const String& input);
    friend OStream& operator<< (OStream& out, const __FlashStringHelper *input);
    friend OStream& operator<< (OStream& out, const CoordPair& coordPair);
    friend OStream& operator<< (OStream& out, const ExactCoordPair& coordPair);
    friend OStream& operator<< (OStream& out, Error error);
    friend OStream& operator<< (OStream& out, IOStreamFlag flag);
  public:
};

extern IOStream iostream;
extern OStream cout;
extern IStream cin;

#endif
