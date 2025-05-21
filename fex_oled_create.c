
// Dont include these, instead create custom datatype specified for oled create with width height pitch and bitmap fields

#include <ft2build.h>
#include <stdint.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

void fex_oled_createOLED1306character(FT_Face face, unsigned char* fontData, int* fontDataIndex, int greatestChar)
{
	
	unsigned int width  = face->glyph->metrics.width/64;
 int height = face->glyph->metrics.height/64;
	unsigned int pitch  = face->glyph->bitmap.pitch;
	unsigned char* bitmap = (unsigned char*)face->glyph->bitmap.buffer;
	// Introduce FILENAME as parameter and capitalize letter to create header file
	
	
		printf("Width: %d Height: %d\n", width, height);
	unsigned char byteArray[500];
	
	int i=0, j= 0;
	int row = 0;
	for(i = 0; i < 500; i++)
	{
		byteArray[i]= 0;
	}
	i = 0;

	int k = 0;
	
	unsigned char bmpBuff[4*32];
	
	for(k = 0; k < 4*32; k++)
	{
		bmpBuff[k] = 0;
	}
	
	k = 0;
	
	while(k < pitch * height)
	{
		bmpBuff[k] = bitmap[k];
		k++;
	}

k = 0;	
int m = 0;
int jmax = 0;	
// LOAD 8 bytes
byteArray[0] = width;

int auxHeight = height;
int oledHeight = 0;

while(auxHeight > 0)
{
	auxHeight -= 8;
	oledHeight++;
}





byteArray[1] = oledHeight;

j = 2;                                // Izmenjeno za visinu bilo je 1
//row = pitch * 8;
int vrsta = 0; // kada se obradi 8 osam vrsta bit po bit napravljen je jedan stupac visine za OLED, 
do
{
	while(k < width)
	{
		
		if(bmpBuff[row] & (0x80 >> m))                          // One byte, jedan stupac visine
		byteArray[j] |= 0x80;
		byteArray[j] >>= 1;
		
		if(bmpBuff[row+pitch] & (0x80 >> m))
		byteArray[j] |= 0x80;
		byteArray[j] >>= 1;
		
		if(bmpBuff[row+2*pitch] & (0x80 >> m))
		byteArray[j] |= 0x80;
		byteArray[j] >>= 1;
		
		if(bmpBuff[row+3*pitch] & (0x80 >> m))
		byteArray[j] |= 0x80;
		byteArray[j] >>= 1;
		
		if(bmpBuff[row+4*pitch] & (0x80 >> m))
		byteArray[j] |= 0x80;
		byteArray[j] >>= 1;
		
		if(bmpBuff[row+5*pitch] & (0x80 >> m))
		byteArray[j] |= 0x80;
		byteArray[j] >>= 1;
		
		if(bmpBuff[row+6*pitch] & (0x80 >> m))
		byteArray[j] |= 0x80;
		byteArray[j] >>= 1;
		
		if(bmpBuff[row+7*pitch] & (0x80 >> m))                  // Stupac napravljen
		byteArray[j] |= 0x80;
	
		// 8 rows processed here, processed bit by bit and stored as height stupac
		
		j++;
		m++;
		k++;
		if((k % 8) == 0)
		{
			// Load next 8 bytes
			row += 1;
			m = 0;
		}
		
		
	}
	
	
	k = 0;
	m = 0;
	vrsta++;
row = pitch * 8 * vrsta;
height = height-8;

}while(height > 0);
	
	i = 0;
	
	
	
//	while(i < 47)    // i < 8 
		while(i < greatestChar)    // i < 8
	{
		printf("0x%02x, ", byteArray[i]);
		fontData[*fontDataIndex] = byteArray[i];
		(*fontDataIndex)++;
		
		i++;
	}
	printf("\n");
	
}
