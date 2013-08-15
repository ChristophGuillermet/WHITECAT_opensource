/*

  Copyright © Grame 2001

  This library is free software; you can redistribute it and modify it under 
  the terms of the GNU Library General Public License as published by the 
  Free Software Foundation version 2 of the License, or any later version.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public License 
  for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  grame@grame.fr
  
  modifications history:

*/

#ifndef __msCnct__
#define __msCnct__

#define ICON_1	1
#define Version	235
#define BITMAP_2	2
#define BITMAP_1	1

// ------------------------------------------------------------------
// DECLARATION DES CONSTANTES
// ------------------------------------------------------------------
#define IDD_LB_SRC	200
#define IDD_LB_APL	201
#define IDD_LB_DST	202

#define IDD_QUITTER	2
#define AboutButton	800

#define MAXLEN		80

// ------------------------------------------------------------------
// DECLARATION DES MACROS
// ------------------------------------------------------------------
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

// ------------------------------------------------------------------
// DECLARATION DES TYPES
// ------------------------------------------------------------------
typedef unsigned char	uchar;
typedef unsigned int	uint;
typedef unsigned long	ulong;

typedef char TSTR[MAXLEN];

#ifdef WIN32
#define CALLBACKAPI		CALLBACK
#else
#define CALLBACKAPI		FAR PASCAL _export
#endif

// ------------------------------------------------------------------
// DECLARATION DES PROTOTYPES
// ------------------------------------------------------------------
BOOL CALLBACKAPI msConnectDlgProc(HWND,UINT,UINT,LONG);

#endif
