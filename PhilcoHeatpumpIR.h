/*
    Philco heatpump control (remote control P/N DG11L1-12)
*/
#ifndef PhilcoHeatpumpIR_h
#define PhilcoHeatpumpIR_h

#include <HeatpumpIR.h>


// Philco timing constants
#define PHILCO_AIRCON1_HDR_MARK   9050
#define PHILCO_AIRCON1_HDR_SPACE  4550
#define PHILCO_AIRCON1_BIT_MARK   550
#define PHILCO_AIRCON1_ONE_SPACE  1700
#define PHILCO_AIRCON1_ZERO_SPACE 550

// Power state
#define PHILCO_AIRCON1_POWER_ON   0x00
#define PHILCO_AIRCON1_POWER_OFF  0x08

// Mode temperatures
#define PHILCO_AIRCON1_OFF_TEMP   0x10
#define PHILCO_AIRCON1_DRY_TEMP   0x17
#define PHILCO_AIRCON1_FAN_TEMP   0x17

// Operating modes
// Philco codes
#define PHILCO_AIRCON1_MODE_HEAT  0x00
#define PHILCO_AIRCON1_MODE_COOL  0x02
#define PHILCO_AIRCON1_MODE_DRY   0x03
#define PHILCO_AIRCON1_MODE_FAN   0x04

// Fan speeds. Note that some heatpumps have less than 5 fan speeds

#define PHILCO_AIRCON1_FAN_AUTO   0x00 // Fan speed
#define PHILCO_AIRCON1_FAN1       0x03 // * low
#define PHILCO_AIRCON1_FAN2       0x02 // * med
#define PHILCO_AIRCON1_FAN3       0x01 // * high

// TODO: Swing available in this model but not implemented here.
#define PHILCO_VDIR_SWING  0

class PhilcoHeatpumpIR : public HeatpumpIR
{
  public:
    PhilcoHeatpumpIR();
    void send(IRSender& IR, uint8_t powerModeCmd, uint8_t operatingModeCmd, uint8_t fanSpeedCmd, uint8_t temperatureCmd, uint8_t swingVCmd, uint8_t swingHCmd);

  private:
    void sendPhilco(IRSender& IR, uint8_t powerModeCmd, uint8_t operatingMode, uint8_t fanSpeed, uint8_t temperature, uint8_t swingV, uint8_t swingH);
};

#endif
