#include <pebble.h>
#include "confirm.h"
#include "ancom.h"

#define NUM_MENU_SECTIONS 1

static struct {
   Window* window;
   TextLayer* text_symbol;
   TextLayer* text_value_diff;
   TextLayer* text_value_info;
   TextLayer* background_workaround;
} ui;

int callindex;
bool firstmenuitem = true;
static MenuLayer *menu_layer;

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    return 2;
}

// A callback is used to specify the height of the section header
static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  // This is a define provided in pebble.h that you may use for the default height
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

// Here we draw what each header is
static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  // Draw title text in the section header
  menu_cell_basic_header_draw(ctx, cell_layer, "Are you sure?");
}

// This is the menu item draw callback where you specify what each item should look like
static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
	if(firstmenuitem){
		menu_cell_basic_draw(ctx, cell_layer,"Cancel", "Return to previous page",NULL);
		firstmenuitem = false;
	}else{
		char str[80];
		strcpy (str,"Emergency Call ");
		strcat (str,getContactName(callindex));
	    menu_cell_basic_draw(ctx, cell_layer,"Confirm", str,NULL);
	    firstmenuitem = true;
	}
}

// Here we capture when a user selects a menu item
void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  if(cell_index->row == 0){
  	window_stack_pop(true);
  }else{
  	//call
  }
}

void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  // Create the menu layer
  menu_layer = menu_layer_create(bounds);
/*
  ui.text_symbol = text_layer_create((GRect) {
         .origin = { 8, 8 },
         .size = { bounds.size.w-16, 20 }
   });
   text_layer_set_text(ui.text_symbol, "No Stocks");
*/
  // Set all the callbacks for the menu layer
  menu_layer_set_callbacks(menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback,
  });

  // Bind the menu layer's click config provider to the window for interactivity
  menu_layer_set_click_config_onto_window(menu_layer, window);

  // Add it to the window for display
  layer_add_child(window_layer, menu_layer_get_layer(menu_layer));
}

void window_unload(Window *window) {
  // Destroy the menu layer
  menu_layer_destroy(menu_layer);
}

void page_confirm_init(void)
{
   //Create window
   ui.window = window_create();

   window_set_window_handlers(ui.window, (WindowHandlers) {
      .load = window_load,
      .unload = window_unload
   });

}

void page_confirm_deinit(void)
{
   window_destroy(ui.window);
   ui.window = NULL;
}

void page_confirm_show(int index)
{
  window_stack_push (ui.window, true); 
  callindex = index;
}