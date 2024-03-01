/* This file is the counterpart of "read.c".  It contains the code for writing
 * insofar as this is not contained in read_write().
 *
 * The entry points into this file are
 *   do_write:     call read_write to perform the WRITE system call
 *   write_map:    add a new zone to an inode
 *   clear_zone:   erase a zone in the middle of a file
 *   new_block:    acquire a new block
 
                                                    Toby Du, 2015/3/2
 ---------------------------------------------------------------------*/

#include "../H/const.h"
#include "../H/type.h"
#include "../H/error.h"
#include "const.h"
#include "type.h"
#include "buf.h"
#include "file.h"
#include "fproc.h"
#include "glo.h"
#include "inode.h"
#include "super.h"
