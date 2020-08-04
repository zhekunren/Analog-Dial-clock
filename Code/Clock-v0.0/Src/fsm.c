/***********************************
�ļ���fsm.c
���ߣ��ź���
�汾��
���ڣ�2016/11/11
������״̬����ȫ����ָ��ż���
************************************/

#include "fsm.h"


//״̬����ʼ��
void fsm_init(fsm_t *fsm, fsmsta_t *sta_tab, uint16_t sta_num)
{
	memset(fsm, 0, sizeof(*fsm));
	
	fsm->sta_num = sta_num;
	fsm->sta_tab = sta_tab;
}

//״̬��״̬�л�
void fsm_switch(fsm_t *fsm, uint16_t new_sta)
{
	if(new_sta != fsm->ct_sta)
	{
		void (*pfun)(void) = pfun = (fsm->sta_tab + new_sta)->init_fun;
		
		if(pfun != NULL)
		{
			(*pfun)();
		}
		
		fsm->ct_sta = new_sta;
	}
}

//״̬����ѯִ������
void fsm_loop(fsm_t *fsm)
{
	void (*pfun)(void) = (fsm->sta_tab + fsm->ct_sta)->run_fun;
	
	if(pfun != NULL)
	{
		(*pfun)();
	}
}
