// All EEPROM and SD Card stuff will go here

inline unsigned long channelToFrequency() {
  return channel * 2.5;
}

inline unsigned long frequencyToChannel() {
  return storage.frequency / 2.5;
}

inline bool loadConfig()
{
  EEPROM.readBlock(configAddress, storage);
  return strcmp_P(storage.version, CONFIG_VERSION) == 0;
}

inline void applyConfig() {
  //
  //  Tune to the desired frequency.
  //
  volume = storage.volume;
  channel = storage.frequency / 2.5;
  Radio.setVolume(volume);
  Radio.tune();  //  6 digits only.
}

inline void saveConfig()
{
  storage.frequency = channel * 2.5;
  storage.volume = volume;
  EEPROM.updateBlock(configAddress, storage);
}

inline void setDefaults()
{
  strcpy_P(storage.version, CONFIG_VERSION);
  storage.frequency = 162550; //  6 digits only.
  // 0x003F 63
  // 0x002D 45 75% of 63
  storage.volume = 0x002D;
}
