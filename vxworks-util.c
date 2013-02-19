#include <stdbool.h>
#include <stdint.h>

#include "vxworks-util.h"

#include <moduleLib.h> /* SEGMENT_ID, moduleSegFirst(), moduleSegNext() */

#include <private/symLibP.h>
#include <private/symbolP.h>

#include <sysSymTbl.h>

#include <semLib.h>
#include <hashLib.h>

#define container_of(item, type, member) \
		((type *)((char *)(item) - offsetof(type, member)))

/* Based on documentation in vxworks-6.3/target/h/private/symbolP.h */
static MODULE_ID symref_is_module_or_null(uintptr_t symref)
{
	switch (symref) {
	case (uintptr_t)SYMREF_SHELL:
	case (uintptr_t)SYMREF_KERNEL:
	case (uintptr_t)SYMREF_WTX:
		return NULL;
	default:
		return (MODULE_ID)symref;
	}
}

static bool segment_contains_addr(SEGMENT_ID seg, void *addr)
{
	return seg->address <= addr && addr < (seg->address + seg->size);
}

static bool module_contains_addr(MODULE *mod, void *addr)
{
	SEGMENT_ID seg;
	for_each_segment_in_module(seg, mod) {
		if (segment_contains_addr(seg, addr))
			return true;
	}

	return false;
}

static BOOL module_find_by_addr__each(MODULE_ID mod, int arg)
{
	if (module_contains_addr(mod, (void *)arg))
		return FALSE;
	return TRUE;
}

MODULE *module_find_by_addr(void *addr)
{
	/* XXX: kernelModuleListId _appears_ to be a global that would allow us
	 * to do for_XXX loop over the DL_LIST here. Unfortunately, it is
	 * "private". */
	return moduleEach((FUNCPTR)module_find_by_addr__each, (uintptr_t)addr);
}

/* This does terrible, terrible, things. */
SYMBOL *sym_find_earliest_by_name(const char *sym_name)
{
	SYMBOL sym = { .name = (char *)sym_name };

	semTake(&sysSymTbl->symMutex, WAIT_FOREVER);
	HASH_NODE *node = hashTblFind(sysSymTbl->nameHashId, &sym.nameHNode, 0);

	if (!node) {
		semGive(&sysSymTbl->symMutex);
		return NULL;
	}

	/* Walk to the end of the node */
	while (node->next)
		node = node->next;

	semGive(&sysSymTbl->symMutex);

	return container_of(node, SYMBOL, nameHNode);
}

MODULE *module_find_earliest_by_symbolname(const char *sym_name)
{
	SYMBOL *sym = sym_find_earliest_by_name(sym_name);
	if (!sym)
		return NULL;

	return symref_is_module_or_null(sym->symRef);
}

/* This uses symFindSymbol(), from vxworks-6.3/target/h/private/symLibP.h
 * - uses a single hash table lookup to find the module. */
MODULE *module_find_by_symbolname(const char *sym_name)
{
	SYMBOL *sym;
	/* when 'name' is passed, the 'value', 'type', and 'mask' are ignored */
	if (symFindSymbol(sysSymTbl, (char *)sym_name, 0, 0, 0, &sym) == ERROR)
		return NULL;

	return symref_is_module_or_null(sym->symRef);
}

