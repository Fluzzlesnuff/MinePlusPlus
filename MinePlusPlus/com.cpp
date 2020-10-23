#include "includes.h"

void CommunicationChannel::init(const long bitrate = 1000000) {
  Serial.begin(bitrate);
}
