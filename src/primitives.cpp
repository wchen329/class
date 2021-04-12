#include "primitives.h"

namespace priscas
{

	UPString dynamicHexBuilder(uint8_t* ui, size_t count)
	{
		UPString ret = "";
		UPString interm = "";

		int ind = 0;
		int tbl = 0;

		while(ind < count)
		{
			char val = (ui[ind] >> tbl) & 0x0F;

			switch(val)
			{
				case 0:
					interm = "0" + interm;
					break;
				case 1:
					interm = "1" + interm;
					break;
				case 2:
					interm = "2" + interm;
					break;
				case 3:
					interm = "3" + interm;
					break;
				case 4:
					interm = "4" + interm;
					break;
				case 5:
					interm = "5" + interm;
					break;
				case 6:
					interm = "6" + interm;
					break;
				case 7:
					interm = "7" + interm;
					break;
				case 8:
					interm = "8" + interm;
					break;
				case 9:
					interm = "9" + interm;
					break;
				case 10:
					interm = "a" + interm;
					break;
				case 11:
					interm = "b" + interm;
					break;
				case 12:
					interm = "c" + interm;
					break;
				case 13:
					interm = "d" + interm;
					break;
				case 14:
					interm = "e" + interm;
					break;
				case 15:
					interm = "f" + interm;
					break;
			};


			tbl += 4;
			
			if(tbl == 8)
			{
				ind++;
				tbl = 0;
			}
		}

		return ret + interm;
	}
}
