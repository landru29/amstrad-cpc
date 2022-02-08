/*
 *  dskinfo (c) Kevin Thacker, May 2002
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
#ifndef __DSK_INFO_HEADER_INCLUDED__
#define __DSK_INFO_HEADER_INCLUDED__

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;

#ifdef WIN32
#pragma pack(1)
#endif

/* identification string for a standard disc image */
#define DSK_STANDARD_IDENT "MV - CPC"
/* identification string for an extended disc image */
#define DSK_EXTENDED_IDENT "EXTENDED"
/* track ident */
#define TRACK_IDENT "Track-Info\r\n"

/* same for extended and standard */
typedef struct 
{
	uint8	Ident[12];			/* ident string */
	uint8	pad0[4];			/* unused */
	uint8	track;				/* physical track number */
	uint8	side;				/* physical side number */
	uint8	pad1[2];			/* unused */
	uint8	BPS;				/* sector size code, used in FDC format command */
	uint8	SPT;				/* number of sectors in this track */
	uint8	GAP3;				/* GAP#3 size, used in FDC format command */
	uint8	FillerByte;			/* sector data filler byte, used in FDC format command */
	uint8	ids[29*8];
} dsk_track_header;

/* id structure for extended disc image */
typedef struct
{
	uint8	C;		/* C from ID field on disc */
	uint8	H;		/* H from ID field on disc */
	uint8	R;		/* R from ID field on disc */
	uint8	N;		/* N from ID field on disc */
	uint8	ST1;	
	uint8	ST2;	
					/* bit 6: if "1" then data field uses "deleted data" address mark
							  if "0" then data field uses "data" address mark */
					/* bit 5: if "1" then there is a error in the data field */
	uint8	sizeLow; /* size of data stored for the corresponding
						sector in the disc image file */
	uint8	sizeHigh;
} extended_dsk_id;

/* id structure for standard disc image */
typedef struct
{
	uint8	C;		/* same function as in extended_dsk_id */
	uint8	H;		/* same function as in extended_dsk_id */
	uint8	R;		/* same function as in extended_dsk_id */
	uint8	N;		/* same function as in extended_dsk_id */
	uint8	ST1;	/* same function as in extended_dsk_id */
	uint8	ST2;	/* same function as in extended_dsk_id */
	uint8	unused[2];
} standard_dsk_id;

typedef struct
{
	uint8	Ident[34];
	uint8	Creator[14];
	uint8	NumTracks;
	uint8	NumSides;
} common_dsk_header;

/* header for standard disc image */
typedef struct
{
	uint8	Ident[34];		/* "MV - CPC" identification */
	uint8	Creator[14];	/* name of program that created this image */
	uint8	NumTracks;		/* physical number of tracks stored in the disc image */
	uint8	NumSides;		/* physical number of sides stored in the disc image */
	uint8	TrackSizeLow;	/* size of the complete disc image track description and sector data
							of the largest track */
	uint8	TrackSizeHigh;
	uint8	pad0[204];		/* unused */
} standard_dsk_header;

typedef struct
{
	uint8	Ident[34];		/* same function as in standard disc image */
	uint8	Creator[14];	/* same function as in standard disc image */
	uint8	NumTracks;		/* same function as in standard disc image */
	uint8	NumSides;		/* same function as in standard disc image */
	uint8	pad0[2];		/* unused */
	uint8	TrackSizeTable[204]; /* size of the complete disc image track description
								and sector data / 256 */
} extended_dsk_header;


#ifdef WIN32
#pragma pack()
#endif

enum
{
	UNKNOWN_DISC_IMAGE = 0,	/* unknown */
	STANDARD_DISC_IMAGE, /* standard */
	EXTENDED_DISC_IMAGE, /* extended */
};

#endif