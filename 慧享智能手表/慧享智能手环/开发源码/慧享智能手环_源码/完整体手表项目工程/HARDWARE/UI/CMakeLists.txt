SET(SOURCES screens/ui_MainScreen.c
    screens/ui_dateScreen.c
    screens/ui_setsrceen.c
    screens/ui_weatherscreen.c
    screens/ui_xlxyScreen.c
    screens/ui_miaobiaoScreen.c
    screens/ui_naozhongScreen.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    images/ui_img_wen_png.c
    images/ui_img_shidu_png.c
    images/ui_img_xing_png.c
    images/ui_img_di_png.c
    images/ui_img_bs_png.c
    images/ui_img_wifi_png.c
    images/ui_img_liandu_png.c
    images/ui_img_guanji_png.c
    images/ui_img_lanya_png.c
    images/ui_img_shezhi_png.c
    images/ui_img_rili_png.c
    images/ui_img_742136671.c
    images/ui_img_xlxy_png.c
    images/ui_img_twlp_png.c
    images/ui_img_dgh_png.c
    images/ui_img_tq_png.c
    images/ui_img_1969274251.c
    images/ui_img_miaobiao_png.c
    images/ui_img_1809758872.c
    images/ui_img_608756609.c
    images/ui_img_9_png.c
    images/ui_img_1379335529.c
    images/ui_img_sx_png.c
    images/ui_img_307400400.c
    images/ui_img_zt_png.c
    images/ui_img_358139227.c
    images/ui_img_0_png.c
    images/ui_img_15_png.c
    images/ui_img_4_png.c
    fonts/ui_font_kaiti16.c
    fonts/ui_font_kaiti20.c
    fonts/ui_font_shu16.c
    fonts/ui_font_shu20.c)

add_library(ui ${SOURCES})
