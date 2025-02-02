/* $Header: svn://localhost/dtapublic/projs/emts/trunk/src/c_tk_base_7_5_w_mods/tkimgutil.c 71 2016-11-05 11:07:06Z dashley $ */

/* 
 * tkImgUtil.c --
 *
 *	This file contains image related utility functions.
 *
 * Copyright (c) 1995 Sun Microsystems, Inc.
 *
 * See the file "license.terms" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 * RCS: @(#) $Id: tkimgutil.c,v 1.1.1.1 2001/06/13 05:03:31 dtashley Exp $
 */

#include "tkInt.h"
#include "tkPort.h"
#include "xbytes.h"


/*
 *----------------------------------------------------------------------
 *
 * TkAlignImageData --
 *
 *	This function takes an image and copies the data into an
 *	aligned buffer, performing any necessary bit swapping.
 *
 * Results:
 *	Returns a newly allocated buffer that should be freed by the
 *	caller.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

char *
TkAlignImageData(image, alignment, bitOrder)
    XImage *image;		/* Image to be aligned. */
    int alignment;		/* Number of bytes to which the data should
				 * be aligned (e.g. 2 or 4) */
    int bitOrder;		/* Desired bit order: LSBFirst or MSBFirst. */
{
    long dataWidth;
    char *data, *srcPtr, *destPtr;
    int i, j;

    if (image->bits_per_pixel != 1) {
	panic("TkAlignImageData: Can't handle image depths greater than 1.");
    }

    /*
     * Compute line width for output data buffer.
     */

    dataWidth = image->bytes_per_line;
    if (dataWidth % alignment) {
	dataWidth += (alignment - (dataWidth % alignment));
    }

    data = ckalloc(dataWidth * image->height);

    destPtr = data;
    for (i = 0; i < image->height; i++) {
	srcPtr = &image->data[i * image->bytes_per_line];
	for (j = 0; j < dataWidth; j++) {
	    if (j >= image->bytes_per_line) {
		*destPtr = 0;
	    } else if (image->bitmap_bit_order != bitOrder) {
		*destPtr = xBitReverseTable[(unsigned char)(*(srcPtr++))];
	    } else {
		*destPtr = *(srcPtr++);
	    }
	    destPtr++;
	}
    }
    return data;
}

/* End of tkimgutil.c */
