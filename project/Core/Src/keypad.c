

#include "keypad.h"

#include "main.h"


static uint8_t keypad_debounce(uint16_t GPIO_Pin)
{
	static uint16_t last_pressed = 0xFFFF;
	static uint32_t last_tick = 0;

	if (last_pressed == GPIO_Pin) {
		if (HAL_GetTick() < (last_tick + 200)) {
			return 0;
		}
	}
	last_pressed = GPIO_Pin;
	last_tick = HAL_GetTick();

	return 1;
}

uint8_t keypad_scan_row(GPIO_TypeDef *COLUMNx_GPIO_Port, uint16_t COLUMNx_Pin)
{
	uint8_t row_pressed = 0xFF;
	HAL_GPIO_WritePin(ROW1_GPIO_Port, ROW1_Pin, GPIO_PIN_RESET);
	if (HAL_GPIO_ReadPin(COLUMNx_GPIO_Port, COLUMNx_Pin) == 0) {
		row_pressed = 1;
	}
	HAL_GPIO_WritePin(ROW1_GPIO_Port, ROW1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ROW2_GPIO_Port, ROW2_Pin, GPIO_PIN_RESET);
	if (HAL_GPIO_ReadPin(COLUMNx_GPIO_Port, COLUMNx_Pin) == 0) {
		row_pressed = 2;
	}
	HAL_GPIO_WritePin(ROW2_GPIO_Port, ROW2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ROW3_GPIO_Port, ROW3_Pin, GPIO_PIN_RESET);
	if (HAL_GPIO_ReadPin(COLUMNx_GPIO_Port, COLUMNx_Pin) == 0) {

	}
	HAL_GPIO_WritePin(ROW3_GPIO_Port, ROW3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ROW4_GPIO_Port, ROW4_Pin, GPIO_PIN_RESET);
	if (HAL_GPIO_ReadPin(COLUMNx_GPIO_Port, COLUMNx_Pin) == 0) {

	}
	HAL_GPIO_WritePin(ROW4_GPIO_Port, ROW4_Pin, GPIO_PIN_SET);
	return row_pressed;
}

uint8_t keypad_chars[4][4] = {
		{'1', '2', '3', 'A'},
		{'4', '5', '6', 'B'},
		{'7', '8', '9', 'C'},
		{'*', '0', '#', 'D'},
};

uint8_t keypad_scan(uint16_t GPIO_Pin)
{
	uint8_t key_pressed = 0xFF;

	if (keypad_debounce(GPIO_Pin) == 0) {
		return key_pressed;
	}
	uint8_t row = 0xFF;
	switch (GPIO_Pin) {
	case COLUMN1_Pin:
		row = keypad_scan_row(COLUMN1_GPIO_Port, COLUMN1_Pin);
		key_pressed = keypad_chars[row - 1][1 - 1];
		break;

	case COLUMN2_Pin:
		row = keypad_scan_row(COLUMN2_GPIO_Port, COLUMN2_Pin);
		key_pressed = keypad_chars[row - 1][2 - 1];
		break;
	default:
		break;
	}
	return key_pressed;
}
