/* -*- mode: C; c-basic-offset: 4; indent-tabs-mode: nil -*- */

#ifndef __CONFIG_H__
#define __CONFIG_H__

#ident "$Id$"
#ident "Copyright (c) 2007-2012 Tokutek Inc.  All rights reserved."
#ident "The technology is licensed by the Massachusetts Institute of Technology, Rutgers State University of New Jersey, and the Research Foundation of State University of New York at Stony Brook under United States of America Serial No. 11/760379 and to the patents and/or patent applications resulting from it."

#define TOKUDB_REVISION 0

/* #undef TOKU_DEBUG_PARANOID */
/* #undef USE_VALGRIND */

#define HAVE_ALLOCA_H 1
#define HAVE_ARPA_INET_H 1
/* #undef HAVE_BYTESWAP_H */
/* #undef HAVE_ENDIAN_H */
#define HAVE_FCNTL_H 1
#define HAVE_INTTYPES_H 1
#define HAVE_LIBKERN_OSATOMIC_H 1
#define HAVE_LIBKERN_OSBYTEORDER_H 1
#define HAVE_LIMITS_H 1
#define HAVE_MACHINE_ENDIAN_H 1
/* #undef HAVE_MALLOC_H */
#define HAVE_MALLOC_MALLOC_H 1
/* #undef HAVE_MALLOC_NP_H */
#define HAVE_PTHREAD_H 1
/* #undef HAVE_PTHREAD_NP_H */
#define HAVE_STDINT_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRING_H 1
/* #undef HAVE_SYSCALL_H */
/* #undef HAVE_SYS_ENDIAN_H */
#define HAVE_SYS_FILE_H 1
#define HAVE_SYS_MALLOC_H 1
#define HAVE_SYS_RESOURCE_H 1
#define HAVE_SYS_STATVFS_H 1
#define HAVE_SYS_SYSCALL_H 1
#define HAVE_SYS_SYSCTL_H 1
#define HAVE_SYS_SYSLIMITS_H 1
#define HAVE_SYS_TIME_H 1
#define HAVE_UNISTD_H 1

/* #undef HAVE_M_MMAP_THRESHOLD */
/* #undef HAVE_CLOCK_REALTIME */
/* #undef HAVE_O_DIRECT */
#define HAVE_F_NOCACHE 1

#define HAVE_MALLOC_SIZE 1
/* #undef HAVE_MALLOC_USABLE_SIZE */
/* #undef HAVE_MEMALIGN */
#define HAVE_VALLOC 1
#define HAVE_NRAND48 1
/* #undef HAVE_RANDOM_R */

/* #undef HAVE_PTHREAD_RWLOCKATTR_SETKIND_NP */
/* #undef HAVE_PTHREAD_YIELD */
#define HAVE_PTHREAD_YIELD_NP 1
/* #undef HAVE_PTHREAD_GETTHREADID_NP */

/* #undef PTHREAD_YIELD_RETURNS_INT */
/* #undef PTHREAD_YIELD_RETURNS_VOID */

#define HAVE_GNU_TLS 1

#endif /* __CONFIG_H__ */
