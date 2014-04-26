#include <pebble.h>
#include "ancom.h"

int ack = 0;


void out_sent_handler(DictionaryIterator *sent, void *context) {
   ack = 1;
}


void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
   ack = -1;
}


void in_dropped_handler(AppMessageResult reason, void *context) {
   ack = -1;
}

static void in_received_handler(DictionaryIterator *iter, void *context) {

          // Check for fields you expect to receive
          Tuple *text_tuple = dict_find(iter, AKEY_TEXT);

          // Act on the found fields received
          if (text_tuple) {
            APP_LOG(APP_LOG_LEVEL_DEBUG, "Text: %s", text_tuple->value->cstring);
          }

}


static void fetchContacts(){
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);
	Tuplet value = TupletInteger(0, 0);
	dict_write_tuplet(iter, &value);
	app_message_outbox_send();

}


void init_msg(void) {

   app_message_register_inbox_received(in_received_handler);
   app_message_register_inbox_dropped(in_dropped_handler);
   app_message_register_outbox_sent(out_sent_handler);
   app_message_register_outbox_failed(out_failed_handler);


   const uint32_t inbound_size = 256;
   const uint32_t outbound_size = 64;
   app_message_open(inbound_size, outbound_size);
   fetchContacts();

}
