#include <jo/jo.h>

jo_palette game_palette;

jo_palette      *my_game_palette_handling(void)
{
    jo_create_palette(&game_palette);
    return (&game_palette);
}

void init_font(void) {
    jo_img_8bits    img;
    // set palette handler
    jo_set_tga_palette_handling(my_game_palette_handling);
    img.data = NULL;
    jo_tga_8bits_loader(&img, JO_ROOT_DIR, "FONT.TGA", 2);
    // unset palette handler (useful if you are loading multiple assets with the same palette)
    jo_set_tga_palette_handling(JO_NULL);
    
    slCharNbg0(COL_TYPE_256, CHAR_SIZE_1x1);
    jo_vdp2_set_nbg0_8bits_font(&img, " 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!?,.:;\"'&()=\%/\\*+-$<>", game_palette.id, false, true);
    jo_free_img(&img);
    #if defined(MY_TV_704x240)
        slZoomNbg0(toFIXED(0.5), toFIXED(1.0));
    #endif
}