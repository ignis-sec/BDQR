//	Name: Isolate Text From 24 bit BMP

//	Author: Ata Hakcil 
//	Description:Isolate text inside BMP image from the background

#include <stdio.h>																		
#include <stdlib.h>
#include <fstream>
#include <iostream>
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define FILENAME "qr.bmp"																											//
//
#define HEADERSIZE 54																												//																																	
//
#define SHADE 196																													//
#define SHADE_RED 45								//if color is grayscale keep SHADERED/GREEN/BLUE defined as SHADE; change SHADE	//
#define SHADE_BLUE 90																												//
#define SHADE_GREEN 36																												//
//
#define TOLERANCE 50									//change below if you want custom tolerance on each color					//
#define TOLERANCE_RED 100						//just keep it positive																//
#define TOLERANCE_BLUE 50																											//
#define TOLERANCE_GREEN 100																											//
//
#define COLOR_TEXT 0								//color of your text															//
#define COLOR_TEXT_RED COLOR_TEXT																									//
#define COLOR_TEXT_GREEN COLOR_TEXT																									//
#define COLOR_TEXT_BLUE COLOR_TEXT																									//
//
#define COLOR_BG 255								//color of your background														//
#define COLOR_BG_RED COLOR_BG																										//
#define COLOR_BG_GREEN COLOR_BG																										//
#define COLOR_BG_BLUE COLOR_BG																										//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
//BMP architecture
// 54 bytes of header for 24 bit image 
// BM tag image sizes colors definition etc
// *****************VERY IMPORTANT NULL BYTES**********************
//If a line of pixels is not size of 4n, file will be filled with 000 pixels until its size of 4n
//BMP extension has no end file, it instead declares size at the start. Instead i declared 0,255,0 (green) as my EOF
//

int main(void)
{
	std::ofstream output("Output.csv");
	std::ofstream unmask("Unmasked.csv");
	FILE * ioftp;								//pointer to the file
	int row = 1;
	int red, green, blue;					//variables
	int i = 0;						//iterator; starting from 54 because the first 54 bytes are reserved for bitmap headers
	int width;
	int nullCount = 0;
	int ch = 0;						//char variable to catch bytes from filestream

	if (!(ioftp = fopen(FILENAME, "rb+")))			//open file for read and write
	{
		std::cout << "failure\n";
		return EXIT_FAILURE;
	}
	red = 0;										//initializing variables
	green = 0;										// 
	blue = 0;										//


	fseek(ioftp, 18, SEEK_SET);                  //calculate image row size from the headers
	width = (getc(ioftp) + getc(ioftp) * 16 * 16 + getc(ioftp) * 16 * 16 * 16 * 16) * 3;

	fseek(ioftp, 0, SEEK_CUR);
	if (width % 4) nullCount = 4 - width % 4; else nullCount = 0;
	fseek(ioftp, HEADERSIZE, SEEK_SET);						//seek for 54th byte from start   :: SEEK_SET->From Start
	while ((ch = getc(ioftp)) != EOF)				//get char as long as it is	      :: SEEK_CUR->Current	     Also EOF doesn't do anything 
	{												//not end of file                 :: SEEK_END->From End	     here, but might as well stay
		switch (i % 3)				//switch on i%3 to check if it is r,g or b
		{
		case 0:
			blue = ch;
			break;

		case 1:
			green = ch;
			break;

		case 2:										//Color cells in bmp end with red, so logic is applied here. 
			red = ch;

			if (red == 0 && green == 0 && blue == 0)
			{
				std::cout << "black ";
				output << "1;";
				if (row == 7 || i == 20 || (i < 27 && row < 10) || (i > width - 24 && row < 10) || (i<27 && row> width / 3 - 8) || ((i > width - 27 && i < width - 12) && (row< width / 3 - 3 && row> width / 3 - 9)))
					unmask << "1;";
				else if (!(row % 2))
					unmask << "1;";
				else unmask << ";";
			}
			else {
				std::cout << "white ";
				output << ";";
				if (row == 7 || i == 20 || (i<27 && row<10) || (i>width - 24 && row<10) || (i<27 && row> width / 3 - 8) || ((i>width - 27 && i< width - 12) && (row< width / 3 - 3 && row> width / 3 - 9)))
					unmask << ";";
				else if (!(row % 2))
					unmask << ";";
				else unmask << "1;";
			}	
			break;
		}
		if (i == width)
		{
			fseek(ioftp, nullCount, SEEK_CUR);	            //If its the end of the pixel row, skip null bytes
			fseek(ioftp, 0, SEEK_CUR);
			i = 0;											//reset row iteration
			std::cout << "\n";
			output << "\n";
			unmask << "\n";
			row++;
		}
		i++;												//iterate i
	}
	fclose(ioftp);									//close file
	std::cout << "success\n";
	return EXIT_SUCCESS;							//gtfo
}