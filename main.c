#include<stdio.h>
#include <ft2build.h>
#include <stdint.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

void CreateFontLeter(unsigned int width, unsigned int height, unsigned int pitch, unsigned char bitmap[]);
void AltCreateFontLeter(unsigned int width, unsigned int height, unsigned int pitch, unsigned char bitmap[]);

// https://www.fontsquirrel.com/fonts/Metro             fonts download site

int main()
{


	FT_Library  library;
	FT_Face     face;

	int error = FT_Init_FreeType( &library );
	if ( error )
	{
 	// ... an error occurred during library initialization ...
	}


	char fontfile[] = "../fontfiles/LiberationSans-Regular.ttf";
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
          face,    /* handle to face object         */
          0,       /* char_width in 1/64 of points  */
          7*64,   /* char_height in 1/64 of points */ 
          150,     /* horizontal device resolution  */                 // SET FOR OLED 128 X 64 in dpi
          150 );   /* vertical device resolution    */
 
 	if(error)
 	{
 		printf("Unable to set the font size\n");	
	}
 	
 	
 	// Ucitaj indeks glifa koji prethodi prvom slovu nase azbuke a to je u+0401, kada se inkrementira indeks u petlji dolazimo do naseg slova Dj
 	// koje je prvo prvo u skripti https://en.wikipedia.org/wiki/Cyrillic_(Unicode_block)
 	
 	
 	
 	uint32_t ccode = 0x0401;
 	
 	FT_UInt glyph_index = FT_Get_Char_Index(face, ccode);
 	
 	printf("charcode = %d\n", glyph_index);
 	
 	glyph_index = 409;

 	int fontNum = 0;
 	printf("____________________________________________\n");
 	for(fontNum = 0; fontNum < 0x042F - 0x0401; fontNum++)
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
	
	
	glyph_index = FT_Get_Char_Index(face, 0x0411);
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

}


void CreateFontLeter(unsigned int width, unsigned int height, unsigned int pitch, unsigned char bitmap[])
{
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
		printf("0x%02x, ", byteArray[i++]);
	}
	printf("\n");
	
}

void AltCreateFontLeter(unsigned int width, unsigned int height, unsigned int pitch, unsigned char bitmap[])
{
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
j = 1;
//row = pitch * 8;
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
row = pitch * 8;
height = height/8;

}while(height);
	
	i = 0;
	
	
	
	while(i < 47)    // i < 8
	{
		printf("0x%02x, ", byteArray[i++]);
	}
	printf("\n");
}
