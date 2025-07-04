// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.2
// LVGL version: 8.3.11
// Project name: SquareLine_Project

#include "../ui.h"

void ui_Screen6_screen_init(void)
{
    ui_Screen6 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen6, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Screen6, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Screen6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel12 = lv_obj_create(ui_Screen6);
    lv_obj_set_width(ui_Panel12, 230);
    lv_obj_set_height(ui_Panel12, 78);
    lv_obj_set_x(ui_Panel12, 0);
    lv_obj_set_y(ui_Panel12, -102);
    lv_obj_set_align(ui_Panel12, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel12, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Panel12, lv_color_hex(0xECE610), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel12, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Panel12, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label29 = lv_label_create(ui_Panel12);
    lv_obj_set_width(ui_Label29, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label29, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label29, -88);
    lv_obj_set_y(ui_Label29, -25);
    lv_obj_set_align(ui_Label29, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label29, "滑冰");
    lv_obj_set_style_text_color(ui_Label29, lv_color_hex(0x050303), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label29, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label29, &ui_font_s7time1, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label34 = lv_label_create(ui_Panel12);
    lv_obj_set_width(ui_Label34, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label34, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label34, -72);
    lv_obj_set_y(ui_Label34, 14);
    lv_obj_set_align(ui_Label34, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label34, "运动时长:");
    lv_obj_set_style_text_font(ui_Label34, &ui_font_s7time1, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label35 = lv_label_create(ui_Panel12);
    lv_obj_set_width(ui_Label35, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label35, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label35, 16);
    lv_obj_set_y(ui_Label35, 14);
    lv_obj_set_align(ui_Label35, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label35, "1小时30分钟");
    lv_obj_set_style_text_font(ui_Label35, &ui_font_s7time1, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label36 = lv_label_create(ui_Panel12);
    lv_obj_set_width(ui_Label36, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label36, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label36, -88);
    lv_obj_set_y(ui_Label36, -5);
    lv_obj_set_align(ui_Label36, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label36, "消耗:");
    lv_obj_set_style_text_font(ui_Label36, &ui_font_s7time1, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label37 = lv_label_create(ui_Panel12);
    lv_obj_set_width(ui_Label37, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label37, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label37, -14);
    lv_obj_set_y(ui_Label37, -5);
    lv_obj_set_align(ui_Label37, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label37, "千卡");
    lv_obj_set_style_text_font(ui_Label37, &ui_font_s7time1, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label47 = lv_label_create(ui_Panel12);
    lv_obj_set_width(ui_Label47, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label47, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label47, -51);
    lv_obj_set_y(ui_Label47, -6);
    lv_obj_set_align(ui_Label47, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label47, "100");

    ui_Label48 = lv_label_create(ui_Panel12);
    lv_obj_set_width(ui_Label48, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label48, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label48, 92);
    lv_obj_set_y(ui_Label48, -22);
    lv_obj_set_align(ui_Label48, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label48, "5:30");

    ui_Panel29 = lv_obj_create(ui_Screen6);
    lv_obj_set_width(ui_Panel29, 230);
    lv_obj_set_height(ui_Panel29, 78);
    lv_obj_set_x(ui_Panel29, 0);
    lv_obj_set_y(ui_Panel29, -12);
    lv_obj_set_align(ui_Panel29, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel29, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Panel29, lv_color_hex(0xECE610), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel29, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Panel29, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label63 = lv_label_create(ui_Panel29);
    lv_obj_set_width(ui_Label63, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label63, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label63, -88);
    lv_obj_set_y(ui_Label63, -25);
    lv_obj_set_align(ui_Label63, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label63, "滑冰");
    lv_obj_set_style_text_color(ui_Label63, lv_color_hex(0x050303), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label63, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label63, &ui_font_s7time1, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label51 = lv_label_create(ui_Panel29);
    lv_obj_set_width(ui_Label51, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label51, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label51, -72);
    lv_obj_set_y(ui_Label51, 14);
    lv_obj_set_align(ui_Label51, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label51, "运动时长:");
    lv_obj_set_style_text_font(ui_Label51, &ui_font_s7time1, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label57 = lv_label_create(ui_Panel29);
    lv_obj_set_width(ui_Label57, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label57, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label57, 16);
    lv_obj_set_y(ui_Label57, 14);
    lv_obj_set_align(ui_Label57, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label57, "1小时30分钟");
    lv_obj_set_style_text_font(ui_Label57, &ui_font_s7time1, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label58 = lv_label_create(ui_Panel29);
    lv_obj_set_width(ui_Label58, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label58, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label58, -88);
    lv_obj_set_y(ui_Label58, -5);
    lv_obj_set_align(ui_Label58, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label58, "消耗:");
    lv_obj_set_style_text_font(ui_Label58, &ui_font_s7time1, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label59 = lv_label_create(ui_Panel29);
    lv_obj_set_width(ui_Label59, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label59, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label59, -14);
    lv_obj_set_y(ui_Label59, -5);
    lv_obj_set_align(ui_Label59, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label59, "千卡");
    lv_obj_set_style_text_font(ui_Label59, &ui_font_s7time1, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label60 = lv_label_create(ui_Panel29);
    lv_obj_set_width(ui_Label60, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label60, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label60, -51);
    lv_obj_set_y(ui_Label60, -6);
    lv_obj_set_align(ui_Label60, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label60, "100");

    ui_Label61 = lv_label_create(ui_Panel29);
    lv_obj_set_width(ui_Label61, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label61, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label61, 92);
    lv_obj_set_y(ui_Label61, -22);
    lv_obj_set_align(ui_Label61, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label61, "5:30");
	
	lv_obj_add_flag(ui_Panel12, LV_OBJ_FLAG_HIDDEN); 
	lv_obj_add_flag(ui_Panel29, LV_OBJ_FLAG_HIDDEN); 

    lv_obj_add_event_cb(ui_Screen6, ui_event_Screen6, LV_EVENT_ALL, NULL);

}


void change_txt(uint8_t sport)
{
	if(sport == 1)
	{
		lv_label_set_text(ui_Label29, "篮球");
	}
	else if(sport == 2)
	{
		lv_label_set_text(ui_Label29, "户外跑步");
	}
	else if(sport == 3)
	{
		lv_label_set_text(ui_Label29, "游泳");
	}
	else if(sport == 4)
	{
		lv_label_set_text(ui_Label29, "滑冰");
	}
	else if(sport == 5)
	{
		lv_label_set_text(ui_Label29, "健走");
	}
}

void change2_txt(uint8_t sport)
{
	if(sport == 1)
	{
		lv_label_set_text(ui_Label63, "篮球");
	}
	else if(sport == 2)
	{
		lv_label_set_text(ui_Label63, "户外跑步");
	}
	else if(sport == 3)
	{
		lv_label_set_text(ui_Label63, "游泳");
	}
	else if(sport == 4)
	{
		lv_label_set_text(ui_Label63, "滑冰");
	}
	else if(sport == 5)
	{
		lv_label_set_text(ui_Label63, "健走");
	}
	  
}