#ifndef WEARLEVELING_H
#define WEARLEVELING_H

  void loadEeprom(uint8_t ASegment);
  uint8_t getValue(uint8_t ASegment);
  void writeValue(uint8_t ASegment, uint8_t value);

#endif	/* WEARLEVELING_H */

