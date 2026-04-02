#include <dragonruby.h>

#define DWIDTH 1280
#define DHEIGHT 720
#define MAXCOLORS 100

static drb_api_t *drb_api;
uint32_t pixels[DWIDTH*DHEIGHT];
int scroll;
int colors;
int colorRGB[MAXCOLORS];

void color_sethex(int hexcolor) {
	int r,g,b;

	r = (hexcolor >> 16);
	g = ((hexcolor >> 8) & 0xFF);
	b = (hexcolor & 0xFF);
	colorRGB[colors] = (255<<24)+(b<<16)+(g<<8)+r; // dragonruby uses AGBR instead of RGBA
	colors++;
}

DRB_FFI
mrb_value colors_init() {
	// create palette
	colors = 0;
	// endesga64
	color_sethex(0xff0040); color_sethex(0x131313); color_sethex(0x1b1b1b); color_sethex(0x272727); color_sethex(0x3d3d3d); color_sethex(0x5d5d5d);
	color_sethex(0x858585); color_sethex(0xb4b4b4); color_sethex(0xffffff); color_sethex(0xc7cfdd); color_sethex(0x92a1b9); color_sethex(0x657392);
	color_sethex(0x424c6e); color_sethex(0x2a2f4e); color_sethex(0x1a1932); color_sethex(0x0e071b); color_sethex(0x1c121c); color_sethex(0x391f21);
	color_sethex(0x5d2c28); color_sethex(0x8a4836); color_sethex(0xbf6f4a); color_sethex(0xe69c69); color_sethex(0xf6ca9f); color_sethex(0xf9e6cf);
	color_sethex(0xedab50); color_sethex(0xe07438); color_sethex(0xc64524); color_sethex(0x8e251d); color_sethex(0xff5000); color_sethex(0xed7614);
	color_sethex(0xffa214); color_sethex(0xffc825); color_sethex(0xffeb57); color_sethex(0xd3fc7e); color_sethex(0x99e65f); color_sethex(0x5ac54f);
	color_sethex(0x33984b); color_sethex(0x1e6f50); color_sethex(0x134c4c); color_sethex(0x0c2e44); color_sethex(0x00396d); color_sethex(0x0069aa);
	color_sethex(0x0098dc); color_sethex(0x00cdf9); color_sethex(0x0cf1ff); color_sethex(0x94fdff); color_sethex(0xfdd2ed); color_sethex(0xf389f5);
	color_sethex(0xdb3ffd); color_sethex(0x7a09fa); color_sethex(0x3003d9); color_sethex(0x0c0293); color_sethex(0x03193f); color_sethex(0x3b1443);
	color_sethex(0x622461); color_sethex(0x93388f); color_sethex(0xca52c9); color_sethex(0xc85086); color_sethex(0xf68187); color_sethex(0xf5555d);
	color_sethex(0xea323c); color_sethex(0xc42430); color_sethex(0x891e2b); color_sethex(0x571c27);
	return mrb_nil_value();
}

DRB_FFI
mrb_value update_scanner_texture(mrb_state *state, mrb_value value) {
	int x,y,yoff;

	scroll++;
	for (y = 0; y < DHEIGHT; y++) {
		yoff = (y*DWIDTH);
		x = 0;
		while (x < DWIDTH) {
			pixels[yoff++] = colorRGB[(x+y+0+scroll) % colors];
			pixels[yoff++] = colorRGB[(x+y+1+scroll) % colors];
			pixels[yoff++] = colorRGB[(x+y+2+scroll) % colors];
			pixels[yoff++] = colorRGB[(x+y+3+scroll) % colors];
			pixels[yoff++] = colorRGB[(x+y+4+scroll) % colors];
			pixels[yoff++] = colorRGB[(x+y+5+scroll) % colors];
			pixels[yoff++] = colorRGB[(x+y+6+scroll) % colors];
			pixels[yoff++] = colorRGB[(x+y+7+scroll) % colors];
			x += 8;
		}
	}
	// Send it to the renderer to create/update a sprite.
	drb_api->drb_upload_pixel_array("scanner", DWIDTH, DHEIGHT, pixels);
	return mrb_nil_value();
}

DRB_FFI_EXPORT
void drb_register_c_extensions_with_api(mrb_state *state, struct drb_api_t *api) {
  drb_api = api;
  struct RClass *FFI = drb_api->mrb_module_get(state, "FFI");
  struct RClass *module = drb_api->mrb_define_module_under(state, FFI, "CExt");
  drb_api->mrb_define_module_function(state, module, "update_scanner_texture", update_scanner_texture, MRB_ARGS_REQ(0));
  drb_api->mrb_define_module_function(state, module, "colors_init", colors_init, MRB_ARGS_REQ(0));
}
