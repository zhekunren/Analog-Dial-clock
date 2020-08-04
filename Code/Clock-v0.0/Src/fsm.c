/***********************************
文件：fsm.c
作者：张涵宇
版本：
日期：2016/11/11
描述：状态机，全都是指针偶买噶
************************************/

#include "fsm.h"


//状态机初始化
void fsm_init(fsm_t *fsm, fsmsta_t *sta_tab, uint16_t sta_num)
{
	memset(fsm, 0, sizeof(*fsm));
	
	fsm->sta_num = sta_num;
	fsm->sta_tab = sta_tab;
}

//状态机状态切换
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

//状态机轮询执行任务
void fsm_loop(fsm_t *fsm)
{
	void (*pfun)(void) = (fsm->sta_tab + fsm->ct_sta)->run_fun;
	
	if(pfun != NULL)
	{
		(*pfun)();
	}
}
