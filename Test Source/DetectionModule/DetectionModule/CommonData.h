#pragma once
#define SSE_PROCESS_PIXELS 32

const int MAX_CAMERA_CCD_SIZE = 16384;
const int MAX_FRAME_HEIGHT = 8192;

#define	COLOR_0			RGB(60, 128, 50)		// PadB
#define	COLOR_1			RGB(5, 64, 128)			// PadW
#define	COLOR_2			RGB(0, 200, 200)		//(33,216,184)		// RB
#define	COLOR_3			RGB(88, 224, 88)		//(148,138,84)		// RW
#define	COLOR_4			RGB(255, 0, 0)			//(136,30,30)		// TB
#define	COLOR_5			RGB(0, 0, 255)			//(105,161,191)	// TW
#define	COLOR_6			RGB(100, 0, 100)		//(255,100,255)	// MD
#define	COLOR_7			RGB(128, 128, 128)		// CD
#define	COLOR_8			RGB(0, 100, 0)			//(82,55,56)		// EG
#define	COLOR_9			RGB(255, 255, 0)		// NO

//GUI Color
#define BG_COLOR					RGB(146, 136, 86)
#define FONT_COLOR					RGB(230, 230, 230)		//글자색상
#define GRID_FIX_COLOR				RGB(50, 50, 50)			//고정 Grid 배경색상
#define GRID_COLOR					RGB(230, 230, 230)		//고정이 아닌 Grid 배경색상
#define GRID_TEXT_COLOR				RGB(255, 255, 255)		//고정 Grid 글자색상
#define GRID_LINE_COLOR				RGB(255, 255, 255)		//Grid 선색상
#define TITLE_COLOR					RGB(155, 117, 78)
#define TITLE_GRAD_COLOR			RGB(85, 47, 8)
#define TITLE_TEXT_COLOR			RGB(255, 255, 255)
#define TITLE_OUTLINE_COLOR1		RGB(0, 0, 0)
#define TITLE_OUTLINE_COLOR2		RGB(70, 50, 10)
#define BUTTON_COLOR1				RGB(24, 19, 02)
#define BUTTON_COLOR2				RGB(80, 80, 80)
#define TAB_BTN_COLOR1				RGB(27, 0, 0)
#define TAB_BTN_COLOR2				RGB(117, 16, 16)