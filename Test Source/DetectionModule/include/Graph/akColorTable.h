#pragma once


#include "akGraph/akGraphLinker.h"

#include <iostream>
#include <vector>








class AFX_EXT_CLASS CakColorTable 
{
public:
	CakColorTable(void);
	~CakColorTable(void);

public:
	struct AFX_EXT_CLASS _COLOR_TABLE 
	{
		char name[255];
		unsigned char R[64];
		unsigned char G[64];
		unsigned char B[64];
	};




	void setFile(char *file);
	void setColorTable(int index , int level , float min , float max , float value , unsigned char &R , unsigned char &G , unsigned char &B , bool inverse);
	void setColorTableSmooth(int index , int level , float min , float max , float value , unsigned char &R , unsigned char &G , unsigned char &B , bool inverse);

	int  getTotalColorTable(void);
	char *getColorName(int index);

	void rendColorTableBar(CDC *dc , int x , int y , int width , int height , int index , int level , bool inverse , bool vertical);



protected:

private:


public:
	std::vector<_COLOR_TABLE> m_vecColorTable;

protected:


private:

};
