#ifndef __FLASH_H__
#define __FLASH_H__
#include "stm32f10x.h"
#include "public.h"
#include "board.h"


#define sFLASH_CS_GPIO_PORT  GPIOB
#define sFLASH_SPI					 SPI2
#define sFLASH_CS_PIN        SPI2_NSS




#define ResetTest_ADDR 0x0010

#define sFLASH_CMD_WRITE          0x02  /*!< Write to Memory instruction */
#define sFLASH_CMD_WRSR           0x01  /*!< Write Status Register instruction */
#define sFLASH_CMD_WREN           0x06  /*!< Write enable instruction */
#define sFLASH_CMD_READ           0x03  /*!< Read from Memory instruction */
#define sFLASH_CMD_FSTRD          0x0B  /*!< Fast Read from Memory instruction */
#define sFLASH_CMD_RDSR           0x05  /*!< Read Status Register instruction  */
#define sFLASH_CMD_RDID           0x9F  /*!< Read identification */
#define sFLASH_CMD_WRDI           0x04  /*!< Reset write enable latch */
#define sFLASH_CMD_SLEEP          0xB9  /*!< Enter sleep mode */

#define sFLASH_WIP_FLAG           0x01  /*!< Write In Progress (WIP) flag */
#define sFLASH_DUMMY_BYTE         0xA5
#define sFLAsh_FM25V02GT_ID				0x7F7F7F7F7F7FC22200
/**
  * @brief  Select sFLASH: Chip Select pin low
  */
#define sFLASH_CS_LOW()       GPIO_ResetBits(sFLASH_CS_GPIO_PORT, sFLASH_CS_PIN)
/**
  * @brief  Deselect sFLASH: Chip Select pin high
  */
#define sFLASH_CS_HIGH()      GPIO_SetBits(sFLASH_CS_GPIO_PORT, sFLASH_CS_PIN)


/** @defgroup STM32_EVAL_SPI_FLASH_Exported_Functions
  * @{
  */
/**
  * @brief  High layer functions
  */
void sFLASH_Init(void);
void sFLASH_Reset(void);
void sFLASH_WriteObject(void *pBuffer,uint16_t WriteAddr, uint16_t NumByteToWrite);
void sFLASH_ReadObject(void *pBuffer,uint16_t ReadAddr, uint16_t NumByteToRead);
void sFLASH_WriteHalfWord(uint16_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
void sFLASH_ReadHalfWord(uint16_t* pBuffer, uint16_t ReadAddr, uint16_t NumByteToRead);
void sFLASH_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
void sFLASH_ReadBuffer(uint8_t* pBuffer,   uint16_t ReadAddr, uint16_t NumByteToRead);
uint32_t sFLASH_ReadID(void);
void sFLASH_StartReadSequence(uint16_t ReadAddr);

/**
  * @brief  Low layer functions
  */
uint8_t sFLASH_ReadByte(void);
uint8_t sFLASH_SendByte(uint8_t byte);
uint16_t sFLASH_SendHalfWord(uint16_t HalfWord);
void sFLASH_WriteEnable(void);

#endif
