#include "sysdep.h"
#include "bfd.h"
#include "libbfd.h"

const bfd_arch_info_type
bfd_eco32_arch =
{
  32,				/* 32 bits in a word */
  32,				/* 32 bits in an address */
  8,				/* 8 bits in a byte */
  bfd_arch_eco32,		/* architecture */
  bfd_mach_eco32,		/* machine */
  "eco32",			/* architecture name */
  "eco32",			/* printable name */
  4,				/* section align power */
  TRUE,				/* this is the default architecture */
  bfd_default_compatible,	/* architecture comparison function */
  bfd_default_scan,		/* string to architecture conversion */
  bfd_arch_default_fill,	/* fill routine */
  NULL				/* next in list */
};
