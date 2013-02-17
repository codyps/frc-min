#include <stdint.h>

#include <taskLibCommon.h> /* taskNameToId() */
#include <moduleLib.h> /* moduleFindByName(), MODULE_ID */
#include <unldLib.h> /* unldByModuleId() */
#include <limits.h> /* NAME_MAX */

#include "vxworks-util.h"

#define INIT_FUNC FRC_UserProgram_StartupLibraryInit
#define TASK_NAME "FRC_RobotTask"

#define _STR(x) #x
#define STR(x) _STR(x)

static void unload_old_module(void)
{
	/* FIXME: This code currently does what WPILib does, but it isn't clear
	 *        why we should check that the task is running at all */
	int32_t old_task_id = taskNameToId((char *)TASK_NAME);
	if (old_task_id == ERROR)
		return;

	/* XXX: how do we avoid finding ourselves and ensure we find the old
	 * module? */
	MODULE_ID old_module_id = module_find_by_symbolname(STR(INIT_FUNC));
	if (!old_module_id)
		return;

	unldByModuleId(old_module_id, 0);
}

extern int32_t INIT_FUNC(void);

/* Called by the code that loads this module (FRC_UserProgram.out) */
int32_t INIT_FUNC(void)
{
	unload_old_module();

	/* TODO: establish net comms. */

	/* TODO: spawn some task(s) */
#if 0
	/* XXX: note that WPILib documentation indicate a new task is spawned
	 * to ensure the task running user code has the VX_FP_TASK option set.
	 */
	int32_t task_id = taskSpawn(TASK_NAME,
				    101 /* default priority */,
				    VX_FP_TASK, /* options */
				    64000, /* stack size */
				    my_func, /* function to start */
				    0,0,0,0,0,0,0,0,0,0 /* 10 uint32_t arguments */
				    /* In C++: arg0 = FRC_userClassFactory,
				     * arg1 = (Task *) */
				   );
#endif
	return 0;
}
