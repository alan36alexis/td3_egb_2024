typedef enum{
	USER,
	MASTER
}privilege_t;

typedef struct level{
	uint32_t userNo;
	char row1[16];
	char row2[16];
	struct level *next;
	struct level *prev;
	struct level *maestra;
	struct level *cancelar;
	struct level *aceptar;
	struct level *menu;
	struct level *last;
	void (*func_Cancelar)(void);
	void (*func_Aceptar)(void);
	privilege_t privilege;
	uint32_t state;
}level_t;

void MEN_BuildMenu(level_t *currentNode,uint32_t userNo, char row1[16], char row2[16], void (*func_Cancelar)(void), void (*func_Aceptar)(void) ,level_t *prevNode, level_t *nextNode,level_t *maestraNode, level_t *cancelarNode, level_t *aceptarNode, level_t *menuNode, level_t *lastNode)
{
	currentNode->userNo = userNo;
    strcpy(currentNode->row1, row1);
    strcpy(currentNode->row2, row2);
    currentNode->prev = prevNode;
    currentNode->next = nextNode;
    currentNode->maestra = maestraNode;
    currentNode->cancelar = cancelarNode;
    currentNode->aceptar = aceptarNode;
    currentNode->func_Cancelar = func_Cancelar;
    currentNode->func_Aceptar = func_Aceptar;
    currentNode->menu = menuNode;
    currentNode->last = lastNode;
    currentNode->privilege = USER;
}

void MEN_Next(level_t **currentNode)
{
  if( (*currentNode) ->next != NULL){
	  level_t *auxLastNode = *currentNode;
	  (*currentNode) = (*currentNode)->next;
	  (*currentNode)->last = auxLastNode;
  }
}

void MEN_Prev(level_t **currentNode)
{
  if((*currentNode)->prev != NULL){
	  level_t *auxLastNode = *currentNode;
	  (*currentNode) = (*currentNode)->prev;
	  (*currentNode)->last = auxLastNode;
  }
}
void MEN_Maestra(level_t **currentNode)
{
  if((*currentNode)->maestra != NULL){
	  level_t *auxLastNode = *currentNode;
	  (*currentNode) = (*currentNode)->maestra;
	  (*currentNode)->last = auxLastNode;
  }
}

void MEN_Cancelar(level_t **currentNode)
{
  if((*currentNode)->func_Cancelar != NULL)
	  (*currentNode)->func_Cancelar();
  else if((*currentNode)->cancelar != NULL){
	  level_t *auxLastNode = *currentNode;
	  (*currentNode) = (*currentNode)->cancelar;
	  (*currentNode)->last = auxLastNode;
  }
}
void MEN_Aceptar(level_t **currentNode)
{
  if((*currentNode)->func_Aceptar != NULL)
	  //(*currentNode)->func_Aceptar();
  	  execute_Function((*currentNode)->func_Aceptar);
  else if((*currentNode)->aceptar != NULL){
	  level_t *auxLastNode = *currentNode;
	  (*currentNode) = (*currentNode)->aceptar;
	  (*currentNode)->last = auxLastNode;
  }
}
void MEN_Menu(level_t **currentNode)
{
  if((*currentNode)->menu != NULL){
	  level_t *auxLastNode = *currentNode;
	  (*currentNode) = (*currentNode)->menu;
	  (*currentNode)->last = auxLastNode;
  }
}

void MEN_Last(level_t **currentNode){
  level_t *auxLastNode = *currentNode;
  (*currentNode)->last = auxLastNode;
}

//void MEN_State_ON (level_t **currentNode){
//	(*currentNode)->state = ON;
//}

void MEN_DeleteMenu(level_t **currentNode){
	if((*currentNode)->next != NULL && (*currentNode)->prev != NULL){

	}
}
