#include "includes.h"

void CommunicationChannel::init(long bitrate) {
  Serial.begin(bitrate);
}
