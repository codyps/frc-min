#include <stdbool.h>
#include <stdint.h>

#include "vxworks-util.h"

#include <sysSymTbl.h> /* sysSymTbl */
#include <moduleLib.h> /* SEGMENT_ID, moduleSegFirst(), moduleSegNext() */

static bool segment_contains_addr(SEGMENT_ID seg, void *addr)
{
	return seg->address <= addr && addr < (seg->address + seg->size);
}

/* XXX: confirm that the termination check is correct. */
#define for_each_segment_in_module(segment_cursor, module)	\
	for (segment_cursor = moduleSegFirst(module);		\
	     segment_cursor;					\
	     segment_cursor = moduleSegNext(segment_cursor))

static bool module_contains_addr(MODULE_ID mod, void *addr)
{
	SEGMENT_ID seg;
	for_each_segment_in_module(seg, mod) {
		if (segment_contains_addr(seg, addr))
			return true;
	}

	return false;
}

static BOOL on_each_module(MODULE_ID mod, int addr)
{
	if (module_contains_addr(mod, (void *)(intptr_t)addr)) {
		return FALSE;
	}
	return TRUE;
}

static MODULE_ID module_find_by_addr(void *addr)
{
	/* XXX: kernelModuleListId _appears_ to be a global that would allow us
	 * to do for_XXX loop over the DL_LIST here. Unfortunately, it is
	 * "private". */
	return moduleEach((FUNCPTR)on_each_module, (int)addr);
}

MODULE_ID module_find_by_symbolname(const char *sym)
{
	char *val;
	SYM_TYPE type;
	STATUS r = symFindByName(sysSymTbl, (char *)sym, &val, &type);
	if (r == ERROR)
		return NULL;

	return module_find_by_addr(val);
}
