/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "iicb_interface_ci.h"
#include "iicb_interface.h"

#define I2C_ADDRESS        0x68F
#define I2Cx                             I2C1


#define mpu_6050_adress 0x68 // 0b1101000
#define mpu_6050_sig_path_rst 104

#define init_byte_107 0b00001000
#define init_byte_106 0b00000001
#define init_byte_104 0b00000111

#define mpu_6050_pwr_mgmnt_1 107 //0x6B
#define mpu_6050_smprt_div 25

#define mpu_6050_config 26
#define mpu_6050_gyro_config 27
#define mpu_6050_accel_config 28

#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef huart6;
I2C_HandleTypeDef I2cHandle;

uint8_t u8asax[3];
uint8_t aTxBuffer[]="";
uint8_t aRxBuffer[RXBUFFERSIZE];
uint8_t bb[RXBUFFERSIZE];
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART6_UART_Init(void);
void debug(void);
void testChip(void);


/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  usart_gpio_init();
  MX_USART6_UART_Init();


  Config_I2C_Peripheral();

  testChip();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  debug();

	  //HAL_UART_Transmit(&huart6, (uint8_t *)&buff, 10, 0xFFFF);
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /**Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART6;
  PeriphClkInitStruct.Usart6ClockSelection = RCC_USART6CLKSOURCE_PCLK2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}


void Init__vMPU_6050()
{

	unsigned char dest;
	// reset chip 1 - exit sleep mode
		//_delay_ms(1000);
		I2C__vWriteSingleByteBuffer(mpu_6050_adress,mpu_6050_pwr_mgmnt_1,init_byte_107);
		//_delay_ms(1000);
		I2C__vReadBuffer(mpu_6050_adress,mpu_6050_pwr_mgmnt_1,&dest,1);

		if (dest == init_byte_107)
		{
			printf("\r\n MPU chip 1 reseted successfully !\r\n");

		}
		else
		{
			printf("\r\n MPU chip 1 reset failed !\r\n");
		}
	//	_delay_ms(1000);
	// reset chip 2
		I2C__vWriteSingleByteBuffer(mpu_6050_adress, mpu_6050_sig_path_rst,init_byte_104);
		//_delay_ms(1000);
		I2C__vReadBuffer(mpu_6050_adress,mpu_6050_sig_path_rst,&dest,1);
		if ((dest == 0x00))
		{
			// dupa ce se da reset valorile devin 0 din nou
			printf("\r\n MPU chip 2 reseted successfully !\r\n");

		}
		else
		{
			printf("\r\n MPU chip 2 reset failed !\r\n");
		}
	//	_delay_ms(1000);
	// sample divide rate  sample rate = ex 8kHz / (1+divide rate)  =  8000/110 = 72 Hz
		I2C__vWriteSingleByteBuffer(mpu_6050_adress, mpu_6050_smprt_div,80);
		_delay_ms(1000);
		I2C__vReadBuffer(mpu_6050_adress,mpu_6050_smprt_div,&dest,1);
		if ((dest == init_byte_104))
		{

			printf("\r\n MPU sample divide rate configured successfully !\r\n");
		}
		else
		{
			printf("\r\n MPU sample divide rate configuration failed !\r\n");
		}
		_delay_ms(1000);

	// digital low pass filter
		I2C__vWriteSingleByteBuffer(mpu_6050_adress, mpu_6050_config,0x00);
		_delay_ms(1000);
		I2C__vReadBuffer(mpu_6050_adress,mpu_6050_config,&dest,1);
		if (dest == 0x01)
		{
			printf("\r\n MPU digital low pass filter configured successfully !\r\n");

		}
		else
		{
			printf("\r\n MPU digital low pass filter configuration failed !\r\n");
		}
		_delay_ms(1000);
	// gyroscope config - gyro full scale = +/- 2000dps
		I2C__vWriteSingleByteBuffer(mpu_6050_adress, mpu_6050_gyro_config,0b00011000);
		_delay_ms(1000);
		I2C__vReadBuffer(mpu_6050_adress,mpu_6050_gyro_config,&dest,1);
		if (dest == 0b00001000)
		{
			printf("\r\n MPU gyroscope configured successfully !\r\n");

		}
		else
		{
			printf("\r\n MPU gyroscope configuration failed !\r\n");
		}
		_delay_ms(10);
	// accelerometer config - accelerometer full scale = +/- 4g
		I2C__vWriteSingleByteBuffer(mpu_6050_adress, mpu_6050_accel_config,0b00001000);
		_delay_ms(1000);
		I2C__vReadBuffer(mpu_6050_adress,mpu_6050_accel_config,&dest,1);
		if (dest == 0b00010000)
		{
			printf("\r\n MPU accelerometer configured successfully !\r\n");

		}
		else
		{
			printf("\r\n MPU accelerometer configuration failed !\r\n");
		}
		_delay_ms(1000);

	return ;
}

void WHO_AM_I_vTest()
{
	/* Initialize number of data variables */
	static uint8_t registerContent;
	uint8_t regV;

	/*Step 1 - Transmit the adress and the register adress that shall be read*/
	/* Update bTransferRequest to send buffer write request for Slave */

	I2C__vReadBuffer(0x68,117,(uint8_t*)&registerContent,1);

	//Com_Write_ComLayer_IMUData(registerContent);

	//Com_Read_ComLayer_IMUData(&regV);

	printf("\n\r Who are you ? \n\r");
	printf("\n\r My chip address is = %d \n\r",regV);

	/* Flush Rx buffers */
	Flush_Buffer((uint8_t*)aRxBuffer,RXBUFFERSIZE);
}




void Config_I2C_Peripheral(void)
{

	/*##-1- Configure the I2C peripheral #######################################*/
	I2cHandle.Instance             = I2Cx;

	I2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;

	I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;

	I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
	I2cHandle.Init.OwnAddress1     = I2C_ADDRESS;
	//I2cHandle.Init.OwnAddress2     = 0xFE;  // no use the dev board as slave

	if(HAL_I2C_Init(&I2cHandle) != HAL_OK)
	{
	  /* Initialization Error */
//	  Error_Handler("Error in I2C configuration !!!");
	}

}

void I2C__vReadBuffer(uint8_t I2c_add, uint8_t RegAddr, uint8_t *aRxBuffer, uint8_t rxbuffsz)
{
    /* -> Lets ask for register's address */
	I2C__vWriteBuffer(I2c_add, &RegAddr, 1);

    /* -> Put I2C peripheral in reception process */
    while(HAL_I2C_Master_Receive(&I2cHandle, (uint16_t)(I2c_add<<1), aRxBuffer, (uint16_t)rxbuffsz, (uint32_t)1000) != HAL_OK)
    {
        /* Error_Handler() function is called when Timeout error occurs.
         * When Acknowledge failure occurs (Slave don't acknowledge it's address)
         * Master restarts communication
         */
        if (HAL_I2C_GetError(&I2cHandle) != HAL_I2C_ERROR_AF)
        {
//            Error_Handler("Error in I2C read !!!");
        }
    }

    /* -> Wait for the end of the transfer */
    /* Before starting a new communication transfer, you need to check the current
     * state of the peripheral; if it�s busy you need to wait for the end of current
     * transfer before starting a new one.
     * For simplicity reasons, this example is just waiting till the end of the
     * transfer, but application may perform other tasks while transfer operation
     * is ongoing.
     **/
    while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY)
    {
    }
}

void I2C__vWriteBuffer(uint8_t I2c_add, uint8_t *aTxBuffer, uint16_t txbuffsz)
{
    /* -> Start the transmission process */
    /* While the I2C in reception process, user can transmit data through "aTxBuffer" buffer */
    while(HAL_I2C_Master_Transmit(&I2cHandle, (uint16_t)(I2c_add<<1),(uint8_t*)aTxBuffer,txbuffsz,(uint32_t)1000)!= HAL_OK)
    {
        /*
         * Error_Handler() function is called when Timeout error occurs.
         * When Acknowledge failure occurs (Slave don't acknowledge it's address)
         * Master restarts communication
         */

        if (HAL_I2C_GetError(&I2cHandle) != HAL_I2C_ERROR_AF)
        {
//            Error_Handler("Error in I2C write !!!");
        }

    }

    /* -> Wait for the end of the transfer */
    /* Before starting a new communication transfer, you need to check the current
     * state of the peripheral; if it�s busy you need to wait for the end of current
     * transfer before starting a new one.
     * For simplicity reasons, this example is just waiting till the end of the
     * transfer, but application may perform other tasks while transfer operation
     * is ongoing.
     */
      while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY)
      {
      }
}

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart6, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}

void usart_gpio_init()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	__GPIOC_CLK_ENABLE();

	/**USART2 GPIO Configuration
	PD5     ------> USART2_TX
	PD6     ------> USART2_RX
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

//	__USART6_CLK_ENABLE();
  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 9600;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  huart6.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart6.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */



  	/* Peripheral interrupt init*/
//  	HAL_NVIC_SetPriority(USART6_IRQn, 0, 0);
//  	HAL_NVIC_EnableIRQ(USART6_IRQn);

}

void testChip(void)
{
	uint8_t u8destVal;

	I2C__vReadBuffer(mpu_6050_adress,117,&u8destVal,1);

	if (u8destVal == 0x68)
	{
		printf("\r\n----Who am I...--------[ OK ]");
	}
	else
	{
		printf("\r\n---nWho am I...[ NOK ]---");
	}
}


void debug(void)
{
	printf("\r\n%f;%f;%f;%f",0.01, 0.00, 1.00, 0.00);
	printf("\r\n%f;%f;%f;%f",0.02, 0.00, 1.00, 0.00);
	printf("\r\n%f;%f;%f;%f",0.03, 0.00, 1.00, 0.00);
	printf("\r\n%f;%f;%f;%f",0.04, 0.00, 1.00, 0.00);
	printf("\r\n%f;%f;%f;%f",0.05, 0.00, 1.00, 0.00);
	printf("\r\n%f;%f;%f;%f",0.06, 0.00, 1.00, 0.00);
	printf("\r\n%f;%f;%f;%f",0.07, 0.00, 1.00, 0.00);
	printf("\r\n%f;%f;%f;%f",0.08, 0.00, 1.00, 0.00);
	printf("\r\n%f;%f;%f;%f",0.09, 0.00, 1.00, 0.00);

	printf("\r\n%f;%f;%f;%f",0.01, 1.00, 0.00, 0.00);
	printf("\r\n%f;%f;%f;%f",0.02, 1.00, 0.00, 0.00);
	printf("\r\n%f;%f;%f;%f",0.03, 1.00, 0.00, 0.00);
	printf("\r\n%f;%f;%f;%f",0.04, 1.00, 0.00, 0.00);
	printf("\r\n%f;%f;%f;%f",0.05, 1.00, 0.00, 0.00);
	printf("\r\n%f;%f;%f;%f",0.06, 1.00, 0.00, 0.00);
	printf("\r\n%f;%f;%f;%f",0.07, 1.00, 0.00, 0.00);
	printf("\r\n%f;%f;%f;%f",0.08, 1.00, 0.00, 0.00);
	printf("\r\n%f;%f;%f;%f",0.09, 1.00, 0.00, 0.00);

	printf("\r\n%f;%f;%f;%f",0.01, 0.00, 0.00, 1.00);
	printf("\r\n%f;%f;%f;%f",0.02, 0.00, 0.00, 1.00);
	printf("\r\n%f;%f;%f;%f",0.03, 0.00, 0.00, 1.00);
	printf("\r\n%f;%f;%f;%f",0.04, 0.00, 0.00, 1.00);
	printf("\r\n%f;%f;%f;%f",0.05, 0.00, 0.00, 1.00);
	printf("\r\n%f;%f;%f;%f",0.06, 0.00, 0.00, 1.00);
	printf("\r\n%f;%f;%f;%f",0.07, 0.00, 0.00, 1.00);
	printf("\r\n%f;%f;%f;%f",0.08, 0.00, 0.00, 1.00);
	printf("\r\n%f;%f;%f;%f",0.09, 0.00, 0.00, 1.00);
}


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
