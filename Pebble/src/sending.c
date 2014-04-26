#include <pebble.h>
#include "confirm.h"
#include "ancom.h"
#include "sending.h"

static TextLayer *sendingText;
static TextLayer *resultText;
static TextLayer *pressText;
static Window *window;


static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(sendingText,"");
  text_layer_set_text(resultText,"");
  text_layer_set_text(pressText,"");
  window_stack_pop(false);
}
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(sendingText,"");
  text_layer_set_text(resultText,"");
  text_layer_set_text(pressText,"");
  window_stack_pop(false);
}
static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(sendingText,"");
  text_layer_set_text(resultText,"");
  text_layer_set_text(pressText,"");
  window_stack_pop(false);
}
static void changeText(char* text){
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  sendingText = text_layer_create((GRect) { .origin = { 0, 4 }, .size = { bounds.size.w, 30} });
  text_layer_set_text(sendingText, text);
  text_layer_set_font(sendingText, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  text_layer_set_text_alignment(sendingText, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(sendingText));
}

static void pressanytext(){
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  pressText = text_layer_create((GRect) { .origin = { 0, 90 }, .size = { bounds.size.w, 30} });
  text_layer_set_text(pressText, "Press Any Button");
  text_layer_set_font(pressText, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  text_layer_set_text_alignment(pressText, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(pressText));
}

void successful(){
  changeText("Message Sent!");
  pressanytext();
}

void failure(char* failreason){
  changeText("Message Failed To Send!");
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  resultText = text_layer_create((GRect) { .origin = { 0, 45 }, .size = { bounds.size.w, 30} });
  text_layer_set_text(resultText, failreason);
  text_layer_set_font(resultText, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  text_layer_set_text_alignment(resultText, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(resultText));
  pressanytext();
  
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
   window_set_click_config_provider(window, click_config_provider);

}

void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
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
