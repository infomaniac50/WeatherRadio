// All EEPROM and SD Card stuff will go here
#define MEMORY_BASE 32
// 20 + 1 characters
#define VERSION __DATE__ " " __TIME__
// EEPROM Globals
// #define CONFIG_VERSION "wr1"
#define VERSION_SIZE 21

class Settings
{
private:
  int channelAddress;
  int volumeAddress;
  int versionAddress;
  char version[VERSION_SIZE];
public:
  static const unsigned int DEFAULT_CHANNEL = (unsigned int)(162550.0 / 2.5); //162.550 MHz 162550 / 2.5
  // 0x003F 63
  // 0x002D 45 75% of 63
  static const unsigned char DEFAULT_VOLUME = (unsigned char)(63.0 * 0.75);
  Settings();
  unsigned long channelToFrequency();
  unsigned long frequencyToChannel();
  bool checkVersion();
  void loadConfig();
  void saveConfig();
  void setDefaults();
};
