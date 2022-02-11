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

int main()
{
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

    while (!lv_win32_quit_signal)
    {
        //lv_task_handler();
        lv_timer_handler();

        LV_LOG_INFO("main sleep 16ms --->> ");
        Sleep(16);
    }

    return 0;
}
