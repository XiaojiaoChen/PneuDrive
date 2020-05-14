#include <DABoard.h>
#if (DABOARD_NUM>0)
#include "main.h"
#include "math.h"
#include "string.h"
#include "spi.h"

#define hspi_DA 		hspi6
#define DA_CS_GPIO_Port	SPI6_CS_GPIO_Port
#define DA_CS_Pin	SPI6_CS_Pin

#define DA_CS_LOW()  DA_CS_GPIO_Port->BSRR=(uint32_t)DA_CS_Pin<<16U;
#define DA_CS_HIGH() DA_CS_GPIO_Port->BSRR=DA_CS_Pin;


static uint16_t uChannel[LTC2668_CHANNELNUM];
static float fChannel[LTC2668_CHANNELNUM];
static float fMinVoltage = 0;
static float fMaxVoltage = 10;

static void LTC2668_write(uint8_t dac_command, uint8_t dac_address, uint16_t dac_code)
{

  uint16_t DA_Transfer_Send[2];
  DA_Transfer_Send[0] = (uint16_t)(dac_command | dac_address);
  DA_Transfer_Send[1] = dac_code;

  DA_CS_LOW();
  HAL_SPI_Transmit_IT(&hspi_DA,(uint8_t *)DA_Transfer_Send,2);
  DA_CS_HIGH();
  
}

static uint16_t LTC2668_voltage_to_code(float dac_voltage)
{
  uint16_t dac_code;
  float float_code;
  float_code = 65535.0f * (dac_voltage - fMinVoltage) / (fMaxVoltage -fMinVoltage) +fMinVoltage;                    // Calculate the DAC code
  float_code = (float_code > (floor(float_code) + 0.5f)) ? ceil(float_code) : floor(float_code);     // Round
  if (float_code < 0.0f) float_code = 0.0f;
  if (float_code > 65535.0f) float_code = 65535.0f;
  dac_code = (uint16_t) (float_code);                                                               // Convert to unsigned integer
  return (dac_code);
}

void DABoard_writeVoltage(uint16_t selected_dac, float vol)
{
	fChannel[selected_dac]=vol;
	uChannel[selected_dac] = LTC2668_voltage_to_code(vol);
	LTC2668_write(LTC2668_CMD_WRITE_N_UPDATE_N, selected_dac, uChannel[selected_dac]);
}
#endif
