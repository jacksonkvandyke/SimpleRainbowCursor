#define OEMRESOURCE
#include <windows.h>
#include <winuser.h>
#include <wingdi.h>
#include <stdio.h>
#include <shellapi.h>

void SetCursorInfo(COLORREF color);
void RainbowCursorLoop();
int AddColor(int* colorArray);
int RemoveColor(int* colorArray);

int main(){
    //Hide console
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);

    RainbowCursorLoop();
    return 0;
}

void SetCursorInfo(COLORREF color){
    //Get current icon info
    ICONINFO user_cursor;
    GetIconInfo(LoadCursor(nullptr, IDC_HAND), &user_cursor);

    //Get bitmap
    BITMAP bitmap;
    GetObject(user_cursor.hbmColor, sizeof(BITMAP), &bitmap);

    //Get bitmap info
    BITMAPINFO bitmapinfo = { 0 };
    bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmapinfo.bmiHeader.biWidth = bitmap.bmWidth;
    bitmapinfo.bmiHeader.biHeight = -bitmap.bmHeight;
    bitmapinfo.bmiHeader.biPlanes = 1;
    bitmapinfo.bmiHeader.biBitCount = 24;
    bitmapinfo.bmiHeader.biCompression = BI_RGB;

    //Create buffer to hold bitmap data
    int size = bitmap.bmHeight * bitmap.bmWidthBytes;
    BYTE* bitmapBuffer = (BYTE*)malloc(size);
    GetDIBits(GetDC(NULL), user_cursor.hbmColor, 0, bitmap.bmHeight, bitmapBuffer, &bitmapinfo, DIB_RGB_COLORS);

    //Change the color of each pixel
    for (int i = 0; i < size; i += 3){
        bitmapBuffer[i] = GetBValue(color);
        bitmapBuffer[i + 1] = GetGValue(color);
        bitmapBuffer[i + 2] = GetRValue(color);
    }

    //Assign the new colors to the bitmap
    SetDIBits(GetDC(NULL), user_cursor.hbmColor, 0, bitmap.bmHeight, bitmapBuffer, &bitmapinfo, DIB_RGB_COLORS);

    //Create the new icon
    HCURSOR new_cursor = CreateIconIndirect(&user_cursor);
    SetSystemCursor(new_cursor, OCR_NORMAL);

    //Free
    DeleteObject(user_cursor.hbmMask);
    DeleteObject(user_cursor.hbmColor);
    DestroyCursor(new_cursor);
    free(bitmapBuffer);
}

void RainbowCursorLoop(){
    //Start the cursor loop
    COLORREF color;
    int decend = 0;
    int colorArray[3] = { 0 };
    for(;;){
        //First assign the new color
        color = RGB(colorArray[0], colorArray[1], colorArray[2]);
        SetCursorInfo(color);

        //Now determine what values to change in the colorArray
        if (decend == 1){
            int ret = AddColor(colorArray);
            if (ret == 1){
                decend = 0;
            }
        }else{
            int ret = RemoveColor(colorArray);
            if (ret == 1){
                decend = 1;
            }
        }
        Sleep(5);
    }
}

int AddColor(int* colorArray){
    if (colorArray[0] != 255){
        colorArray[0] = colorArray[0] + 1;
        return 0;
    }
    if (colorArray[1] != 255){
        colorArray[1] = colorArray[1] + 1;
        return 0;
    }
    if (colorArray[2] != 255){
        colorArray[2] = colorArray[2] + 1;
        return 0;
    }
    return 1;
}

int RemoveColor(int* colorArray){
    if (colorArray[0] != 0){
        colorArray[0] = colorArray[0] - 1;
        return 0;
    }
    if (colorArray[1] != 0){
        colorArray[1] = colorArray[1] - 1;
        return 0;
    }
    if (colorArray[2] != 0){
        colorArray[2] = colorArray[2] - 1;
        return 0;
    }
    return 1;
}