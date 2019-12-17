
#include <stdint.h>
#include <rthw.h>
#include <rtthread.h>
#include "cy_device_headers.h"
#include "board.h"
#include "uart.h"
#include "cy_systick.h"
#include "cycfg.h"
#include "cybsp.h"
#include "rtconfig.h"

/**
 * This is the timer interrupt service routine.
 *
 */
void SysTick_Handler_CB(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();
}


void rt_hw_board_init()
{
	cy_rslt_t result;

	/* Initialize the device and board peripherals */
	result = cybsp_init();
	/* Board init failed. Stop program execution */
	if (result != CY_RSLT_SUCCESS)
	{
		CY_ASSERT(0);
	}

    Cy_SysTick_Init(CY_SYSTICK_CLOCK_SOURCE_CLK_CPU, SystemCoreClock/RT_TICK_PER_SECOND);
    Cy_SysTick_SetCallback(0, SysTick_Handler_CB);
    
    /* initialize UART device */
    rt_hw_uart_init();

#ifdef RT_USING_CONSOLE
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif

    rt_system_heap_init((void *)HEAP_BEGIN, (void *)HEAP_END);

#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif
}

#ifdef RT_USING_FINSH
#include <finsh.h>
static void reboot(uint8_t argc, char **argv)
{
    (void)argc;
    (void)argv;
    rt_hw_cpu_reset();
}
FINSH_FUNCTION_EXPORT_ALIAS(reboot, __cmd_reboot, Reboot System);
#endif /* RT_USING_FINSH */

/*@}*/
