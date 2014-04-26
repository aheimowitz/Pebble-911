#include <pebble.h>
#include "ancom.h"
#include "Pebble-911.h"

int ack = 0;
Contact* contacts;
int confirm_flag = -1;
int num_contacts = 0;
void out_sent_handler(DictionaryIterator *sent, void *context) {
   APP_LOG(APP_LOG_LEVEL_DEBUG, "Sending has succeded");
}


void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
   APP_LOG(APP_LOG_LEVEL_DEBUG, "Sending has failed: %d", reason);
}


void in_dropped_handler(AppMessageResult reason, void *context) {
   APP_LOG(APP_LOG_LEVEL_DEBUG, "Receiving has failed");
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
          Tuple *mode_tuple = dict_find(iter, 0);
	  int mode = mode_tuple->value->int32;
	  switch (mode){
		  case 0: {
			  Tuple *name_tuple;
			  Tuple *number_tuple;
			  Tuple *val_tuple = dict_find(iter, 1);
			  confirm_flag = val_tuple->value->int32;
			  Tuple *num_tuple = dict_find(iter, 2);
			  num_contacts = num_tuple->value->int32;
			  contacts = malloc((sizeof(Contact)) * num_contacts);
			  int contact_count = 0;
			  APP_LOG(APP_LOG_LEVEL_DEBUG, "Confirm? %d", confirm_flag);
			  APP_LOG(APP_LOG_LEVEL_DEBUG, "numcontacts %d", num_contacts);
			  for(int x=0; x<(num_contacts*2); x= x+2){
				name_tuple = dict_find(iter, x+3);
				number_tuple = dict_find(iter, x+4);
				strncpy(contacts[contact_count].name, name_tuple->value->cstring,20);
				strncpy(contacts[contact_count].number, number_tuple->value->cstring,12);
			  	APP_LOG(APP_LOG_LEVEL_DEBUG, "Name: %s Number: %s", contacts[contact_count].name, contacts[contact_count].number);
				contact_count++;
			  }
		  	  if(num_contacts > 0){
		 	 	initialPhoneConnection(contacts[0].name, contacts[0].number);
		  	  }
			  return;
		 }
	  }
	  

}

void placeCall(int position){
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);
	Tuplet value = TupletInteger(0, 1);
	dict_write_tuplet(iter, &value);
	Tuplet numvalue = TupletInteger(1, position);
	dict_write_tuplet(iter, &numvalue);
	app_message_outbox_send();
}

static void fetchContacts(){
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);
	Tuplet value = TupletInteger(0, 0);
	dict_write_tuplet(iter, &value);
	//Tuplet value2 = TupletInteger(1, 2);
	//dict_write_tuplet(iter, &value2);
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
