#ifndef _FSM_H_
#define _FSM_H_


#include "stdint.h"
#include "stdbool.h"
#include "string.h"


typedef struct FSM_STATE_STRUCT
{
	uint16_t sta_num;		//״̬��
	void (*init_fun)(void);	//��ʼ����������
	void (*run_fun)(void);	//���к�������
}fsmsta_t;

typedef struct FSM_STRUCT
{
	uint16_t sta_num;		//��״̬��
	uint16_t ct_sta;		//��ǰ״̬
	fsmsta_t *sta_tab;
}fsm_t;


void fsm_init(fsm_t *fsm, fsmsta_t *sta_tab, uint16_t sta_num);
void fsm_switch(fsm_t *fsm, uint16_t new_sta);
void fsm_loop(fsm_t *fsm);


#endif
