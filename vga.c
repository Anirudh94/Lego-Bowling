
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
//name  of bmp file

const char FileName[50];
void VGAdrawvertline(int length, int xorg, int yorg, short colour);
void VGAdrawhorzline(int length, int xorg, int yorg, short colour);
void drawArrayofBoxes(int x,int y, int sidelength,short colour);
void drawBox(int x, int y, int sidelength, short colour);
void drawfilledBox(int x, int y, int lengthx, int lengthy,  short colour);
void blink(int);
void write_tomainscore(int score);
#define endofscreenx 320
#define endofscreeny 240
#define pixelbufferx 80
#define pixelbuffery 60

//int player;



//14 bytes
typedef struct tagBITMAPFILEHEADER
{
    //2 bytes
    char  bfType[2];
    //4 bytes
    unsigned int bfSize;
    // 2 bytes
    unsigned short int  bfReserved1;
    // 2 bytes
    unsigned short int  bfReserved2;
    // 4 bytes
    unsigned int bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

//40 bytes

typedef struct tagBITMAPINFOHEADER
{

// 4 bytes
    unsigned int biSize;
    // 4 bytes
    int  biWidth;
    // 4 bytes
    int  biHeight;
    // 2 bytes
    unsigned short int biPlanes;
    // 2 bytes
    unsigned short int biBitCount;
    // 4 bytes
    unsigned int biCompression;
    // 4 bytes
    unsigned int biSizeImage;
    // 4 bytes
    int biXPelsPerMeter;
    // 4 bytes
    int  biYPelsPerMeter;
    // 4 bytes
    unsigned int biClrUsed;
    // 4 bytes
    unsigned int biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

//colour info is afterwards
//functional prototypes
typedef struct
{
    // 16-bits
    short colour;
}
Colours;



//global variables
int height,width;

//check if it's a valid BMP 16-bit, as well as read the attributes data
void validBMP(FILE * file);


// retrieve height,width from info header
BITMAPINFOHEADER retrieveInfo(FILE * file);

//creates the matrix and dynamically allocate it
Colours ** createMatrix();

//
void loadImage(FILE * file, Colours ** matrix);
void VGAwriteline(char * str, int xpos,int ypos);



//write bmp pixel data to matrix
//void writeBMP(FILE * file, Colours ** matrix,BITMAPFILEHEADER);

/* set a single pixel on the screen at x,y
 * x in [0,319], y in [0,239], and colour in [0,65535]
 */

void write_pixel(int x, int y, short colour) {
  volatile short *vga_addr=(volatile short*)(0x08000000 + (y<<10) + (x<<1));
  *vga_addr=colour;
}

/* use write_pixel to set entire screen to black (does not clear the character buffer) */
void clear_screen() {
  int x, y;
  for (x = 0; x < 320; x++) {
    for (y = 0; y < 240; y++) {
	  write_pixel(x,y,0);
	}
  }
}

/* write a single character to the character buffer at x,y
 * x in [0,79], y in [0,59]
 */
void write_char(int x, int y, char c) {
  // VGA character buffer
  volatile char * character_buffer = (char *) (0x09000000 + (y<<7) + x);
  *character_buffer = c;
}

void write_line(char * str, int x,int y)
{
int i;
for(i=0; i<strlen(str);i++)
{
write_char(x+i,y,str[i]);

}

}
void write_tomainscore(int score)
{
    write_score(20,52,score);
}

void write_score(int x, int y, int score)
{
	char string[4];
	sprintf(string,"%d",score);

	write_line(string,x,y);

}

void draw_linehorz(int x, int y,int length, short colour)
{
int i=0;
for(i=0; i<length; i++)
{
 write_pixel(x+i,y,colour);



}

}
void draw_linevert(int x, int y,int length, short colour)
{
int i=0;
for(i=0; i<length; i++)
{
 write_pixel(x,y+i,colour);



}

}

void initialization()
{

int horizontal_offset=0;
int vert_offset=130;
clear_screen();

//write_line("Score:",1,2+50);
//write_line("Round:",1,3+50);
draw_linehorz(horizontal_offset,16+vert_offset,300,0xf800);
draw_linehorz(horizontal_offset,32+vert_offset,300,0xf800);
int i;

for(i=0; i<=300; i=i+30)
{
draw_linevert(i+horizontal_offset,16+vert_offset,35,0xf800);


}







// /****VARIABLES*********/

    // FILE * bmpfile;
    // Colours ** pixeldata;
    // BITMAPINFOHEADER info;
    // BITMAPFILEHEADER head;


    // //check if file exists

    // bmpfile= fopen("s.bmp","r+b");
    // if(!bmpfile)
	// {
        // printf("Bmp File Does Not Exist. ");
        // return 0;
    // }

    // // find starting point
    // fseek(bmpfile, 0,0);
    // // determine if valid bmp and 16-bits
    // validBMP(bmpfile);
// // retrieve dimensions info from header
   // info=retrieveInfo(bmpfile);
    // // set global variables to make available in all functions
    // height= info.biHeight;
    // width= info.biWidth;


    // pixeldata=createMatrix();

    // //loadImage(bmpfile,pixeldata);


        // int k,j;
        // Colours tmp;
        // //offset position
        // long pos = 52;

        // fseek(bmpfile,0,0);

        // for (j=0; i<height; i++){
                // for (k=0; j<width; j++){
                        // pos+= 2;
                        // fseek(bmpfile,pos,0);
                        // fread(&tmp,(sizeof(Colours)),1,bmpfile);

                        // pixeldata[j][k] = tmp;

                // }
        // }
		// write_pixel(25,25,pixeldata[4][4].colour);
		// drawbackgroundimage(pixeldata);

		drawArrayofBoxes((endofscreenx-20)/2,40,16,0xf800);
		//updatepins(10,2);
		//player 1

		drawfilledBox(0,0,50,10,0xf800);
		
		//player 2
		//write_line("Player 1",0,0);
		//write_line("Player 2",pixelbufferx-50/4,0);




}

Colours ** createMatrix()
{

    Colours** Matrix=(Colours**)malloc(sizeof(Colours*)*height);



    //for loop ctr
    int i;
    for(i=0; i<height; i++)
    {

        Matrix[i]= (Colours *)malloc(sizeof(Colours*)*width);

    }
    return Matrix;

}

//BITMAPINFOHEADER retrieveInfo(FILE * file)
//{
//    BITMAPINFOHEADER info;
//    // read image width
//    fseek(file, 18, 0);
//
//    fread(&info.biWidth,1,4,file);
//    //printf("%d\n", info.biWidth);
//
//    // read image height
//    fseek(file,22, 0);
//
//
//
//    fread(&info.biHeight,1,4,file);
//    // printf("%d\n", info.biHeight);
//
//    // Image size in bytes
//    fseek(file,34,0);
//    fread(&info.biSizeImage,1,4,file);
//    printf("%d\n", info.biSizeImage);
//
//    return info;
//
//}

//void validBMP(FILE * file)
//{
//
//    char type[3];
//    unsigned short bit;
//    //assign starting position to check HEADER
//    fseek(file,0,0);
//
//    //read first two bytes
//    fread(type, 1,2, file);
//
//    //since fread does not insert null terminating character, must insert manually
//
//    type[2]='\0';
//    //
//    if (strcmp(type,"BM")==0)
//    {
//        printf("The file is  BMP format\n");
//
//    }
//    else
//    {
//
//
//        printf("Error, not a BMP");
//        exit(0);
//    }
//
//
//
//// 28 bytes offset
//    fseek(file,28,0);
//    fread(&bit,1,2, file);
//    if(bit!= 16)
//    {
//        printf("The file is not 16-bit format \n");
//        exit(0);
//    }
//
//
//
//}
void drawbackgroundimage(Colours ** pixels)
{

    int i,j;
    int offsetx=25;
    int offsety=25;

    for(i=0;i<height;i++)
    {
        for(j=0;j<width;j++)
        {


        write_pixel(offsetx+j,offsety+i,pixels[i][j].colour);
        }
    }
}

void drawBox(int x, int y, int sidelength, short colour)
{
    int i;
    for(i=0; i<=sidelength; i++)
    {

        write_pixel(x,y+i,colour);
        write_pixel(x+i,y,colour);
        write_pixel(x+sidelength,y+i,colour);
        write_pixel(x+i,y+sidelength,colour);
    }



}
void drawArrayofBoxes(int x,int y, int sidelength,short colour)
{
    int i;

    //draw 1st row
    for(i=0;i<4; i++)
    {

        drawBox(x+sidelength*i,y,sidelength,colour);

    }
    // draw 2nd row
    int offset;
    offset=(4*sidelength-3*sidelength)/2;

    for(i=0;i<3;i++)
    {

        drawBox(x+i*sidelength+offset,y+sidelength,sidelength,colour);

    }
     offset=offset+(3*sidelength-2*sidelength)/2;

    for(i=0;i<2;i++)
    {

        drawBox(x+i*sidelength+offset,y+sidelength*2,sidelength,colour);

    }
    offset=offset+(2*sidelength-sidelength)/2;
     for(i=0;i<1;i++)
    {

        drawBox(x+i*sidelength+offset,y+sidelength*3,sidelength,colour);

    }


}

void drawX(int x, int y, int side, short colour)

{
    int i;
    for(i=0; i<=side; i++)
    {
        write_pixel(x+i,y+i,colour);//makes backslash shape
        write_pixel(x+i,y+side-i, colour);//makes slash shape

    }


}
void updatepins(int score, int round)
{
    int sizeofBox=16;
    int sizeofXs=5;
    int rowoffset2=(4*sizeofBox-3*sizeofBox)/2;
    int rowoffset3=rowoffset2+(3*sizeofBox-2*sizeofBox)/2;
    int rowoffset4=rowoffset3+(2*sizeofBox-sizeofBox)/2;
    int boxnum;
int horzposition=(endofscreenx-20)/2;
    int offset;
    offset=(sizeofBox-sizeofXs)/2;
	drawX(horzposition+offset, 40+offset,5,0);
      drawX(horzposition+offset+sizeofBox, 40+offset,5,0);
      drawX(horzposition+offset+sizeofBox*2, 40+offset,5,0);
      drawX(horzposition+offset+sizeofBox*3, 40+offset,5,0);
    drawX(horzposition+offset+rowoffset2,40+offset+sizeofBox,5,0);
      drawX(horzposition+offset+rowoffset2+sizeofBox,40+offset+sizeofBox,5,0);
      drawX(horzposition+offset+rowoffset2+sizeofBox*2,40+offset+sizeofBox,5,0);
      drawX(horzposition+offset+rowoffset3, 40+offset+sizeofBox*2,5,0);
      drawX(horzposition+offset+rowoffset3+sizeofBox, 40+offset+sizeofBox*2,5,0);
      drawX(horzposition+offset+rowoffset4,40+offset+sizeofBox*3,5,0);
    //drawArrayofBoxes((endofscreenx-20)/2,40,16,0xf800);


//erase and redraw

//if score
if(score==1)
{
    drawX(horzposition+offset, 40+offset,5,0XFFFF);
}

if(score==2)
{
    drawX(horzposition+offset, 40+offset,5,0XFFFF);

    drawX(horzposition+offset+rowoffset2,40+offset+sizeofBox,5,0XFFFF);
}

if(score==3)
{
    drawX(horzposition+offset, 40+offset,5,0XFFFF);

    drawX(horzposition+offset+sizeofBox, 40+offset,5,0XFFFF);
    drawX(horzposition+offset+rowoffset2,40+offset+sizeofBox,5,0XFFFF);

}
if(score==4)
{
    drawX(horzposition+offset, 40+offset,5,0XFFFF);
      drawX(horzposition+offset+sizeofBox, 40+offset,5,0XFFFF);
    drawX(horzposition+offset+rowoffset2,40+offset+sizeofBox,5,0XFFFF);
      drawX(horzposition+offset+rowoffset2+sizeofBox,40+offset+sizeofBox,5,0xFFFF);


}

if(score==5)
{
    drawX(horzposition+offset, 40+offset,5,0XFFFF);
      drawX(horzposition+offset+sizeofBox, 40+offset,5,0XFFFF);
      drawX(horzposition+offset+sizeofBox*2, 40+offset,5,0XFFFF);
    drawX(horzposition+offset+rowoffset2,40+offset+sizeofBox,5,0XFFFF);
      drawX(horzposition+offset+rowoffset2+sizeofBox,40+offset+sizeofBox,5,0xFFFF);



}
if(score==6)
{
    drawX(horzposition+offset, 40+offset,5,0XFFFF);
      drawX(horzposition+offset+sizeofBox, 40+offset,5,0XFFFF);
      drawX(horzposition+offset+sizeofBox*2, 40+offset,5,0XFFFF);
    drawX(horzposition+offset+rowoffset2,40+offset+sizeofBox,5,0XFFFF);
      drawX(horzposition+offset+rowoffset2+sizeofBox,40+offset+sizeofBox,5,0xFFFF);
      drawX(horzposition+offset+rowoffset3, 40+offset+sizeofBox*2,5,0XFFFF);




}
if(score==7)
{
     drawX(horzposition+offset, 40+offset,5,0XFFFF);
      drawX(horzposition+offset+sizeofBox, 40+offset,5,0XFFFF);
      drawX(horzposition+offset+sizeofBox*2, 40+offset,5,0XFFFF);
    drawX(horzposition+offset+rowoffset2,40+offset+sizeofBox,5,0XFFFF);
      drawX(horzposition+offset+rowoffset2+sizeofBox,40+offset+sizeofBox,5,0xFFFF);
      drawX(horzposition+offset+rowoffset3, 40+offset+sizeofBox*2,5,0XFFFF);
      drawX(horzposition+offset+rowoffset3+sizeofBox, 40+offset+sizeofBox*2,5,0XFFFF);


}
if(score==8)
{
     drawX(horzposition+offset, 40+offset,5,0XFFFF);
      drawX(horzposition+offset+sizeofBox, 40+offset,5,0XFFFF);
      drawX(horzposition+offset+sizeofBox*2, 40+offset,5,0XFFFF);
    drawX(horzposition+offset+rowoffset2,40+offset+sizeofBox,5,0XFFFF);
      drawX(horzposition+offset+rowoffset2+sizeofBox,40+offset+sizeofBox,5,0xFFFF);
      drawX(horzposition+offset+rowoffset3, 40+offset+sizeofBox*2,5,0XFFFF);
      drawX(horzposition+offset+rowoffset3+sizeofBox, 40+offset+sizeofBox*2,5,0XFFFF);
      drawX(horzposition+offset+rowoffset4,40+offset+sizeofBox*3,5,0XFFFF);


}

if(score==9)
{
    drawX(horzposition+offset, 40+offset,5,0XFFFF);
      drawX(horzposition+offset+sizeofBox, 40+offset,5,0XFFFF);
      drawX(horzposition+offset+sizeofBox*2, 40+offset,5,0XFFFF);
    drawX(horzposition+offset+rowoffset2,40+offset+sizeofBox,5,0XFFFF);
      drawX(horzposition+offset+rowoffset2+sizeofBox,40+offset+sizeofBox,5,0xFFFF);
      drawX(horzposition+offset+rowoffset2+sizeofBox*2,40+offset+sizeofBox,5,0xFFFF);
      drawX(horzposition+offset+rowoffset3, 40+offset+sizeofBox*2,5,0XFFFF);
      drawX(horzposition+offset+rowoffset3+sizeofBox, 40+offset+sizeofBox*2,5,0XFFFF);
      drawX(horzposition+offset+rowoffset4,40+offset+sizeofBox*3,5,0XFFFF);
}
if(score==10)
{
    drawX(horzposition+offset, 40+offset,5,0XFFFF);
      drawX(horzposition+offset+sizeofBox, 40+offset,5,0XFFFF);
      drawX(horzposition+offset+sizeofBox*2, 40+offset,5,0XFFFF);
      drawX(horzposition+offset+sizeofBox*3, 40+offset,5,0XFFFF);
    drawX(horzposition+offset+rowoffset2,40+offset+sizeofBox,5,0XFFFF);
      drawX(horzposition+offset+rowoffset2+sizeofBox,40+offset+sizeofBox,5,0xFFFF);
      drawX(horzposition+offset+rowoffset2+sizeofBox*2,40+offset+sizeofBox,5,0xFFFF);
      drawX(horzposition+offset+rowoffset3, 40+offset+sizeofBox*2,5,0XFFFF);
      drawX(horzposition+offset+rowoffset3+sizeofBox, 40+offset+sizeofBox*2,5,0XFFFF);
      drawX(horzposition+offset+rowoffset4,40+offset+sizeofBox*3,5,0XFFFF);

}


//	print score in location: round*offset
//	make pattern 1
//if score==2

//	make pattern 2
//if score==3
//	make pattern 3
//if score==5
//	make pattern 4
//if score==8
//	make pattern 5
//if score==9
//	make pattern 6
//if score==10
//	make pattern strike
	}

void drawfilledBox(int x, int y, int lengthx, int lengthy,  short colour)
{
    int i,j;

    for( i=0; i<lengthy; i++)
    {
        for(j=0; j<lengthx;j++)
        {
            write_pixel(x+j,y+i,colour);
        }
    }
}

void fillinscore(int score, int round, int player)
{
    int columnwidth=30;
    int columnlength=4;
    round = round/2;
	round++;
	
	if(player==0)
    {

    int offset=1;
    int roundoffset=(round-1)*30/4;

    write_score(roundoffset+offset,(146)/4+offset,score);
    }

    else if(player==1)
    {

    int offset=1;
    int roundoffset=(round-1)*30/4;

    write_score(roundoffset+offset,146/4+offset+columnlength,score);

    }

	updatepins(score,round);

}


void blink(int player)
{

    if(player == 1)
    {
        drawfilledBox(0,0,50,10,0xf800);
		write_line("Player 1", 1,1);
    }
    else if (player == 0)
        {
            drawfilledBox(0,0,50,10,0x000f);
			write_line("Player 2", 1,1);
        }
	else if (player == 2)
        {
            drawfilledBox(0,0,50,10,0x00ff);
			write_line("GAMEOVER", 1,1);
        }

}












