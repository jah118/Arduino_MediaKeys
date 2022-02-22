#include <EEPROM.h>

 typedef struct configData_t
 {
   uint8_t signature[2];
   uint8_t version;
   // application config data starts below
   uint16_t delaySelect;
   uint16_t lastSlotID;
 };
 
 class programAppConfig
 {
 public:
   inline uint16_t getSelectDelay() { return(_D.delaySelect); };
   inline void setSelectDelay(uint16_t n) { _D.delaySelect = n; };
   inline uint16_t getLastSlot() { return(_D.lastSlotID); };
   inline void setLastSlot(uint16_t n) { _D.lastSlotID = n; };
 
   void begin()
   { 
     if (!configLoad())
     {
       configDefaultValues();
       configSave();
     }
   };
 
   void configDefaultValues(void)
   {
     _D.signature[0] = EEPROM_SIG[0];
     _D.signature[1] = EEPROM_SIG[1];
     _D.version = CONFIG_VERSION;
     _D.delaySelect = SELECT_DELAY_DEFAULT; 
     _D.lastSlotID = LAST_SLOT_DEFAULT; 
   }
  
   bool configLoad(void)
   {
     EEPROM.get(EEPROM_ADDR, _D);
     if (_D.signature[0] != EEPROM_SIG[0] && 
         _D.signature[1] != EEPROM_SIG[1])   
       return(false);
    // handle any version adjustments here
    if (_D.version != CONFIG_VERSION)
    {
      // do something here
    }
 
    // update version number to current
    _D.version = CONFIG_VERSION;
 
    return(true);
   }
  
   bool configSave(void)
   {
     EEPROM.put(EEPROM_ADDR, _D);
     return(true);
   }
 
 private:
   const uint16_t SELECT_DELAY_DEFAULT = 1000; // milliseconds
   const uint16_t LAST_SLOT_DEFAULT = 99;      // number
   const uint16_t EEPROM_ADDR = 0;
   const uint8_t EEPROM_SIG[2] = { 0xee, 0x11 };
   const uint8_t CONFIG_VERSION = 0;
   configData_t _D;
 };