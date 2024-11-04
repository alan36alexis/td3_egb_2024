# td3_ega_2024

## Notas de proyecto

[Link a Miro](https://miro.com/app/board/uXjVKB7eTkc=/)

### Requerimientos de sistema

**A nivel usuario:**
* claves de 4 dígitos
* 3 intentos disponibles para ingreso de contraseña
* reseteo de intentos disponibles luego de 1 minuto

**A nivel maestro:**
* clave de 8 digitos
* posibilidad de agregar usuarios con contraseñas nuevas
* posibilidad de borrar usuarios
* la clave maestra desactiva alarma si es que suena y abre la puerta
* posibilidad de exportar registros de acciones

**El sistema debe contar con:**
* teclado 0-9, enter, cancelar, izq, der
* display 16x2
* actuadores de apertura de puerta y de alarma
* sensor de estado de puerta
* registrar todas las acciones:
  * ingreso de clave usuario y qué usuario ingreso
  * apertura de puerta
  * ingreso de clave maestra
  * ingreso de clave erronea (intento 1, intento 2, intento 3)
  * alarma activada por clave erroneas
  * alarma activada por timeout de puerta
  * encendido de dispositivo

## Interface física

![image](https://github.com/alan36alexis/td3_ega_2024/assets/29176007/15d845d2-9e1f-448f-8a7c-d77ef638429c)

## Manual de funcionamiento

![image](https://github.com/alan36alexis/td3_ega_2024/assets/29176007/6a8c66a0-167f-4268-a11e-6a378ad10bb6)

## Diagrama de tareas opción 1

![image](https://github.com/alan36alexis/td3_ega_2024/assets/29176007/683217fa-f4d1-45a9-95da-0d2573a88ac2)

## Diagrama de tareas opción 2

![image](https://github.com/alan36alexis/td3_ega_2024/assets/29176007/62ca65b9-3caf-4c6d-920e-91cd6dcc285b)
