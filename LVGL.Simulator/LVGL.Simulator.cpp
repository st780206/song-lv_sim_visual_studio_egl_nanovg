/*
 * PROJECT:   LVGL PC Simulator using Visual Studio
 * FILE:      LVGL.Simulator.cpp
 * PURPOSE:   Implementation for LVGL ported to Windows Desktop
 *
 * LICENSE:   The MIT License
 *
 * DEVELOPER: Mouri_Naruto (Mouri_Naruto AT Outlook.com)
 */

#include <Windows.h>

#include "resource.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "esUtil.h"
//#include <GLES3/gl3.h>
//#include <EGL/egl.h>
//#include <EGL/eglext.h>


#if _MSC_VER >= 1200
 // Disable compilation warnings.
#pragma warning(push)
// nonstandard extension used : bit field types other than int
#pragma warning(disable:4214)
// 'conversion' conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable:4244)
#endif

#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lv_demos/lv_demo.h"
#include "lv_drivers/win32drv/win32drv.h"

#define GWL_USERDATA GWLP_USERDATA

#if _MSC_VER >= 1200
// Restore compilation warnings.
#pragma warning(pop)
#endif

#include <stdio.h>

#define HIDDEN_WIN  (0)
static lv_obj_t* win1_contanier = NULL;
static lv_obj_t* win2_contanier = NULL;

static void win_btn_event_callback1(lv_event_t* event);
static void win_btn_event_callback2(lv_event_t* event);


static void show_button_win1()
{
    static lv_style_t obj_style;
    //lv_style_reset(&obj_style);
    lv_style_init(&obj_style); // 初始化按钮样式
    lv_style_set_radius(&obj_style, 0); // 设置样式圆角弧度

    lv_obj_t* btn = NULL;
    win1_contanier = lv_obj_create(lv_scr_act());  // 基于屏幕创建一个和屏幕大小一样的容器win1_contanier
    if (win1_contanier != NULL)
    {
        lv_obj_set_style_bg_color(win1_contanier, lv_palette_main(LV_PALETTE_RED), 0);
        lv_obj_set_size(win1_contanier, 800, 480);
        lv_obj_add_style(win1_contanier, &obj_style, 0);

        btn = lv_btn_create(win1_contanier);
        if (btn != NULL)
        {
            lv_obj_set_size(btn, 100, 50);
            lv_obj_add_event_cb(btn, win_btn_event_callback1, LV_EVENT_ALL, NULL); // 给对象添加CLICK事件和事件处理回调函数
            lv_obj_center(btn);

            lv_obj_t* label = lv_label_create(btn); // 给按钮添加标签
            if (label != NULL)
            {
                lv_label_set_text(label, "Button1"); // 设置标签文本
                lv_obj_center(label); // 标签居中显示
            }
        }
    }
}


static void show_button_win2()
{
    static lv_style_t obj_style;
    //lv_style_reset(&obj_style);
    lv_style_init(&obj_style); // 初始化按钮样式
    lv_style_set_radius(&obj_style, 0); // 设置样式圆角弧度

    lv_obj_t* btn = NULL;
    win2_contanier = lv_obj_create(lv_scr_act()); // 基于屏幕创建一个和屏幕大小一样的容器win2_contanier
    if (win2_contanier != NULL)
    {
        lv_obj_set_style_bg_color(win2_contanier, lv_palette_main(LV_PALETTE_BLUE), 0);
        lv_obj_set_size(win2_contanier, 800, 480);
        lv_obj_add_style(win2_contanier, &obj_style, 0);

        btn = lv_btn_create(win2_contanier);
        if (btn != NULL)
        {
            lv_obj_set_size(btn, 100, 50);
            lv_obj_add_event_cb(btn, win_btn_event_callback2, LV_EVENT_ALL, NULL); // 给对象添加CLICK事件和事件处理回调函数
            lv_obj_center(btn);
            lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_RED), 0);

            lv_obj_t* label = lv_label_create(btn); // 给按钮添加标签
            if (label != NULL)
            {
                lv_label_set_text(label, "Button2"); // 设置标签文本
                lv_obj_center(label); // 标签居中显示
            }
        }
    }
}


static void win_btn_event_callback1(lv_event_t* event)
{
    lv_event_code_t code = lv_event_get_code(event);
    if (code == LV_EVENT_CLICKED)
    {
#if HIDDEN_WIN
        // 给窗口1的容器添加隐藏属性，清除窗口2的隐藏属性
        lv_obj_add_flag(win1_contanier, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(win2_contanier, LV_OBJ_FLAG_HIDDEN);
#else
        // 删除窗口1的win1_contanier容器，这样就可以把win1_contanier上的子对象全部删除，然后显示窗口2
        lv_obj_del(win1_contanier);
        show_button_win2();
#endif
    }
}


static void win_btn_event_callback2(lv_event_t* event)
{
    lv_event_code_t code = lv_event_get_code(event);
    if (code == LV_EVENT_CLICKED)
    {
#if HIDDEN_WIN
        // 给窗口2的容器添加隐藏属性，清除窗口1的隐藏属性
        lv_obj_add_flag(win2_contanier, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(win1_contanier, LV_OBJ_FLAG_HIDDEN);
#else
        // 删除窗口2的win2_contanier容器，这样就可以把win2_contanier上的子对象全部删除，然后显示窗口1
        lv_obj_del(win2_contanier);
        show_button_win1();
#endif
    }
}



typedef struct
{
    // Handle to a program object
    GLuint programObject;

} UserData;




//////////////////////////////////////////////////////////////////
//
//  Private Functions
//
//

///
//  ESWindowProc()
//
//      Main window procedure
//
LRESULT WINAPI ESWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT  lRet = 1;

    switch (uMsg)
    {
    case WM_CREATE:
        break;

    case WM_PAINT:
    {
        ESContext* esContext = (ESContext*)(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA);

        if (esContext && esContext->drawFunc)
        {
            esContext->drawFunc(esContext);
            eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
        }

        if (esContext)
        {
            ValidateRect(esContext->eglNativeWindow, NULL);
        }
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_CHAR:
    {
        POINT      point;
        ESContext* esContext = (ESContext*)(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA);

        GetCursorPos(&point);

        if (esContext && esContext->keyFunc)
            esContext->keyFunc(esContext, (unsigned char)wParam,
                (int)point.x, (int)point.y);
    }
    break;

    default:
        lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
        break;
    }

    return lRet;
}

//////////////////////////////////////////////////////////////////
//
//  Public Functions
//
//

///
//  WinCreate()
//
//      Create Win32 instance and window
//
GLboolean WinCreate(ESContext* esContext, const char* title)
{
    WNDCLASS wndclass = { 0 };
    DWORD    wStyle = 0;
    RECT     windowRect;
    HINSTANCE hInstance = GetModuleHandle(NULL);


    wndclass.style = CS_OWNDC;
    wndclass.lpfnWndProc = (WNDPROC)ESWindowProc;
    wndclass.hInstance = hInstance;
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    //wndclass.lpszClassName = "opengles3.0";
    wndclass.lpszClassName = TEXT("opengles3.0");

    if (!RegisterClass(&wndclass))
    {
        return FALSE;
    }

    wStyle = WS_VISIBLE | WS_POPUP | WS_BORDER | WS_SYSMENU | WS_CAPTION;

    // Adjust the window rectangle so that the client area has
    // the correct number of pixels
    windowRect.left = 0;
    windowRect.top = 0;
    windowRect.right = esContext->width;
    windowRect.bottom = esContext->height;

    AdjustWindowRect(&windowRect, wStyle, FALSE);



    esContext->eglNativeWindow = CreateWindow(
        TEXT("opengles3.0"),
        TEXT("title------------"),
        wStyle,
        0,
        0,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        NULL,
        NULL,
        hInstance,
        NULL);

    // Set the ESContext* to the GWL_USERDATA so that it is available to the
    // ESWindowProc
#ifdef _WIN64
   //In LLP64 LONG is stll 32bit.
    SetWindowLongPtr(esContext->eglNativeWindow, GWL_USERDATA, (LONGLONG)(LONG_PTR)esContext);
#else
    SetWindowLongPtr(esContext->eglNativeWindow, GWL_USERDATA, (LONG)(LONG_PTR)esContext);
#endif


    if (esContext->eglNativeWindow == NULL)
    {
        return GL_FALSE;
    }

    ShowWindow(esContext->eglNativeWindow, TRUE);

    return GL_TRUE;
}

///
//  WinLoop()
//
//      Start main windows loop
//
void WinLoop(ESContext* esContext)
{
    MSG msg = { 0 };
    int done = 0;
    DWORD lastTime = GetTickCount();

    while (!done)
    {
        int gotMsg = (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0);
        DWORD curTime = GetTickCount();
        float deltaTime = (float)(curTime - lastTime) / 1000.0f;
        lastTime = curTime;

        if (gotMsg)
        {
            if (msg.message == WM_QUIT)
            {
                done = 1;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            SendMessage(esContext->eglNativeWindow, WM_PAINT, 0, 0);
        }

        // Call update function if registered
        if (esContext->updateFunc != NULL)
        {
            esContext->updateFunc(esContext, deltaTime);
        }
    }
}

///
//  Global extern.  The application must declare this function
//  that runs the application.
//
extern int esMain(ESContext* esContext);







int main()
{

#if 0
    lv_init();

    if (!lv_win32_init(
        GetModuleHandleW(NULL),
        SW_SHOW,
        800,
        480,
        LoadIconW(GetModuleHandleW(NULL), MAKEINTRESOURCE(IDI_LVGL))))
    {
        return -1;
    }

    lv_win32_add_all_input_devices_to_group(NULL);

    /*
     * Demos, benchmarks, and tests.
     *
     * Uncomment any one (and only one) of the functions below to run that
     * item.
     */

    // ----------------------------------
    // my freetype application
    // ----------------------------------

    ///*Init freetype library
    // *Cache max 64 faces and 1 size*/
    //lv_freetype_init(64, 1, 0);

    ///*Create a font*/
    //static lv_ft_info_t info;
    //info.name = "./lvgl/src/extra/libs/freetype/arial.ttf";
    //info.weight = 36;
    //info.style = FT_FONT_STYLE_NORMAL;
    //lv_ft_font_init(&info);

    ///*Create style with the new font*/
    //static lv_style_t style;
    //lv_style_init(&style);
    //lv_style_set_text_font(&style, info.font);

    ///*Create a label with the new style*/
    //lv_obj_t* label = lv_label_create(lv_scr_act());
    //lv_obj_add_style(label, &style, 0);
    //lv_label_set_text(label, "FreeType Arial Test");

    // ----------------------------------
    // my Win32 filesystem driver application
    // ----------------------------------

    /*::lv_fs_win32_init();

    lv_fs_dir_t d;
    if (lv_fs_dir_open(&d, "/") == LV_FS_RES_OK)
    {
        char b[MAX_PATH];
        memset(b, 0, MAX_PATH);
        while (lv_fs_dir_read(&d, b) == LV_FS_RES_OK)
        {
            printf("%s\n", b);
        }

        lv_fs_dir_close(&d);
    }*/

    // ----------------------------------
    // Demos from lv_examples
    // ----------------------------------

    // lv_demo_widgets();           // ok


    show_button_win1();
#if HIDDEN_WIN
    show_button_win2();
    // 给窗口2的容器添加隐藏属性
    lv_obj_add_flag(win2_contanier, LV_OBJ_FLAG_HIDDEN);
#endif

    // lv_demo_benchmark();
    // lv_demo_keypad_encoder();    // ok
    // lv_demo_music();             // removed from repository
    // lv_demo_printer();           // removed from repository
    // lv_demo_stress();            // ok

    // ----------------------------------
    // LVGL examples
    // ----------------------------------

    /*
     * There are many examples of individual widgets found under the
     * lvgl\exampless directory.  Here are a few sample test functions.
     * Look in that directory to find all the rest.
     */

    // lv_ex_get_started_1();
    // lv_ex_get_started_2();
    // lv_ex_get_started_3();

    // lv_example_flex_1();
    // lv_example_flex_2();
    // lv_example_flex_3();
    // lv_example_flex_4();
    // lv_example_flex_5();
    // lv_example_flex_6();        // ok

    // lv_example_grid_1();
    // lv_example_grid_2();
    // lv_example_grid_3();
    // lv_example_grid_4();
    // lv_example_grid_5();
    // lv_example_grid_6();

    // lv_port_disp_template();
    // lv_port_fs_template();
    // lv_port_indev_template();

    // lv_example_scroll_1();
    // lv_example_scroll_2();
    // lv_example_scroll_3();

    // lv_example_style_1();
    // lv_example_style_2();
    // lv_example_style_3();
    // lv_example_style_4();        // ok
    // lv_example_style_6();        // file has no source code
    // lv_example_style_7();
    // lv_example_style_8();
    // lv_example_style_9();
    // lv_example_style_10();
    // lv_example_style_11();       // ok

    // ----------------------------------
    // LVGL widgets examples
    // ----------------------------------

    // lv_example_arc_1();
    // lv_example_arc_2();

    // lv_example_bar_1();          // ok
    // lv_example_bar_2();
    // lv_example_bar_3();
    // lv_example_bar_4();
    // lv_example_bar_5();
    // lv_example_bar_6();          // issues

    // lv_example_btn_1();
    // lv_example_btn_2();
    // lv_example_btn_3();

    // lv_example_btnmatrix_1();
    // lv_example_btnmatrix_2();
    // lv_example_btnmatrix_3();

    // lv_example_calendar_1();

    // lv_example_canvas_1();
    // lv_example_canvas_2();

    // lv_example_chart_1();        // ok
    // lv_example_chart_2();        // ok
    // lv_example_chart_3();        // ok
    // lv_example_chart_4();        // ok
    // lv_example_chart_5();        // ok
    // lv_example_chart_6();        // ok

    // lv_example_checkbox_1();

    // lv_example_colorwheel_1();   // ok

    // lv_example_dropdown_1();
    // lv_example_dropdown_2();
    // lv_example_dropdown_3();

    // lv_example_img_1();
    // lv_example_img_2();
    // lv_example_img_3();
    // lv_example_img_4();         // ok

    // lv_example_imgbtn_1();

    // lv_example_keyboard_1();    // ok

    // lv_example_label_1();
    // lv_example_label_2();       // ok

    // lv_example_led_1();

    // lv_example_line_1();

    // lv_example_list_1();

    // lv_example_meter_1();
    // lv_example_meter_2();
    // lv_example_meter_3();
    // lv_example_meter_4();       // ok

    // lv_example_msgbox_1();

    // lv_example_obj_1();         // ok

    // lv_example_roller_1();
    // lv_example_roller_2();      // ok

    // lv_example_slider_1();      // ok
    // lv_example_slider_2();      // issues
    // lv_example_slider_3();      // issues

    // lv_example_spinbox_1();

    // lv_example_spinner_1();     // ok

    // lv_example_switch_1();      // ok

    // lv_example_table_1();
    // lv_example_table_2();       // ok

    // lv_example_tabview_1();

    // lv_example_textarea_1();    // ok
    // lv_example_textarea_2();
    // lv_example_textarea_3();    // ok, but not all button have functions

    // lv_example_tileview_1();    // ok

    // lv_example_win_1();         // ok

    // ----------------------------------
    // Task handler loop
    // ----------------------------------




float dashesArr[] = { 7.0f, 32.0f, 7.0f, 32.0f, -1 };

float* dashes = dashesArr;

int i, j, ndashes, idash0 = 0;
float allDashLen = 0, dashOffset;

for (ndashes = 0; dashes[ndashes] >= 0; ++ndashes) {}
// Figure out dash offset.
for (j = 0; j < ndashes; j++)
    allDashLen += dashes[j];
if (ndashes & 1)
allDashLen *= 2.0f;
// Find location inside pattern
dashOffset = fmodf(0.0f, allDashLen);
if (dashOffset < 0.0f)
    dashOffset += allDashLen;
while (dashOffset > dashes[idash0]) {
    dashOffset -= dashes[idash0];
    idash0 = (idash0 + 1) % ndashes;
}






    while (!lv_win32_quit_signal)
    {
        //lv_task_handler();
        lv_timer_handler();

        LV_LOG_INFO("main sleep 16ms --->> ");
        Sleep(16);
    }

#endif

    ESContext esContext;

    memset(&esContext, 0, sizeof(ESContext));

    if (esMain(&esContext) != GL_TRUE)
    {
        return 1;
    }

    WinLoop(&esContext);

    if (esContext.shutdownFunc != NULL)
    {
        esContext.shutdownFunc(&esContext);
    }

    if (esContext.userData != NULL)
    {
        free(esContext.userData);
    }



    return 0;
}


///
// Create a shader object, load the shader source, and
// compile the shader.
//
GLuint LoadShader(GLenum type, const char* shaderSrc)
{
    GLuint shader;
    GLint compiled;

    // Create the shader object
    shader = glCreateShader(type);

    if (shader == 0)
    {
        return 0;
    }

    // Load the shader source
    glShaderSource(shader, 1, &shaderSrc, NULL);

    // Compile the shader
    glCompileShader(shader);

    // Check the compile status
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled)
    {
        GLint infoLen = 0;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

        if (infoLen > 1)
        {
            char* infoLog = (char*)malloc(sizeof(char) * infoLen);

            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            esLogMessage("Error compiling shader:\n%s\n", infoLog);

            free(infoLog);
        }

        glDeleteShader(shader);
        return 0;
    }

    return shader;

}

///
// Initialize the shader and program object
//
int Init(ESContext* esContext)
{
    //UserData* userData = (UserData*)esContext->userData;
    //char vShaderStr[] =
    //    "#version 300 es                          \n"
    //    "layout(location = 0) in vec4 vPosition;  \n"
    //    "void main()                              \n"
    //    "{                                        \n"
    //    "   gl_Position = vPosition;              \n"
    //    "}                                        \n";

    //char fShaderStr[] =
    //    "#version 300 es                              \n"
    //    "precision mediump float;                     \n"
    //    "out vec4 fragColor;                          \n"
    //    "void main()                                  \n"
    //    "{                                            \n"
    //    "   fragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );  \n"
    //    "}                                            \n";

    //GLuint vertexShader;
    //GLuint fragmentShader;
    //GLuint programObject;
    //GLint linked;

    //// Load the vertex/fragment shaders
    //vertexShader = LoadShader(GL_VERTEX_SHADER, vShaderStr);
    //fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fShaderStr);

    //// Create the program object
    //programObject = glCreateProgram();

    //if (programObject == 0)
    //{
    //    return 0;
    //}

    //glAttachShader(programObject, vertexShader);
    //glAttachShader(programObject, fragmentShader);

    //// Link the program
    //glLinkProgram(programObject);

    //// Check the link status
    //glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

    //if (!linked)
    //{
    //    GLint infoLen = 0;

    //    glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

    //    if (infoLen > 1)
    //    {
    //        char* infoLog = (char*)malloc(sizeof(char) * infoLen);

    //        glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
    //        esLogMessage("Error linking program:\n%s\n", infoLog);

    //        free(infoLog);
    //    }

    //    glDeleteProgram(programObject);
    //    return FALSE;
    //}

    //// Store the program object
    //userData->programObject = programObject;

    //glClearColor(1.0f, 1.0f, 1.0f, 0.0f);




    return TRUE;
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw(ESContext* esContext)
{
    UserData* userData = (UserData*)esContext->userData;
    GLfloat vVertices[] = { 0.0f,  0.5f, 0.0f,
                             -0.5f, -0.5f, 0.0f,
                             0.5f, -0.5f, 0.0f
    };


    //// Set the viewport
    //glViewport(0, 0, esContext->width, esContext->height);

    // Clear the color buffer
    //glClear(GL_COLOR_BUFFER_BIT);

    //// Use the program object
    //glUseProgram(userData->programObject);

    //// Load the vertex data
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    //glEnableVertexAttribArray(0);

    //glDrawArrays(GL_TRIANGLES, 0, 3);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);



    //eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);

}

void Shutdown(ESContext* esContext)
{
    UserData* userData = (UserData*)esContext->userData;

    glDeleteProgram(userData->programObject);
}


int esMain(ESContext* esContext)
{
    esContext->userData = malloc(sizeof(UserData));

    esCreateWindow(esContext, "Hello Triangle", 640, 480, ES_WINDOW_RGB);

    if (!Init(esContext))
    {
        return GL_FALSE;
    }

    // Set the viewport
    glViewport(0, 0, esContext->width, esContext->height);
    glEnable(GL_DEPTH_TEST);

    glClearDepthf(1.0f);
    glClearColor(1.0f, 0.0f, 0.5f, 1.0f);




    esRegisterShutdownFunc(esContext, Shutdown);
    esRegisterDrawFunc(esContext, Draw);

    return GL_TRUE;
}
