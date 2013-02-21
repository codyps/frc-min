#ifndef VXWORKS_UTIL_H_
#define VXWORKS_UTIL_H_

#include <moduleLib.h> /* MODULE */
#include <symLib.h>    /* SYMBOL */

MODULE *module_find_by_addr(void *addr);
MODULE *module_find_by_symbolname(const char *sym);

SYMBOL *sym_find_earliest_by_name(const char *sym_name);
MODULE *module_find_earliest_by_symbolname(const char *sym);

MODULE *symbol_to_module(SYMBOL *sym);

#define for_each_segment_in_module(segment_cursor, module)	\
	for (segment_cursor = moduleSegFirst(module);		\
	     segment_cursor;					\
	     segment_cursor = moduleSegNext(segment_cursor))

#endif
