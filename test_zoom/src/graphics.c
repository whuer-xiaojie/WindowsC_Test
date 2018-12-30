

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "zoom.h"


int imageWidth;
int imageHeight;

unsigned char *buf = NULL;

int new_width = 200;
int new_height = 200;
unsigned char *new_buf = NULL;

// 函数power_of_two用于判断一个整数是不是2的整数次幂    
static int power_of_two(int n)
{
	if (n <= 0)
		return 0;
	return (n & (n - 1)) == 0;
	/*
	if (n == 256)
	{
		return 1;
	}
	else
		return 0;
		*/
}

/************************************************************************/
/*   
*函数：read_Bmp(char *filePath)
*功能：读取BMP文件的位图数据，并实现位图数据的三原色的存储方式的转换
*     （BGR——RGB）
*参数：char *filePath BMP文件的文件路径
*返回值：无
*/
/************************************************************************/
static void  read_Bmp(char *filePath)
{
	int bfsize;
	int bfOffBits;
	int size;
    int planes=0;
	int bBitCount=0;
	int imagePixelSize;
	int biSize;
	int biCompression;
    int read_returnNum;
	FILE *  fd = fopen(filePath, "r");
	if (fd==0)
	{
		fprintf(stderr, "can't open the bmp file\n");
		return 0;
	}
	//read bfSize
	fseek(fd, 2, SEEK_SET);
	if ((read_returnNum = fread(&bfsize, sizeof(bfsize), 1, fd)) != 1)
	{
		fprintf(stderr, "read bfsize error!\n");
		fclose(fd);
		return;
	}
	//read bfOffBits
	fseek(fd, 10, SEEK_SET);
	if ((read_returnNum = fread(&bfOffBits, sizeof(bfOffBits), 1, fd))!=1)
	{
		fprintf(stderr, "read bfOffBits error!\n");
		fclose(fd);
		return;
	}
	//read Bitmap Information Header Size
	if ((read_returnNum = fread(&biSize, 4, 1, fd)) != 1)
	{
		fprintf(stderr, "read biSize error!\n");
		fclose(fd);
		return;
	}
	if (biSize!=40)
	{
		fprintf(stderr, "Bitmap Information Size not 40 !! biSize=%d  bfOffbits=%d\n",biSize,bfOffBits);
		fclose(fd);
		return;
	}
	//read Image Width
	fseek(fd, 0x0012, SEEK_SET);
	if ((read_returnNum = fread(&imageWidth, sizeof(imageWidth), 1, fd))!=1)
	{
		fprintf(stderr, "read imageWidth error!\n");
		fclose(fd);
		return;
	}
	//read Image Height
	if ((read_returnNum = fread(&imageHeight, sizeof(imageHeight), 1, fd))!=1)
	{
		fprintf(stderr, "read imageHeight error!\n");
		fclose(fd);
		return;
	}
    //read Planes
	if ((read_returnNum=fread(&planes,2,1,fd))!=1)
	{
		fprintf(stderr, "read planes error!\n");
		fclose(fd);
		return;
	}
	if (planes!=1)
	{
		fprintf(stderr, "not BMP file\n");
		fclose(fd);
		return;
	}
	//read Bit Count Information
	fseek(fd, 0x001c, SEEK_SET);
	if ((read_returnNum = fread(&bBitCount, 2, 1, fd))!=1)
	{
		fprintf(stderr, "read bBitCount error!\n");
		fclose(fd);
		return;
	}
	if (bBitCount != 24)
	{
		fprintf(stderr, "not 24bit Map  BitCount=%d\n", bBitCount);
		fclose(fd);
		return;
	}
	//read biCompression
	if ((read_returnNum = fread(&biCompression, 4, 1, fd)) != 1)
	{
		fprintf(stderr, "read biCompression error!\n");
		fclose(fd);
		return;
	}
	if (biCompression==3)
	{
		fprintf(stderr, "biCompression==BI_BITFIELDS\n");
	}
	fprintf(stderr, "width=%d   height=%d  bBitCount=%d  bfOffBits=%d\n", imageWidth, imageHeight,bBitCount,bfOffBits);
	//calculate pixel size
	size = imageWidth * 3;
	while (size%4!=0)
	{
		size++;
	}
	size *= imageHeight;
	imagePixelSize=size;
	fprintf(stderr, "size = %d\n",size );

	//imagePixelSize = bfsize - bfOffBits;
	//fprintf(stderr, "imagePixelSize = %d\n", imagePixelSize );

    //imagePixelSize = imageWidth*imageHeight * 3;
	
	buf = (unsigned char  *)malloc(imagePixelSize);
	if (buf==0)
	{
		fprintf(stderr, "buf malloc failed\n");
		fclose(fd);
		return 0;
	}

	//read pixel data
	fseek(fd, bfOffBits, SEEK_SET);
	if ((fread(buf, 1, imagePixelSize,fd)) <= 0)
	{
		fprintf(stderr, "read pixel data Error\n");
		free(buf);
		buf = NULL;
	}

	//把原来的像素的 BGR 转化为 RGB
    int i = 0; char temp;
	while (i < imagePixelSize)
	{
		temp = buf[i];
		buf[i] = buf[i + 2];
		buf[i + 2] = temp;
		i += 3;
	}
	
	zoom_image((unsigned char*)buf, imageWidth, imageHeight, (unsigned char*)new_buf,
		new_width, new_height);

	fclose(fd);
	//return 0;
}

int  main()
{
	
	new_buf = (unsigned char **)malloc(new_width*new_height * 3);
	if (new_buf == NULL)
	{
		fprintf(stderr, "%s()malloc for new_buf failed\n", __FUNCTION__);
		return -1;
	}
	int count = 0;
	while(count<8)
	{
		memset(new_buf, 0, new_width*new_height * 3);
		char *s;
		switch (count%4)
		{
		case 0:
		{
			s = "photo2.bmp";
			read_Bmp(s);
			//display_Bmp();
			count++;
			//sleep(2);
			break;
		}
		case 1:
		{
			s = "photo1.bmp";
			read_Bmp(s);
			//display_Bmp();
			count++;
			//sleep(2);
			break;
		}
		case 2:
		{
			s = "photo2.bmp";
			read_Bmp(s);
			//display_Bmp();
			count++;
			//sleep(2);
			break;
		}
		case 3:
		{
			s = "photo3.bmp";
			read_Bmp(s);
			//display_Bmp();
			count++;
			//sleep(2);
			break;
		}
		default:
			break;
		}
	}
	//read_Bmp();
	//display_Bmp();
	//sleep(5);
	if (new_buf)free(new_buf);
	return 0;
	//glutMainLoop();
}
