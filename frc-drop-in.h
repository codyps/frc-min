#ifndef FRC_DROP_IN_H_
#define FRC_DROP_IN_H_

#include "vxworks-util.h"

/* XXX: moduleNameFIndBySymbolName() is defined in the header
 * WPILib/NetworkCommunication/symModuleLink.h
 *
 * And probably provided in the FRC Networking Library (or something).
 * Basically: we have no source code.
 *
 * Users should use module_find_by_symbolname() instead.
 */
static inline STATUS moduleNameFindBySymbolName(const char *sym, char *module)
{
	MODULE_ID m = module_find_by_symbolname(sym);
	if (!m)
		return ERROR;

	memcpy(module, m->name, sizeof(m->name));
	return 0;
}

#endif
