// All EEPROM and SD Card stuff will go here

bool loadConfig()
{
  EEPROM.readBlock(configAddress, storage);
  return (storage.version == CONFIG_VERSION);
}

void saveConfig()
{
  EEPROM.writeBlock(configAddress, storage);
}

void setDefaults()
{
  strcpy_P(storage.version, CONFIG_VERSION);
  // storage.version = CONFIG_VERSION;
  storage.frequency = 162550; //  6 digits only.
}
