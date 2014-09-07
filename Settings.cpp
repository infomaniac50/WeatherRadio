#include "Settings.h"
#include <EEPROMex.h>
#include <SI4707.h>

const char CONFIG_VERSION[VERSION_SIZE] PROGMEM = VERSION;

Settings::Settings() {
  // Setup EEPROM with defaults
  EEPROM.setMemPool(MEMORY_BASE, EEPROMSizeMega);
  channelAddress = EEPROM.getAddress(sizeof(channel));
  volumeAddress = EEPROM.getAddress(sizeof(volume));
  versionAddress = EEPROM.getAddress(VERSION_SIZE);
}

unsigned long Settings::channelToFrequency() {
  return channel * 2.5;
}

unsigned long Settings::frequencyToChannel() {
  return frequency / 2.5;
}

bool Settings::checkVersion() {
  return strcmp_P(version, CONFIG_VERSION) == 0;
}

void Settings::loadConfig()
{
  EEPROM.readBlock(versionAddress, version);
  channel = EEPROM.readInt(channelAddress);
  volume = EEPROM.readInt(volumeAddress);
}

void Settings::saveConfig()
{
  EEPROM.updateBlock(versionAddress, version);
  EEPROM.updateInt(channelAddress, channel);
  EEPROM.updateInt(volumeAddress, volume);
}

void Settings::setDefaults()
{
  strcpy_P(version, CONFIG_VERSION);

  channel = 65020; //162.550 MHz 162550 / 2.5
  // 0x003F 63
  // 0x002D 45 75% of 63
  volume = 0x002D;
}
