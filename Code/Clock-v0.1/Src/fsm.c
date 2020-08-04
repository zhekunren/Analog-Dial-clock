#include "fsm.h"


//×´Ì¬»ú³õÊ¼»¯
void fsm_init(fsm_t *fsm, fsmsta_t *sta_tab, uint16_t sta_num)
{
	memset(fsm, 0, sizeof(*fsm));
	
	fsm->sta_num = sta_num;
	fsm->sta_tab = sta_tab;
}


//×´Ì¬»ú×´Ì¬ÇÐ»»
void fsm_switch(fsm_t *fsm, uint16_t new_sta)
{
	if(new_sta != fsm->ct_sta)
	{
		void (*pfun)(void) = (fsm->sta_tab + new_sta)->init_fun;
		
		if(pfun != NULL)
		{
			(*pfun)();
		}
		
		fsm->ct_sta = new_sta;
	}
}


//×´Ì¬»úÂÖÑ¯Ö´ÐÐÈÎÎñ
void fsm_loop(fsm_t *fsm)
{
	void (*pfun)(void) = (fsm->sta_tab + fsm->ct_sta)->run_fun;
	
	if(pfun != NULL)
	{
		(*pfun)();
	}
}
