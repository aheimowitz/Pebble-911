#include <pebble.h>
#include "ancom.h"
#include "Pebble-911.h"

int ack = 0;
Contact* contacts;
int confirm_flag = -1;
int num_contacts = 0;
void out_sent_handler(DictionaryIterator *sent, void *context) {
   ack = 1;
}


void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
   ack = -1;
}


void in_dropped_handler(AppMessageResult reason, void *context) {
   ack = -1;
}

int getFlagValue(){
	return confirm_flag;
}

char* getContactName(int position){
	return contacts[position].name;
}

char* getContactNumber(int position){
	return contacts[position].number;
}

int getNumContacts(){
	return num_contacts;
}

void in_received_handler(DictionaryIterator *iter, void *context) {

          // Check for fields you expect to receive
          Tuple *text_tuple = dict_find(iter, 0);
	  num_contacts = text_tuple->value->int32;
	  if(num_contacts == -1){

	  }
	  Tuple *name_tuple;
	  Tuple *number_tuple;
	  contacts = malloc((sizeof(Contact)) * num_contacts);
	  int contact_count = 0;
	  for(int x=0; x<(num_contacts*2); x= x+2){
		name_tuple = dict_find(iter, x+1);
		number_tuple = dict_find(iter, x+2);
		strncpy(contacts[contact_count].name, name_tuple->value->cstring,20);
		strncpy(contacts[contact_count].number, number_tuple->value->cstring,12);
	  	APP_LOG(APP_LOG_LEVEL_DEBUG, "Name: %s Number: %s", contacts[contact_count].name, contacts[contact_count].number);
		contact_count++;
	  }
	  Tuple *val_tuple = dict_find(iter, (num_contacts*2)+1);
	  confirm_flag = val_tuple->value->int32;
	  APP_LOG(APP_LOG_LEVEL_DEBUG, "Confirm? %d", confirm_flag);
	  APP_LOG(APP_LOG_LEVEL_DEBUG, "numcontacts %d", num_contacts);
	  if(num_contacts > 0){
	 	 initialPhoneConnection(contacts[0].name, contacts[0].number);
	  }

}


static void fetchContacts(){
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);
	Tuplet value = TupletInteger(0, 0);
	dict_write_tuplet(iter, &value);
	Tuplet value2 = TupletInteger(1, 2);
	dict_write_tuplet(iter, &value2);
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
