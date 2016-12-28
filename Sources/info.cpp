/*
		This program is free software; you can redistribute it and/or modify
		it under the terms of the GNU General Public License as published by
		the Free Software Foundation.

		This program is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
		GNU General Public License for more details.

		You should have received a copy of the GNU General Public License
		along with this program; if not, write to the Free Software
		Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

		The author can be contacted at:
		Donald Graft
		neuron2@attbi.com.
*/


#include	"info.h"

#define	NOGDI
#define	NOMINMAX
#define	WIN32_LEAN_AND_MEAN
//#include "Windows.h"
#include	"avisynth.h"
#include <stdint.h>



static unsigned short font[][20] = {
//STARTCHAR space
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR !
	{
		0x0000,0x0000,0x0c00,0x0c00,
		0x0c00,0x0c00,0x0c00,0x0c00,
		0x0c00,0x0c00,0x0c00,0x0c00,
		0x0000,0x0c00,0x0c00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR "
	{
		0x0000,0x0000,0x3300,0x3300,
		0x3300,0x1200,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR #
	{
		0x0000,0x0000,0x0000,0x0d80,
		0x0d80,0x0d80,0x3fc0,0x1b00,
		0x1b00,0x1b00,0x7f80,0x3600,
		0x3600,0x3600,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR $
	{
		0x0000,0x0000,0x0c00,0x3f00,
		0x6d80,0x6c00,0x6c00,0x6c00,
		0x3f00,0x0d80,0x0d80,0x0d80,
		0x6d80,0x3f00,0x0c00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR %
	{
		0x0000,0x0000,0x0000,0x3980,
		0x6d80,0x6f00,0x3b00,0x0600,
		0x0600,0x0c00,0x0c00,0x1b80,
		0x1ec0,0x36c0,0x3380,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR &
	{
		0x0000,0x0000,0x1c00,0x3600,
		0x3600,0x3600,0x3c00,0x1800,
		0x3800,0x6c00,0x66c0,0x6380,
		0x6300,0x7780,0x3cc0,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR '
	{
		0x0000,0x0000,0x0f00,0x0e00,
		0x1800,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR (
	{
		0x0000,0x0000,0x0300,0x0600,
		0x0c00,0x0c00,0x1800,0x1800,
		0x1800,0x1800,0x1800,0x0c00,
		0x0c00,0x0600,0x0300,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR )
	{
		0x0000,0x0000,0x3000,0x1800,
		0x0c00,0x0c00,0x0600,0x0600,
		0x0600,0x0600,0x0600,0x0c00,
		0x0c00,0x1800,0x3000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR *
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x3300,0x3300,0x1e00,
		0x7f80,0x1e00,0x3300,0x3300,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR +
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0c00,0x0c00,0x0c00,
		0x7f80,0x0c00,0x0c00,0x0c00,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR ,
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0e00,0x0e00,0x1800,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR -
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x7f80,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR .
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0e00,0x0e00,0x0e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR /
	{
		0x0000,0x0000,0x0000,0x0180,
		0x0180,0x0300,0x0300,0x0600,
		0x0600,0x0c00,0x0c00,0x1800,
		0x1800,0x3000,0x3000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR 0
	{
		0x0000,0x0000,0x0c00,0x1e00,
		0x3300,0x3300,0x6180,0x6180,
		0x6180,0x6180,0x6180,0x3300,
		0x3300,0x1e00,0x0c00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR 1
	{
		0x0000,0x0000,0x0c00,0x1c00,
		0x3c00,0x6c00,0x0c00,0x0c00,
		0x0c00,0x0c00,0x0c00,0x0c00,
		0x0c00,0x0c00,0x7f80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR 2
	{
		0x0000,0x0000,0x1e00,0x3300,
		0x6180,0x6180,0x0180,0x0180,
		0x0300,0x0e00,0x1800,0x3000,
		0x6000,0x6000,0x7f80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR 3
	{
		0x0000,0x0000,0x1e00,0x3300,
		0x6180,0x6180,0x0180,0x0300,
		0x0e00,0x0300,0x0180,0x6180,
		0x6180,0x3300,0x1e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR 4
	{
		0x0000,0x0000,0x0100,0x0300,
		0x0700,0x0f00,0x1b00,0x3300,
		0x6300,0x6300,0x7f80,0x0300,
		0x0300,0x0300,0x0300,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR 5
	{
		0x0000,0x0000,0x7f80,0x6000,
		0x6000,0x6000,0x6000,0x6e00,
		0x7300,0x0180,0x0180,0x0180,
		0x6180,0x3300,0x1e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR 6
	{
		0x0000,0x0000,0x1e00,0x3300,
		0x6100,0x6000,0x6000,0x6e00,
		0x7300,0x6180,0x6180,0x6180,
		0x6180,0x3300,0x1e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR 7
	{
		0x0000,0x0000,0x7f80,0x0180,
		0x0180,0x0300,0x0300,0x0600,
		0x0600,0x0c00,0x0c00,0x1800,
		0x1800,0x3000,0x3000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR 8
	{
		0x0000,0x0000,0x1e00,0x3300,
		0x6180,0x6180,0x6180,0x3300,
		0x1e00,0x3300,0x6180,0x6180,
		0x6180,0x3300,0x1e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR 9
	{
		0x0000,0x0000,0x1e00,0x3300,
		0x6180,0x6180,0x6180,0x6180,
		0x3380,0x1d80,0x0180,0x0180,
		0x2180,0x3300,0x1e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR :
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0e00,0x0e00,0x0000,
		0x0000,0x0000,0x0000,0x0e00,
		0x0e00,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR ;
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0e00,0x0e00,0x0000,
		0x0000,0x0000,0x0000,0x0e00,
		0x0e00,0x1c00,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR <
	{
		0x0000,0x0000,0x0100,0x0300,
		0x0600,0x0c00,0x1800,0x3000,
		0x6000,0x3000,0x1800,0x0c00,
		0x0600,0x0300,0x0100,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR =
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x7f80,0x0000,
		0x0000,0x0000,0x0000,0x7f80,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR >
	{
		0x0000,0x0000,0x2000,0x3000,
		0x1800,0x0c00,0x0600,0x0300,
		0x0180,0x0300,0x0600,0x0c00,
		0x1800,0x3000,0x2000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR ?
	{
		0x0000,0x0000,0x1e00,0x3300,
		0x6180,0x6180,0x6180,0x0300,
		0x0600,0x0c00,0x0c00,0x0c00,
		0x0000,0x0c00,0x0c00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR @
	{
		0x0000,0x0000,0x1e00,0x3300,
		0x6180,0x6780,0x6f80,0x6d80,
		0x6d80,0x6d80,0x6f00,0x6600,
		0x6000,0x3180,0x1f00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR A
	{
		0x0000,0x0000,0x0c00,0x1e00,
		0x3300,0x3300,0x6180,0x6180,
		0x6180,0x7f80,0x6180,0x6180,
		0x6180,0x6180,0x6180,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR B
	{
		0x0000,0x0000,0x7c00,0x6600,
		0x6300,0x6300,0x6300,0x6600,
		0x7e00,0x6300,0x6180,0x6180,
		0x6180,0x6300,0x7e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR C
	{
		0x0000,0x0000,0x1e00,0x3300,
		0x6180,0x6000,0x6000,0x6000,
		0x6000,0x6000,0x6000,0x6000,
		0x6180,0x3300,0x1e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR D
	{
		0x0000,0x0000,0x7e00,0x6300,
		0x6180,0x6180,0x6180,0x6180,
		0x6180,0x6180,0x6180,0x6180,
		0x6180,0x6300,0x7e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR E
	{
		0x0000,0x0000,0x7f80,0x6000,
		0x6000,0x6000,0x6000,0x6000,
		0x7e00,0x6000,0x6000,0x6000,
		0x6000,0x6000,0x7f80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR F
	{
		0x0000,0x0000,0x7f80,0x6000,
		0x6000,0x6000,0x6000,0x6000,
		0x7e00,0x6000,0x6000,0x6000,
		0x6000,0x6000,0x6000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR G
	{
		0x0000,0x0000,0x1e00,0x3300,
		0x6180,0x6000,0x6000,0x6000,
		0x6780,0x6180,0x6180,0x6180,
		0x6180,0x3380,0x1e80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR H
	{
		0x0000,0x0000,0x6180,0x6180,
		0x6180,0x6180,0x6180,0x6180,
		0x7f80,0x6180,0x6180,0x6180,
		0x6180,0x6180,0x6180,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR I
	{
		0x0000,0x0000,0x7f80,0x0c00,
		0x0c00,0x0c00,0x0c00,0x0c00,
		0x0c00,0x0c00,0x0c00,0x0c00,
		0x0c00,0x0c00,0x7f80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR J
	{
		0x0000,0x0000,0x0f80,0x0180,
		0x0180,0x0180,0x0180,0x0180,
		0x0180,0x0180,0x0180,0x6180,
		0x6180,0x3300,0x1e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR K
	{
		0x0000,0x0000,0x6180,0x6180,
		0x6300,0x6300,0x6600,0x6600,
		0x7c00,0x6600,0x6600,0x6300,
		0x6300,0x6180,0x6180,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR L
	{
		0x0000,0x0000,0x6000,0x6000,
		0x6000,0x6000,0x6000,0x6000,
		0x6000,0x6000,0x6000,0x6000,
		0x6000,0x6000,0x7f80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR M
	{
		0x0000,0x0000,0x6180,0x6180,
		0x7380,0x7380,0x7f80,0x6d80,
		0x6d80,0x6d80,0x6d80,0x6180,
		0x6180,0x6180,0x6180,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR N
	{
		0x0000,0x0000,0x6180,0x7180,
		0x7180,0x7980,0x7980,0x6d80,
		0x6d80,0x6780,0x6780,0x6380,
		0x6380,0x6180,0x6180,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR O
	{
		0x0000,0x0000,0x1e00,0x3300,
		0x6180,0x6180,0x6180,0x6180,
		0x6180,0x6180,0x6180,0x6180,
		0x6180,0x3300,0x1e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR P
	{
		0x0000,0x0000,0x7e00,0x6300,
		0x6180,0x6180,0x6180,0x6180,
		0x6300,0x7e00,0x6000,0x6000,
		0x6000,0x6000,0x6000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR Q
	{
		0x0000,0x0000,0x1e00,0x3300,
		0x6180,0x6180,0x6180,0x6180,
		0x6180,0x6180,0x6180,0x6d80,
		0x6780,0x3300,0x1f00,0x0180,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR R
	{
		0x0000,0x0000,0x7e00,0x6300,
		0x6180,0x6180,0x6180,0x6180,
		0x6300,0x7e00,0x6600,0x6300,
		0x6300,0x6180,0x6180,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR S
	{
		0x0000,0x0000,0x1e00,0x3300,
		0x6180,0x6000,0x6000,0x3000,
		0x1e00,0x0300,0x0180,0x0180,
		0x6180,0x3300,0x1e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR T
	{
		0x0000,0x0000,0x7f80,0x0c00,
		0x0c00,0x0c00,0x0c00,0x0c00,
		0x0c00,0x0c00,0x0c00,0x0c00,
		0x0c00,0x0c00,0x0c00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR U
	{
		0x0000,0x0000,0x6180,0x6180,
		0x6180,0x6180,0x6180,0x6180,
		0x6180,0x6180,0x6180,0x6180,
		0x6180,0x3300,0x1e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR V
	{
		0x0000,0x0000,0x6180,0x6180,
		0x6180,0x6180,0x3300,0x3300,
		0x3300,0x1e00,0x1e00,0x1e00,
		0x0c00,0x0c00,0x0c00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR W
	{
		0x0000,0x0000,0x6180,0x6180,
		0x6180,0x6180,0x6180,0x6d80,
		0x6d80,0x6d80,0x6d80,0x7380,
		0x7380,0x6180,0x6180,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR X
	{
		0x0000,0x0000,0x6180,0x6180,
		0x3300,0x3300,0x1e00,0x1e00,
		0x0c00,0x1e00,0x1e00,0x3300,
		0x3300,0x6180,0x6180,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR Y
	{
		0x0000,0x0000,0x6180,0x6180,
		0x3300,0x3300,0x1e00,0x1e00,
		0x0c00,0x0c00,0x0c00,0x0c00,
		0x0c00,0x0c00,0x0c00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR Z
	{
		0x0000,0x0000,0x7f80,0x0180,
		0x0180,0x0300,0x0600,0x0600,
		0x0c00,0x1800,0x1800,0x3000,
		0x6000,0x6000,0x7f80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR [
	{
		0x0000,0x0000,0x3f00,0x3000,
		0x3000,0x3000,0x3000,0x3000,
		0x3000,0x3000,0x3000,0x3000,
		0x3000,0x3000,0x3f00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR 
	{
		0x0000,0x0000,0x0000,0x3000,
		0x3000,0x1800,0x1800,0x0c00,
		0x0c00,0x0600,0x0600,0x0300,
		0x0300,0x0180,0x0180,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR ]
	{
		0x0000,0x0000,0x3f00,0x0300,
		0x0300,0x0300,0x0300,0x0300,
		0x0300,0x0300,0x0300,0x0300,
		0x0300,0x0300,0x3f00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR ^
	{
		0x0000,0x0000,0x0c00,0x1e00,
		0x3300,0x6180,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR _
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x7fc0,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR `
	{
		0x0000,0x0000,0x3c00,0x1c00,
		0x0600,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR a
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x1f00,
		0x3180,0x0180,0x3f80,0x6180,
		0x6180,0x6180,0x3e80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR b
	{
		0x0000,0x0000,0x6000,0x6000,
		0x6000,0x6000,0x6000,0x6e00,
		0x7300,0x6180,0x6180,0x6180,
		0x6180,0x7300,0x6e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR c
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x1f00,
		0x3180,0x6000,0x6000,0x6000,
		0x6000,0x3180,0x1f00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR d
	{
		0x0000,0x0000,0x0180,0x0180,
		0x0180,0x0180,0x0180,0x1d80,
		0x3380,0x6180,0x6180,0x6180,
		0x6180,0x3380,0x1d80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR e
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x1e00,
		0x3300,0x6180,0x7f80,0x6000,
		0x6000,0x3180,0x1f00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR f
	{
		0x0000,0x0000,0x0f00,0x1980,
		0x1980,0x1800,0x1800,0x1800,
		0x1800,0x7e00,0x1800,0x1800,
		0x1800,0x1800,0x1800,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR g
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x3e80,
		0x6380,0x6300,0x6300,0x6300,
		0x3e00,0x6000,0x3f00,0x6180,
		0x6180,0x6180,0x3f00,0x0000,
	},
	//STARTCHAR h
	{
		0x0000,0x0000,0x6000,0x6000,
		0x6000,0x6000,0x6000,0x6e00,
		0x7300,0x6180,0x6180,0x6180,
		0x6180,0x6180,0x6180,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR i
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0c00,0x0c00,0x0000,0x3c00,
		0x0c00,0x0c00,0x0c00,0x0c00,
		0x0c00,0x0c00,0x7f80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR j
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0180,0x0180,0x0000,0x0780,
		0x0180,0x0180,0x0180,0x0180,
		0x0180,0x0180,0x0180,0x3180,
		0x3180,0x3180,0x1f00,0x0000,
	},
	//STARTCHAR k
	{
		0x0000,0x0000,0x6000,0x6000,
		0x6000,0x6000,0x6000,0x6300,
		0x6600,0x6c00,0x7800,0x7c00,
		0x6600,0x6300,0x6180,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR l
	{
		0x0000,0x0000,0x3c00,0x0c00,
		0x0c00,0x0c00,0x0c00,0x0c00,
		0x0c00,0x0c00,0x0c00,0x0c00,
		0x0c00,0x0c00,0x7f80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR m
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x5b00,
		0x7f80,0x6d80,0x6d80,0x6d80,
		0x6d80,0x6d80,0x6d80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR n
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x6e00,
		0x7300,0x6180,0x6180,0x6180,
		0x6180,0x6180,0x6180,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR o
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x1e00,
		0x3300,0x6180,0x6180,0x6180,
		0x6180,0x3300,0x1e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR p
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x6e00,
		0x7300,0x6180,0x6180,0x6180,
		0x6180,0x7300,0x6e00,0x6000,
		0x6000,0x6000,0x6000,0x0000,
	},
	//STARTCHAR q
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x1d80,
		0x3380,0x6180,0x6180,0x6180,
		0x6180,0x3380,0x1d80,0x0180,
		0x0180,0x0180,0x0180,0x0000,
	},
	//STARTCHAR r
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x6f00,
		0x3980,0x3000,0x3000,0x3000,
		0x3000,0x3000,0x3000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR s
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x3f00,
		0x6180,0x6000,0x3f00,0x0180,
		0x0180,0x6180,0x3f00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR t
	{
		0x0000,0x0000,0x0000,0x0000,
		0x1800,0x1800,0x1800,0x7e00,
		0x1800,0x1800,0x1800,0x1800,
		0x1800,0x1980,0x0f00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR u
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x6180,
		0x6180,0x6180,0x6180,0x6180,
		0x6180,0x3380,0x1d80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR v
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x6180,
		0x6180,0x3300,0x3300,0x1e00,
		0x1e00,0x0c00,0x0c00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR w
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x6180,
		0x6180,0x6180,0x6d80,0x6d80,
		0x6d80,0x7f80,0x3300,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR x
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x6180,
		0x3300,0x1e00,0x0c00,0x0c00,
		0x1e00,0x3300,0x6180,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR y
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x6180,
		0x6180,0x6180,0x6180,0x6180,
		0x6180,0x3380,0x1d80,0x0180,
		0x6180,0x3300,0x1e00,0x0000,
	},
	//STARTCHAR z
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x3f80,
		0x0180,0x0300,0x0600,0x0c00,
		0x1800,0x3000,0x3f80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR {
	{
		0x0000,0x0000,0x0780,0x0c00,
		0x0c00,0x0c00,0x0c00,0x0c00,
		0x7800,0x0c00,0x0c00,0x0c00,
		0x0c00,0x0c00,0x0780,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR bar
	{
		0x0000,0x0000,0x0c00,0x0c00,
		0x0c00,0x0c00,0x0c00,0x0c00,
		0x0c00,0x0c00,0x0c00,0x0c00,
		0x0c00,0x0c00,0x0c00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR }
	{
		0x0000,0x0000,0x7800,0x0c00,
		0x0c00,0x0c00,0x0c00,0x0c00,
		0x0780,0x0c00,0x0c00,0x0c00,
		0x0c00,0x0c00,0x7800,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR ~
	{
		0x0000,0x0000,0x3980,0x6d80,
		0x6700,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR C177
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR C240
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR exclamdown
	{
		0x0000,0x0000,0x0c00,0x0c00,
		0x0000,0x0c00,0x0c00,0x0c00,
		0x0c00,0x0c00,0x0c00,0x0c00,
		0x0c00,0x0c00,0x0c00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR cent
	{
		0x0000,0x0000,0x0000,0x0c00,
		0x0c00,0x1e00,0x3300,0x6100,
		0x6000,0x6000,0x6100,0x3300,
		0x1e00,0x0c00,0x0c00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR sterling
	{
		0x0000,0x0000,0x0000,0x0f00,
		0x1980,0x1980,0x1800,0x1800,
		0x7e00,0x1800,0x1800,0x1800,
		0x7c00,0x56c0,0x7380,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR currency
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x4040,0x2e80,0x1f00,
		0x3180,0x3180,0x3180,0x1f00,
		0x2e80,0x4040,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR yen
	{
		0x0000,0x0000,0x0000,0x0000,
		0x4080,0x6180,0x3300,0x1e00,
		0x3f00,0x0c00,0x3f00,0x0c00,
		0x0c00,0x0c00,0x0c00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR brokenbar
	{
		0x0000,0x0000,0x0c00,0x0c00,
		0x0c00,0x0c00,0x0c00,0x0000,
		0x0000,0x0000,0x0c00,0x0c00,
		0x0c00,0x0c00,0x0c00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR section
	{
		0x0000,0x0000,0x3e00,0x6300,
		0x6000,0x7000,0x7800,0x4c00,
		0x6600,0x3300,0x1900,0x0f00,
		0x0300,0x6300,0x3e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR dieresis
	{
		0x0000,0x0000,0x3300,0x3300,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR copyright
	{
		0x0000,0x0000,0x0000,0x0000,
		0x1e00,0x3300,0x6180,0x5e80,
		0x5280,0x5080,0x5280,0x5e80,
		0x6180,0x3300,0x1e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR ordfeminine
	{
		0x0000,0x0000,0x1f00,0x2180,
		0x0180,0x3f80,0x6180,0x6180,
		0x3e80,0x0000,0x7f80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR guillmotleft
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0480,0x0d80,0x1b00,0x3600,
		0x6c00,0xd800,0x6c00,0x3600,
		0x1b00,0x0d80,0x0480,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR logicalnot
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x7f80,
		0x7f80,0x0180,0x0180,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR hyphen
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x3f00,
		0x3f00,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR registered
	{
		0x0000,0x0000,0x0000,0x0000,
		0x1e00,0x3300,0x6180,0x5e80,
		0x5280,0x5e80,0x5480,0x5680,
		0x6180,0x3300,0x1e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR macron
	{
		0x0000,0x0000,0x0000,0x7f00,
		0x7f00,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR degree
	{
		0x0000,0x0000,0x0c00,0x1e00,
		0x3300,0x3300,0x1e00,0x0c00,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR plusminus
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0c00,0x0c00,0x7f80,0x7f80,
		0x0c00,0x0c00,0x0000,0x7f80,
		0x7f80,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR twosuperior
	{
		0x0000,0x0000,0x1c00,0x3600,
		0x0600,0x0c00,0x1800,0x3000,
		0x3e00,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR threesuperior
	{
		0x0000,0x0000,0x1c00,0x3600,
		0x0200,0x0e00,0x0200,0x3600,
		0x1c00,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR acute
	{
		0x0000,0x0000,0x0600,0x0c00,
		0x1800,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR mu
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x6300,0x6300,0x6300,0x6300,
		0x6300,0x7700,0x7d00,0x6000,
		0x6000,0x6000,0x0000,0x0000,
	},
	//STARTCHAR paragraph
	{
		0x0000,0x0000,0x1f80,0x3f80,
		0x7d80,0x7d80,0x7d80,0x3d80,
		0x1d80,0x0580,0x0580,0x0580,
		0x0580,0x0580,0x0580,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR periodcentered
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0e00,
		0x0e00,0x0e00,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR cedilla
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0c00,
		0x0400,0x1200,0x0c00,0x0000,
	},
	//STARTCHAR onesuperior
	{
		0x0000,0x0000,0x1800,0x3800,
		0x1800,0x1800,0x1800,0x3c00,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR ordmasculine
	{
		0x0000,0x0000,0x1c00,0x3600,
		0x6300,0x6300,0x6300,0x3600,
		0x1c00,0x0000,0x7f00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR guillemotright
	{
		0x0000,0x0000,0x0000,0x0000,
		0x4800,0x6c00,0x3600,0x1b00,
		0x0d80,0x06c0,0x0d80,0x1b00,
		0x3600,0x6c00,0x4800,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR onequarter
	{
		0x0000,0x0000,0x2000,0x6000,
		0x2080,0x2100,0x7200,0x0400,
		0x0900,0x1300,0x2500,0x4f00,
		0x0100,0x0100,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR onehalf
	{
		0x0000,0x0000,0x2000,0x6000,
		0x2080,0x2100,0x7200,0x0400,
		0x0b00,0x1480,0x2080,0x4100,
		0x0200,0x0780,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR threequarters
	{
		0x0000,0x0000,0x7000,0x0800,
		0x3080,0x0900,0x7200,0x0400,
		0x0900,0x1300,0x2500,0x4f80,
		0x0100,0x0100,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR questiondown
	{
		0x0000,0x0000,0x0c00,0x0c00,
		0x0000,0x0c00,0x0c00,0x0c00,
		0x1800,0x3000,0x6180,0x6180,
		0x6180,0x3300,0x1e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR Agrave
	{
		0x0000,0x0000,0x3000,0x1800,
		0x0c00,0x0000,0x0c00,0x1e00,
		0x3300,0x6180,0x6180,0x7f80,
		0x6180,0x6180,0x6180,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR Aacute
	{
		0x0000,0x0000,0x0300,0x0600,
		0x0c00,0x0000,0x0c00,0x1e00,
		0x3300,0x6180,0x6180,0x7f80,
		0x6180,0x6180,0x6180,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR Acircumflex
	{
		0x0000,0x0000,0x0c00,0x1e00,
		0x3300,0x0000,0x0c00,0x1e00,
		0x3300,0x6180,0x6180,0x7f80,
		0x6180,0x6180,0x6180,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR Atilde
	{
		0x0000,0x0000,0x1900,0x3f00,
		0x2600,0x0000,0x0c00,0x1e00,
		0x3300,0x6180,0x6180,0x7f80,
		0x6180,0x6180,0x6180,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR Adieresis
	{
		0x0000,0x0000,0x3300,0x3300,
		0x0000,0x0000,0x0c00,0x1e00,
		0x3300,0x6180,0x6180,0x7f80,
		0x6180,0x6180,0x6180,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR Aring
	{
		0x0000,0x0000,0x0c00,0x1200,
		0x1200,0x0c00,0x0c00,0x1e00,
		0x3300,0x6180,0x6180,0x7f80,
		0x6180,0x6180,0x6180,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR AE
	{
		0x0000,0x0000,0x0f80,0x1e00,
		0x3600,0x3600,0x6600,0x6600,
		0x7f80,0x6600,0x6600,0x6600,
		0x6600,0x6600,0x6780,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR Ccedilla
	{
		0x0000,0x0000,0x1e00,0x3300,
		0x6180,0x6000,0x6000,0x6000,
		0x6000,0x6000,0x6000,0x6000,
		0x6180,0x3300,0x1e00,0x0c00,
		0x0400,0x1200,0x0c00,0x0000,
	},
	//STARTCHAR Egrave
	{
		0x0000,0x0000,0x3000,0x1800,
		0x0c00,0x0000,0x7f80,0x6000,
		0x6000,0x6000,0x7e00,0x6000,
		0x6000,0x6000,0x7f80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR Eacute
	{
		0x0000,0x0000,0x0600,0x0c00,
		0x1800,0x0000,0x7f80,0x6000,
		0x6000,0x6000,0x7e00,0x6000,
		0x6000,0x6000,0x7f80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR Ecircumflex
	{
		0x0000,0x0000,0x0c00,0x1e00,
		0x3300,0x0000,0x7f80,0x6000,
		0x6000,0x6000,0x7e00,0x6000,
		0x6000,0x6000,0x7f80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR Edieresis
	{
		0x0000,0x0000,0x3300,0x3300,
		0x0000,0x0000,0x7f80,0x6000,
		0x6000,0x6000,0x7e00,0x6000,
		0x6000,0x6000,0x7f80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR Igrave
	{
		0x0000,0x0000,0x3000,0x1800,
		0x0c00,0x0000,0x3f00,0x0c00,
		0x0c00,0x0c00,0x0c00,0x0c00,
		0x0c00,0x0c00,0x3f00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR Iacute
	{
		0x0000,0x0000,0x0600,0x0c00,
		0x1800,0x0000,0x3f00,0x0c00,
		0x0c00,0x0c00,0x0c00,0x0c00,
		0x0c00,0x0c00,0x3f00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR Icircumflex
	{
		0x0000,0x0000,0x0c00,0x1e00,
		0x3300,0x0000,0x3f00,0x0c00,
		0x0c00,0x0c00,0x0c00,0x0c00,
		0x0c00,0x0c00,0x3f00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR Idieresis
	{
		0x0000,0x0000,0x3300,0x3300,
		0x0000,0x0000,0x3f00,0x0c00,
		0x0c00,0x0c00,0x0c00,0x0c00,
		0x0c00,0x0c00,0x3f00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR Eth
	{
		0x0000,0x0000,0x7e00,0x6300,
		0x6180,0x6180,0x6180,0x6180,
		0xf980,0x6180,0x6180,0x6180,
		0x6180,0x6300,0x7e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR Ntilde
	{
		0x0000,0x0000,0x1900,0x3f00,
		0x2600,0x0000,0x4180,0x6180,
		0x7180,0x7980,0x7d80,0x6f80,
		0x6780,0x6380,0x6180,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR Ograve
	{
		0x0000,0x0000,0x3000,0x1800,
		0x0c00,0x0000,0x1e00,0x3300,
		0x6180,0x6180,0x6180,0x6180,
		0x6180,0x3300,0x1e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR Oacute
	{
		0x0000,0x0000,0x0300,0x0600,
		0x0c00,0x0000,0x1e00,0x3300,
		0x6180,0x6180,0x6180,0x6180,
		0x6180,0x3300,0x1e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR Ocircumflex
	{
		0x0000,0x0000,0x0c00,0x1e00,
		0x3300,0x0000,0x1e00,0x3300,
		0x6180,0x6180,0x6180,0x6180,
		0x6180,0x3300,0x1e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR Otilde
	{
		0x0000,0x0000,0x1900,0x3f00,
		0x2600,0x0000,0x1e00,0x3300,
		0x6180,0x6180,0x6180,0x6180,
		0x6180,0x3300,0x1e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR Odieresis
	{
		0x0000,0x0000,0x3300,0x3300,
		0x0000,0x0000,0x1e00,0x3300,
		0x6180,0x6180,0x6180,0x6180,
		0x6180,0x3300,0x1e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR multiply
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x2080,
		0x3180,0x1b00,0x0e00,0x0e00,
		0x1b00,0x3180,0x2080,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR Oslash
	{
		0x0000,0x0080,0x1f00,0x3300,
		0x6380,0x6380,0x6580,0x6580,
		0x6580,0x6980,0x6980,0x6980,
		0x7180,0x3300,0x3e00,0x4000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR Ugrave
	{
		0x0000,0x0000,0x3000,0x1800,
		0x0c00,0x0000,0x6180,0x6180,
		0x6180,0x6180,0x6180,0x6180,
		0x6180,0x3300,0x1e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR Uacute
	{
		0x0000,0x0000,0x0300,0x0600,
		0x0c00,0x0000,0x6180,0x6180,
		0x6180,0x6180,0x6180,0x6180,
		0x6180,0x3300,0x1e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR Ucircumflex
	{
		0x0000,0x0000,0x0c00,0x1e00,
		0x3300,0x0000,0x6180,0x6180,
		0x6180,0x6180,0x6180,0x6180,
		0x6180,0x3380,0x1e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR Udieresis
	{
		0x0000,0x0000,0x3300,0x3300,
		0x0000,0x0000,0x6180,0x6180,
		0x6180,0x6180,0x6180,0x6180,
		0x6180,0x3300,0x1e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR Yacute
	{
		0x0000,0x0000,0x0300,0x0600,
		0x0c00,0x0000,0x4080,0x6180,
		0x3300,0x1e00,0x0c00,0x0c00,
		0x0c00,0x0c00,0x0c00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR Thorn
	{
		0x0000,0x0000,0x0000,0x0000,
		0x3c00,0x1800,0x1f00,0x1980,
		0x1980,0x1980,0x1f00,0x1800,
		0x1800,0x1800,0x3c00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR germandbls
	{
		0x0000,0x0000,0x0000,0x1c00,
		0x3e00,0x7300,0x6300,0x6300,
		0x6600,0x6c00,0x6600,0x6300,
		0x6100,0x6300,0x6e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR agave
	{
		0x0000,0x0000,0x3000,0x1800,
		0x0c00,0x0000,0x0000,0x3f00,
		0x6180,0x0180,0x3f80,0x6180,
		0x6180,0x6180,0x3e80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR aacute
	{
		0x0000,0x0000,0x0600,0x0c00,
		0x1800,0x0000,0x0000,0x3f00,
		0x6180,0x0180,0x3f80,0x6180,
		0x6180,0x6180,0x3e80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR acircumflex
	{
		0x0000,0x0000,0x0c00,0x1e00,
		0x3300,0x0000,0x0000,0x3f00,
		0x6180,0x0180,0x3f80,0x6180,
		0x6180,0x6180,0x3e80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR atilde
	{
		0x0000,0x0000,0x1900,0x3f00,
		0x2600,0x0000,0x0000,0x3f00,
		0x6180,0x0180,0x3f80,0x6180,
		0x6180,0x6180,0x3e80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR adieresis
	{
		0x0000,0x0000,0x3300,0x3300,
		0x0000,0x0000,0x0000,0x3f00,
		0x6180,0x0180,0x3f80,0x6180,
		0x6180,0x6180,0x3e80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR aring
	{
		0x0000,0x0000,0x0000,0x0c00,
		0x1200,0x1200,0x0c00,0x3f00,
		0x6180,0x0180,0x3f80,0x6180,
		0x6180,0x6180,0x3e80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR ae
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x3b00,
		0x4d80,0x0d80,0x0f00,0x3c00,
		0x6c00,0x6c80,0x3700,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR 0xccedilla
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x1f00,
		0x3180,0x6000,0x6000,0x6000,
		0x6000,0x3180,0x1f00,0x0c00,
		0x0400,0x1200,0x0c00,0x0000,
	},
	//STARTCHAR egrave
	{
		0x0000,0x0000,0x3000,0x1800,
		0x0c00,0x0000,0x0000,0x1e00,
		0x3300,0x6180,0x7f80,0x6000,
		0x6000,0x3180,0x1f00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR eacute
	{
		0x0000,0x0000,0x0600,0x0c00,
		0x1800,0x0000,0x0000,0x1e00,
		0x3300,0x6180,0x7f80,0x6000,
		0x6000,0x3180,0x1f00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR ecircumflex
	{
		0x0000,0x0000,0x0c00,0x1e00,
		0x3300,0x0000,0x0000,0x1e00,
		0x3300,0x6180,0x7f80,0x6000,
		0x6000,0x3180,0x1f00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR edieresis
	{
		0x0000,0x0000,0x3300,0x3300,
		0x0000,0x0000,0x0000,0x1e00,
		0x3300,0x6180,0x7f80,0x6000,
		0x6000,0x3180,0x1f00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR igrave
	{
		0x0000,0x0000,0x3000,0x1800,
		0x0c00,0x0000,0x0000,0x3c00,
		0x0c00,0x0c00,0x0c00,0x0c00,
		0x0c00,0x0c00,0x7f80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR iacute
	{
		0x0000,0x0000,0x0600,0x0c00,
		0x1800,0x0000,0x0000,0x3c00,
		0x0c00,0x0c00,0x0c00,0x0c00,
		0x0c00,0x0c00,0x7f80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR icircumflex
	{
		0x0000,0x0000,0x0c00,0x1e00,
		0x3300,0x0000,0x0000,0x3c00,
		0x0c00,0x0c00,0x0c00,0x0c00,
		0x0c00,0x0c00,0x7f80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR idieresis
	{
		0x0000,0x0000,0x3300,0x3300,
		0x0000,0x0000,0x0000,0x3c00,
		0x0c00,0x0c00,0x0c00,0x0c00,
		0x0c00,0x0c00,0x7f80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR eth
	{
		0x0000,0x0000,0x4400,0x6c00,
		0x3800,0x3800,0x6c00,0x4600,
		0x1f00,0x3380,0x6180,0x6180,
		0x6180,0x3300,0x1e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR ntilde
	{
		0x0000,0x0000,0x1900,0x3f00,
		0x2600,0x0000,0x0000,0x6e00,
		0x7300,0x6180,0x6180,0x6180,
		0x6180,0x6180,0x6180,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR ograve
	{
		0x0000,0x0000,0x3000,0x1800,
		0x0c00,0x0000,0x0000,0x1e00,
		0x3300,0x6180,0x6180,0x6180,
		0x6180,0x3300,0x1e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR oacute
	{
		0x0000,0x0000,0x0600,0x0c00,
		0x1800,0x0000,0x0000,0x1e00,
		0x3300,0x6180,0x6180,0x6180,
		0x6180,0x3300,0x1e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR ocircumflex
	{
		0x0000,0x0000,0x0c00,0x1e00,
		0x3300,0x0000,0x0000,0x1e00,
		0x3300,0x6180,0x6180,0x6180,
		0x6180,0x3300,0x1e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR otilde
	{
		0x0000,0x0000,0x1900,0x3f00,
		0x2600,0x0000,0x0000,0x1e00,
		0x3300,0x6180,0x6180,0x6180,
		0x6180,0x3300,0x1e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR odieresis
	{
		0x0000,0x0000,0x3300,0x3300,
		0x0000,0x0000,0x0000,0x1e00,
		0x3300,0x6180,0x6180,0x6180,
		0x6180,0x3300,0x1e00,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR divide
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0c00,0x0c00,0x0000,0x0000,
		0x7f80,0x7f80,0x0000,0x0000,
		0x0c00,0x0c00,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR oslash
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0080,0x1f00,
		0x3300,0x6580,0x6580,0x6980,
		0x6980,0x3300,0x3e00,0x4000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR ugrave
	{
		0x0000,0x0000,0x3000,0x1800,
		0x0c00,0x0000,0x0000,0x6180,
		0x6180,0x6180,0x6180,0x6180,
		0x6180,0x3380,0x1d80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR uacute
	{
		0x0000,0x0000,0x0600,0x0c00,
		0x1800,0x0000,0x0000,0x6180,
		0x6180,0x6180,0x6180,0x6180,
		0x6180,0x3380,0x1d80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR ucircumflex
	{
		0x0000,0x0000,0x0c00,0x1e00,
		0x3300,0x0000,0x0000,0x6180,
		0x6180,0x6180,0x6180,0x6180,
		0x6180,0x3380,0x1d80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR udieresis
	{
		0x0000,0x0000,0x3300,0x3300,
		0x0000,0x0000,0x0000,0x6180,
		0x6180,0x6180,0x6180,0x6180,
		0x6180,0x3380,0x1d80,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR yacute
	{
		0x0000,0x0000,0x0600,0x0c00,
		0x1800,0x0000,0x0000,0x0000,
		0x6180,0x6180,0x6180,0x6180,
		0x6180,0x3380,0x1d80,0x0180,
		0x6180,0x3300,0x1e00,0x0000,
	},
	//STARTCHAR thorn
	{
		0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x3800,
		0x1e00,0x1b00,0x1b00,0x1e00,
		0x1800,0x1800,0x3800,0x0000,
		0x0000,0x0000,0x0000,0x0000,
	},
	//STARTCHAR ydieresis
	{
		0x0000,0x0000,0x3300,0x3300,
		0x0000,0x0000,0x0000,0x0000,
		0x6180,0x6180,0x6180,0x6180,
		0x6180,0x3380,0x1d80,0x0180,
		0x6180,0x3300,0x1e00,0x0000,
	}
};

template <typename pixel_t>
void DrawDigit(PVideoFrame &dst, int x, int y, int num, int bits_per_pixel, int xRatioShift, int yRatioShift, bool chroma)
{
  x = x * 10;
  y = y * 20;

  const pixel_t color = 235 << (bits_per_pixel - 8);

  int pitch = dst->GetPitch() / sizeof(pixel_t);

  pixel_t* dstp = reinterpret_cast<pixel_t *>(dst->GetWritePtr());

  // write only to luma
  for (int tx = 0; tx < 10; tx++) {
    for (int ty = 0; ty < 20; ty++) {
      pixel_t* dp = &dstp[(x + tx) + (y + ty) * pitch];
      if (font[num][ty] & (1 << (15 - tx))) 
        *dp = color;
      else 
        *dp = (*dp * 3) >> 2;
    }
  }
  if (chroma)
  {
    pixel_t* dstpU = reinterpret_cast<pixel_t *>(dst->GetWritePtr(PLANAR_U));
    pixel_t* dstpV = reinterpret_cast<pixel_t *>(dst->GetWritePtr(PLANAR_V));

    int pitchUV = dst->GetPitch(PLANAR_U);
    int midChroma = 1 << (bits_per_pixel - 1);
    for (int tx = 0; tx < 10; tx++)
    {
      for (int ty = 0; ty < 20; ty++)
      {
        int pos = ((x + tx) >> xRatioShift) + ((y + ty) >> yRatioShift) * pitchUV;
        pixel_t* dpU = &dstpU[pos];
        pixel_t* dpV = &dstpV[pos];
        if (font[num][ty] & (1 << (15 - tx)))
        {
          *dpU = midChroma;
          *dpV = midChroma;
        }
        else
        {
          *dpU = (pixel_t)((*dpU + midChroma) >> 1);
          *dpV = (pixel_t)((*dpV + midChroma) >> 1);
        }
      }
    }
  }
}

// instantiate
template void DrawDigit<uint8_t>(PVideoFrame &dst, int x, int y, int num, int bits_per_pixel, int xRatioShift, int yRatioShift, bool chroma);
template void DrawDigit<uint16_t>(PVideoFrame &dst, int x, int y, int num, int bits_per_pixel, int xRatioShift, int yRatioShift, bool chroma);

// for YUY2 and any planar
void DrawString(PVideoFrame &dst, VideoInfo &vi, int x, int y, const char *s)
{
  if (vi.IsYUY2()) {
    DrawStringYUY2(dst, x, y, s);
    return;
  }

  int bits_per_pixel = vi.BitsPerComponent();
  int xRatioShift = vi.GetPlaneWidthSubsampling(PLANAR_U);
  int yRatioShift = vi.GetPlaneHeightSubsampling(PLANAR_U);
  bool grey = vi.IsY();
  for (int xx = 0; *s; ++s, ++xx) {
    if(bits_per_pixel == 8)
      DrawDigit<uint8_t>(dst, x + xx, y, *s - ' ', bits_per_pixel, xRatioShift, yRatioShift, !grey);
    else
      DrawDigit<uint16_t>(dst, x + xx, y, *s - ' ', bits_per_pixel, xRatioShift, yRatioShift, !grey);
  }
}

#if 0
void DrawDigit(::PVideoFrame &dst, int x, int y, int num) 
{
	int tx, ty;
	unsigned char *dpY, *dpU, *dpV;

	x = x * 10;
	y = y * 20;

	int pitchY = dst->GetPitch(PLANAR_Y);
	for (tx = 0; tx < 10; tx++)
	{
		for (ty = 0; ty < 20; ty++)
		{
			dpY = &dst->GetWritePtr(PLANAR_Y)[(x + tx) + (y + ty) * pitchY];
			if (font[num][ty] & (1 << (15 - tx)))
			{
				*dpY = 235;
			} else
			{
				*dpY = (unsigned char) ((*dpY * 3) >> 2);
			}
		}
	}
	int pitchUV = dst->GetPitch(PLANAR_V);
	for (tx = 0; tx < 10; tx++)
	{
		for (ty = 0; ty < 20; ty++)
		{
			dpU = &dst->GetWritePtr(PLANAR_U)[((x + tx)/2) + ((y + ty)/2) * pitchUV];
			dpV = &dst->GetWritePtr(PLANAR_V)[((x + tx)/2) + ((y + ty)/2) * pitchUV];
			if (font[num][ty] & (1 << (15 - tx)))
			{
				*dpU = 128;
				*dpV = 128;
			} else
			{
				*dpU = (unsigned char) ((*dpU + 128) >> 1);
				*dpV = (unsigned char) ((*dpV + 128) >> 1);
			}
		}
	}
}

void DrawString(::PVideoFrame &dst, int x, int y, const char *s) 
{
	for (int xx = 0; *s; ++s, ++xx) {
		DrawDigit(dst, x + xx, y, *s - ' ');
	}
}
#endif

void DrawDigitYUY2(::PVideoFrame &dst, int x, int y, int num) 
{
	extern unsigned short font[][20];

	x = x * 10;
	y = y * 20;

	int pitch = dst->GetPitch();
	for (int tx = 0; tx < 10; tx++) {
		for (int ty = 0; ty < 20; ty++) {
			unsigned char *dp = &dst->GetWritePtr()[(x + tx) * 2 + (y + ty) * pitch];
			if (font[num][ty] & (1 << (15 - tx))) {
				if (tx & 1) {
					dp[0] = 250;
					dp[-1] = 128;
					dp[1] = 128;
				} else {
					dp[0] = 250;
					dp[1] = 128;
					dp[3] = 128;
				}
			} else {
				if (tx & 1) {
					dp[0] = (unsigned char) ((dp[0] * 3) >> 2);
					dp[-1] = (unsigned char) ((dp[-1] + 128) >> 1);
					dp[1] = (unsigned char) ((dp[1] + 128) >> 1);
				} else {
					dp[0] = (unsigned char) ((dp[0] * 3) >> 2);
					dp[1] = (unsigned char) ((dp[1] + 128) >> 1);
					dp[3] = (unsigned char) ((dp[3] + 128) >> 1);
				}
			}
		}
	}
}

void DrawStringYUY2(::PVideoFrame &dst, int x, int y, const char *s) 
{
	for (int xx = 0; *s; ++s, ++xx) {
		DrawDigitYUY2(dst, x + xx, y, *s - ' ');
	}
}
