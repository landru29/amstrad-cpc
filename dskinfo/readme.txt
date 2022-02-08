DSKINFO (c) Kevin Thacker, May 2002

DSKINFO is a tool to display information concerning the formatdisc format that
are described by the standard or extended CPCEMU style disc image file format.


The following information is displayed:

For each track:
- the ID field for each sector on the track (C,H,R,N)
- a code for a sector filled with a single byte, and the value of the byte
- a code for the address mark of the data field ("CM" indicates the
  data field has a "deleted data" address mark. If this code is not present
  then "data" address mark is assumed.
- a code indicating any CRC errors

This tool can be used to identify the features of a copyprotected program
which is described in a disc image.

This source is released under the GNU Public License. See the file "Copying"
for the full license.

License:

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.