#include "includes.h"

void CommunicationChannel::init(long bitrate = 1000000) {
  Serial.begin(bitrate);
}
