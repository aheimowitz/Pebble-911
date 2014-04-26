#include <pebble.h>
#include "antom.h";
#include "Pebble-911.h";

static Window *window;
static TextLayer *text_layer_contact_name;
static TextLayer *text_layer_contact_number;


void changeText(){
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);
	text_layer_contact_name = text_layer_create((GRect) { .origin = { 0, 4 }, .size = { bounds.size.w, 30} });
	text_layer_set_text(text_layer_contact_name, name);
	text_layer_set_font(text_layer_contact_name, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
	text_layer_set_text_alignment(text_layer_contact_name, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(text_layer_contact_name));
	text_layer_contact_number = text_layer_create((GRect) { .origin = { 0, 28 }, .size = { bounds.size.w, 30 } });
	text_layer_set_text(text_layer_contact_number, phoneNumber);
	text_layer_set_font(text_layer_contact_number, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
	text_layer_set_text_alignment(text_layer_contact_number, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(text_layer_contact_number));
}

void initalPhoneConnection(char* namein, char* phonein){
	phoneconnected = true;
	name = namein;
	phoneNumber = phonein;
	changeText();
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
	if(phoneconnected){
		Layer *window_layer = window_get_root_layer(window);
		if(!inverted){
			select_button = bitmap_layer_create(GRect(22,50,100,100));
		  	bitmap_layer_set_bitmap(select_button, gbitmap_create_with_resource(RESOURCE_ID_SELECT_BUTTON_INVERSE));
		  	layer_add_child(window_layer, (Layer*) select_button);
		  	inverted = true;
		}else{
		  select_button = bitmap_layer_create(GRect(22,50,100,100));
		  bitmap_layer_set_bitmap(select_button, gbitmap_create_with_resource(RESOURCE_ID_SELECT_BUTTON));
		  layer_add_child(window_layer, (Layer*) select_button);
		  inverted = false;
		}
	}
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  count++;
  if(count == listsize){
  	count = 0;
  }
  name = getContactName(count);
  phoneNumber = getContactNumber(count);
  changeText();
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  count--;
  if(count == -1){
  	count = listsize-1;
  }
  changeText();
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
	phoneconnected = false;
	//get contact list size
	count = 0;
	listsize = getNumContacts();
	//get first contact
	phoneNumber = "No Connection";
	changeText();
	Layer *window_layer = window_get_root_layer(window);
	select_button = bitmap_layer_create(GRect(22,50,100,100));
	bitmap_layer_set_bitmap(select_button, gbitmap_create_with_resource(RESOURCE_ID_SELECT_BUTTON));
	layer_add_child(window_layer, (Layer*) select_button);
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer_contact_number);
  text_layer_destroy(text_layer_contact_name);
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
