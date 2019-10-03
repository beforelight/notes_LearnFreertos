#ifndef SYS_PITMGR_H
#define SYS_PITMGR_H
#include <listl.h>
#include "MK66F18.h"

#include "fsl_pit.h"
#include "stdint.h"
#include "limits.h"

#define RTE_PIT_CLKFREQ CLOCK_GetFreq(kCLOCK_BusClk)

#ifdef __cplusplus
extern "C" {
#endif
	/**
	 * @brief : PIT�������жϷ�������
	 */
	typedef void (*pitmgr_handler_t)(void);

	/**
	 * @brief : PIT�������������ã�����־λö�١�
	 */
	typedef enum pitmgr_pptFlag// : uint32_t
	{
		pitmgr_pptEnable = 1 << 0,
		pitmgr_pptRunOnce = 1 << 1,
	}pitmgr_pptFlag_t;

	/**
	 * @brief : PIT��������������ڴ洢ÿ��PIT�жϵ����ݡ�
	 */
	typedef struct pitmgr_handle
	{
		uint32_t ms, mso;			/* ���ж���ÿms����ĵ�_mso�������С� */
		pitmgr_handler_t handler;	/* ָ���жϷ�������ָ�롣 */
		uint32_t pptFlag;			/* ���Ա�־λ����pitmgr_pptFlag_t�İ�λ��ϡ� */
		uint64_t prevTime_us;		/* ���ж��ϴ����к�ʱ�� */
	}pitmgr_handle_t;

	/**
	 * @brief : �жϷ������ļ�ʱ��������λΪms
	 */
	extern uint32_t pitmgr_timer_ms;

	/**
	 * PIT�жϱ��Ե�������洢��
	 */
	extern listl_t pitmgr_isrSet;

	/**
	 * @brief : PIT��������ʼ����
	 *	��ʼ��������PIT���衢PIT�жϡ�
	 *
	 * @return {status_t} : �ɹ�����kStatus_Success���쳣����kStatus_Fail��
	 */
	status_t PITMGR_Init(void);

	/**
	 * @brief : ��ȡLifeTimeCounter��ǰ��ʱ����
	 *
	 *	PIT�Ǽ����������������õ���ֵ�Ѿ���ת����Ϊ����������
	 *
	 * @return {uint64_t} : ����LTC��������������ʱ����
	 */
	uint64_t PITMGR_GetLTC(void);

	/**
	 * @brief : ��ȡLifeTimeCounter��ǰ��ʱ΢������
	 *
	 *	PIT�Ǽ����������������õ���ֵ�Ѿ���ת����Ϊ����������
	 *
	 * @return {uint64_t} : ����LTC��������������΢����
	 */
	uint64_t PITMGR_GetLTC_us(void);

	/**
	 * @brief : ��ȡLifeTimeCounter��ǰ��ʱ��������
	 *
	 *	PIT�Ǽ����������������õ���ֵ�Ѿ���ת����Ϊ����������
	 *
	 * @return {uint64_t} : ����LTC�������������ĺ�����
	 */
	uint64_t PITMGR_GetLTC_ms(void);

	/**
	 * @brief : ����LifeTimeCounter����ʱ�Ӽ����������ʱ��
	 *
	 * @param : {uint64_t} _t : Ҫ�ӳٵ�ʱ����
	 */
	void PITMGR_Delay(uint64_t _t);

	/**
	 * @brief : ����LifeTimeCounter����΢�뼶���������ʱ��
	 *
	 * @param : {uint64_t} _t : Ҫ�ӳٵ�΢����
	 */
	void PITMGR_Delay_us(uint64_t _t);

	/**
	 * @brief : ����LifeTimeCounter���к��뼶���������ʱ��
	 *
	 * @param : {uint64_t} _t : Ҫ�ӳٵĺ�����
	 */
	void PITMGR_Delay_ms(uint64_t _t);

	/**
	 * @brief : ��ʼ��һ��PIT��ʱ�жϾ����
		�ú����������ݼ�鲢��ֵ����Ҫ���Ᵽ����

	 * @param {pitmgr_handle_t*} _h       : Ҫ���õ�PIT�жϾ��
	 * @param {uint32_t} _ms              : ��PIT�жϵ��жϼ����
	 * @param {uint32_t} _mso             : ��PIT�жϵ��ж���λ�������ֶ����ؾ��⡣
	 * @param {pitmgr_handler_t} _handler : ��PIT�жϵ��жϷ�������
	 * @param {uint32_t} _ppt             : ��PIT�жϵ�����Flag��
	 */
	void PITMGR_HandleSetup(pitmgr_handle_t* _h, uint32_t _ms, uint32_t _mso, pitmgr_handler_t _handler, uint32_t _ppt);

	/**
	 * @brief : ��PIT�жϱ�ĩβ����һ���µ��жϾ����
	 *	�ú����������ݼ�鲢��ֵ����Ҫ���Ᵽ����
	 *
	 * @param {uint32_t} _ms              : ��PIT�жϵ��жϼ����
	 * @param {uint32_t} _mso             : ��PIT�жϵ��ж���λ�������ֶ����ؾ��⡣
	 * @param {pitmgr_handler_t} _handler : ��PIT�жϵ��жϷ�������
	 * @param {uint32_t} _ppt             : ��PIT�жϵ�����Flag��
	 * @return {pitmgr_handle_t*}         : ����ָ���²����PIT�жϾ����ָ�롣�������ʧ���򷵻ؿ�ָ�롣�û������м�顣
	 */
	pitmgr_handle_t* PITMGR_HandleInsert(uint32_t _ms, uint32_t _mso, pitmgr_handler_t _handler, uint32_t _ppt);

	/**
	 * @brief : ����ĳ��PIT�жϾ�������ԡ�
	 *
	 * @param {pitmgr_handle_t*} _h : Ҫ���õ�PIT�жϾ��
	 * @param {bool} b              : �Ƿ����ø��жϾ��
	 */
	void PITMGR_HandleSetEnable(pitmgr_handle_t* _h, bool b);
	status_t PITMGR_HandleRemove(pitmgr_handle_t* _h);


	/**
	 * @brief : PIT�жϵĴ���������IRQHandler���á�
	 */
	void PITMGR_Isr(void);

#ifdef __cplusplus
}
#endif

#endif // ! SYS_PITMGR_H