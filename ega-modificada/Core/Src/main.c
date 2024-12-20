/* USER CODE BEGIN Header */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "liquidcrystal_i2c.h"
#include "FLASH_PAGE_F1.h"

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "common.h"

#include <stdlib.h>
#include "menu.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define prioridad_tMenu				1
#define prioridad_tTeclado			2
#define prioridad_tDisplay			1
#define prioridad_tFlash			2
#define prioridad_tControlAcceso	1
#define prioridad_tActuadores		3

#define stackSize					512

#define SIN_INTENTOS_DISP			0
#define CANT_MAX_INTENTOS			3
#define CANT_MAX_USUARIOS			11

#define	PAGE_ADDR_1					0x0801FC00
#define FLASH_BUFFER_SIZE 			32

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
TaskHandle_t tMenu_h = NULL, tDisplay_h = NULL, tFlash_h = NULL, tTeclado_h = NULL, tControlAcceso_h = NULL
		, tActuadores_h = NULL;
QueueHandle_t columna_q, tecla_q, display_q, actuador_q, reqAcceso_q, respAcceso_q, flashcmd_q, flashdata_q;
SemaphoreHandle_t acAlarma_s = NULL, acPuerta_s = NULL, sensor_s = NULL;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
static void err (uint8_t errCode);
void display_Print(uint8_t accion, char *linea1, char *linea2);
//declaraciones de menu
void ME_conteo_digitos();
static void dummyDataMemoryRecording (void);


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void dummyDataMemoryRecording (void){
	uint32_t bufferAux[FLASH_BUFFER_SIZE];

	bufferAux[0] = 0;
	bufferAux[1] = 87654321;
	bufferAux[2] = 1;
	bufferAux[3] = 1111;

	for(uint32_t j = 2; j < (CANT_MAX_USUARIOS); j++){
		bufferAux[(2*j)] = j;
		bufferAux[(2*j)+1] = 0;
	}
	Flash_Write_Data(PAGE_ADDR_1, bufferAux, FLASH_BUFFER_SIZE);
}

void display_Print(uint8_t accion, char *linea1, char *linea2){
	display_t display;

	display.accion = accion;
	strcpy(display.linea1, linea1);
	strcpy(display.linea2, linea2);
	xQueueSend(display_q, &display, blockForever);
}

static void t_Menu (void *pvParameters){
	struct level claveUsuario1, claveUsuario2, claveUsuario3, claveUsuario4, claveUsuario5, claveUsuario6,
	 	 	 	 	 claveUsuario7, claveUsuario8, claveUsuario9, claveUsuario10,
					claveMaestra, agregarUsuario, borrarUsuario, resetFabrica, menu, *currentM;

	usuario_t usuario[CANT_MAX_USUARIOS];

	uint32_t i;

	for(i = 0; i < CANT_MAX_USUARIOS; i++){
		usuario[i].clave = 0;
		usuario[i].nroUsuario = i;
		usuario[i].accion = SCANN;

		xQueueSendToBack(reqAcceso_q, &usuario[i], blockForever);
		xQueueReceive(respAcceso_q, &usuario[i], blockForever);
	}

	//BuildMenu(nodoActual, nroUsuario, imprimir1, imprimir2, funcCancelar, funcAceptar, nodoArriba, nodoAbajo, nodoMaestra, nodoCancelar, nodoAceptar, nodoMenu, lastMenu)

    MEN_BuildMenu(&claveUsuario1, USUARIO_1,"USUARIO 1", ">>>", 0, 0, &claveUsuario10, &claveUsuario2, &claveMaestra, &claveUsuario1, 0, &menu, 0);
    MEN_BuildMenu(&claveUsuario2, USUARIO_2,"USUARIO 2", ">>>", 0, 0, &claveUsuario1, &claveUsuario3, &claveMaestra, &claveUsuario1, 0, &menu, 0);
    MEN_BuildMenu(&claveUsuario3, USUARIO_3,"USUARIO 3", ">>>", 0, 0, &claveUsuario2, &claveUsuario4, &claveMaestra, &claveUsuario1, 0, &menu, 0);
    MEN_BuildMenu(&claveUsuario4, USUARIO_4,"USUARIO 4", ">>>", 0, 0, &claveUsuario3, &claveUsuario5, &claveMaestra, &claveUsuario1, 0, &menu, 0);
    MEN_BuildMenu(&claveUsuario5, USUARIO_5,"USUARIO 5", ">>>", 0, 0, &claveUsuario4, &claveUsuario6, &claveMaestra, &claveUsuario1, 0, &menu, 0);
    MEN_BuildMenu(&claveUsuario6, USUARIO_6,"USUARIO 6", ">>>", 0, 0, &claveUsuario5, &claveUsuario7, &claveMaestra, &claveUsuario1, 0, &menu, 0);
    MEN_BuildMenu(&claveUsuario7, USUARIO_7,"USUARIO 7", ">>>", 0, 0, &claveUsuario6, &claveUsuario8, &claveMaestra, &claveUsuario1, 0, &menu, 0);
    MEN_BuildMenu(&claveUsuario8, USUARIO_8,"USUARIO 8", ">>>", 0, 0, &claveUsuario7, &claveUsuario9, &claveMaestra, &claveUsuario1, 0, &menu, 0);
    MEN_BuildMenu(&claveUsuario9, USUARIO_9,"USUARIO 9", ">>>", 0, 0, &claveUsuario8, &claveUsuario10, &claveMaestra, &claveUsuario1, 0, &menu, 0);
    MEN_BuildMenu(&claveUsuario10, USUARIO_10,"USUARIO 10", ">>>", 0, 0, &claveUsuario9, &claveUsuario1, &claveMaestra, &claveUsuario1, 0, &menu, 0);
    MEN_BuildMenu(&claveMaestra, 0,"CLAVE MAESTRA", ">>>", 0, 0, 0, 0, 0, &claveUsuario1, 0, &menu, 0);
    MEN_BuildMenu(&menu, 0,"MENU CONFIG.", "<<< >>>", 0, 0, &borrarUsuario, &agregarUsuario, 0, &claveUsuario1, 0, 0, 0);
    MEN_BuildMenu(&agregarUsuario, 11,"AGREGAR USUARIO", ">>>", 0, 0, &borrarUsuario, &borrarUsuario, 0, &claveUsuario1, 0, 0, 0);
    MEN_BuildMenu(&borrarUsuario, 12,"BORRAR USUARIO", ">>>", 0, 0, &agregarUsuario, &agregarUsuario, 0, &claveUsuario1, 0, 0, 0);
    MEN_BuildMenu(&resetFabrica, 13,"RESET FABRICA", ">>>", 0, 0, &agregarUsuario, &borrarUsuario, 0, &claveUsuario1, 0, 0, 0);

	char lcdStr[16] = ">>>";
	uint32_t claveIngresada = 0, contDig = 0;
	lcd_init(lcdSize);

    currentM = &claveUsuario1;
    MEN_Last(&currentM);
	display_Print(CLEAR_PRINT, currentM->row1, currentM->row2);

    tecla_t tecla;

	for(;;){
		xQueueReceive(tecla_q, &tecla, blockForever);
		switch(tecla.accion){
			case NUM:

				if(contDig < 8){
					claveIngresada += tecla.tecla * pow(10, contDig);
					contDig++;
					strcat(lcdStr,"*");
				}
				else
					display_Print(TEMPORARY_PRINTING, '\0', "ERROR 5");

				display_Print(PRINT, '\0', lcdStr);
				break;
			case ARRIBA:
				MEN_Prev(&currentM);
				if(usuario[currentM->userNo].accion == ACC_SIN_ASIGNAR)
					display_Print(CLEAR_PRINT, currentM->row1, "Sin asignar");
				else display_Print(CLEAR_PRINT, currentM->row1, currentM->row2);
				if(menu.privilege == MASTER)
					currentM->last = &borrarUsuario;
				reset_values();
				break;

			case ABAJO:
				MEN_Next(&currentM);
				if(usuario[currentM->userNo].accion == ACC_SIN_ASIGNAR)
					display_Print(CLEAR_PRINT, currentM->row1, "Sin asignar");
				else display_Print(CLEAR_PRINT, currentM->row1, currentM->row2);
				if(menu.privilege == MASTER)
					currentM->last = &borrarUsuario;
				reset_values();
				break;

			case MENU:
				MEN_Maestra(&currentM);
				currentM->last = &menu;
				display_Print(TEMPORARY_PRINTING, "MENU CONFIG.", 0);
				display_Print(CLEAR_PRINT, currentM->row1, currentM->row2);
				reset_values();
				break;

			case ACEPTAR:

				if((currentM == &claveUsuario1 || currentM == &claveUsuario2 || currentM == &claveUsuario3 || currentM == &claveUsuario4 || currentM == &claveMaestra) && menu.privilege == USER){
					if(currentM->userNo == 0){
						//clave maestra
						if(contDig != 8) display_Print(TEMPORARY_PRINTING, 0, "Error de digitos");
						else{
							//cantidad correcta de digitos
							usuario[currentM->userNo].clave = claveIngresada;
							usuario[currentM->userNo].accion = READ;
							xQueueSendToBack(reqAcceso_q, &usuario[currentM->userNo], blockForever);
							xQueueReceive(respAcceso_q, &usuario[currentM->userNo], blockForever);
							reset_values();

							switch(usuario[currentM->userNo].accion){
								case ACC_OK:
									if(currentM->last == &menu){
										MEN_Menu(&currentM);
										display_Print(CLEAR_PRINT, currentM->row1, currentM->row2);

										menu.privilege = MASTER;
										agregarUsuario.state = 0;
										borrarUsuario.state = 0;
									}
									else{
										display_Print(TEMPORARY_PRINTING, 0, "Acceso maestro");
										//se abre la puerta por tiempo indefinido
										reset_values();
										currentM = &claveUsuario1;
										display_Print(CLEAR_PRINT, currentM->row1, currentM->row2);
									}
									break;
								case ACC_DENEGADO:
									display_Print(TEMPORARY_PRINTING, 0, "Acceso denegado");
									display_Print(CLEAR_PRINT, currentM->row1, currentM->row2);
									break;
								case ACC_ERROR:
									display_Print(TEMPORARY_PRINTING_10s, "Espere 10 seg", "Acceso denegado");
									vTaskDelay(sleep_10s);
									reset_values();
									display_Print(CLEAR_PRINT, currentM->row1, currentM->row2);
									break;

								default:
									break;
							}
						}
					}
					else{
						if(contDig != 4) display_Print(TEMPORARY_PRINTING, 0, "ERROR 3");
						else{
							//cantidad correcta de digitos
							usuario[currentM->userNo].clave = claveIngresada;
							usuario[currentM->userNo].accion = READ;
							xQueueSendToBack(reqAcceso_q, &usuario[currentM->userNo], blockForever);
							xQueueReceive(respAcceso_q, &usuario[currentM->userNo], blockForever);
							reset_values();

							switch(usuario[currentM->userNo].accion){
								case ACC_OK:
									display_Print(TEMPORARY_PRINTING, 0, "Acceso permitido");
									vTaskDelay(sleep_2s);
									MEN_Last(&currentM);
									display_Print(CLEAR_PRINT, currentM->row1, currentM->row2);
									break;
								case ACC_DENEGADO:
									display_Print(TEMPORARY_PRINTING, 0, "Acceso denegado");
									display_Print(CLEAR_PRINT, currentM->row1, currentM->row2);
									break;
								case ACC_ERROR:

									break;
								default:
									break;
							}
						}
					}
				}
				else if(menu.privilege == MASTER){
					if(currentM == &agregarUsuario){
						switch(agregarUsuario.state){
						case 0:
							i = 1;
							while(usuario[i].accion != ACC_SIN_ASIGNAR){
								if(i < CANT_MAX_USUARIOS) i++;
								else {
									display_Print(TEMPORARY_PRINTING, 0, "Sin espacio");
									break;
								}
							}
							if(usuario[i].accion == ACC_SIN_ASIGNAR){
								sprintf(lcdStr, "Usuario %u libre", i);
								display_Print(TEMPORARY_PRINTING, 0, lcdStr);
								agregarUsuario.state = 1;
							}
							break;
						case 1:
							usuario[i].clave = claveIngresada;
							usuario[i].accion = WRITE;
							usuario[i].nroUsuario = i;

							xQueueSendToBack(reqAcceso_q, &usuario[i], blockForever);
							sprintf(lcdStr, "Usuario %u", i);
							display_Print(TEMPORARY_PRINTING, lcdStr, "Guardado");
						    currentM = &claveUsuario1;
							display_Print(CLEAR_PRINT, currentM->row1, currentM->row2);
							agregarUsuario.state = 0;
							menu.privilege = USER;
							break;
						}

					}
					else if(currentM == &borrarUsuario){
						currentM = &claveUsuario1;
						display_Print(CLEAR_PRINT, currentM->row1, currentM->row2);
						display_Print(TEMPORARY_PRINTING, currentM->row1, currentM->row2);
						borrarUsuario.state = 1;
					}
					else if(currentM->last == &borrarUsuario){
						usuario[currentM->userNo].clave = 0;
						usuario[currentM->userNo].accion = WRITE;

						xQueueSendToBack(reqAcceso_q, &usuario[currentM->userNo], blockForever);

						usuario[currentM->userNo].accion = SCANN;
						xQueueSendToBack(reqAcceso_q, &usuario[currentM->userNo], blockForever);
						xQueueReceive(respAcceso_q, &usuario[currentM->userNo], blockForever);

						if(usuario[currentM->userNo].accion == ACC_SIN_ASIGNAR){
							sprintf(lcdStr, "Usuario %u", currentM->userNo);
							display_Print(TEMPORARY_PRINTING, lcdStr, "Borrado");
						}
						else
							display_Print(TEMPORARY_PRINTING, 0, "Error en borrado");
					    currentM = &claveUsuario1;
						display_Print(CLEAR_PRINT, currentM->row1, currentM->row2);

						borrarUsuario.state = 0;
						menu.privilege = USER;
						MEN_Last(&currentM);
					}
				}
				reset_values();
				break;
			case CANCELAR:
				MEN_Cancelar(&currentM);
				reset_values();
				menu.privilege = USER;
				display_Print(CLEAR_PRINT, currentM->row1, currentM->row2);

				break;

			case MAESTRA:
				MEN_Maestra(&currentM);
				reset_values();
				menu.privilege = USER;
				display_Print(CLEAR_PRINT, currentM->row1, currentM->row2);
				break;
		}
	};
}

static void t_Display (void *pvParameters){
	display_t display;
	uint8_t lastRow1[16], lastRow2[16];
	for(;;){
		if(strcmp(display.linea1,"\0") != 0) strcpy(lastRow1, display.linea1);
		if(strcmp(display.linea2,"\0") != 0) strcpy(lastRow2, display.linea2);

		xQueueReceive(display_q, &display, blockForever);
		switch(display.accion){
			case PRINT:
				if(display.linea1 != 0){
					lcd_msgPrint(0, 0, display.linea1);
				}
				if(display.linea2 != 0){
					lcd_msgPrint(0, 1, display.linea2);
				}
				break;
			case TEMPORARY_PRINTING:
				lcd_clear();
				if(display.linea1 != 0){
					lcd_msgPrint(0, 0, display.linea1);
				}
				if(display.linea2 != 0){
					lcd_msgPrint(0, 1, display.linea2);
				}
				vTaskDelay(sleep_2s);
				lcd_clear();
				lcd_msgPrint(0, 0, lastRow1);
				lcd_msgPrint(0, 1, lastRow2);
				break;
			case TEMPORARY_PRINTING_10s:
				lcd_clear();
				if(display.linea1 != 0){
					lcd_msgPrint(0, 0, display.linea1);
				}
				if(display.linea2 != 0){
					lcd_msgPrint(0, 1, display.linea2);
				}
				vTaskDelay(sleep_10s);
				lcd_clear();
				lcd_msgPrint(0, 0, lastRow1);
				lcd_msgPrint(0, 1, lastRow2);
				break;
			case CLEAR:
				lcd_clear();
				break;
			case CLEAR_1:
				lcd_msgPrint(0, 0, "                ");
				break;
			case CLEAR_2:
				lcd_msgPrint(0, 1, "                ");
				break;
			case CLEAR_PRINT:
				lcd_clear();
				if(display.linea1 != 0){
					lcd_msgPrint(0, 0, display.linea1);
				}
				if(display.linea2 != 0){
					lcd_msgPrint(0, 1, display.linea2);
				}
				break;
			default:
				lcd_clear();
				lcd_msgPrint(0, 1, "ERROR 1");
				vTaskDelay(sleep_1s);
				lcd_clear();
				break;
		}
	}
}

static void t_Flash (void *pvParameters){
	/*----------------------------------------------------------
	 * Estructura de momoria:
	 * en la memoria, los codigos y usuarios se almacenan como numeros en apalabras de 32 bits.
	 * Se intercala el numero de usuario con su codigo correspondiente, los indices pares (incluyendo 0) corresponden al numero de ussario
	 * y el siguiente indice impar es su codigo de acceso
	 *
	 *
	 * 0-user0 (master)
	 * 1-pass maestro
	 * 2-user1
	 * 3-pass1
	 * 4-user2
	 * 5-pass2
	 * etc...
	 *
	 * la cantidad de ususarios estara definida por el tamaño del buffer de la flash, definido por la constante FLASH_BUFFER_SIZE.
	 * la mitad de FLASH_BUFFER_SIZE sera el numero maximo de usuarios permitidos. FLASH_BUFFER_SIZE debe ser un numero par.
	 * el maximo teorico de usuarios pemitidos es 512 (la mitad del tamaño de una pagina de la memoria flash)
	 *
	 */

	uint32_t index = 0, nroUsuario;
	usuario_t value;
	uint32_t buffer[FLASH_BUFFER_SIZE];

	for(;;){
		xQueueReceive(flashcmd_q, &value, portMAX_DELAY);
		switch(value.accion){
		case READ:		//lectura de un valor en particular
			index = (2 * (value.nroUsuario));							//se calcula el indice del usuario solicitado
			Flash_Read_Data(PAGE_ADDR_1, buffer, FLASH_BUFFER_SIZE);	//se realiza la lectura de la memoria

			value.nroUsuario = buffer[index];								//se asigna a la estructura el nro de usuario, si esta vacio se asigna "SIN_ASIGNAR"
			value.clave = buffer[index + 1];								//se asigna a la estructura de usuario la clave correspondiente

			xQueueSend(flashdata_q,&value,portMAX_DELAY);

			break;
		case WRITE:		//escritura de un codigo nuevo en la memoria
			index = (value.nroUsuario)*2;
			nroUsuario = value.nroUsuario;
			Flash_Read_Data(PAGE_ADDR_1, buffer, FLASH_BUFFER_SIZE);

			buffer[index] = nroUsuario;				//se escrive el numero de usuario en el indice par
			buffer[index + 1] = value.clave;					//se escrive el codigo en el indice impar

			Flash_Write_Data(PAGE_ADDR_1, buffer, FLASH_BUFFER_SIZE);
			break;
		default:
			break;
		}
	}
}

static void t_ControlAcceso (void *pvParameters){
	usuario_t usuario[CANT_MAX_USUARIOS], usuarioAux;
	nroUsuario_t nroUsuario;
	actuador_t actuador;

	for(uint32_t i = 0; i < CANT_MAX_USUARIOS; i++){
		usuario[i].intentosDisp = CANT_MAX_INTENTOS;
	}

	for(;;){
		if(xQueueReceive(reqAcceso_q, &usuarioAux, blockForever) == pdPASS){

			nroUsuario = usuarioAux.nroUsuario;
			usuario[nroUsuario].nroUsuario = usuarioAux.nroUsuario;
			usuario[nroUsuario].clave = usuarioAux.clave;
			usuario[nroUsuario].accion = usuarioAux.accion;

			switch(usuario[nroUsuario].accion){
				case READ:
					if(nroUsuario == MAESTRO){
						usuario[nroUsuario].accion = READ;
						xQueueSend(flashcmd_q, &usuario[nroUsuario], blockForever);
						xQueueReceive(flashdata_q, &usuarioAux, blockForever);
						if(usuarioAux.clave == usuario[nroUsuario].clave){
							usuario[nroUsuario].accion = ACC_OK;

							actuador.tipo = PUERTA;
							actuador.accion = ACC_MASTER;

							for(uint32_t i = 0; i < CANT_MAX_USUARIOS; i++){
								usuario[i].intentosDisp = CANT_MAX_INTENTOS;
							}
						}
						else{
							usuario[nroUsuario].accion = ACC_ERROR;

						}

						xQueueSend(actuador_q, &actuador, blockForever);
						xQueueSendToBack(respAcceso_q, &usuario[nroUsuario], blockForever);
					}
					else if(nroUsuario != MAESTRO && nroUsuario != SIN_ASIGNAR){

						if(usuario[nroUsuario].intentosDisp != SIN_INTENTOS_DISP){
							usuario[nroUsuario].accion = READ;
							xQueueSend(flashcmd_q, &usuario[nroUsuario], blockForever);
							xQueueReceive(flashdata_q, &usuarioAux, blockForever);

							if(usuarioAux.clave == usuario[nroUsuario].clave){
								usuario[nroUsuario].accion = ACC_OK;

								actuador.tipo = PUERTA;
								actuador.accion = ACC_OK;
								usuario[nroUsuario].intentosDisp = CANT_MAX_INTENTOS;
								xQueueSend(actuador_q, &actuador, blockForever);
							}
							else{
								usuario[nroUsuario].accion = ACC_DENEGADO;
								usuario[nroUsuario].intentosDisp--;
								if(usuario[nroUsuario].intentosDisp == SIN_INTENTOS_DISP){
									actuador.tipo = PUERTA;
									actuador.accion = ACC_DENEGADO;
									xQueueSend(actuador_q, &actuador, nonBlocking);
								}
							}
							xQueueSendToBack(respAcceso_q, &usuario[nroUsuario], blockForever);
						}

						else{
							usuario[nroUsuario].accion = ACC_DENEGADO;
							actuador.tipo = PUERTA;
							actuador.accion = ACC_DENEGADO;
							xQueueSend(actuador_q, &actuador, nonBlocking);
							xQueueSendToBack(respAcceso_q, &usuario[nroUsuario], blockForever);
						}
					}
					else{

					}
					break;
				case WRITE:
					xQueueSend(flashcmd_q, &usuario[nroUsuario], blockForever);
					break;
				case SCANN:
					usuario[nroUsuario].accion = READ;
					xQueueSend(flashcmd_q, &usuario[nroUsuario], blockForever);
					xQueueReceive(flashdata_q, &usuarioAux, blockForever);
					if(usuarioAux.clave == 0)
						usuario[nroUsuario].accion = ACC_SIN_ASIGNAR;
					else
						usuario[nroUsuario].accion = ACC_ASIGNADO;
					xQueueSendToBack(respAcceso_q, &usuario[nroUsuario], blockForever);
					break;
			}
		}

	}
}

static void t_Actuadores (void *pvParameters){
	actuador_t actuador;
	accion_t acceso = ACC_DENEGADO;
	for(;;){
		xQueueReceive(actuador_q, &actuador, blockForever);
		if(actuador.tipo == PUERTA){
			switch (actuador.accion){
				case ACC_OK:
					acceso = ACC_OK;
					hal_digitalOutput(LOW, ledVerde);
					hal_digitalOutput(HIGH, acPuerta);
					vTaskDelay(sleep_10s);
					if(hal_digitalInput(PA) == LOW){
						hal_digitalOutput(LOW, ledRojo);
						hal_digitalOutput(HIGH, acAlarma);
					}
					else{
						hal_digitalOutput(HIGH, ledVerde);
						hal_digitalOutput(LOW, acPuerta);
					}
					acceso = ACC_DENEGADO;
					break;
				case ACC_DENEGADO:
					acceso = ACC_DENEGADO;
					hal_digitalOutput(LOW, ledRojo);
					hal_digitalOutput(HIGH, acAlarma);
//					vTaskDelay(sleep_10s);
//					hal_digitalOutput(HIGH, ledRojo);
//					hal_digitalOutput(LOW, acAlarma);
					break;
				case ACC_MASTER:
					acceso = ACC_OK;
					hal_digitalOutput(HIGH, ledRojo);
					hal_digitalOutput(LOW, acAlarma);
					hal_digitalOutput(LOW, ledVerde);
					hal_digitalOutput(HIGH, acPuerta);
//					vTaskDelay(sleep_10s);
//					if(hal_digitalInput(PA) == LOW){
//						hal_digitalOutput(LOW, ledRojo);
//						hal_digitalOutput(HIGH, acAlarma);
//					}
//					else{
//						hal_digitalOutput(HIGH, ledVerde);
//						hal_digitalOutput(LOW, acPuerta);
//					}
					break;
				case CAMBIO_ESTADO:
					vTaskDelay(sleep_100ms);
					if(hal_digitalInput(PA) == LOW){
						if(acceso == ACC_DENEGADO){
							hal_digitalOutput(LOW, ledRojo);
							hal_digitalOutput(HIGH, acAlarma);
						}
					}
					else{
						if(acceso == ACC_OK){
							acceso = ACC_DENEGADO;
							hal_digitalOutput(HIGH, ledRojo);
							hal_digitalOutput(LOW, acAlarma);
							hal_digitalOutput(HIGH, ledVerde);

						}
					}
					break;
			}
		}
	}
}

static void t_Teclado (void *pvParameters){
	columna_t columna;
	fila_t fila;
	tecla_t tecla;
	uint8_t teclado[4][4] = {{1,4,7,MENU},{2,5,8,0},{3,6,9,ACEPTAR},{MAESTRA,ARRIBA,ABAJO,CANCELAR}};

	hal_digitalOutput(LOW, fil1);
	hal_digitalOutput(LOW, fil2);
	hal_digitalOutput(LOW, fil3);
	hal_digitalOutput(LOW, fil4);

	for(;;){
		xQueueReceive(columna_q, &columna, blockForever);

		hal_digitalOutput(HIGH, fil1);
		hal_digitalOutput(HIGH, fil2);
		hal_digitalOutput(HIGH, fil3);
		hal_digitalOutput(HIGH, fil4);

		hal_digitalOutput(LOW, fil1);
		if(xQueueReceive(columna_q, &columna, nonBlocking) == pdPASS){
			fila = FILA_1;
		}
		hal_digitalOutput(HIGH, fil1);

		hal_digitalOutput(LOW, fil2);
		if(xQueueReceive(columna_q, &columna, nonBlocking) == pdPASS){
			fila = FILA_2;
		}
		hal_digitalOutput(HIGH, fil2);

		hal_digitalOutput(LOW, fil3);
		if(xQueueReceive(columna_q, &columna, nonBlocking) == pdPASS){
			fila = FILA_3;
		}
		hal_digitalOutput(HIGH, fil3);

		hal_digitalOutput(LOW, fil4);
		if(xQueueReceive(columna_q, &columna, nonBlocking) == pdPASS){
			fila = FILA_4;
		}
		hal_digitalOutput(HIGH, fil4);

		tecla.tecla = teclado[columna][fila];
		if(tecla.tecla < 10) tecla.accion = NUM;
		else tecla.accion = tecla.tecla;

		xQueueSendToBack(tecla_q, &tecla, blockForever);

		vTaskDelay(sleep_500ms);
		xQueueReset(columna_q);
		hal_digitalOutput(LOW, fil1);
		hal_digitalOutput(LOW, fil2);
		hal_digitalOutput(LOW, fil3);
		hal_digitalOutput(LOW, fil4);

	}
}

//Debugging function
static void err (uint8_t errCode){
	while(1);
}

//ISR
void  HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	columna_t columna;
	actuador_t actuador;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if(GPIO_Pin != PA_Pin){
		switch(GPIO_Pin){
			case col1_Pin:
				columna = COLUMNA_1;
				break;
			case col2_Pin:
				columna = COLUMNA_2;
				break;
			case col3_Pin:
				columna = COLUMNA_3;
				break;
			case col4_Pin:
				columna = COLUMNA_4;
				break;
		}
		xQueueSendFromISR(columna_q, &columna, &xHigherPriorityTaskWoken);
	}
	else{
		actuador.tipo = PUERTA;
		actuador.accion = CAMBIO_ESTADO;
		xQueueSendFromISR(actuador_q, &actuador, &xHigherPriorityTaskWoken);
	}
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	columna_t columna;
	tecla_t tecla;
	display_t display;
	actuador_t actuador;
	usuario_t usuario;
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
  MX_I2C1_Init();
//  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  hal_digitalOutput(LOW, acAlarma);
  hal_digitalOutput(LOW, acPuerta);
  hal_digitalOutput(HIGH, ledRojo);
  hal_digitalOutput(HIGH, ledVerde);
  //dummyDataMemoryRecording();

  hal_digitalOutput(HIGH,builtinLed);

  columna_q = xQueueCreate(1, sizeof(columna));
  tecla_q = xQueueCreate(1, sizeof(tecla));
  display_q = xQueueCreate(2,sizeof(display));
  actuador_q = xQueueCreate(1, sizeof(actuador));
  reqAcceso_q = xQueueCreate(1, sizeof(usuario));
  respAcceso_q = xQueueCreate(1, sizeof(usuario));
  flashcmd_q = xQueueCreate(1,sizeof(usuario));
  flashdata_q = xQueueCreate(1,sizeof(usuario));


  if(xTaskCreate(t_Menu, "", stackSize, NULL, tskIDLE_PRIORITY + prioridad_tMenu, &tMenu_h) != pdPASS)
	  err(0);
  if(xTaskCreate(t_Teclado, "", stackSize, NULL, tskIDLE_PRIORITY + prioridad_tTeclado, &tTeclado_h) != pdPASS)
	  err(1);
  if(xTaskCreate(t_Display, "", stackSize, NULL, tskIDLE_PRIORITY + prioridad_tDisplay, &tDisplay_h) != pdPASS)
	  err(2);
  if(xTaskCreate(t_Flash, "", stackSize, NULL, tskIDLE_PRIORITY + prioridad_tFlash, &tFlash_h) != pdPASS)
	  err(3);
  if(xTaskCreate(t_ControlAcceso, "", stackSize, NULL, tskIDLE_PRIORITY + prioridad_tControlAcceso, &tControlAcceso_h) != pdPASS)
	  err(4);
  if(xTaskCreate(t_Actuadores, "", 1024, NULL, tskIDLE_PRIORITY + prioridad_tActuadores, &tActuadores_h) != pdPASS)
	  err(5);

  vTaskStartScheduler();

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 10000;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(builtinLed_GPIO_Port, builtinLed_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, acPuerta_Pin|acAlarma_Pin|ledRojo_Pin|ledVerde_Pin
                          |fil2_Pin|fil1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, fil4_Pin|fil3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : builtinLed_Pin */
  GPIO_InitStruct.Pin = builtinLed_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(builtinLed_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PA_Pin */
  GPIO_InitStruct.Pin = PA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(PA_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : acPuerta_Pin acAlarma_Pin ledRojo_Pin ledVerde_Pin
                           fil2_Pin fil1_Pin */
  GPIO_InitStruct.Pin = acPuerta_Pin|acAlarma_Pin|ledRojo_Pin|ledVerde_Pin
                          |fil2_Pin|fil1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : col4_Pin col3_Pin col2_Pin col1_Pin */
  GPIO_InitStruct.Pin = col4_Pin|col3_Pin|col2_Pin|col1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : fil4_Pin fil3_Pin */
  GPIO_InitStruct.Pin = fil4_Pin|fil3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI1_IRQn, 8, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 8, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 8, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */
//	actuador_t acc2;
  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
//  if (htim->Instance == TIM2) {
//	  HAL_TIM_Base_Stop(&htim2);
//	  acc2.tipo = CIERRE; 											/* accion tipo 0:accion de usuario, accion tipo 1: alarma, accion tipo 2: accion de apagado*/
//	  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
//	  xQueueSendFromISR(actuador_q, &acc2, &xHigherPriorityTaskWoken);
//	  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
//  }

  /* USER CODE END Callback 1 */
}

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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
