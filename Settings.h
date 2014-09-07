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
  Settings();
  unsigned long channelToFrequency();
  unsigned long frequencyToChannel();
  bool checkVersion();
  void loadConfig();
  void saveConfig();
  void setDefaults();
};
