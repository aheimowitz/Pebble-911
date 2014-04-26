#include <pebble.h>
#include "confirm.h"
#include "ancom.h"
#include "sending.h"

static TextLayer *sendingText;
static Window *window;

static void changeText(char* text){
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  sendingText = text_layer_create((GRect) { .origin = { 0, 4 }, .size = { bounds.size.w, 30} });
  text_layer_set_text(sendingText, text);
  text_layer_set_font(sendingText, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  text_layer_set_text_alignment(sendingText, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(sendingText));
}

void successful(){
  changeText("Message Sent!");
}

static void window_load(Window *window) {
  //Test
}

static void window_unload(Window *window) {
  //Test
}

void page_sending_init(void)
{
   //Create window
   window = window_create();

   window_set_window_handlers(window, (WindowHandlers) {
      .load = window_load,
      .unload = window_unload
   });

}

void page_sending_deinit(void)
{
   window_destroy(window);
   window = NULL;
}

void page_sending_show()
{
  window_stack_push (window, true); 
  changeText("Sending ...");
}
