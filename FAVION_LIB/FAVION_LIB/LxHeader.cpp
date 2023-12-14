// LxHeader.cpp: implementation of the CHeader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LxHeader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CString ToString(int    n) { CString Data; Data.Format("%d"  , n); return Data; }
CString ToString(float  n) { CString Data; Data.Format("%.2f", n); return Data; }
CString ToString(double n) { CString Data; Data.Format("%.2f", n); return Data; }

LPSTR ToChar(CString Txt) { return LPSTR(LPCSTR(Txt));}

char* IntToHex(int val, int n) {

   if (n>9) n = 9;  
   char *hexVal = new char[9];
   int root = val;
   int rem;
   for(int i=n-1;i>=0;i--) {
      rem = root%16;
      root /= 16;
      if(rem<10) {
         hexVal[i] = '0' + rem;
      }
      else{
         hexVal[i] = 'A' + rem - 10;
      }
   }
   return hexVal;
}

double GetCount()
{
    LARGE_INTEGER Counter;
    LARGE_INTEGER Frequency;
    QueryPerformanceCounter  (&Counter);
    QueryPerformanceFrequency(&Frequency);
    double Clock = (((double)Counter.QuadPart/(double)Frequency.QuadPart)*1000.0);
    return Clock;
}

