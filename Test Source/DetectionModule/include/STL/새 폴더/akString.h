#pragma once


#include "akLinker.h"

#include <iostream>
#include <tchar.h>


extern akSTL_DLLSPEC wchar_t* m_pWstrTemp;
akSTL_DLLSPEC wchar_t* charToWchar(char* pstrSrc);


namespace akSTL
{
	class akSTL_DLLSPEC CakString
	{
	public:
		CakString(void);
		~CakString(void);

	public:
		//wstring ToWString(const char * in_val);
		//string ToString(const wstring &in_val);

		

		//찾고자하는 문자의 위치를 반환해주는 함수(-1이 반환되면 찾지 못한것임, 찾았으면 1이상의 값을 반환)
		static int StringFind(const char* str, const char* keyward);
		

		static inline std::wstring ToWString(const char * in_val)
		{
			std::wstring temp;
			while (*in_val != '\0')
				temp += *in_val++;
			return temp;
		}

		static inline std::string ToString(const std::wstring &in_val)
		{
			std::string temp;
			std::wstring::const_iterator b = in_val.begin();
			const std::wstring::const_iterator e = in_val.end();
			while (b != e)
			{
				temp += static_cast<char>(*b);
				++b;
			}
			return temp;
		}

		static bool IsNumber(char* strSource);
		static void StringReplace(char* strSource, char cSourceChar, char cDestChar);
		static void StringReplace(char* strSource, char* strSourceString, char* strDestString);
		static bool GetFullPathName(char* strDest, int nDestSize, const char* strSourceFront, const char* strSourceEnd);
		static void GetPath(char* strDest, int nDestSize, const char* strSource);
		static void GetName(char* strDest, int nDestSize, const char* strSource);
		static bool GetExt(char* strDest, int nDestSize, const char* strSource);
		static char* GetExt(const char* strSource);
		static int Find(const char* strSource, char cChar);
		static int Find(const char* strSource, char* strFindString);
		static int ReverseFind(const char* strSource, char cChar);
		static void MakeLower(char* strSource);
		static void MakeUpper(char* strSource);
		static bool Left(const char* strSource, char* strDest, int nDestLength, int nLeftCount);
		static bool Mid(const char* strSource, char* strDest, int nDestLength, int nFirstIndex, int nCount = -1);
		static bool Right(const char* strSource, char* strDest, int nDestLength, int nRightCount);
		


	private:
		
	};
}


//
//void CksgFontMaker::DrawStringOverray(
//									  GLfloat x, 
//									  GLfloat y,  
//									  int windowWidth,
//									  int windowHeight,
//									  char* s
//									  ,...)
//{
//	// Draws the given text string at the given location.
//
//	char text[256]={};
//	va_list ap;
//	va_start(ap, s);
//	vsprintf(text, s, ap);
//	va_end(ap);
//
//	GLsizei len = GLsizei(strlen(text));
//	if (text && len > 0) 
//	{
//		glPushMatrix();
//		{
//			glPushAttrib( GL_LIGHTING_BIT );
//			glDisable(GL_LIGHTING);
//			glMatrixMode(GL_PROJECTION);
//			glPushMatrix();
//			{
//				glLoadIdentity();
//				gluOrtho2D(0, windowWidth, 0,windowHeight);
//
//				glMatrixMode(GL_MODELVIEW);
//
//				glPushMatrix();
//				{
//					glLoadIdentity();
//
//
//
//
//					glRasterPos2i(x, windowHeight+m_logfont.lfHeight-y);
//
//					glPushAttrib(GL_LIST_BIT);
//					{
//						glListBase(m_fontListBase);
//						glCallLists(len, GL_UNSIGNED_BYTE, (const GLvoid*)text);
//					} glPopAttrib();
//
//
//				}glPopMatrix();
//
//				glMatrixMode(GL_PROJECTION);
//
//			}glPopMatrix();
//
//			glMatrixMode(GL_MODELVIEW);
//
//
//			glPopAttrib();
//			//glEnable(GL_DEPTH_TEST);
//
//		}glPopMatrix();
//
//
//
//	}
//}