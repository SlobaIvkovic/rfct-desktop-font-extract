/*
	This program was initialy developed as a helper formating tool for the PIC16F(76) OLED driver,
	The idea was to extract the characters bitmaps from some UTF8 range (Cyrillic was done first) and then format them into an array to be printed on the 128 * 64 OLED,
	Freetype2 library is the main prereqizite, it is used to open font files (.ttf, .otf) and to load characters, various info about the font itself can be retrieved using this library.
	
	The program itself comprises from two main parts which can be easily divided into two separate programs, first one utilizing Freetype2 to extract the desired font data,
	and the second one which formats the extracted data to be printed on the OLED. OLED controller displays characters in a special way where each byte sent to the controller to be 
	printed is printed verticaly from top left top of the screen as starting position. Char bitmap is extracted as series of bytes representing horizontal rows of the char so these
	bytes are processed bit by bit to create a map suitable for the OLED where each byte represents 8bit vertical column to be printed.   

*/

#include<stdio.h>
#include <ft2build.h>
#include <stdint.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "fex_oled_create.h"

void CreateFontLeter(unsigned int width, unsigned int height, unsigned int pitch, unsigned char bitmap[]);
void AltCreateFontLeter(unsigned int width, int height, unsigned int pitch, unsigned char bitmap[]);

void fex_fontOpen(FT_Library* library, FT_Face* face);
void fex_fontInfo(FT_Library library, FT_Face face);

// Pro Tip
// If you're working in pixel units and want precise control (e.g. for pixel-perfect UIs), you might use FT_Set_Pixel_Sizes() instead — that sets size directly in pixels, not points.
int fex_setFontSize(FT_Library library, FT_Face face, int height, int horizontalDPI, int verticalDPI);

int fex_renderFont(FT_Library library, FT_Face face, uint32_t firstCharCode, unsigned fontRangeStart, unsigned fontRangeEnd);

int fex_renderSingleGlyph(FT_Library library, FT_Face face, uint32_t ccode);

// https://www.fontsquirrel.com/fonts/Metro             fonts download site

int main()
{


	FT_Library  library;
	FT_Face     face;
	
	unsigned char byteArray[500];
	unsigned int byteArrayIndex = 0;

	fex_fontOpen(&library, &face);
	
	fex_fontInfo(library, face);
	
	fex_setFontSize(library, face, 10, 150, 150);
	
	
	
	unsigned char fontData[500];
	int fontDataIndex = 0;
	memset(fontData, 0, sizeof(fontData));
	uint32_t i = 0;
	uint32_t ccode = 0x0402;
	for(i = 0; i < 0x0430 - 0x0402; i++)
	{
		fex_renderSingleGlyph(library, face, ccode);
		ccode++;
		fex_oled_createOLED1306character(face, fontData, &fontDataIndex);
//		AltCreateFontLeter(face->glyph->metrics.width/64, face->glyph->metrics.height/64, face->glyph->bitmap.pitch, (unsigned char*)face->glyph->bitmap.buffer);
	}
	
	printf("**********************************************************");
	int j = 0;
	i=0;
	for(i = 0; i < 0x0430-0x0402; i++)
	{
		printf("0x%02x", fontData[i]);
		if(!(i%46))
		{
			printf("\n");
		}
	}
	
//	fex_renderFont(library, face, 0x0401, 0x0401, 0x042f);
	
	
	
	FT_Done_Face    ( face );
    FT_Done_FreeType( library );

/*	int error = FT_Init_FreeType( &library );
	if ( error )
	{
 	// ... an error occurred during library initialization ...
	}


	char fontfile[] = "../fontfiles/Coolvetica_Rg_Cond.otf";
//                    "cmunbtl.ttf",	
	error = FT_New_Face( library,
//	"fontfiles/cmunbtl.ttf",
// "fontfiles/troika.otf",
// "fontfiles/Metro.otf",
// "fontfiles/Play-Regular.ttf",
// "fontfiles/coolvetica_rg.ttf",
// "fontfiles/okolaksRegular.ttf",
	fontfile,
                     0,
                     &face );
	if ( error == FT_Err_Unknown_File_Format )
	{
  	//	... the font file could be opened and read, but it appears
  	//	... that its font format is unsupported
  		printf("ERROR 1\n Font file opened and read but its font format is unknown or unsupported");
	}
	else if ( error )
	{
  	//	... another error code means that the font file could not
  	//	... be opened or read, or that it is broken...
  		printf("ERROR 2\n Cannot open file: %s", fontfile);
	}
	else
	{
		printf("Font opened\n");
	}
	
	printf("Number of glyphs in the font file: %d\n", face->num_glyphs);
	printf("Number of fixed sizes in the font file: %d\n", face->num_fixed_sizes);
	printf("Number of faces in the font file: %d\n", face->num_faces);
	printf("Number of fixed sizes in the font file: %d\n", face->num_fixed_sizes);
	
	printf("Family name: %s\n", face->family_name);
	printf("Style: %s\n", face->style_name);
	
	printf("Charmap platform: %d encoding: %d\n", face->charmap->platform_id, face->charmap->encoding_id);
	
	printf("Bit box: %d\n", face->bbox);
// 	printf("Available sizes in the font file: %d\n", face->available_sizes[0].height);

	error = FT_Set_Char_Size(
          face,    // handle to face object         
          0,       // char_width in 1/64 of points  
          10*64,   // char_height in 1/64 of points              // bilo je 7
          150,     // horizontal device resolution                   // SET FOR OLED 128 X 64 in dpi
          150 );   // vertical device resolution    
 
 	if(error)
 	{
 		printf("Unable to set the font size\n");	
	}
 	
 	
 	// Ucitaj indeks glifa koji prethodi prvom slovu nase azbuke a to je u+0401, kada se inkrementira indeks u petlji dolazimo do naseg slova Dj
 	// koje je prvo prvo u skripti https://en.wikipedia.org/wiki/Cyrillic_(Unicode_block)
 	
 	
 	
 	uint32_t ccode = 0x0401;     // Za srpski
//	uint32_t ccode = 0x0620;     // Za arapsk
 	
 	FT_UInt glyph_index = FT_Get_Char_Index(face, ccode);
 	
 	printf("charcode = %d\n", glyph_index);
 	
 	glyph_index = 409;

 	int fontNum = 0;
 	printf("____________________________________________\n");
 	for(fontNum = 0; fontNum < 0x042F - 0x0401; fontNum++)     // Za srpski
//	for(fontNum = 0; fontNum < 0x065F - 0x0621; fontNum++)
 	{
 	
 		// sada smo na Dj
// 		glyph_index++;
 	
		ccode++;
		glyph_index = FT_Get_Char_Index(face, ccode);
		
 		error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT );  
	
	
		error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_MONO);
		if(error)
		{
			printf("Rendering failed\n");
		}
	
//	CreateFontLeter(face->glyph->metrics.width/64, face->glyph->metrics.height/64, face->glyph->bitmap.pitch, (unsigned char*)face->glyph->bitmap.buffer);
	AltCreateFontLeter(face->glyph->metrics.width/64, face->glyph->metrics.height/64, face->glyph->bitmap.pitch, (unsigned char*)face->glyph->bitmap.buffer);
	
	}
	printf("_____________________________________________\n");
	
	
	glyph_index = FT_Get_Char_Index(face, 0x0402);
	error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT );
	
	FT_Glyph  glyph;
	FT_BBox  bbox;
	FT_Get_Glyph( face->glyph, &glyph );
	FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_TRUNCATE, &bbox );
	
	printf("bbxXmin %d bbxYmin %d,  bbxXmax %d bbxYmax %d\n", bbox.xMin, bbox.yMin, bbox.xMax, bbox.yMax);
	
	
	printf("___________Metrics part__________________\n");
	printf("Glyph width: %d\n", face->glyph->metrics.width/64);
	printf("Glyph height: %d\n", face->glyph->metrics.height/64);
	printf("Bearing x: %d\n", face->glyph->metrics.horiBearingX/64);
	printf("Advance: %d\n", face->glyph->metrics.horiAdvance/64);
	
	printf("BM left: %d\n", face->glyph->bitmap_left/64);
	printf("BM top: %d\n", face->glyph->bitmap_top/64);
	
	printf("__________________________________________\n");
	
	
	
	error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_MONO);
	CreateFontLeter(face->glyph->metrics.width/64, face->glyph->metrics.height/64, face->glyph->bitmap.pitch, (unsigned char*)face->glyph->bitmap.buffer);
	
	
	FT_GlyphSlot  slot;
	
	slot = face->glyph;
	
	printf("BM rows %d BM pixels %d BM pitch %d\n", slot->bitmap.rows, slot->bitmap.width, slot->bitmap.pitch);
	
	
	int i, j, k = 0;
	for(i = 0; i < 8; i++)
	{
		for(j = 0; j < 2; j++)
		{
		//	printf("0x%02x, ", slot->bitmap.buffer[k++]);
		}
//		printf("\n");
		
	}
	FT_Done_Face    ( face );
  FT_Done_FreeType( library );
*/
}


void CreateFontLeter(unsigned int width, unsigned int height, unsigned int pitch, unsigned char bitmap[])
{
	FILE* fp;
	fp = fopen("../fontincludefiles/genericfont.h", "w");
	
	// Introduce FILENAME as parameter and capitalize letter to create header file
	
	fprintf(fp, "#ifndef THIS_HFILE_H\n#define THIS_HFILE_H\n#endif\n\n");
	fprintf(fp, "const unsigned char font[] = {\n");
	
//	printf("Width: %d Height: %d\n", width, height);
	unsigned char byteArray[500];
	
	int i=0, j= 0;
	int row = 0;
	for(i = 0; i < 500; i++)
	{
		byteArray[i]= 0;
	}
	i = 0;


	while(j < width)
	{
		
		if(bitmap[row] & (0x80 >> j))                          // One byte, jedan stupac visine
		byteArray[j] |= 0x80;
		byteArray[j] >>= 1;
		
		if(bitmap[row+pitch] & (0x80 >> j))
		byteArray[j] |= 0x80;
		byteArray[j] >>= 1;
		
		if(bitmap[row+2*pitch] & (0x80 >> j))
		byteArray[j] |= 0x80;
		byteArray[j] >>= 1;
		
		if(bitmap[row+3*pitch] & (0x80 >> j))
		byteArray[j] |= 0x80;
		byteArray[j] >>= 1;
		
		if(bitmap[row+4*pitch] & (0x80 >> j))
		byteArray[j] |= 0x80;
		byteArray[j] >>= 1;
		
		if(bitmap[row+5*pitch] & (0x80 >> j))
		byteArray[j] |= 0x80;
		byteArray[j] >>= 1;
		
		if(bitmap[row+6*pitch] & (0x80 >> j))
		byteArray[j] |= 0x80;
		byteArray[j] >>= 1;
		
		if(bitmap[row+7*pitch] & (0x80 >> j))                  // Stupac napravljen
		byteArray[j] |= 0x80;
	
		
		j++;	
	}

	
	i = 0;
	
	while(i < 8)    // i < 8
	{
		printf("0x%02x, ", byteArray[i]);
		fprintf(fp, "%s", byteArray[i]);
		i++;
	}
	printf("\n");
	fprintf(fp, "\n");
	fprintf(fp, "};\n");
	
}

void AltCreateFontLeter(unsigned int width, int height, unsigned int pitch, unsigned char bitmap[])
{
	FILE* fp;
	fp = fopen("../fontincludefiles/genericfont.h", "w");
	if(fp == NULL)
	{
		printf("error opening fontfile");
	}
	
	// Introduce FILENAME as parameter and capitalize letter to create header file
	
	fprintf(fp, "#ifndef THIS_HFILE_H\n#define THIS_HFILE_H\n\n");
	fprintf(fp, "const unsigned char font[] = {\n");
	//	printf("Width: %d Height: %d\n", width, height);
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
		while(i < 47)    // i < 8
	{
		printf("0x%02x, ", byteArray[i]);
		fprintf(fp, "0x%02x, ", byteArray[i]);
		i++;
	}
	printf("\n");
	fprintf(fp, "\n");
	fprintf(fp, "};\n");
	fprintf(fp,"#endif\n\n");
	fclose(fp);
}



// Must use pointer (of pointers acctually) for the initialization, FT_ variables must be initialized on their exact address
void fex_fontOpen(FT_Library* library, FT_Face* face)
{

	int error = FT_Init_FreeType( library );
	if ( error )
	{
 	// ... an error occurred during library initialization ...
	}


	char fontfile[] = "../fontfiles/Coolvetica_Rg_Cond.otf";
	
	error = FT_New_Face( *library, fontfile, 0, face );
	if ( error == FT_Err_Unknown_File_Format )
	{
  	//	... the font file could be opened and read, but it appears
  	//	... that its font format is unsupported
  		printf("ERROR 1\n Font file opened and read but its font format is unknown or unsupported");
	}
	else if ( error )
	{
  	//	... another error code means that the font file could not
  	//	... be opened or read, or that it is broken...
  		printf("ERROR 2\n Cannot open file: %s", fontfile);
	}
	else
	{
		printf("Font opened\n");
	}
	

	return;
}


void fex_fontInfo(FT_Library library, FT_Face face)
{
	printf("Number of glyphs in the font file: %d\n", face->num_glyphs);
	printf("Number of fixed sizes in the font file: %d\n", face->num_fixed_sizes);
	printf("Number of faces in the font file: %d\n", face->num_faces);
	printf("Number of fixed sizes in the font file: %d\n", face->num_fixed_sizes);
	
	printf("Family name: %s\n", face->family_name);
	printf("Style: %s\n", face->style_name);
	
	printf("Charmap platform: %d encoding: %d\n", face->charmap->platform_id, face->charmap->encoding_id);
	
	printf("Bit box: %d\n", face->bbox);
// 	printf("Available sizes in the font file: %d\n", face->available_sizes[0].height);
}


/*
	0.96inch OLED	Width: 21.74 mm ÷ 25.4 ≈ 0.856 inches

					Height: 11.18 mm ÷ 25.4 ≈ 0.440 inches
					
					
	FreeType uses 26.6 fixed-point format
	This means values are stored as integers, but represent fractional numbers.

	Specifically:

	26 bits are used for the integer part

	6 bits are for the fraction

	So: 1.0 is represented as 1 << 6 = 64

	2.5 → 2.5 * 64 = 160

	10.0 → 10 * 64 = 640
	
	That is why height value in POINTS is multiplied by 64, to get the exact size in POINTS
	In typography, a "point" is a traditional unit of measurement:
	1 point = 1/72 inch So when we say "10pt" at 96 DPI FreeType calculates how many pixels that should be: 10 pt × (96 pixels/inch) / (72 points/inch) = 13.33 pixels
	That pixel value becomes the height of the rendered bitmap (more or less — actual size may vary due to hinting).
	
	No need to set up width if the height is set the library will calculate the width automatically


					
*/
	

int fex_setFontSize(FT_Library library, FT_Face face, int height, int horizontalDPI, int verticalDPI)
{
	int error;
	error = FT_Set_Char_Size(
          face,    // handle to face object         
          0,       // char_width in 1/64 of points  
          height*64,   // char_height in 1/64 of points              // 
          horizontalDPI,     // horizontal device resolution                   // OLED 128 * 64 is H 133 and V 67 in dots per inch DPI 128/0.96inch 64/0.
          verticalDPI );   // vertical device resolution    
 
 	if(error)
 	{
 		printf("Unable to set the font size\n");
		return error;	
	}
	else
	{
		printf("Font size set sucessfully\n");
		return error;
	}
}


int fex_renderFont(FT_Library library, FT_Face face, uint32_t firstCharCode, unsigned fontRangeStart, unsigned fontRangeEnd)
{
	int error;
	
	
 	// Ucitaj indeks glifa koji prethodi prvom slovu nase azbuke a to je u+0401, kada se inkrementira indeks u petlji dolazimo do naseg slova Dj
 	// koje je prvo prvo u skripti https://en.wikipedia.org/wiki/Cyrillic_(Unicode_block)
 	
 	
 	uint32_t ccode = firstCharCode;
// 	uint32_t ccode = 0x0401;     // Za srpski
//	uint32_t ccode = 0x0620;     // Za arapsk
 	
 	FT_UInt glyph_index = FT_Get_Char_Index(face, ccode);
 	
 	printf("charcode = %d\n", glyph_index);
 	
 	glyph_index = 409;

 	int fontNum = 0;
 	printf("____________________________________________\n");
 	for(fontNum = 0; fontNum < fontRangeEnd - fontRangeStart; fontNum++)     // Za srpski
//	for(fontNum = 0; fontNum < 0x065F - 0x0621; fontNum++)
 	{
 	
 		// sada smo na Dj
// 		glyph_index++;
 	
		ccode++;
		glyph_index = FT_Get_Char_Index(face, ccode);
		
 		error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT );  
	
	
		error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_MONO);
		if(error)
		{
			printf("Rendering failed\n");
			return error;
		}
		AltCreateFontLeter(face->glyph->metrics.width/64, face->glyph->metrics.height/64, face->glyph->bitmap.pitch, (unsigned char*)face->glyph->bitmap.buffer);
	}
	printf("Render success\n");
	return error;
}

int fex_renderSingleGlyph(FT_Library library, FT_Face face, uint32_t ccode)
{
	int error;
	    ccode;
	    FT_UInt glyph_index = FT_Get_Char_Index(face, ccode);
	    
	//	glyph_index = FT_Get_Char_Index(face, ccode);
		
 		error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT );  
	
	
		error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_MONO);
		if(error)
		{
			printf("Rendering failed\n");
			return error;
		}
}

void fex_oled_CreateFontFile()
{
/*	fprintf(fp, "0x%02x, ", byteArray[i]);
		i++;
	}
	printf("\n");
	fprintf(fp, "\n");
	fprintf(fp, "};\n");
	fprintf(fp,"#endif\n\n");
	fclose(fp);*/
}
// Grčki Alfabet
// Gruzijski Modern (Mkherduli)
// Jevrejski
// Arapski
