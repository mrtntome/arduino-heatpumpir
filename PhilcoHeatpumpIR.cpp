#include <PhilcoHeatpumpIR.h>

PhilcoHeatpumpIR::PhilcoHeatpumpIR() : HeatpumpIR()
{
  static const char model[] PROGMEM = "philco";
  static const char info[]  PROGMEM = "{\"mdl\":\"philco\",\"dn\":\"Philco\"}";

  _model = model;
  _info = info;
}


void PhilcoHeatpumpIR::send(IRSender& IR, uint8_t powerModeCmd, uint8_t operatingModeCmd , uint8_t fanSpeedCmd , uint8_t temperatureCmd , uint8_t swingVCmd , uint8_t swingHCmd )
{
  (void)swingVCmd;
  (void)swingHCmd;

  // Sensible defaults for the heat pump mode
  uint8_t powerMode = PHILCO_AIRCON1_POWER_ON;
  uint8_t operatingMode = PHILCO_AIRCON1_MODE_HEAT;
  uint8_t fanSpeed = PHILCO_AIRCON1_FAN_AUTO;
  uint8_t temperature = 21;
  uint8_t swingV=0;
  uint8_t swingH=0;

  if (powerModeCmd == POWER_OFF)
  {
    powerMode = PHILCO_AIRCON1_POWER_OFF;
    temperature = PHILCO_AIRCON1_OFF_TEMP;
    operatingMode = PHILCO_AIRCON1_MODE_COOL;
  }
  else
  {
    switch (operatingModeCmd)
    {
      case MODE_HEAT:
        operatingMode = PHILCO_AIRCON1_MODE_HEAT;
        break;
      case MODE_COOL:
        operatingMode = PHILCO_AIRCON1_MODE_COOL;
        break;
      case MODE_DRY:
        operatingMode = PHILCO_AIRCON1_MODE_DRY;
        
        fanSpeedCmd = FAN_AUTO; // Fan speed is always 'AUTO' in DRY mode
        break;
      case MODE_FAN:
        operatingMode = PHILCO_AIRCON1_MODE_FAN;
        if ( fanSpeedCmd == FAN_AUTO ) {
          fanSpeedCmd = FAN_1; // Fan speed cannot be 'AUTO' in FAN mode
          temperatureCmd = 25; // Fixed temperature FAN mode
        }
        break;
    }
    if ( temperatureCmd > 15 && temperatureCmd < 31)
    {
      temperature = temperatureCmd;
    }
  }

  switch (fanSpeedCmd)
  {
    case FAN_AUTO:
      fanSpeed = PHILCO_AIRCON1_FAN_AUTO;
      break;
    case FAN_1:
      fanSpeed = PHILCO_AIRCON1_FAN1;
      break;
    case FAN_2:
      fanSpeed = PHILCO_AIRCON1_FAN2;
      break;
    case FAN_3:
      fanSpeed = PHILCO_AIRCON1_FAN3;
      break;
  }

  sendPhilco(IR, powerMode, operatingMode, fanSpeed, temperature, swingV, swingH);
}

// Send the Philco code
void PhilcoHeatpumpIR::sendPhilco(IRSender& IR, uint8_t powerMode, uint8_t operatingMode, uint8_t fanSpeed, uint8_t temperature, uint8_t swingV ,uint8_t swingH)
{
  (void)swingV;
  (void)swingH;

  uint8_t PhilcoTemplate[] = { 0x83, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,    // Header uint8_t 0-1
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00 };  //

  uint8_t  i;

  // Set Power state
  PhilcoTemplate[18] ^= powerMode;
  PhilcoTemplate[19] ^= powerMode;
  // Set the Fan speed, Temperature and Operating Mode
  PhilcoTemplate[2] = fanSpeed;
  PhilcoTemplate[3] = (temperature - 16) << 4;
  PhilcoTemplate[3] = PhilcoTemplate[3] + operatingMode;

  // Calculate checksums
  for (int i = 2; i < 13; i++) {
    PhilcoTemplate[13] ^= PhilcoTemplate[i];
  };
  for (int i = 14; i < 20; i++) {
    PhilcoTemplate[20] ^= PhilcoTemplate[i];
  };

  // 38 kHz PWM frequency
  IR.setFrequency(38);

  // Send Header mark
  IR.mark(PHILCO_AIRCON1_HDR_MARK);
  IR.space(PHILCO_AIRCON1_HDR_SPACE);

  // Data
  for (unsigned int i=0; i<sizeof(PhilcoTemplate); i++) {
    IR.sendIRbyte(PhilcoTemplate[i], PHILCO_AIRCON1_BIT_MARK, PHILCO_AIRCON1_ZERO_SPACE, PHILCO_AIRCON1_ONE_SPACE);
  }

  // End mark
  IR.mark(PHILCO_AIRCON1_BIT_MARK);
  IR.space(0);
}