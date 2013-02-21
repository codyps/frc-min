/* This File: LOLOLOL
 *
 * Issues
 * - uses "INTERNAL" argument from symEach
 * - ordering of ctor/dtor calls is wrong.
 */

#include "vxworks-util.h"
#include <stdint.h>
#include <string.h>
#include <sysSymTbl.h>

extern void (*_ctors[])(void);
extern void (*_dtors[])(void);

/* takes advantage of SYMBOL * passed as last argument. Marked "INTERNAL". */
static BOOL call_ctor_syms_in_module(char *name, int val, SYM_TYPE type,
		int arg, uint16_t group, SYMBOL *symbol)
{
	MODULE *mod = (MODULE *)arg;
	if (mod != symbol_to_module(symbol))
		return TRUE;

	/* check name */
	if (strncmp(name, "_GLOBAL__I", sizeof("_GLOBAL__I")))
		return TRUE;

	((FUNCPTR)val)();

	return TRUE;
}

static BOOL call_dtor_syms_in_module(char *name, int val, SYM_TYPE type,
		int arg, uint16_t group, SYMBOL *symbol)
{
	MODULE *mod = (MODULE *)arg;
	if (mod != symbol_to_module(symbol))
		return TRUE;

	/* check name */
	if (strncmp(name, "_GLOBAL__D", sizeof("_GLOBAL__D")))
		return TRUE;

	((FUNCPTR)val)();

	return TRUE;
}

static void call_ctors(void)
{
	MODULE *us = module_find_by_addr(&_ctors);
	symEach(sysSymTbl, (FUNCPTR)call_ctor_syms_in_module, (uintptr_t)us);
}

static void call_dtors(void)
{
	MODULE *us = module_find_by_addr(&_dtors);
	symEach(sysSymTbl, (FUNCPTR)call_dtor_syms_in_module, (uintptr_t)us);
}

void (*_ctors[])(void) = {
	call_ctors,
};

void (*_dtors[])(void) = {
	call_dtors,
};
