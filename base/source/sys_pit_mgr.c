#include "sys_pit_mgr.h"



#ifdef __cplusplus
extern "C" {
#endif

	uint32_t pitmgr_timer_ms;
	listl_t pitmgr_isrSet;

	status_t PITMGR_Init(void)
	{
		List_Constructor(&pitmgr_isrSet, sizeof(pitmgr_handle_t));
		pitmgr_timer_ms = 0;

		pit_config_t cfg;
		{
			cfg.enableRunInDebug = true;
		}
		PIT_Init(PIT, &cfg);
		PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, ULONG_MAX);
		PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, ULONG_MAX);
		PIT_SetTimerChainMode(PIT, kPIT_Chnl_1, true);
		//PIT_SetTimerPeriod(PIT, kPIT_Chnl_2, USEC_TO_COUNT(1000, RTE_PIT_CLKFREQ));
		//PIT_EnableInterrupts(PIT, kPIT_Chnl_2, kPIT_TimerInterruptEnable);
		//NVIC_SetPriority(PIT2_IRQn, 8);
		//EnableIRQ(PIT2_IRQn);
		PIT_StartTimer(PIT, kPIT_Chnl_0);
		PIT_StartTimer(PIT, kPIT_Chnl_1);
		//PIT_StartTimer(PIT, kPIT_Chnl_2);
		return kStatus_Success;
	}

	inline uint64_t PITMGR_GetLTC(void)
	{
		return ULLONG_MAX - PIT_GetLifetimeTimerCount(PIT);
	}

	inline uint64_t PITMGR_GetLTC_us(void)
	{
		return COUNT_TO_USEC(PITMGR_GetLTC(), RTE_PIT_CLKFREQ);
	}

	inline uint64_t PITMGR_GetLTC_ms(void)
	{
		return COUNT_TO_MSEC(PITMGR_GetLTC(), RTE_PIT_CLKFREQ);
	}

	void PITMGR_Delay(uint64_t _t)
	{
		uint64_t cnt = PITMGR_GetLTC() + _t;
		while (PITMGR_GetLTC() < cnt);
	}

	void PITMGR_Delay_us(uint64_t _t)
	{
		uint64_t cnt = PITMGR_GetLTC() + USEC_TO_COUNT(_t, RTE_PIT_CLKFREQ);
		while (PITMGR_GetLTC() < cnt);
	}

	void PITMGR_Delay_ms(uint64_t _t)
	{
		uint64_t cnt = PITMGR_GetLTC() + MSEC_TO_COUNT(_t, RTE_PIT_CLKFREQ);
		while (PITMGR_GetLTC() < cnt);
	}

	void PITMGR_HandleSetup(pitmgr_handle_t* _h, uint32_t _ms, uint32_t _mso, pitmgr_handler_t _handler, uint32_t _ppt)
	{
		assert(_h);
		assert(_ms);
		assert(_handler);
		_h->ms = _ms;
		_h->mso = _mso;
		_h->handler = _handler;
		_h->pptFlag = _ppt;
	}

	pitmgr_handle_t* PITMGR_HandleInsert(uint32_t _ms, uint32_t _mso, pitmgr_handler_t _handler, uint32_t _ppt)
	{
		pitmgr_handle_t* ret = List_Create(&pitmgr_isrSet, List_ItEnd(&pitmgr_isrSet))->data;
		PITMGR_HandleSetup(ret, _ms, _mso, _handler, _ppt);
		return ret;
	}

	void PITMGR_HandleSetEnable(pitmgr_handle_t* _h, bool b)
	{
		if (b)
		{
			_h->pptFlag |= pitmgr_pptEnable;
		}
		else
		{
			_h->pptFlag &= ~pitmgr_pptEnable;
		}
	}

	status_t PITMGR_HandleRemove(pitmgr_handle_t* _h)
	{
		for (list_node_t* it = List_ItBegin(&pitmgr_isrSet); it != List_ItEnd(&pitmgr_isrSet); it = it->next)
		{
			if (it->data == _h)
			{
				List_Erase(&pitmgr_isrSet, it);
				return kStatus_Success;
			}
		}
		return kStatus_Fail;
	}

	void PITMGR_Isr(void)
	{
		uint64_t initialTime = PITMGR_GetLTC_us();

		for (list_node_t* it = List_ItBegin(&pitmgr_isrSet); it != List_ItEnd(&pitmgr_isrSet); it = it->next)
		{
			pitmgr_handle_t* handle = (pitmgr_handle_t*)(it->data);
			if (handle->pptFlag & pitmgr_pptEnable && pitmgr_timer_ms % handle->ms == handle->mso)
			{
				uint64_t timeBuf = PITMGR_GetLTC_us();
				(*(handle->handler))();
				if (handle->pptFlag & pitmgr_pptRunOnce)
				{
					handle->pptFlag ^= pitmgr_pptEnable;
				}
				handle->prevTime_us = PITMGR_GetLTC_us() - timeBuf;
			}
		}
		++pitmgr_timer_ms;
	}


	//void PIT2_IRQHandler(void)
	//{
	//	PIT_ClearStatusFlags(PIT, kPIT_Chnl_2, kPIT_TimerFlag);
	//	PITMGR_Isr();
	//}




#ifdef __cplusplus
}
#endif