/*
*main.cpp
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sys/stat.h>
//#include <time.h>

#include "config.h"
#include "tokenize.h"

typedef struct _EZ_ITEM
{
	int  m_Type;
	char *szFileName;
	int  m_Transition;
	int  m_Speed;
	int  m_Duration;
	int  m_Font;
	int  m_Color;
} EZ_ITEM;

int file_size(char *filename)
{
	struct stat statbuf;
	stat(filename, &statbuf);
	long size =(long) statbuf.st_size;
	return size;
}
static void get_Item_Info(const char * info, EZ_ITEM *ez_item)
{
	unsigned int nPos = 0; int item_info = 0;
	while (info[nPos] != '\0')
	{
		switch (item_info)
		{
		case 0:
			ez_item->m_Type = atoi(Tokenize(info, ',', &nPos));
			fprintf(stderr, "m_Type=%d\n", ez_item->m_Type);
			item_info++;
			break;
		case 1:
			ez_item->m_Transition = atoi(Tokenize(info, ',', &nPos));
			fprintf(stderr, "m_Transition=%d\n", ez_item->m_Transition);
			item_info++;
			break;
		case 2:
			ez_item->m_Speed = atoi(Tokenize(info, ',', &nPos));
			fprintf(stderr, "m_Speed=%d\n", ez_item->m_Speed);
			item_info++;
			break;
		case 3:
			ez_item->m_Duration = atoi(Tokenize(info, ',', &nPos));
			fprintf(stderr, "m_Duration=%d\n", ez_item->m_Duration);
			item_info++;
			break;
		case 4:
			ez_item->m_Font = atoi(Tokenize(info, ',', &nPos));
			fprintf(stderr, "m_Font=%d\n", ez_item->m_Font);
			item_info++;
			break;
		case 5:
			ez_item->m_Color = atoi(Tokenize(info, ',', &nPos));
			fprintf(stderr, "m_Color=%d\n", ez_item->m_Color);
			item_info++;
			break;
		default:
			item_info++;
			break;
		}
	}
}
static void EZ_DrawImage()
{
	char *imageFile = "0.edi";
	//unsigned long = GetTickCount();
	//get image file path
	//sprintf(imageFile, "%s%d%s", _PLAYLIST_DIR_, n_Item, ".edi");
	FILE *fd;
	if ((fd = fopen(imageFile, "r")) == NULL)
	{
		fprintf(stderr, "%s:can't open the %s file", __FUNCTION__, imageFile);
		return;
	}
	//get the 4byte head info
	char head[5]; int  count;
	if ((count = fread(head, 4, 1, fd)) != 1)
	{
		fprintf(stderr, "%s:read head failed", __FUNCTION__);
		return;
	}
	head[4] = '\0';
	if (strcmp(head, "EDI0") != 0)
	{
		fprintf(stderr, "%s:the head info not EDI0", __FUNCTION__);
		return;
	}
	//get the Width and Height info
	 unsigned short image_Width; unsigned short image_Height;
	if ((count = fread(&image_Width, 2, 1, fd) != 1))
	{
		fprintf(stderr, "%s:read image_width failed", __FUNCTION__);
		return;
	}
	if ((count = fread(&image_Height, 2, 1, fd)) != 1)
	{
		fprintf(stderr, "%s:read image_Height failed", __FUNCTION__);
		return;
	}
	fprintf(stderr, "image_Width=%d  image_Height=%d\n", image_Width, image_Height);
	int lineBytes = (image_Width + 7) / 8;
	fprintf(stderr, "%s():lineBytes=%d\n", __FUNCTION__,lineBytes);
	long bitSize = (long)lineBytes*image_Height;
	long filesize=file_size(imageFile);
	fprintf(stderr, "%s:bitSize=%d  filesize=%d\n", __FUNCTION__, bitSize, filesize);

}
int   screen_Width  = 600;
int   screen_Height = 300;

//buffer info
int   lineBytes = (screen_Width + 7) / 8;
int   bufSize = lineBytes*screen_Height;
int main()

{
	//FILE *file;

	
	
	/*
	char s[100];
	char *section = "Items";
	int key;
	int erro;
	char*test;
	char Item_n[10];
	int count = 0;
	sprintf(Item_n, "%s%d", "Item", count);
	if (sectionexist(section,fileName)==-1)
	{
		fprintf(stderr, "error!,no such Section\n");
	}
	*/
	/*
	erro = getconfigstr(section, keyName, s, 100, fileName);
	
	if (erro!=0 )
	{
		fprintf(stderr, "error:%d\n", erro);
		system("pause");
		return 0;
	}
	*/
	//fprintf(stderr, "LastWriteTimeHigh=%s\n", s);

	
	//get_Item_info(s, &ez_test);

	/*
	//close(file);

	unsigned long test;
	printf("%d\n", sizeof(test));
	printf("%d\n", lineBytes);
	printf("%d\n", bufSize);
	//EZ_DrawImage();
	unsigned char*buf; int size = 20;
	buf = (unsigned char*)malloc(size);
	for (int i = 0; i < 20;i++)
	buf[i] = 'A';
	printf("bufsize=%d  \n", sizeof(buf));
	printf("%s\n", buf);
	*/
	char *fileName = "Playlist.ini";
	EZ_ITEM *ez_test;
	int n_Items;
	if (sectionexist("Items", fileName) != -1)
	{
		//get Item's number
		getconfigint("Items", "NumItems", &n_Items, fileName);
		//malloc for items;
		ez_test = (EZ_ITEM *)(malloc(sizeof(EZ_ITEM)*n_Items));
		int count = 0; char Item_n[10]; char Item_info[20];
		while (count <n_Items)
		{
			//build item name
			sprintf(Item_n, "%s%d", "Item", count);
			//get item_x info
			getconfigstr("Items", Item_n, Item_info, 20, fileName);
			//save item_x info to playlist
			get_Item_Info(Item_info, &ez_test[count]);
			count++;
		}
	}

	system("pause");
	return 0;

}