#include "printpacket.h"

const static int BUF_SIZE = 40032;

char  hexTable[16] =
  { '0','1','2','3','4','5','6','7',
    '8','9','A','B','C','D','E','F' };

char hexTmp[3];

char* hexByte ( unsigned char z )

{
	hexTmp[0] = hexTable[z >>  4];
	hexTmp[1] = hexTable[z &  15];
	hexTmp[2] = 0;
	return &hexTmp[0];
}

char* hexWord ( unsigned short z )

{
	unsigned short y = z >> 8;

	hexTmp[0] = hexTable[y >>  4];
	hexTmp[1] = hexTable[y &  15];

	y = z << 8; y >>= 8;

	hexTmp[2] = hexTable[y >>  4];
	hexTmp[3] = hexTable[y &  15];
	hexTmp[4] = 0;

	return &hexTmp[0];
}

char* hexDword ( unsigned z )

{
	unsigned y = z >> 24;

	hexTmp[0] = hexTable[y >>  4];
	hexTmp[1] = hexTable[y &  15];

	y = z << 8; y >>= 24;

	hexTmp[2] = hexTable[y >>  4];
	hexTmp[3] = hexTable[y &  15];

	y = z << 16; y >>= 24;

	hexTmp[4] = hexTable[y >>  4];
	hexTmp[5] = hexTable[y &  15];

	y = z << 24; y >>= 24;

	hexTmp[6] = hexTable[y >>  4];
	hexTmp[7] = hexTable[y &  15];
	
	hexTmp[8] = 0;

	return &hexTmp[0];
}


void display_packet ( char *buf, int len, int start_, bool isinhex)
{
	int c,c2,c3;
	unsigned char c4;
	int it,god,damn;
#define display_size BUF_SIZE*7
	char pkt_hex[display_size];
	char work_with[display_size];
	char side_line[255];
	char middle_line[255];
	unsigned linerr = start_;
	int jrr = 0;
	int lastrr = 0;
	int xyor = 0;

	memset( &middle_line[0], '\0', 255 );
	memset( &side_line[0], '\0', 255 );
	memset( &work_with[0], '\0', display_size );
	
	/*
    j  = sprintf( buffer,     "   String:    %s\n", s );
    j += sprintf( buffer + j, "   Character: %c\n", c );
    j += sprintf( buffer + j, "   Integer:   %d\n", i );
    j += sprintf( buffer + j, "   Real:      %f\n", fp );

	*/

 
	c2 = c3 = 0;
	for (c=0;c<len;c++)
	{
		c4 = buf[c];
		if (c2==16)
		{
			side_line[ lastrr ] = 0x00;
			lastrr = 0;

			if( !isinhex )
			{
				if( linerr < 10 )
					jrr += sprintf( &work_with[jrr], "0000%u| %s |%s\n", linerr, middle_line, side_line );
				else if( linerr < 100 )
					jrr += sprintf( &work_with[jrr], "000%u| %s |%s\n", linerr, middle_line, side_line );
				else if( linerr < 1000 )
					jrr += sprintf( &work_with[jrr], "00%u| %s |%s\n", linerr, middle_line, side_line );
				else if( linerr < 10000 )
					jrr += sprintf( &work_with[jrr], "0%u| %s |%s\n", linerr, middle_line, side_line );
				else
					jrr += sprintf( &work_with[jrr], "%u| %s |%s\n", linerr, middle_line, side_line );
			}
			else
				jrr += sprintf( &work_with[jrr], "%s| %s |%s\n", hexDword( linerr ), middle_line, side_line );

			linerr += 16;

			pkt_hex[c3++] = 0x0A;
			c2 = 0;

			memset( &side_line[0], '\0', 255 );
			memset( &middle_line[0], '\0', 255 );
			xyor = 0;
		}

		if( (unsigned int) c4 < 0x20 )
			side_line[ lastrr ] = '.';
		else
			side_line[ lastrr ] = c4;

		lastrr++;

	    middle_line[xyor++] = pkt_hex[c3++] = hexTable[c4 >> 4];
	    middle_line[xyor++] = pkt_hex[c3++] = hexTable[c4 & 15];
	    middle_line[xyor++] = pkt_hex[c3++] = 0x20;

		c2++;

		if( c2 == 16 )
			middle_line[xyor++] = 0x00;

	}
	pkt_hex[c3++] = 0x0A;
	pkt_hex[c3++] = 0x00;

	if( !isinhex )
	{
		if( linerr < 10 )
			jrr += sprintf( &work_with[jrr], "0000%u| %s ", linerr, middle_line );
		else if( linerr < 100 )
			jrr += sprintf( &work_with[jrr], "000%u| %s ", linerr, middle_line );
		else if( linerr < 1000 )
			jrr += sprintf( &work_with[jrr], "00%u| %s ", linerr, middle_line );
		else if( linerr < 10000 )
			jrr += sprintf( &work_with[jrr], "0%u| %s ", linerr, middle_line );
		else
			jrr += sprintf( &work_with[jrr], "%u| %s ", linerr, middle_line );
	}
	else
		jrr += sprintf( &work_with[jrr], "%s| %s |%s\n", hexDword( linerr ), middle_line, side_line );

	//j += sprintf( &work_with[j], "0%u| %s |%s\n", line, middle_line, side_line );

	it = len % 16;
	god = it;
	if( it != 0 )
	{
		it = ( 16 - god );
		for( damn = 0; damn < it; damn ++ )
		{
			jrr += sprintf( &work_with[jrr], "   " );
		}
	}

	jrr += sprintf( &work_with[jrr], "|%s\n", side_line );

	printf("%s", &work_with[0] );
}

void pkt_dump(char *buf, int len)
{
    display_packet(buf, len, 0, false);
}
