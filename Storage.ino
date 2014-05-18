// All EEPROM and SD Card stuff will go here

bool loadConfig()
{
  EEPROM.readBlock(configAddress, storage);
  return strcmp_P(storage.version, CONFIG_VERSION) == 0;
}

void applyConfig() {
  //
  //  Tune to the desired frequency.
  //
  Radio.tune(storage.frequency);  //  6 digits only.
}

void saveConfig()
{
  EEPROM.updateBlock(configAddress, storage);
}

void setDefaults()
{
  strcpy_P(storage.version, CONFIG_VERSION);
  storage.frequency = 162550; //  6 digits only.
}
