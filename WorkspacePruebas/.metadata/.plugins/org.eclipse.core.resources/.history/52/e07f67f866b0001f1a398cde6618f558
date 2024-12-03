#ifndef INC_COMMON_H_
#define INC_COMMON_H_

#define LOW			0
#define HIGH 		1

#define OFF			0
#define ON			1

#define sleep_10s	10000/portTICK_PERIOD_MS
#define sleep_2s	2000/portTICK_PERIOD_MS
#define sleep_1s	1000/portTICK_PERIOD_MS
#define sleep_500ms	500/portTICK_PERIOD_MS
#define sleep_300ms	300/portTICK_PERIOD_MS
#define sleep_100ms	100/portTICK_PERIOD_MS
#define sleep_50ms	50/portTICK_PERIOD_MS

#define blockForever	portMAX_DELAY
#define nonBlocking		0

#define hal_toggleOutput(pin)			HAL_GPIO_TogglePin(pin##_GPIO_Port, pin##_Pin)
#define hal_digitalOutput(status, pin) ((status == HIGH) ? HAL_GPIO_WritePin(pin##_GPIO_Port, pin##_Pin, GPIO_PIN_SET) : HAL_GPIO_WritePin(pin##_GPIO_Port, pin##_Pin, GPIO_PIN_RESET))
#define	hal_digitalInput(pin)			HAL_GPIO_ReadPin(pin##_GPIO_Port, pin##_Pin)

#define lcdSize							2
#define lcd_init(maxRow)				HD44780_Init(maxRow)
#define lcd_clear()						HD44780_Clear()
#define lcd_setCursor(col, row)			HD44780_SetCursor(col, row)
#define	lcd_print(lcdStr)				HD44780_PrintStr(lcdStr)
#define lcd_msgPrint(col,row,lcdStr)	HD44780_SetCursor(col,row);\
										HD44780_PrintStr(lcdStr)

#define reset_values()					claveIngresada = 0;\
										contDig = 0;\
										strcpy(lcdStr,">>>")

typedef enum{
	READ,
	WRITE,
	SCANN,
	CONT_DIG_OK,
	CONT_DIG_ERR,
	ACC_OK,
	ACC_DENEGADO,
	ACC_ERROR,
	ACC_MAESTRA,
	ACC_30SEG,
	ACC_SIN_ASIGNAR,
	ACC_ASIGNADO,
	ACC_MASTER,
	CAMBIO_ESTADO
}accion_t;

typedef enum{
	ALARMA,
	PUERTA,
	CIERRE
}tipo_t;

typedef enum{
	COLUMNA_1,
	COLUMNA_2,
	COLUMNA_3,
	COLUMNA_4
}columna_t;

typedef enum{
	FILA_1,
	FILA_2,
	FILA_3,
	FILA_4
}fila_t;

typedef struct{
	uint8_t accion;
	uint8_t tecla;
}tecla_t;

typedef struct{
	uint8_t accion;
	uint8_t linea1[16];
	uint8_t linea2[16];
}display_t;

typedef struct{
	tipo_t tipo;
	accion_t accion;
	accion_t anterior;
	uint32_t id;
}actuador_t;

typedef enum{
	PRINT,
	TEMPORARY_PRINTING,
	TEMPORARY_PRINTING_10s,
	CLEAR,
	CLEAR_1,
	CLEAR_2,
	CLEAR_PRINT
}display_e;

typedef enum{
	MAESTRO,
	USUARIO_1,
	USUARIO_2,
	USUARIO_3,
	USUARIO_4,
	USUARIO_5,
	USUARIO_6,
	USUARIO_7,
	USUARIO_8,
	USUARIO_9,
	USUARIO_10,
	SIN_ASIGNAR
}nroUsuario_t;

typedef struct{
	accion_t accion;
	nroUsuario_t nroUsuario;
	uint32_t clave;
	uint32_t intentosDisp;
	uint32_t id;
}usuario_t;



#define MAESTRA		10
#define ARRIBA		11
#define ABAJO		12
#define CANCELAR	13
#define MENU		14
#define ACEPTAR		15
#define NUM			16

typedef enum function {None,chequearClaveUsuario,chequearClaveMaestro};

#endif
