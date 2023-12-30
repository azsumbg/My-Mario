#include "framework.h"
#include "My Mario.h"
#include "resource.h"

#include <mmsystem.h>
#include <d2d1.h>
#include <dwrite.h>
#include <ctime>
#include <fstream>
#include <vector>

#include "FCheck.h"
#include "errh.h"
#include "Helper.h"
#include "D2BMPLOADER.h"

#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "d2d1.lib")
#pragma comment (lib, "dwrite.lib")
#pragma comment (lib, "fcheck.lib")
#pragma comment (lib, "errh.lib")
#pragma comment (lib, "helper.lib")
#pragma comment (lib, "d2bmploader.lib")

#define bWinClassName L"New_Mario"

#define mNew 1001
#define mExit 1002
#define mLoad 1003
#define mSave 1004
#define mHoF 1005

#define record 2001
#define no_record 2002
#define first_record 2003

#define temp_file ".\\res\\data\\temp.dat"
#define Ltemp_file L".\\res\\data\\temp.dat"
#define sound_file L".\\res\\snd\\main.wav"
#define record_file L".\\res\\data\\rec.dat"
#define help_file L".\\res\\data\\help.dat"
#define save_file L".\\res\\data\\save.dat"
#define log_file L".\\res\\data\\err_log.dat"

////////////////////////////////////////////////////

WNDCLASS bWin;
HINSTANCE bIns = nullptr;
HWND bHwnd = nullptr;
HMENU bBar = nullptr;
HMENU bMain = nullptr;
HMENU bStore = nullptr;
HDC PaintDC = nullptr;
HICON mainIcon = nullptr;
HCURSOR bCursor = nullptr;
HCURSOR outCursor = nullptr;

MSG bMsg;
BOOL bRet = 0;

PAINTSTRUCT bPaint;
POINT cur_pos = { 0,0 };
UINT bTimer = -1;

wchar_t current_player[16] = L"A PLAYER";
int name_size = 10;

bool pause = false;
bool show_help = false;
bool name_set = false;
bool in_client = true;
bool sound = true;

bool b1Hglt = false;
bool b2Hglt = false;
bool b3Hglt = false;

float cl_height = 0;
float cl_width = 0;
int seconds = 0;
int minutes = 0;
int score = 0;
int lifes = 3;

D2D1_RECT_F b1Rect = { 0,0,250.0f,50.0f };
D2D1_RECT_F b2Rect = { 350,0,600.0f,50.0f };
D2D1_RECT_F b3Rect = { 700,0,950.0f,50.0f };
//////////////////////////////////////////////

//D2D1 **************************************

ID2D1Factory* iFactory = nullptr;
ID2D1HwndRenderTarget* Draw = nullptr;
IDWriteFactory* iWriteFactory = nullptr;
IDWriteTextFormat* nrmTextFormat = nullptr;
IDWriteTextFormat* bigTextFormat = nullptr;

ID2D1RadialGradientBrush* ButBckgBrush = nullptr;
ID2D1SolidColorBrush* ButBorderBrush = nullptr;
ID2D1SolidColorBrush* ButTxtBrush = nullptr;
ID2D1SolidColorBrush* ButHgltBrush = nullptr;
ID2D1SolidColorBrush* ButInactiveBrush = nullptr;

ID2D1LinearGradientBrush* FieldBrush = nullptr;
ID2D1SolidColorBrush* FieldTxtBrush = nullptr;

ID2D1Bitmap* bmpBrick = nullptr;
ID2D1Bitmap* bmpGoldBrick = nullptr;
ID2D1Bitmap* bmpCloud1 = nullptr;
ID2D1Bitmap* bmpCloud2 = nullptr;
ID2D1Bitmap* bmpMountain1 = nullptr;
ID2D1Bitmap* bmpMountain2 = nullptr;
ID2D1Bitmap* bmpMountain3 = nullptr;
ID2D1Bitmap* bmpField = nullptr;
ID2D1Bitmap* bmpGold = nullptr;
ID2D1Bitmap* bmpMushroom = nullptr;
ID2D1Bitmap* bmpSun = nullptr;
ID2D1Bitmap* bmpBullet = nullptr;
ID2D1Bitmap* bmpAngry = nullptr;
ID2D1Bitmap* bmpPortal = nullptr;
ID2D1Bitmap* bmpDeath = nullptr;

ID2D1Bitmap* bmpMarioL[12];
ID2D1Bitmap* bmpMarioR[12];
ID2D1Bitmap* bmpMarioJL = nullptr;
ID2D1Bitmap* bmpMarioJR = nullptr;

ID2D1Bitmap* bmpTurtleL[2];
ID2D1Bitmap* bmpTurtleR[2];
ID2D1Bitmap* bmpTurtleBlocked = nullptr;

//////////////////////////

std::vector<obj_ptr> vFields;
obj_ptr Mario = nullptr;
float jump_target_x = 0;
float jump_target_y = 0;
dirs opposite_dir = dirs::stop;

bool Mario_upgraded = false;
bool game_win = false;
bool game_over = false;


obj_ptr Cloud1 = nullptr;
obj_ptr Cloud2 = nullptr;
ATOMS* Sun = nullptr;
ATOMS* Portal = nullptr;
std::vector<obj_ptr>vMountains;
std::vector<obj_ptr>vPlatforms;
std::vector<obj_ptr>vBenefits;
std::vector<obj_ptr>vBullets;
std::vector<obj_ptr>vTurtles;

float base_platform_y = 0;
float platform_rows = 0;

bool mountain1_visible = false;
bool mountain2_visible = false;
bool mountain3_visible = false;

////////////////////////////////////////////

template <typename Handle> void ClearResource(Handle** which)
{
    if ((*which))
    {
        (*which)->Release();
        which = nullptr;
    }
}
void ReleaseCOM()
{
    ClearResource(&iFactory);
    ClearResource(&Draw);

    ClearResource(&iWriteFactory);
    ClearResource(&nrmTextFormat);
    ClearResource(&bigTextFormat);
    
    ClearResource(&ButBckgBrush);
    ClearResource(&ButBorderBrush);
    ClearResource(&ButHgltBrush);
    ClearResource(&ButInactiveBrush);
    ClearResource(&ButTxtBrush);

    ClearResource(&FieldBrush);
    ClearResource(&FieldTxtBrush);

    ClearResource(&bmpBrick);
    ClearResource(&bmpGoldBrick);
    ClearResource(&bmpCloud1);
    ClearResource(&bmpCloud2);
    ClearResource(&bmpMountain1);
    ClearResource(&bmpMountain2);
    ClearResource(&bmpMountain3);
    ClearResource(&bmpGold);
    ClearResource(&bmpField);
    ClearResource(&bmpMushroom);
    ClearResource(&bmpSun);
    ClearResource(&bmpBullet);
    ClearResource(&bmpAngry);
    ClearResource(&bmpPortal);
    ClearResource(&bmpDeath);

    ClearResource(&bmpMarioJL);
    ClearResource(&bmpMarioJR);

    for (int i = 0; i < 12; i++)ClearResource(&bmpMarioL[i]);
    for (int i = 0; i < 12; i++)ClearResource(&bmpMarioR[i]);
    for (int i = 0; i < 2; i++)ClearResource(&bmpTurtleL[i]);
    for (int i = 0; i < 2; i++)ClearResource(&bmpTurtleR[i]);
}
void ErrExit(int which_error)
{
    MessageBeep(MB_ICONERROR);
    MessageBox(NULL, ErrHandle(which_error), L"Критична грешка !", MB_OK | MB_APPLMODAL | MB_ICONERROR);
    ReleaseCOM();

    std::remove(temp_file);
    exit(1);
}
void CreateErrorLog(const wchar_t* err_text)
{
    std::wofstream err(log_file, std::ios::app);
    err << err_text << std::endl;
    err.close();
    ErrExit(eD2D);
}
void InitD2D1()
{
    for (int i = 0; i < 12; i++)bmpMarioL[i] = nullptr;
    for (int i = 0; i < 12; i++)bmpMarioR[i] = nullptr;
    for (int i = 0; i < 2; i++)bmpTurtleL[i] = nullptr;
    for (int i = 0; i < 2; i++)bmpTurtleR[i] = nullptr;

    HRESULT hr = S_OK;
    D2D1_GRADIENT_STOP gStop[2] = { 0,0,0,0 };
    ID2D1GradientStopCollection* gStopCollection = nullptr;

    // FACTORIES AND TEXT *****************************************

    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &iFactory);
    if (hr != S_OK) CreateErrorLog(L"Error creating D2D1 Factory");

    hr = iFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(bHwnd, D2D1::SizeU((UINT32)(cl_width), (UINT32)(cl_height))), &Draw);
    if (hr != S_OK) CreateErrorLog(L"Error creating D2D1 HWND Render Target");

    hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&iWriteFactory));
    if (hr != S_OK) CreateErrorLog(L"Error creating D2D1 Write Factory");

    hr = iWriteFactory->CreateTextFormat(L"Gabriola", NULL, DWRITE_FONT_WEIGHT_EXTRA_BOLD, DWRITE_FONT_STYLE_OBLIQUE,
        DWRITE_FONT_STRETCH_EXPANDED, 64.0f, L"", &bigTextFormat);
    if (hr != S_OK) CreateErrorLog(L"Error creating D2D1 bigTextFormat");

    hr = iWriteFactory->CreateTextFormat(L"Gabriola", NULL, DWRITE_FONT_WEIGHT_EXTRA_BOLD, DWRITE_FONT_STYLE_OBLIQUE,
        DWRITE_FONT_STRETCH_NORMAL, 28.0f, L"", &nrmTextFormat);
    if (hr != S_OK) CreateErrorLog(L"Error creating D2D1 nrmTextFormat");

    // BRUSHES **********************************************

    gStop[0].position = 0;
    gStop[0].color = D2D1::ColorF(D2D1::ColorF::Aqua);
    gStop[1].position = 1.0f;
    gStop[1].color = D2D1::ColorF(D2D1::ColorF::BlueViolet);

    hr = Draw->CreateGradientStopCollection(gStop, 2, &gStopCollection);
    if (hr != S_OK) CreateErrorLog(L"Error creating D2D1 GradientStopCollection");

    if (gStopCollection)
        hr = Draw->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(D2D1::Point2F(cl_width / 2, 25.0f),
            D2D1::Point2F(0, 0), cl_width / 2, 25.0f), gStopCollection, &ButBckgBrush);
    if (hr != S_OK) CreateErrorLog(L"Error creating D2D1 ButBckgBrush for the button background");

    gStop[0].color = D2D1::ColorF(D2D1::ColorF::DeepSkyBlue);
    gStop[1].color = D2D1::ColorF(D2D1::ColorF::AliceBlue);
    gStopCollection->Release();

    hr = Draw->CreateGradientStopCollection(gStop, 2, &gStopCollection);
    if (hr != S_OK) CreateErrorLog(L"Error creating D2D1 GradientStopCollection for the field background");

    if (gStopCollection)
        hr = Draw->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(D2D1::Point2F(0, 50.0f),
            D2D1::Point2F(cl_width, cl_height)), gStopCollection, &FieldBrush);
    if (hr != S_OK) CreateErrorLog(L"Error creating D2D1 Field background");

    hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkGreen), &ButTxtBrush);
    if (hr != S_OK) CreateErrorLog(L"Error creating D2D1 ButTxtBrush");

    hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::YellowGreen), &ButHgltBrush);
    if (hr != S_OK) CreateErrorLog(L"Error creating D2D1 ButHgltBrush");

    hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkSlateGray), &ButInactiveBrush);
    if (hr != S_OK) CreateErrorLog(L"Error creating D2D1 ButInactiveBrush");

    hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkViolet), &ButBorderBrush);
    if (hr != S_OK) CreateErrorLog(L"Error creating D2D1 ButBorderBrush");

    hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Beige), &FieldTxtBrush);
    if (hr != S_OK) CreateErrorLog(L"Error creating D2D1 ButTxtBrush");

    ///////////////////////////////////////////////////////////////////////////

    // BITMAPS ******************************

    bmpBrick = Load(L".\\res\\img\\field\\brick.png", Draw);
    if (!bmpBrick)CreateErrorLog(L"Error creating bmpBrick");

    bmpGoldBrick = Load(L".\\res\\img\\field\\goldbrick.png", Draw);
    if (!bmpGoldBrick)CreateErrorLog(L"Error creating bmpGoldBrick");

    bmpGold = Load(L".\\res\\img\\field\\gold.png", Draw);
    if (!bmpGold)CreateErrorLog(L"Error creating bmpGold");

    bmpCloud1 = Load(L".\\res\\img\\field\\cloud1.png", Draw);
    if (!bmpCloud1)CreateErrorLog(L"Error creating bmpCloud1");

    bmpCloud2 = Load(L".\\res\\img\\field\\cloud2.png", Draw);
    if (!bmpCloud2)CreateErrorLog(L"Error creating bmpCloud2");

    bmpMountain1 = Load(L".\\res\\img\\field\\mountain1.png", Draw);
    if (!bmpMountain1)CreateErrorLog(L"Error creating bmpMountain1");

    bmpMountain2 = Load(L".\\res\\img\\field\\mountain2.png", Draw);
    if (!bmpMountain2)CreateErrorLog(L"Error creating bmpMountain2");

    bmpMountain3 = Load(L".\\res\\img\\field\\mountain3.png", Draw);
    if (!bmpMountain3)CreateErrorLog(L"Error creating bmpMountain3");

    bmpMushroom = Load(L".\\res\\img\\field\\mushroom.png", Draw);
    if (!bmpMushroom)CreateErrorLog(L"Error creating bmpMushroom");

    bmpField = Load(L".\\res\\img\\field\\field.png", Draw);
    if (!bmpField)CreateErrorLog(L"Error creating bmpField");

    bmpSun = Load(L".\\res\\img\\field\\sun.png", Draw);
    if (!bmpSun)CreateErrorLog(L"Error creating bmpSun");

    bmpBullet = Load(L".\\res\\img\\field\\bullet.png", Draw);
    if (!bmpBullet)CreateErrorLog(L"Error creating bmpBullet");

    bmpPortal = Load(L".\\res\\img\\field\\portal.png", Draw);
    if (!bmpPortal)CreateErrorLog(L"Error creating bmpPortal");

    bmpAngry = Load(L".\\res\\img\\mario\\angry.png", Draw);
    if (!bmpAngry)CreateErrorLog(L"Error creating bmpAngry");

    bmpDeath = Load(L".\\res\\img\\mario\\death.png", Draw);
    if (!bmpDeath)CreateErrorLog(L"Error creating bmpDeath");

    bmpMarioJL = Load(L".\\res\\img\\mario\\mario_jumpl.png", Draw);
    if (!bmpMarioJL)CreateErrorLog(L"Error creating bmpMarioJL");

    bmpMarioJR = Load(L".\\res\\img\\mario\\mario_jumpr.png", Draw);
    if (!bmpMarioJR)CreateErrorLog(L"Error creating bmpMarioJR");

    bmpTurtleBlocked = Load(L".\\res\\img\\turtle\\turtleblocked.png", Draw);
    if (!bmpTurtleBlocked)CreateErrorLog(L"Error creating bmpTurtleBlocked");

    for (int i = 0; i < 12; i++)
    {
        wchar_t name[150] = L".\\res\\img\\mario\\left\\";
        wchar_t add[4] = L"\0";
        wsprintf(add, L"%d", i);
        wcscat_s(name, add);
        wcscat_s(name, L".png");

        bmpMarioL[i] = Load(name, Draw);
        if (!bmpMarioL[i])CreateErrorLog(L"Error creating bmpMarioL");
    }
    for (int i = 0; i < 12; i++)
    {
        wchar_t name[150] = L".\\res\\img\\mario\\right\\";
        wchar_t add[4] = L"\0";
        wsprintf(add, L"%d", i);
        wcscat_s(name, add);
        wcscat_s(name, L".png");

        bmpMarioR[i] = Load(name, Draw);
        if (!bmpMarioR[i])CreateErrorLog(L"Error creating bmpMarioR");
    }
    for (int i = 0; i < 2; i++)
    {
        wchar_t name[150] = L".\\res\\img\\turtle\\left\\";
        wchar_t add[4] = L"\0";
        wsprintf(add, L"%d", i);
        wcscat_s(name, add);
        wcscat_s(name, L".png");

        bmpTurtleL[i] = Load(name, Draw);
        if (!bmpTurtleL[i])CreateErrorLog(L"Error creating bmpTurtleL");
    }
    for (int i = 0; i < 2; i++)
    {
        wchar_t name[150] = L".\\res\\img\\turtle\\right\\";
        wchar_t add[4] = L"\0";
        wsprintf(add, L"%d", i);
        wcscat_s(name, add);
        wcscat_s(name, L".png");

        bmpTurtleR[i] = Load(name, Draw);
        if (!bmpTurtleR[i])CreateErrorLog(L"Error creating bmpTurtleR");
    }

    //FIRST SCREEN ****************************
    wchar_t first_text[30] = L"ДЪЛГАТА РАЗХОДКА\n\ndev. Daniel";
    wchar_t show[30] = L"\0";
    for (int i = 0; i < 30; i++)
    {
        show[i] = first_text[i];
        mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
        Draw->BeginDraw();
        Draw->Clear(D2D1::ColorF(D2D1::ColorF::DarkCyan));
        if (bigTextFormat && FieldBrush)
            Draw->DrawText(show, i, bigTextFormat, D2D1::RectF(200.0f, 250.0f, cl_width, cl_height), FieldBrush);
        Draw->EndDraw();
        Sleep(40);
    }
    Sleep(2500);
}
BOOL CheckRecord()
{
    if (score < 1)return no_record;

    int result = 0;

    CheckFile(record_file, &result);

    if (result == FILE_NOT_EXIST)
    {
        std::wofstream rec(record_file);
        rec << score << std::endl;
        for (int i = 0; i < 16; ++i)rec << static_cast<int>(current_player[i]) << std::endl;
        rec.close();
        return first_record;
    }

    std::wifstream check(record_file);
    check >> result;
    check.close();

    if (result < score)
    {
        std::wofstream rec(record_file);
        rec << score << std::endl;
        for (int i = 0; i < 16; ++i)rec << static_cast<int>(current_player[i]) << std::endl;
        rec.close();
        return record;
    }
    return no_record;
}
void InitGame()
{
    wcscpy_s(current_player,L"A PLAYER");
    name_size = 10;
    name_set = false;
    score = 0;
    seconds = 0;
    lifes = 3;
    game_win = false;
    game_over = false;


    vFields.clear();
    if (Mario)
    {
        Mario->Release();
        Mario = nullptr;
    }
    else
    {
        Mario = iCreate(types::mario, 100.0f, cl_height - 340.0f);
        Mario->dir = dirs::right;
        Mario->state = states::run;
    }

    Mario_upgraded = false;
    for (float i = -1000; i < 2000; i += 1000) vFields.push_back(iCreate(types::field, i, scr_height - 100.0f));
    
    vMountains.clear();
    vBenefits.clear();
    vPlatforms.clear();
    vBullets.clear();
    vTurtles.clear();

    if (Cloud1)
    {
        Cloud1->Release();
        Cloud1 = nullptr;
    }
    if (Cloud2)
    {
        Cloud2->Release();
        Cloud2 = nullptr;
    }
    
    Cloud1 = iCreate(types::cloud1, 300.0f, 80 + (float)(rand() % 20));
    Cloud2 = iCreate(types::cloud2, 500.0f, 120 + (float)(rand() % 20));
    
    if(Cloud1) Cloud1->dir = dirs::left;
       
    if (Cloud2) Cloud2->dir = dirs::left;
        

    if (Mario)base_platform_y = Mario->y - 80.0f;

    vPlatforms.push_back(iCreate(types::brick, 500.0f, base_platform_y));
    vPlatforms.push_back(iCreate(types::brick, 560.0f, base_platform_y));
    platform_rows = 1;

    if (Sun) Sun->Release();
    Sun = new ATOMS(500.0f, 100.0f, 100.0f, 100.0f);

    if (Portal)Portal->Release();
    Portal = new ATOMS(1500.0f, scr_width - 200.0f, 100.0f, 100.0f);
}
void GameOver()
{
    PlaySound(NULL, NULL, NULL);
    KillTimer(bHwnd, bTimer);

    if (game_win)
    {
        score += 500;
        if (sound)mciSendString(L"play .\\res\\snd\\tada.wav", NULL, NULL, NULL);
        Draw->BeginDraw();
        Draw->Clear(D2D1::ColorF(D2D1::ColorF::DarkGoldenrod));
        Draw->DrawText(L"ПРЕВЪРТЯ ИГРАТА !", 18, bigTextFormat, D2D1::RectF(100.0f, 200.0f, cl_width, cl_height), FieldTxtBrush);
        Draw->EndDraw();
        Sleep(3000);
    }

    wchar_t finalt[100] = L"КОСТЕНУРКИТЕ ПОБЕДИХА !";
    int fin_size = 0;

    switch (CheckRecord())
    {
        case no_record:
            if (sound)PlaySound(L".\\res\\snd\\loose.wav", NULL, SND_ASYNC);
            break;

        case first_record:
            if (sound)PlaySound(L".\\res\\snd\\record.wav", NULL, SND_ASYNC);
            wcscpy_s(finalt, L"ПЪРВИ РЕКОРД НА ИГРАТА !");
            break;

        case record:
            if (sound)PlaySound(L".\\res\\snd\\record.wav", NULL, SND_ASYNC);
            wcscpy_s(finalt, L"СВЕТОВЕН РЕКОРД НА ИГРАТА !");
            break;
    }

    for (int i = 0; i < 100; i++)
    {
        if (finalt[i] != '\0')fin_size++;
        else break;
    }

    Draw->BeginDraw();
    Draw->Clear(D2D1::ColorF(D2D1::ColorF::DarkGoldenrod));
    Draw->DrawText(finalt, fin_size, bigTextFormat, D2D1::RectF(100.0f, 200.0f, cl_width, cl_height), FieldTxtBrush);
    Draw->EndDraw();
    Sleep(7000);

    bMsg.message = WM_QUIT;
    bMsg.wParam = 0;
}

////////////////////////////////////////////////////////////

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT ReceivedMsg, WPARAM wParam, LPARAM lParam)
{
    switch (ReceivedMsg)
    {
    case WM_INITDIALOG:
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)mainIcon);
        return(INT_PTR)TRUE;
        break;

    case WM_CLOSE:
        EndDialog(hwnd, IDCANCEL);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDCANCEL:
            EndDialog(hwnd, IDCANCEL);
            break;

        case IDOK:
            name_size = GetDlgItemText(hwnd, IDC_NAME, current_player, 15);
            if (name_size < 1)
            {
                wcscpy_s(current_player, L"A PLAYER");
                name_size = 10;
                if (sound)MessageBeep(MB_ICONEXCLAMATION);
                MessageBox(bHwnd, L"Ха, ха, ха ! Забрави си името !", L"Забраватор !", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
                EndDialog(hwnd, IDCANCEL);
                break;
            }
            EndDialog(hwnd, IDOK);
            break;
        }
        break;
    }

    return (INT_PTR)FALSE;
}
LRESULT CALLBACK WinProc(HWND hwnd, UINT ReceivedMsg, WPARAM wParam, LPARAM lParam)
{
    switch (ReceivedMsg)
    {
    case WM_CREATE:
        {
            RECT clR = { 0,0,0,0 };
            GetClientRect(hwnd, &clR);
            cl_width = (float)(clR.right);
            cl_height = (float)(clR.bottom);
            SetTimer(hwnd, bTimer, 1000, NULL);
            srand((unsigned int)(time(0)));

            bBar = CreateMenu();
            bMain = CreateMenu();
            bStore = CreateMenu();

            AppendMenu(bBar, MF_POPUP, (UINT_PTR)bMain, L"Основно меню");
            AppendMenu(bBar, MF_POPUP, (UINT_PTR)bStore, L"Меню за данни");

            AppendMenu(bMain, MF_STRING, mNew, L"Нова игра");
            AppendMenu(bMain, MF_SEPARATOR, NULL, NULL);
            AppendMenu(bMain, MF_STRING, mExit, L"Изход");

            AppendMenu(bStore, MF_STRING, mSave, L"Запази игра");
            AppendMenu(bStore, MF_STRING, mLoad, L"Зареди игра");
            AppendMenu(bStore, MF_SEPARATOR, NULL, NULL);
            AppendMenu(bStore, MF_STRING, mHoF, L"Зала на славата");
    
            SetMenu(hwnd, bBar);
            InitGame();
        }
        break;

    case WM_CLOSE:
        pause = true;
        if (sound)MessageBeep(MB_ICONASTERISK);
        if (MessageBox(hwnd, L"Ако не си запазил играта, ще я загубиш !\n\nНаистина ли излизаш ?",
            L"Изход !", MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION) == IDNO)
        {
            pause = false;
            break;
        }
        GameOver();
        break;

    case WM_PAINT:
        PaintDC = BeginPaint(hwnd, &bPaint);
        FillRect(PaintDC, &bPaint.rcPaint, CreateSolidBrush(RGB(80, 80, 80)));
        EndPaint(hwnd, &bPaint);
        break;

    case WM_TIMER:
        if (pause)break;
        seconds++;
        if (seconds > 59)
        {
            minutes++;
            seconds = 0;
        }
        if (minutes > 5)game_over = true;
        break;

    case WM_SETCURSOR:
        GetCursorPos(&cur_pos);
        ScreenToClient(hwnd, &cur_pos);
        if (LOWORD(lParam) == HTCLIENT)
        {
            if (!in_client)
            {
                pause = false;
                in_client = true;
            }
            if (cur_pos.y <= 50)
            {
                if (cur_pos.x >= b1Rect.left && cur_pos.x <= b1Rect.right)
                {
                    if (!b1Hglt && sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
                    b1Hglt = true;
                }
                else
                {
                    if (b1Hglt && sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
                    b1Hglt = false;
                }

                if (cur_pos.x >= b2Rect.left && cur_pos.x <= b2Rect.right)
                {
                    if (!b2Hglt && sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
                    b2Hglt = true;
                }
                else
                {
                    if (b2Hglt && sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
                    b2Hglt = false;
                }

                if (cur_pos.x >= b3Rect.left && cur_pos.x <= b3Rect.right)
                {
                    if (!b3Hglt && sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
                    b3Hglt = true;
                }
                else
                {
                    if (b3Hglt && sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
                    b3Hglt = false;
                }
                SetCursor(outCursor);
                return true;
                break;
            }
            else
            {
                if (b1Hglt)
                {
                    if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
                    b1Hglt = false;
                }
                if (b2Hglt)
                {
                    if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
                    b2Hglt = false;
                }
                if (b3Hglt)
                {
                    if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
                    b3Hglt = false;
                }
            }
            SetCursor(bCursor);
            return true;
            break;
        }
        else
        {
            if (in_client)
            {
                pause = true;
                in_client = false;
            }
            if (b1Hglt)
            {
                if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
                b1Hglt = false;
            }
            if (b2Hglt)
            {
                if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
                b2Hglt = false;
            }
            if (b3Hglt)
            {
                if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
                b3Hglt = false;
            }
            SetCursor(LoadCursor(NULL, IDC_ARROW));
            return true;
        }
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case mNew:
            pause = true;
            if (sound)MessageBeep(MB_ICONASTERISK);
            if (MessageBox(hwnd, L"Ако не си запазил играта, ще я загубиш !\n\nНаистина ли рестартираш ?",
                L"Рестарт !", MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION) == IDNO)
            {
                pause = false;
                break;
            }
            InitGame();
            break;

        case mExit:
            SendMessage(hwnd, WM_CLOSE, NULL, NULL);
            break;

        }
        break;

    case WM_KEYDOWN:
        switch (LOWORD(wParam))
        {
        case VK_LEFT:
            if (Mario && Mario->state!=states::jump_up && Mario->state != states::jump_down)
            {
                Mario->state = states::run;
                Mario->dir = dirs::left;
            }
            break;

        case VK_RIGHT:
            if (Mario && Mario->state != states::jump_up && Mario->state != states::jump_down)
            {
                Mario->state = states::run;
                Mario->dir = dirs::right;
            }
            break;

        case VK_DOWN:
            if (Mario && Mario->state != states::jump_up && Mario->state != states::jump_down) Mario->state = states::stop;
            break;
        
        case VK_UP:
            if (Mario)
            {
                if (Mario->state == states::jump_up || Mario->state == states::jump_down)break;
                if (sound)mciSendString(L"play .\\res\\snd\\jump.wav", NULL, NULL, NULL);
                if (Mario->dir == dirs::right || Mario->dir == dirs::stop)
                {
                    jump_target_x = Mario->x + 80.0f;
                    jump_target_y = Mario->ey - 130.0f;
                    Mario->Jump(jump_target_x, jump_target_y);

                }
                else if (Mario->dir == dirs::left)
                {
                    jump_target_x = Mario->x - 80.0f;
                    jump_target_y = Mario->ey - 130.0f;
                    Mario->Jump(jump_target_x, jump_target_y);
                }
            }
            break;

        case VK_CONTROL:
            if (!Mario_upgraded)break;
            if (Mario)
            {
                if (sound)mciSendString(L"play .\\res\\snd\\shoot.wav", NULL, NULL, NULL);
                if (Mario->dir == dirs::left)
                {
                    vBullets.push_back(iCreate(types::bullet, Mario->x, Mario->y + 20.0f));
                    vBullets.back()->dir = dirs::left;
                }
                else if (Mario->dir == dirs::right)
                {
                    vBullets.push_back(iCreate(types::bullet, Mario->ex, Mario->y + 20.0f));
                    vBullets.back()->dir = dirs::right;
                }
            }
            break;
        }
        break;

    case WM_LBUTTONDOWN:
        if (cur_pos.x >= b1Rect.left && cur_pos.x <= b1Rect.right && cur_pos.y >= b1Rect.top && cur_pos.y <= b1Rect.bottom)
        {
            if (name_set)
            {
                if (sound)mciSendString(L"play .\\res\\snd\\negative.wav", NULL, NULL, NULL);
                break;
            }
            if (sound)mciSendString(L"play .\\res\\snd\\menu.wav", NULL, NULL, NULL);
            if (DialogBoxW(bIns, MAKEINTRESOURCE(IDD_PLAYER), hwnd, &DlgProc) == IDOK)name_set = true;
            break;
        }



        break;

    default:return DefWindowProc(hwnd, ReceivedMsg, wParam, lParam);

    }

    return(LRESULT)FALSE;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    bIns = hInstance;
    if (!bIns)ErrExit(eClass);

    int check_temp = 0;
    CheckFile(Ltemp_file, &check_temp);
    if (check_temp == FILE_EXIST)ErrExit(eStarted);
    else
    {
        std::wofstream tmp(Ltemp_file);
        tmp << L"Играта рЪботи !";
        tmp.close();
    }
    
    if (GetSystemMetrics(SM_CXSCREEN) < scr_width + 50 || GetSystemMetrics(SM_CYSCREEN) < scr_height + 10)ErrExit(eScreen);

    mainIcon = (HICON)LoadImage(NULL, L".\\res\\main.ico", IMAGE_ICON, 128, 128, LR_LOADFROMFILE);
    if (!mainIcon)ErrExit(eIcon);

    bCursor = LoadCursorFromFile(L".\\res\\bcursor.ani");
    outCursor = LoadCursorFromFile(L".\\res\\out.ani");
    if (!bCursor || !outCursor)ErrExit(eCursor);

    ZeroMemory(&bWin, sizeof(WNDCLASS));

    bWin.hInstance = bIns;
    bWin.lpszClassName = bWinClassName;
    bWin.lpfnWndProc = &WinProc;
    bWin.hbrBackground = CreateSolidBrush(RGB(80, 80, 80));
    bWin.hIcon = mainIcon;
    bWin.hCursor = bCursor;
    bWin.style = CS_DROPSHADOW;

    if (!RegisterClass(&bWin))ErrExit(eClass);

    bHwnd = CreateWindow(bWinClassName, L"ДЪЛГАТА РАЗХОДКА", WS_CAPTION | WS_SYSMENU, 50, 10, (int)(scr_width), 
        (int)(scr_height), NULL, NULL, bIns, NULL);
    if (!bHwnd)ErrExit(eWindow);
    else
    {
        ShowWindow(bHwnd, SW_SHOWDEFAULT);
        InitD2D1();
    }

    while (bMsg.message != WM_QUIT)
    {
        if ((bRet = PeekMessage(&bMsg, bHwnd, NULL, NULL, PM_REMOVE)) != 0)
        {
            if (bRet == -1)ErrExit(eMsg);
            TranslateMessage(&bMsg);
            DispatchMessage(&bMsg);
        }

        if (pause)
        {
            if (show_help)continue;
            Draw->BeginDraw();
            Draw->FillRectangle(D2D1::RectF(0, 0, cl_width, 50.0f), ButBckgBrush);
            Draw->FillRectangle(D2D1::RectF(0, 50.0f, cl_width, cl_height), FieldBrush);
            Draw->DrawLine(D2D1::Point2F(b1Rect.right + 50.0f, 0), D2D1::Point2F(b1Rect.right + 50.0f, 50.0f), ButBorderBrush, 5.0f);
            Draw->DrawLine(D2D1::Point2F(b2Rect.right + 50.0f, 0), D2D1::Point2F(b2Rect.right + 50.0f, 50.0f), ButBorderBrush, 5.0f);

            if (name_set)
                Draw->DrawText(L"ИМЕ НА ИГРАЧ", 13, nrmTextFormat, D2D1::RectF(50.0f, 0, b1Rect.right, 50.0f), ButInactiveBrush);
            else
            {
                if(b1Hglt)
                    Draw->DrawText(L"ИМЕ НА ИГРАЧ", 13, nrmTextFormat, D2D1::RectF(50.0f, 0, b1Rect.right, 50.0f), ButHgltBrush);
                else
                    Draw->DrawText(L"ИМЕ НА ИГРАЧ", 13, nrmTextFormat, D2D1::RectF(50.0f, 0, b1Rect.right, 50.0f), ButTxtBrush);
            }

            if (b2Hglt)
                Draw->DrawText(L"ЗВУЦИ ON / OFF", 15, nrmTextFormat, D2D1::RectF(b2Rect.left + 50.0f, 0, b2Rect.right, 50.0f), 
                    ButHgltBrush);
            else
                Draw->DrawText(L"ЗВУЦИ ON / OFF", 15, nrmTextFormat, D2D1::RectF(b2Rect.left + 50.0f, 0, b2Rect.right, 
                    50.0f),ButTxtBrush);

            if (b3Hglt)
                Draw->DrawText(L"ПОМОЩ ЗА ИГРАТА", 16, nrmTextFormat, D2D1::RectF(b3Rect.left + 50.0f, 0, b3Rect.right, 50.0f),
                    ButHgltBrush);
            else
                Draw->DrawText(L"ПОМОЩ ЗА ИГРАТА", 16, nrmTextFormat, D2D1::RectF(b3Rect.left + 50.0f, 0, b3Rect.right,
                    50.0f), ButTxtBrush);
        
            Draw->DrawText(L"ПАУЗА", 6, bigTextFormat, D2D1::RectF(450.0f, 250.0f, cl_width, cl_height), FieldTxtBrush);
            Draw->EndDraw();
            continue;
        }

        /////////////////////////////////////////////////

        //GAME ENGINE **********************************

        if (!vFields.empty())
        {
            for (std::vector<obj_ptr>::iterator field = vFields.begin(); field < vFields.end(); field++)
            {
                if ((*field)->Move() == return_type::R_OUT)
                {
                    (*field)->Release();
                    vFields.erase(field);
                    break;
                }
            }
        }

        if (!vFields.empty() &&vFields.size() < 2)
        {
            float tempx = (*vFields.begin())->x - 1000.0f;

            vFields.push_back(iCreate(types::field,
                (*vFields.begin())->ex, scr_height - 100.0f));
            vFields.insert(vFields.begin(),iCreate(types::field, tempx, scr_height - 100.0f));
        }        
        if (Mario)
        {
            if (Mario->state == states::stop || Mario->state == states::fall)opposite_dir = dirs::stop;
            else if (Mario->dir == dirs::right)opposite_dir = dirs::left;
            else if (Mario->dir == dirs::left)opposite_dir = dirs::right;
        }
        
        if (!vFields.empty())
            for (int i = 0; i < vFields.size(); i++) vFields[i]->dir = opposite_dir;
            
        //MARIO MOVE ************************************************

        if (Mario)
        {
            return_type return_result = return_type::R_OUT;
            if (Mario->state == states::jump_up)
            {
                return_result = Mario->Jump(jump_target_x, jump_target_y);
            }
            else if (Mario->state == states::jump_down)
            {
                if (Mario->prev_dir == dirs::right || Mario->prev_dir == dirs::stop)
                    return_result = Mario->Jump(jump_target_x + 100, Mario->old_y);
                else if (Mario->prev_dir == dirs::left)
                    return_result = Mario->Jump(jump_target_x - 100, Mario->old_y);
                else Mario->state = states::fall;

                if (return_result == return_type::R_OUT) Mario->state = states::fall;
            }
            else if (Mario->state == states::run)
            {
                Mario->prev_dir = Mario->dir;
                Mario->Move();
            }
            else if (Mario->state == states::fall)
            {
                Mario->dir = dirs::down;
                if (Mario->Move() == return_type::R_OUT)
                {
                    Mario->y = scr_height - 155.0f;
                    Mario->SetDims();
                    Mario->state = states::run;
                    if (Mario->prev_dir != dirs::down && Mario->prev_dir != dirs::up)Mario->dir = Mario->prev_dir;
                    else Mario->dir = dirs::right;
                }
            }
            else if (Mario->state == states::stop)Mario->dir = dirs::right;
            
            if (Mario->x > cl_width && (Mario->state == states::run || Mario->state == states::stop))
            {
                Mario->x = cl_width - 50.0f;
                Mario->SetDims();
            }

            if (Mario->x < 0 && (Mario->state == states::run || Mario->state == states::stop))
            {
                Mario->x = 0;
                Mario->SetDims();
            }
        }
        
        //FALL OR JUMP ON PLATFORM
        if (Mario && !vPlatforms.empty())
        {
            if (Mario->state == states::jump_up)
            {
                for (std::vector<obj_ptr>::iterator platform = vPlatforms.begin(); platform < vPlatforms.end(); ++platform)
                {
                    if (!(Mario->x >= (*platform)->ex || Mario->ex <= (*platform)->x
                        || Mario->y >= (*platform)->ey || Mario->ey <= (*platform)->y))
                    {
                        if (Mario->state == states::jump_up && Mario->x + 5.0f > (*platform)->x && Mario->x < (*platform)->ex - 5.0f)
                        {
                            if ((*platform)->type == types::goldbrick)
                            {
                                if (sound)mciSendString(L"play .\\res\\snd\\goldhit.wav", NULL, NULL, NULL);
                                (*platform)->Transform(types::brick);
                                if (rand() % 5 == 3)
                                    vBenefits.push_back(iCreate(types::mushroom, (*platform)->x + 10.0f, (*platform)->y - 40.0f));
                                else
                                    vBenefits.push_back(iCreate(types::coin, (*platform)->x + 25.0f, (*platform)->y - 20.0f));
                                
                                vBenefits.back()->dir = dirs::left;
                            }
                            Mario->state = states::fall;
                            
                            break;
                        }
                        else
                        {
                            if (sound)mciSendString(L"play .\\res\\snd\\hit.wav", NULL, NULL, NULL);
                            Mario->state = states::stop;
                            if (Mario->prev_dir != dirs::down && Mario->prev_dir != dirs::up)Mario->dir = Mario->prev_dir;
                            else Mario->dir = dirs::right;
                            Mario->x = (*platform)->x;
                            Mario->y = (*platform)->y - 55.0f;

                            Mario->SetDims();
                            break;
                        }
                    }
                }
            }
        }

        if (Mario && !vPlatforms.empty())
        {
            if (Mario->state == states::jump_down || Mario->state == states::fall)
            {
                for (std::vector<obj_ptr>::iterator platform = vPlatforms.begin(); platform < vPlatforms.end(); ++platform)
                {
                    if (!(Mario->x >= (*platform)->ex || Mario->ex <= (*platform)->x
                        || Mario->y >= (*platform)->ey || Mario->ey <= (*platform)->y))
                    {
                        if (Mario->state == states::fall && Mario->y > (*platform)->y)break;
                        Mario->state = states::stop;
                        if (Mario->prev_dir != dirs::down && Mario->prev_dir != dirs::up)Mario->dir = Mario->prev_dir;
                        else Mario->dir = dirs::right;
                        Mario->x = (*platform)->x;
                        Mario->y = (*platform)->y - 55.0f;
                        Mario->SetDims();
                        break;
                    }
                }
            }
        }

        if (Mario && (Mario->state == states::jump_down || Mario->state == states::fall) && !vTurtles.empty())
        {
            for (std::vector<obj_ptr>::iterator tur = vTurtles.begin(); tur < vTurtles.end(); ++tur)
            {
                if (!(Mario->x >= (*tur)->ex || Mario->ex <= (*tur)->x || Mario->y >= (*tur)->ey || Mario->ey <= (*tur)->y))
                {
                    if (sound)mciSendString(L"play .\\res\\snd\\jump.wav", NULL, NULL, NULL);
                    if (Mario->prev_dir == dirs::right || Mario->prev_dir == dirs::stop)
                    {
                        jump_target_x = Mario->x + 80.0f;
                        jump_target_y = Mario->ey - 130.0f;
                        Mario->state = states::run;
                        Mario->dir = dirs::right;
                        Mario->Jump(jump_target_x, jump_target_y);

                    }
                    else if (Mario->prev_dir == dirs::left)
                    {
                        jump_target_x = Mario->x - 80.0f;
                        jump_target_y = Mario->ey - 130.0f;
                        Mario->state = states::run;
                        Mario->dir = dirs::left;
                        Mario->Jump(jump_target_x, jump_target_y);
                    }
                    else Mario->state = states::fall;
                    
                    if ((*tur)->type == types::turtle)
                        (*tur)->Transform(types::turtle_blocked);
                    else
                    {
                        (*tur)->state = states::hit;
                        (*tur)->dir = Mario->dir;
                    }
                    break;
                }
            }
        }

        if (Mario && !vTurtles.empty() && (Mario->state == states::run || Mario->state == states::stop))
        {
            for (std::vector<obj_ptr>::iterator tur = vTurtles.begin(); tur < vTurtles.end(); ++tur)
            {
                if (!(Mario->x >= (*tur)->ex || Mario->ex <= (*tur)->x || Mario->y >= (*tur)->ey || Mario->ey <= (*tur)->y))
                {
                    if ((*tur)->type == types::turtle_blocked)
                    {
                        if (sound)mciSendString(L"play .\\res\\snd\\hit.wav", NULL, NULL, NULL);
                        (*tur)->state = states::hit;
                        if ((*tur)->dir != dirs::left && (*tur)->dir != dirs::right)(*tur)->dir = Mario->dir;
                        break;
                    }
                
                }
            }
        }

        if (!vTurtles.empty())
        {
            for (std::vector<obj_ptr>::iterator tur = vTurtles.begin(); tur < vTurtles.end(); ++tur)
            {
                if ((*tur)->type == types::turtle)continue;
                if ((*tur)->state == states::hit)
                {
                    if ((*tur)->dir == dirs::right)
                    {
                        (*tur)->x += 2.0f;
                        (*tur)->SetDims();
                        if ((*tur)->x >= cl_width)
                        {
                            (*tur)->Release();
                            vTurtles.erase(tur);
                            score += 50;
                            break;
                        }
                    }
                    if ((*tur)->dir == dirs::left)
                    {
                        (*tur)->x -= 2.0f;
                        (*tur)->SetDims();
                        if ((*tur)->ex <= 0)
                        {
                            (*tur)->Release();
                            score += 50;
                            vTurtles.erase(tur);
                            break;
                        }
                    }
                }
            }
        }

        if (vTurtles.size() > 1)
        {
            for (std::vector<obj_ptr>::iterator allturtles = vTurtles.begin(); allturtles < vTurtles.end(); ++allturtles)
            {
                bool killed = false;
                if ((*allturtles)->state == states::hit)
                {
                    for (std::vector<obj_ptr>::iterator aturtle = vTurtles.begin(); aturtle < vTurtles.end(); ++aturtle)
                    {
                        if (allturtles == aturtle)continue;

                        if (!((*allturtles)->x >= (*aturtle)->ex || (*allturtles)->ex <= (*aturtle)->x
                            || (*allturtles)->y >= (*aturtle)->ey || (*allturtles)->ey <= (*aturtle)->y))
                        {
                            if ((*aturtle)->type == types::turtle_blocked)continue;
                            if (sound)mciSendString(L"play .\\res\\snd\\hit.wav", NULL, NULL, NULL);
                            (*aturtle)->Transform(types::turtle_blocked);
                            (*allturtles)->Release();
                            vTurtles.erase(allturtles);
                            score += 50;
                            killed = true;
                            break;
                        }
                    }
                }
                if (killed)break;
            }
        }

        //RUNNING ON PLATFORM
        if (Mario)
        {
            if (Mario->state == states::move || Mario->state == states::run || Mario->state == states::stop)
            {
                if (Mario->y < scr_height - 155.0f)
                {

                    states current_state = Mario->state;
                    dirs current_dir = Mario->dir;

                    Mario->state = states::fall;

                    if (!vPlatforms.empty())
                    {
                        for (std::vector<obj_ptr>::iterator platform = vPlatforms.begin(); platform < vPlatforms.end(); ++platform)
                        {
                            if (!(Mario->x > (*platform)->ex || Mario->ex < (*platform)->x
                                || Mario->y > (*platform)->ey || Mario->ey < (*platform)->y))
                            {
                                if ((Mario->ey > (*platform)->y && Mario->ey < (*platform)->ey)
                                    || (Mario->y > (*platform)->y && Mario->y < (*platform)->ey))break;
                                Mario->state = current_state;
                                Mario->dir = current_dir;
                                break;
                            }
                            
                        }
                    }
                }
            }
        }
        
        //CLOUDS ********************************************

        if (Cloud1)
        {
            if (Cloud1)
            {
                if(Mario)Cloud1->dir = opposite_dir;
                if (Cloud1->dir == dirs::stop)Cloud1->dir = dirs::left;
            }
            
            if (Cloud1->Move() == return_type::R_OUT && ((Cloud1->ex<=0&&Cloud1->dir==dirs::left)
                || (Cloud1->x >= cl_width && Cloud1->dir == dirs::right)))
            {
                Cloud1->Release();
                Cloud1 = nullptr;
            }
        }
        else Cloud1 = iCreate(types::cloud1, cl_width + 300.0f, 80 + (float)(rand() % 20));

        if (Cloud2)
        {
            if (Cloud2)
            {
                Cloud2->dir = opposite_dir;
                if (Cloud2->dir == dirs::stop)Cloud2->dir = dirs::left;
            }

            if (Cloud2->Move() == return_type::R_OUT && ((Cloud2->ex <= 0 && Cloud2->dir == dirs::left)
                || (Cloud2->x >= cl_width && Cloud2->dir == dirs::right)))
            {
                Cloud2->Release();
                Cloud2 = nullptr;
                
            }
        }
        else Cloud2 = iCreate(types::cloud2, 500.0f, 120 + (float)(rand() % 20));

        if (vMountains.size() < 3)
        {
            int atype = rand() % 3 + 7;
            
            float aheight = 0;
            float awidth = cl_width + 400 * vMountains.size() + rand() % 80;

            switch (atype)
            {
            case 7:
                aheight = scr_height - 254.0f;
                break;

            case 8:
                aheight = scr_height - 279.0f;
                break;

            case 9:
                aheight = scr_height - 411.0f;
                break;

            }

            vMountains.push_back(iCreate(static_cast<types>(atype), awidth, aheight));
            vMountains.back()->dir = dirs::left;
        }

        if (!vMountains.empty())
        {
            for (std::vector<obj_ptr>::iterator mount = vMountains.begin(); mount < vMountains.end(); mount++)
            {
                (*mount)->dir = opposite_dir;
                return_type check = (*mount)->Move();
                if ((*mount)->ex <= cl_width && (*mount)->type == types::mountain1)mountain1_visible = true;
                if ((*mount)->ex <= cl_width && (*mount)->type == types::mountain2)mountain2_visible = true;
                if ((*mount)->ex <= cl_width && (*mount)->type == types::mountain3)mountain3_visible = true;
                
                if ((*mount)->type == types::mountain1 && mountain1_visible && check == return_type::R_OUT)
                {
                    int matches = 0;
                    for(int i=0;i<vMountains.size();i++)
                    {
                        if (vMountains[i]->type == types::mountain1)matches++;
                        if(matches<2)mountain1_visible = false;
                    }
                    (*mount)->Release();
                    vMountains.erase(mount);
                    break;
                }

                if ((*mount)->type == types::mountain2 && mountain2_visible && check == return_type::R_OUT)
                {
                    int matches = 0;
                    for (int i = 0; i < vMountains.size(); i++)
                    {
                        if (vMountains[i]->type == types::mountain2)matches++;
                        if (matches < 2)mountain2_visible = false;
                    }(*mount)->Release();
                    vMountains.erase(mount);
                    break;
                }

                if ((*mount)->type == types::mountain3 && mountain3_visible && check == return_type::R_OUT)
                {
                    int matches = 0;
                    for (int i = 0; i < vMountains.size(); i++)
                    {
                        if (vMountains[i]->type == types::mountain3)matches++;
                        if (matches < 2)mountain3_visible = false;
                    }(*mount)->Release();
                    vMountains.erase(mount);
                    break;
                }


            }
        }

        ///////////////////////////////////////////////////

        if (!vPlatforms.empty())
        {
            for (std::vector<obj_ptr>::iterator plat = vPlatforms.begin(); plat < vPlatforms.end(); plat++)
            {
                (*plat)->dir = opposite_dir;
                if ((*plat)->dir == dirs::left && (*plat)->Move() == return_type::R_OUT)
                {
                    (*plat)->Release();
                    vPlatforms.erase(plat);
                    platform_rows--;
                    break;
                }
            }
        }
        else
        {
            vPlatforms.push_back(iCreate(types::brick, cl_width, base_platform_y));
            vPlatforms.push_back(iCreate(types::brick, cl_width + 60.0f, base_platform_y));
            platform_rows = 1;
        }

        if (platform_rows <= 2 && Mario)
        {
            if (rand() % 500 == 6)
            {
                float nextx = cl_width;
                if (!vPlatforms.empty() && vPlatforms.back()->ex > cl_width)nextx = vPlatforms.back()->ex + 80.0f;
                base_platform_y = (Mario->y - (80.0f * (rand() % 4)));
                if (base_platform_y >= Mario->y - 80.0f)base_platform_y = Mario->y - 80.0f;
                
                vPlatforms.push_back(iCreate(types::brick, nextx, base_platform_y));
                if (rand() % 10 == 4)
                    vPlatforms.push_back(iCreate(types::goldbrick, nextx + 60.0f, base_platform_y));
                if (rand() % 5 == 3)
                    vPlatforms.push_back(iCreate(types::brick, nextx + 120.0f, base_platform_y));
                platform_rows++;
            }
        }

        if (!vBenefits.empty())
        {
            for (std::vector<obj_ptr>::iterator ben = vBenefits.begin(); ben < vBenefits.end(); ben++)
            {
                (*ben)->dir = opposite_dir;
                if ((*ben)->dir == dirs::left && (*ben)->Move() == return_type::R_OUT)
                {
                    (*ben)->Release();
                    vBenefits.erase(ben);
                    break;
                }
            }
        }

        if (!vBullets.empty())
        {
            for (std::vector<obj_ptr>::iterator bul = vBullets.begin(); bul < vBullets.end(); ++bul)
            {
                if ((*bul)->Move() == return_type::R_OUT)
                {
                    (*bul)->Release();
                    vBullets.erase(bul);
                    break;
                }
            }

        }

        if (!vBenefits.empty() && Mario)
        {
            for (std::vector<obj_ptr>::iterator ben = vBenefits.begin(); ben < vBenefits.end(); ++ben)
            {
                if (!(Mario->x >= (*ben)->ex || Mario->ex <= (*ben)->x || Mario->y >= (*ben)->ey || Mario->ey <= (*ben)->y))
                {
                    if ((*ben)->type == types::coin)score += 50;
                    else if ((*ben)->type == types::mushroom)
                    {
                        if (sound)mciSendString(L"play .\\res\\snd\\life.wav", NULL, NULL, NULL);

                        if (!Mario_upgraded)Mario_upgraded = true;
                        else
                        {
                            lifes++;
                            score += 100;
                        }
                    }
                    (*ben)->Release();
                    vBenefits.erase(ben);
                    break;
                }
            }
        }

        if (rand() % 500 == 66 && vTurtles.size() <= 4)
            vTurtles.push_back(iCreate(types::turtle, cl_width, cl_height - 140.0f));
        
        if (!vTurtles.empty())
        {
            for (int i = 0; i < vTurtles.size(); i++)
            {
                if (vTurtles[i]->type == types::turtle) vTurtles[i]->Move();
                else
                {
                    if (opposite_dir == dirs::left)
                    {
                        vTurtles[i]->x -= 0.8f;
                        vTurtles[i]->SetDims();
                    }
                    else if (opposite_dir == dirs::right)
                    {
                        vTurtles[i]->x += 0.8f;
                        vTurtles[i]->SetDims();
                    }
                }
                if (Mario)
                {
                    if (!(Mario->x >= vTurtles[i]->ex || Mario->ex <= vTurtles[i]->x
                        || Mario->y >= vTurtles[i]->ey || Mario->ey <= vTurtles[i]->y))
                    {
                        if (vTurtles[i]->type == types::turtle)
                        {
                            if (Mario_upgraded)
                            {
                                Mario_upgraded = false;
                                vTurtles[i]->Transform(types::turtle_blocked);
                                Mario->state = states::fall;
                            }
                            else if (Mario->state != states::jump_down && Mario->state != states::fall)
                            {
                                if (sound)mciSendString(L"play .\\res\\snd\\kill.wav", NULL, NULL, NULL);
                                lifes--;
                                vTurtles[i]->Transform(types::turtle_blocked);
                                Mario->state = states::fall;
                                if (lifes <= 0)game_over = true;
                                break;
                            }
                            
                        }
                    }
                }
            }
        }

        if (Portal && Mario)
        {
            if (Mario->dir == dirs::right)
            {
                Portal->x -= 0.8f;
                Portal->SetDims();
            }
            else if (Mario->dir == dirs::left)
            {
                Portal->x += 0.8f;
                Portal->SetDims();
            }

            if (!(Mario->x >= Portal->ex || Mario->ex +20.0f <= Portal->x || Mario->y >= Portal->ey || Mario->ey <= Portal->y))
            {
                game_win=true;
                GameOver();
            }
        }


        //DRAW THINGS **********************************
        Draw->BeginDraw();
        Draw->FillRectangle(D2D1::RectF(0, 0, cl_width, 50.0f), ButBckgBrush);
        Draw->FillRectangle(D2D1::RectF(0, 50.0f, cl_width, cl_height), FieldBrush);
        Draw->DrawLine(D2D1::Point2F(b1Rect.right + 50.0f, 0), D2D1::Point2F(b1Rect.right + 50.0f, 50.0f), ButBorderBrush, 5.0f);
        Draw->DrawLine(D2D1::Point2F(b2Rect.right + 50.0f, 0), D2D1::Point2F(b2Rect.right + 50.0f, 50.0f), ButBorderBrush, 5.0f);

        if (name_set)
            Draw->DrawText(L"ИМЕ НА ИГРАЧ", 13, nrmTextFormat, D2D1::RectF(50.0f, 0, b1Rect.right, 50.0f), ButInactiveBrush);
        else
        {
            if (b1Hglt)
                Draw->DrawText(L"ИМЕ НА ИГРАЧ", 13, nrmTextFormat, D2D1::RectF(50.0f, 0, b1Rect.right, 50.0f), ButHgltBrush);
            else
                Draw->DrawText(L"ИМЕ НА ИГРАЧ", 13, nrmTextFormat, D2D1::RectF(50.0f, 0, b1Rect.right, 50.0f), ButTxtBrush);
        }

        if (b2Hglt)
            Draw->DrawText(L"ЗВУЦИ ON / OFF", 15, nrmTextFormat, D2D1::RectF(b2Rect.left + 50.0f, 0, b2Rect.right, 50.0f),
                ButHgltBrush);
        else
            Draw->DrawText(L"ЗВУЦИ ON / OFF", 15, nrmTextFormat, D2D1::RectF(b2Rect.left + 50.0f, 0, b2Rect.right,
                50.0f), ButTxtBrush);

        if (b3Hglt)
            Draw->DrawText(L"ПОМОЩ ЗА ИГРАТА", 16, nrmTextFormat, D2D1::RectF(b3Rect.left + 50.0f, 0, b3Rect.right, 50.0f),
                ButHgltBrush);
        else
            Draw->DrawText(L"ПОМОЩ ЗА ИГРАТА", 16, nrmTextFormat, D2D1::RectF(b3Rect.left + 50.0f, 0, b3Rect.right,
                50.0f), ButTxtBrush);

        if (Sun)
            Draw->DrawBitmap(bmpSun, D2D1::RectF(Sun->x, Sun->y, Sun->ex, Sun->ey));

        if (!vMountains.empty())
        {
            for (int i = 0; i < vMountains.size(); ++i)
            {
                switch (vMountains[i]->type)
                {
                case types::mountain1:
                    Draw->DrawBitmap(bmpMountain1, D2D1::RectF(vMountains[i]->x, vMountains[i]->y,
                        vMountains[i]->ex, vMountains[i]->ey));
                    break;

                case types::mountain2:
                    Draw->DrawBitmap(bmpMountain2, D2D1::RectF(vMountains[i]->x, vMountains[i]->y,
                        vMountains[i]->ex, vMountains[i]->ey));
                    break;

                case types::mountain3:
                    Draw->DrawBitmap(bmpMountain3, D2D1::RectF(vMountains[i]->x, vMountains[i]->y,
                        vMountains[i]->ex, vMountains[i]->ey));
                    break;
                }
            }
        }

        if (!vFields.empty())
            for (int i = 0; i < vFields.size(); i++)
                Draw->DrawBitmap(bmpField, D2D1::RectF(vFields[i]->x, vFields[i]->y, vFields[i]->ex, vFields[i]->ey));
        
        if (!vPlatforms.empty())
        {
            for (int i = 0; i < vPlatforms.size(); ++i)
            {
                switch (vPlatforms[i]->type)
                {
                case types::brick:
                    Draw->DrawBitmap(bmpBrick, D2D1::RectF(vPlatforms[i]->x, vPlatforms[i]->y,
                        vPlatforms[i]->ex, vPlatforms[i]->ey));
                    break;

                case types::goldbrick:
                    Draw->DrawBitmap(bmpGoldBrick, D2D1::RectF(vPlatforms[i]->x, vPlatforms[i]->y,
                        vPlatforms[i]->ex, vPlatforms[i]->ey));
                    break;
                }
            }
        }
        
        //TEXT *******************************************

        wchar_t status_text[200] = L"\0";
        wchar_t addon[5] = L"\0";
        int stat_size = 0;

        wcscpy_s(status_text, current_player);

        wcscat_s(status_text, L", животи: ");
        wsprintf(addon, L"%d", lifes);
        wcscat_s(status_text, addon);

        wcscat_s(status_text, L", резултат: ");
        wsprintf(addon, L"%d", score);
        wcscat_s(status_text, addon);

        wcscat_s(status_text, L", време: ");
        wsprintf(addon, L"%d", minutes);
        wcscat_s(status_text, addon);

        wcscat_s(status_text, L" : ");
        wsprintf(addon, L"%d", seconds);
        wcscat_s(status_text, addon);

        wcscat_s(status_text, L", разстояние: ");
        if (Mario && Portal)
            swprintf(addon, 5, L"%lf", (Portal->x - Mario->ex));
        wcscat_s(status_text, addon);

        for (int i = 0; i < 200; i++)
        {
            if (status_text[i] != '\0')stat_size++;
            else break;
        }

        Draw->DrawText(status_text, stat_size, nrmTextFormat, D2D1::RectF(5.0f, cl_height - 50.0f, cl_width, cl_height), FieldTxtBrush);
        ///////////////////////////////////////////////////

        if (!game_over)
        {
            if (Mario)
            {
                switch (Mario->state)
                {
                case states::run:
                    if (Mario->dir == dirs::right || Mario->dir == dirs::stop)
                        Draw->DrawBitmap(bmpMarioR[Mario->GetFrame()], D2D1::RectF(Mario->x, Mario->y, Mario->ex, Mario->ey));
                    else if (Mario->dir == dirs::left)
                        Draw->DrawBitmap(bmpMarioL[Mario->GetFrame()], D2D1::RectF(Mario->x, Mario->y, Mario->ex, Mario->ey));
                    break;

                case states::move:
                    if (Mario->dir == dirs::right || Mario->dir == dirs::stop)
                        Draw->DrawBitmap(bmpMarioR[Mario->GetFrame()], D2D1::RectF(Mario->x, Mario->y, Mario->ex, Mario->ey));
                    else if (Mario->dir == dirs::left)
                        Draw->DrawBitmap(bmpMarioL[Mario->GetFrame()], D2D1::RectF(Mario->x, Mario->y, Mario->ex, Mario->ey));
                    break;

                case states::stop:
                    if (Mario->dir == dirs::right || Mario->dir == dirs::stop)
                        Draw->DrawBitmap(bmpMarioR[3], D2D1::RectF(Mario->x, Mario->y, Mario->ex, Mario->ey));
                    else if (Mario->dir == dirs::left)
                        Draw->DrawBitmap(bmpMarioL[3], D2D1::RectF(Mario->x, Mario->y, Mario->ex, Mario->ey));
                    else if (Mario->dir == dirs::stop && Mario->prev_dir == dirs::left)
                        Draw->DrawBitmap(bmpMarioL[3], D2D1::RectF(Mario->x, Mario->y, Mario->ex, Mario->ey));
                    else if (Mario->dir == dirs::stop && Mario->prev_dir == dirs::right)
                        Draw->DrawBitmap(bmpMarioR[3], D2D1::RectF(Mario->x, Mario->y, Mario->ex, Mario->ey));
                    break;

                case states::jump_up:
                    if (Mario->prev_dir == dirs::right || Mario->prev_dir == dirs::stop)
                        Draw->DrawBitmap(bmpMarioJR, D2D1::RectF(Mario->x, Mario->y, Mario->ex, Mario->ey));
                    else if (Mario->prev_dir == dirs::left)
                        Draw->DrawBitmap(bmpMarioJL, D2D1::RectF(Mario->x, Mario->y, Mario->ex, Mario->ey));
                    break;

                case states::jump_down:
                    if (Mario->prev_dir == dirs::right || Mario->prev_dir == dirs::stop)
                        Draw->DrawBitmap(bmpMarioJR, D2D1::RectF(Mario->x, Mario->y, Mario->ex, Mario->ey));
                    else if (Mario->prev_dir == dirs::left)
                        Draw->DrawBitmap(bmpMarioJL, D2D1::RectF(Mario->x, Mario->y, Mario->ex, Mario->ey));
                    break;

                case states::fall:
                    if (Mario->prev_dir == dirs::right || Mario->prev_dir == dirs::stop)
                        Draw->DrawBitmap(bmpMarioJR, D2D1::RectF(Mario->x, Mario->y, Mario->ex, Mario->ey));
                    else if (Mario->prev_dir == dirs::left)
                        Draw->DrawBitmap(bmpMarioJL, D2D1::RectF(Mario->x, Mario->y, Mario->ex, Mario->ey));
                    break;

                }
            }

            if (Mario && Mario_upgraded)
                Draw->DrawBitmap(bmpAngry, D2D1::RectF(Mario->x + 20.0f, Mario->y - 22.0f, Mario->x + 35.0f, Mario->y));

            if (!vBenefits.empty())
            {
                for (int i = 0; i < vBenefits.size(); i++)
                {
                    switch (vBenefits[i]->type)
                    {
                    case types::mushroom:
                        Draw->DrawBitmap(bmpMushroom,
                            D2D1::RectF(vBenefits[i]->x, vBenefits[i]->y, vBenefits[i]->ex, vBenefits[i]->ey));
                        break;

                    case types::coin:
                        Draw->DrawBitmap(bmpGold,
                            D2D1::RectF(vBenefits[i]->x, vBenefits[i]->y, vBenefits[i]->ex, vBenefits[i]->ey));
                        break;
                    }
                }
            }

            if (!vBullets.empty())
            {
                for (int i = 0; i < vBullets.size(); i++)
                    Draw->DrawBitmap(bmpBullet, D2D1::RectF(vBullets[i]->x, vBullets[i]->y, vBullets[i]->ex, vBullets[i]->ey));
            }

            if (!vTurtles.empty())
            {
                for (int i = 0; i < vTurtles.size(); i++)
                {
                    switch (vTurtles[i]->type)
                    {
                    case types::turtle:
                        if (vTurtles[i]->dir == dirs::left)
                            Draw->DrawBitmap(bmpTurtleL[vTurtles[i]->GetFrame()],
                                D2D1::RectF(vTurtles[i]->x, vTurtles[i]->y, vTurtles[i]->ex, vTurtles[i]->ey));
                        if (vTurtles[i]->dir == dirs::right)
                            Draw->DrawBitmap(bmpTurtleR[vTurtles[i]->GetFrame()],
                                D2D1::RectF(vTurtles[i]->x, vTurtles[i]->y, vTurtles[i]->ex, vTurtles[i]->ey));
                        break;


                    case types::turtle_blocked:
                        Draw->DrawBitmap(bmpTurtleBlocked,
                            D2D1::RectF(vTurtles[i]->x, vTurtles[i]->y, vTurtles[i]->ex, vTurtles[i]->ey));
                        break;
                    }
                }
            }

            if (!vTurtles.empty() && !vBullets.empty())
            {
                for (std::vector<obj_ptr>::iterator bul = vBullets.begin(); bul < vBullets.end(); ++bul)
                {
                    bool killed = false;

                    for (std::vector<obj_ptr>::iterator tur = vTurtles.begin(); tur < vTurtles.end(); ++tur)
                    {
                        if (!((*bul)->x >= (*tur)->ex || (*bul)->ex <= (*tur)->x || (*bul)->y >= (*tur)->ey || (*bul)->ey <= (*tur)->y))
                        {
                            score += 50;
                            killed = true;
                            (*bul)->Release();
                            (*tur)->Release();
                            vBullets.erase(bul);
                            vTurtles.erase(tur);

                        }
                    }
                    if (killed)break;
                }
            }
        }
        else
        {
            if (Mario)
                Draw->DrawBitmap(bmpDeath, D2D1::RectF(Mario->x, Mario->y, Mario->ex, Mario->ey));
        }
        
        if (Portal && Portal->x <= cl_width)Draw->DrawBitmap(bmpPortal, D2D1::RectF(Portal->x, Portal->y, Portal->ex, Portal->ey));
        
        ////////////////////////////////////////////////////////
        
        if (Cloud1)Draw->DrawBitmap(bmpCloud1, D2D1::RectF(Cloud1->x, Cloud1->y, Cloud1->ex, Cloud1->ey));
        if (Cloud2)Draw->DrawBitmap(bmpCloud2, D2D1::RectF(Cloud2->x, Cloud2->y, Cloud2->ex, Cloud2->ey));

        Draw->EndDraw();

        if (game_over)
        {
            Sleep(2500);
            GameOver();
        }
    }

    std::remove(temp_file);
    return (int) bMsg.wParam;
}