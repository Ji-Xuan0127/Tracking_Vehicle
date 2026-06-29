/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "st7735.h"
#include <stdio.h>
#include <string.h>
#include "img.h"
#include "Control.h"
#include "key.h"
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

/* USER CODE BEGIN PV */
uint16_t Tracks[4] = {0, 0, 0, 0};
uint8_t Track_Binary[4] = {0, 0, 0, 0};
uint16_t values[4] = {0, 0, 0, 0};

#define ADC_FULL_SCALE      4095.0f
#define MOTOR_PWM_MAX       4199
#define TRACK_THRESHOLD     2000U
static uint16_t values_filtered[4] = {0, 0, 0, 0};
static uint8_t pot_filter_ready = 0;

int16_t Speed1_now = 0;
int16_t Speed1_last = 0;
int16_t Speed2_now = 0;
int16_t Speed2_last = 0;
int16_t Speed_Target = 0;

uint8_t KeyNum = 0;
volatile uint8_t Start_Flag = 0;

float Track_current = 0.0f;
float Track_last = 0.0f;
float Track_error = 0.0f;
float Track_error_last = 0.0f;
float Track_integral = 0.0f;

float Kp = 0.0f;
float Ki = 0.0f;
float Kd = 0.0f;

float Out = 0.0f;
float Last_Out = 0.0f;

char start[20] = "";
char output[20] = "";
char speed[20] = "";
char kp[20] = "";
char ki[20] = "";
char kd[20] = "";
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int16_t Encoder_Get(TIM_HandleTypeDef *htim);
static uint16_t ADC_Filter(uint16_t raw, uint16_t *state);
static float Map_ADC_To_Float(uint16_t raw, float min, float max);
static int16_t Clamp_PWM(int32_t value);
static void Track_Binary_Process(void);
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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_TIM10_Init();
  MX_TIM11_Init();
  MX_USART1_UART_Init();
  MX_TIM9_Init();
  /* USER CODE BEGIN 2 */
  HAL_Delay(20);
  OLED_Init();
  ST7735_Init();

  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)Tracks, 4);
  HAL_ADC_Start_DMA(&hadc2, (uint32_t*)values, 4);

  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
  HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);

  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_Base_Start_IT(&htim7);
  HAL_TIM_Base_Start_IT(&htim9);

  HAL_TIM_PWM_Start_IT(&htim10, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start_IT(&htim11, TIM_CHANNEL_1);

  Start_Motor();

  ST7735_DrawImage(0, 0, 128, 128, (uint16_t*)back_img_128x128);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    static uint8_t last_start_flag = 0;

    KeyNum = Key_GetNum();

    if(KeyNum == 1)
    {
      Start_Flag = !Start_Flag;
    }

    if (Start_Flag && !last_start_flag)
    {
      Track_integral = 0.0f;
      Track_error = 0.0f;
      Track_error_last = 0.0f;
      Track_last = 0.0f;
    }

    last_start_flag = Start_Flag;

    sprintf(start, "Start: %s", Start_Flag ? "ON" : "OFF");

    if(Start_Flag)
    {
      HAL_GPIO_WritePin(Start_out_GPIO_Port, Start_out_Pin, GPIO_PIN_SET);
    }
    else
    {
      HAL_GPIO_WritePin(Start_out_GPIO_Port, Start_out_Pin, GPIO_PIN_RESET);
    }

    OLED_NewFrame();

    OLED_PrintASCIIString(0, 0, start, &afont8x6, OLED_COLOR_NORMAL);
    OLED_PrintASCIIString(0, 16, speed, &afont8x6, OLED_COLOR_NORMAL);
    OLED_PrintASCIIString(0, 24, output, &afont8x6, OLED_COLOR_NORMAL);
    OLED_PrintASCIIString(0, 32, kp, &afont8x6, OLED_COLOR_NORMAL);
    OLED_PrintASCIIString(0, 40, ki, &afont8x6, OLED_COLOR_NORMAL);
    OLED_PrintASCIIString(0, 48, kd, &afont8x6, OLED_COLOR_NORMAL);

    OLED_ShowFrame();

    char uart_buf[128];
    HAL_Delay(1000);
    sprintf(uart_buf, "ADC:%u,%u,%u,%u\r\nSpeed:%d Kp:%.2f Ki:%.2f Kd:%.2f\r\n",
         values[0], values[1], values[2], values[3],
         Speed_Target, Kp, Ki, Kd);
    HAL_UART_Transmit(&huart1, (uint8_t *)uart_buf, strlen(uart_buf), HAL_MAX_DELAY);
    /* USER CODE END WHILE */

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
int16_t Encoder_Get(TIM_HandleTypeDef *htim)
{
	int16_t temp = 0;
	temp = __HAL_TIM_GET_COUNTER(htim);
	__HAL_TIM_SET_COUNTER(htim,0);
	return temp;
}

static uint16_t ADC_Filter(uint16_t raw, uint16_t *state)
{
  if (!pot_filter_ready)
  {
    *state = raw;
    return raw;
  }

  *state = (uint16_t)((((uint32_t)(*state) * 7U) + raw) / 8U);
  return *state;
}

static float Map_ADC_To_Float(uint16_t raw, float min, float max)
{
  float ratio = (float)raw / ADC_FULL_SCALE;
  return min + (max - min) * ratio;
}

static int16_t Clamp_PWM(int32_t value)
{
  if (value < 0)
  {
    return 0;
  }

  if (value > MOTOR_PWM_MAX)
  {
    return MOTOR_PWM_MAX;
  }

  return (int16_t)value;
}

static void Track_Binary_Process(void)
{
  for (uint8_t i = 0; i < 4U; ++i)
  {
    Track_Binary[i] = (Tracks[i] > TRACK_THRESHOLD) ? 1U : 0U;
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM6)
  {
    uint16_t speed_adc = ADC_Filter(values[0], &values_filtered[0]);
    Speed_Target = Clamp_PWM((int32_t)Map_ADC_To_Float(speed_adc, 0.0f, (float)MOTOR_PWM_MAX));

    Speed1_now = Encoder_Get(&htim3);
    Speed2_now = Encoder_Get(&htim4);
  }
  else if (htim->Instance == TIM7)
  {
    if (!Start_Flag)
    {
      Stop_Motor();
      return;
    }

    if (!pot_filter_ready)
    {
      pot_filter_ready = 1;
    }

    Track_Binary_Process();

    uint32_t track_sum = (uint32_t)Track_Binary[0] + Track_Binary[1] + Track_Binary[2] + Track_Binary[3];
    if ((Track_Binary[0] == 1U) && (Track_Binary[1] == 1U) && (Track_Binary[2] == 1U) && (Track_Binary[3] == 1U))
    {
      Out = 0.0f;
      Track_error = 0.0f;
      Track_error_last = 0.0f;
      Track_integral = 0.0f;
      Track_last = 0.0f;
      Last_Out = 0.0f;
    }
    else if ((Track_Binary[0] == 0U) && (Track_Binary[1] == 0U) && (Track_Binary[2] == 0U) && (Track_Binary[3] == 0U))
    {
      Out = Last_Out;
    }
    else
    {
      if (track_sum == 0U)
      {
        Track_current = Track_last;
        Track_error = 0.0f;
      }
      else
      {
        Track_current = (float)((-3 * Track_Binary[0]) + (-1 * Track_Binary[1]) + (1 * Track_Binary[2]) + (3 * Track_Binary[3]));
        Track_current /= (float)track_sum;
        Track_error = Track_current - Track_last;
        Track_last = Track_current;
      }

      uint16_t kp_adc = ADC_Filter(values[1], &values_filtered[1]);
      uint16_t ki_adc = ADC_Filter(values[2], &values_filtered[2]);
      uint16_t kd_adc = ADC_Filter(values[3], &values_filtered[3]);

      Kp = Map_ADC_To_Float(kp_adc, 0.0f, 200.0f);
      Ki = Map_ADC_To_Float(ki_adc, 0.0f, 20.0f);
      Kd = Map_ADC_To_Float(kd_adc, 0.0f, 100.0f);

      Track_integral += Track_error;
      if (Track_integral > 100.0f)
      {
        Track_integral = 100.0f;
      }
      else if (Track_integral < -100.0f)
      {
        Track_integral = -100.0f;
      }

      Out = (Kp * Track_error) + (Ki * Track_integral) + (Kd * (Track_error - Track_error_last));
      Track_error_last = Track_error;
      Last_Out = Out;
    }

    Set_Speed(
      Clamp_PWM((int32_t)((float)Speed_Target - Out)),
      Clamp_PWM((int32_t)((float)Speed_Target + Out))
    );

    sprintf(kp, "Kp: %.2f", Kp);
    sprintf(ki, "Ki: %.2f", Ki);
    sprintf(kd, "Kd: %.2f", Kd);
    sprintf(speed, "Speed: %d", Speed_Target);
    sprintf(output, "Out: %.2f", Out);
  }
  else if (htim->Instance == TIM9)
  {
    Key_Tick();
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
