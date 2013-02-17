#include <stdbool.h>
#include <stdint.h>

#include "vxworks-util.h"

#include <private/symLibP.h>
#include <private/symbolP.h>
#include <sysSymTbl.h>

/* Based on documentation in vxworks-6.3/target/h/private/symbolP.h */
static MODULE_ID symref_is_module_or_null(intptr_t symref)
{
	switch (symref) {
	case (intptr_t)SYMREF_SHELL:
	case (intptr_t)SYMREF_KERNEL:
	case (intptr_t)SYMREF_WTX:
		return NULL;
	default:
		return (MODULE_ID)symref;
	}
}

/* This uses symFindSymbol(), from vxworks-6.3/target/h/private/symLibP.h
 * - uses a single hash table lookup to find the module. */
MODULE_ID module_find_by_symbolname(const char *sym_name)
{
	SYMBOL_ID sym;
	/* when 'name' is passed, the 'value', 'type', and 'mask' are ignored */
	if (symFindSymbol(sysSymTbl, (char *)sym_name, 0, 0, 0, &sym) == ERROR)
		return NULL;

	return symref_is_module_or_null(sym->symRef);
}
