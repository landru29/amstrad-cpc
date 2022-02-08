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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#include "dskinfo.h"

static int dsk_type;

/* display error message */
static void	DisplayError(const uint8 *Message)
{
	fprintf(stderr,"Error: ");
	fprintf(stderr,Message);
	fprintf(stderr,"\r\n");
}

/* display message */
static void DisplayMessage(const uint8 *Message)
{
	fprintf(stdout,Message);
}

/* initialise dumping codes */
static void DumpCode_Initialise(uint32 *Code)
{
	*Code = 0;
}

/* dump code */
static void DumpCode_Display(const uint8 *pCode, uint32 *Code)
{
	/* if code is not zero, then there is a code before */
	/* display comma */
	if ((*Code)!=0)
	{
		fprintf(stdout,", ");
	}

	/* ensure there is a comma after this code if there is a code following */
	*Code = 1;

	/* display code text */
	fprintf(stdout,pCode);
}

static uint8 IsSameData(const uint8 *pData, uint32 Length, uint8 *pDataByte)
{
	uint8 DataByte;
	uint32 Offset;

	if (Length==0)
		return 1;

	Offset = 0;
	
	/* get initial byte */
	DataByte = pData[Offset];
	Offset++;

	Length--;

	/* check remaining bytes are the same as this byte */
	while (Length!=0)
	{
		uint8 ThisByte;

		/* get this byte */
		ThisByte = pData[Offset];
		Offset++;

		/* same? */
		if (DataByte!=ThisByte)
			return 0;

		Length--;
	}

	/* return the data byte */
	*pDataByte = DataByte;

	return 1;
}

static uint32 GetSectorSize(uint32 N)
{
	return ((1<<N)<<7);
}

static uint8 StandardDiscImage_Info(const uint8 *pData, const uint32 DataLength)
{
	uint32 t,s,h;
	standard_dsk_header *header;
	uint32 track_length;

	
	if (memcmp(pData,DSK_STANDARD_IDENT,8)!=0)
	{
		return 0;
	}
	
	header = (standard_dsk_header *)pData;

	track_length = (header->TrackSizeLow & 0x0ff) |
					((header->TrackSizeHigh & 0x0ff)<<8);

	fprintf(stdout,"Type: Standard CPCEMU style\r\n");

	for (t=0; t<header->NumTracks; t++)
	{
		for (h=0; h<header->NumSides; h++)
		{
			uint32 track_offset;
			uint32 sector_size;
			dsk_track_header *track_header;
			standard_dsk_id *id;
			uint8 *sector_data;

			/* display track and sector */
			fprintf(stdout,"------------------------------------------------------\r\n");
			fprintf(stdout,"Track: %02d Side: %1d\r\n",t,h);
			fprintf(stdout,"\r\n");

			track_offset = (t*header->NumSides)+h;

			/* get address of track header */
			track_header = (dsk_track_header *)
				((uint32)pData+sizeof(standard_dsk_header)+(track_offset*track_length));

			/* get address of sector data */
			sector_data = (uint8 *)((uint32)track_header + sizeof(dsk_track_header));

			/* get pointer to ids */
			id = (standard_dsk_id *)&track_header->ids;

			/* display number of sectors */
			fprintf(stdout,"No. Of Sectors: %d\r\n",track_header->SPT);
			fprintf(stdout,"\r\n");

			sector_size = GetSectorSize(track_header->BPS);

			/* display ids and codes */
			for (s=0; s<track_header->SPT; s++)
			{
				uint8 DataByte;
				uint32 Code;

				fprintf(stdout, "&%02x &%02x &%02x &%02x    ",id->C, id->H, id->R, id->N);
				DumpCode_Initialise(&Code);

				if (id->ST2 & (1<<6))
				{
					/* deleted data address mark */
					DumpCode_Display("CM",&Code);
				}

				if (id->ST2 & (1<<5))
				{
					/* data error */
					DumpCode_Display("DD",&Code);
				}

				/* repeated data? */
				if (IsSameData(sector_data, GetSectorSize(id->N), &DataByte))
				{
					DumpCode_Display("RD ",&Code);
					fprintf(stdout,"(&%02x)",DataByte);
				}

				fprintf(stdout,"\r\n");
				id++;

				sector_data+=sector_size;
			}
		}
	}	

	return 1;
}

static uint8 ExtendedDiscImage_Info(const uint8 *pData, const uint32 DataLength)
{
	uint32 t,s,h;
	extended_dsk_header *header;
	
	if (memcmp(pData,DSK_EXTENDED_IDENT,8)!=0)
	{
		return 0;
	}
	
	header = (extended_dsk_header *)pData;

	fprintf(stdout,"Type: Extended CPCEMU style\r\n");

	for (t=0; t<header->NumTracks; t++)
	{
		for (h=0; h<header->NumSides; h++)
		{
			uint32 track_offset;
			uint32 sector_size;
			dsk_track_header *track_header;
			extended_dsk_id *id;
			uint8 *sector_data;

			/* display track and sector */
			fprintf(stdout,"------------------------------------------------------\r\n");
			fprintf(stdout,"Track: %02d Side: %1d\r\n",t,h);
			fprintf(stdout,"\r\n");

			if (header->TrackSizeTable[(t*header->NumSides)+h]==0)
			{
				fprintf(stdout,"No. Of Sectors: 0\r\n\r\n  Unformatted\r\n");
			}
			else
			{
				uint8 i;

				track_offset = 0;

				for (i=0; i<(t*header->NumSides)+h; i++)
				{
					track_offset+=(header->TrackSizeTable[i] & 0x0ff)<<8;
				}

				/* get address of track header */
				track_header = (dsk_track_header *)
					((unsigned long)pData+sizeof(extended_dsk_header)+track_offset);
				
				/* get address of sector data */
				sector_data = (uint8 *)((unsigned long)track_header + sizeof(dsk_track_header));

				/* get pointer to ids */
				id = (extended_dsk_id *)&track_header->ids;

				/* display number of sectors */
				fprintf(stdout,"No. Of Sectors: %d\r\n",track_header->SPT);
				fprintf(stdout,"\r\n");

				sector_size = GetSectorSize(track_header->BPS);

				/* display ids and codes */
				for (s=0; s<track_header->SPT; s++)
				{
					uint8 DataByte;
					uint32 Code;

					fprintf(stdout, "&%02x &%02x &%02x &%02x    ",id->C, id->H, id->R, id->N);
					DumpCode_Initialise(&Code);

					if (id->ST2 & (1<<6))
					{
						/* deleted data address mark */
						DumpCode_Display("CM",&Code);
					}

					if (id->ST2 & (1<<5))
					{
						/* data error */
						DumpCode_Display("DD",&Code);
					}

					/* repeated data? */
					if (IsSameData(sector_data, GetSectorSize(id->N), &DataByte)!=0)
					{
						DumpCode_Display("RD",&Code);
						fprintf(stdout,"(&%02x)",DataByte);
					}

					fprintf(stdout,"\r\n");
					id++;

					sector_data+=((id->sizeLow & 0x0ff)|((id->sizeHigh & 0x0ff)<<8));
				}
			}
		}
	}	

	return 1;
}

int	main(int argc, char **argv)
{
	if (argc!=2)
	{
		DisplayMessage("dskinfo (c) Kevin Thacker, May 2002\r\n\r\n");
		DisplayMessage("Usage:\r\n");
		DisplayMessage("dskinfo <disc image name>\r\n");
		return -1;
	}
	else
	{
		FILE *fh;

		/* open input file */
		fh = fopen(argv[1],"rb");

		if (fh!=NULL)
		{
			uint32 Offset;
			uint8 *pData = NULL;
			uint32 DataLength = 0;
			
			/* get current offset */
			Offset = ftell(fh);
			/* seek to end */
			fseek(fh, 0, SEEK_END);
			/* report length/position */
			DataLength = ftell(fh);
			/* restore original position */
			fseek(fh, Offset, SEEK_SET);

			if (DataLength==0)
			{
				DisplayError("file is 0 bytes long!");
			}
			else
			{

				/* read entire file into memory */
				pData = (unsigned char *)malloc(DataLength);

				if (pData!=NULL)
				{
					/* read the entire file */
					if (fread(pData, 1, DataLength, fh)!=DataLength)
					{
						DisplayError("failed to read file!");	
					
						/* free the data */
						free(pData);
						pData = NULL;
					}
				}
			}

			/* close input file */
			fclose(fh);

			/* dump output */
			if (pData!=NULL)
			{
				/* identify disc image */
				fprintf(stdout,"** Start of Info **\r\n");
				fprintf(stdout,"\r\n");
				fprintf(stdout,"Legend:\r\n");
				fprintf(stdout,"CM       - 'Control Mark'; sector has 'Deleted Data' address mark\r\n");
				fprintf(stdout,"DD       - 'Data Error in Data Field'; CRC of data on disc does not match calculated CRC\r\n");
				fprintf(stdout,"RD (&xx) - 'Repeated Data'; sector filled with a single byte, &xx is the value\r\n");
				fprintf(stdout,"\r\n");
				fprintf(stdout,"Info of disc image file \"%s\":\r\n",argv[1]);
				fprintf(stdout,"\r\n");

				if (StandardDiscImage_Info(pData, DataLength)==0)
				{
					ExtendedDiscImage_Info(pData, DataLength);
				}

				fprintf(stdout,"** End of Info **\r\n");

				free(pData);
			}
		}
	}

	return 0;
}