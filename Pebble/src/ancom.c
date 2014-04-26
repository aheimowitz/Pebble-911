#include <pebble.h>
#include "ancom.h"
#include "Pebble-911.h"
#include "sending.h"
int ack = 0;
Contact* contacts;
int confirm_flag = -1;
int num_contacts = 0;
int contact_count = 0;

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

void getContact(){
	psleep(200);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Generating tuples, count: %d", contact_count);
	DictionaryIterator *iter2;
	app_message_outbox_begin(&iter2);
	Tuplet value = TupletInteger(0, 1);
	Tuplet value2 = TupletInteger(1, contact_count);
	dict_write_tuplet(iter2, &value);
	dict_write_tuplet(iter2, &value2);
	app_message_outbox_send();
}

void in_received_handler(DictionaryIterator *iter, void *context) {
	  APP_LOG(APP_LOG_LEVEL_DEBUG, "Received handler");
          // Check for fields you expect to receive
          Tuple *mode_tuple = dict_find(iter, 0);
	  int mode = mode_tuple->value->int8;
	  APP_LOG(APP_LOG_LEVEL_DEBUG, "Received mode flag: %d",mode);
	  Tuple *name_tuple;
	  switch (mode){
		  case 0: {
			  name_tuple = dict_find(iter, 1);
			  int values = name_tuple->value->int8;
			  confirm_flag = values  & 1;
	  		  num_contacts = (values >> 1) & 15;
			  contacts = malloc((sizeof(Contact)) * num_contacts);
			  contact_count = 0;
			  APP_LOG(APP_LOG_LEVEL_DEBUG, "Confirm flag: %d", confirm_flag);
			  APP_LOG(APP_LOG_LEVEL_DEBUG, "numcontacts: %d", num_contacts);
			  getContact();
			  APP_LOG(APP_LOG_LEVEL_DEBUG, "Request first contact...");
			  return;
		 }
		 case 1: {
			 name_tuple = dict_find(iter, 1);
			 char* str = name_tuple->value->cstring;
			 char* sep = strchr(str, '|');
			 *sep = 0;
			 strncpy(contacts[contact_count].name, str,20);
			 strncpy(contacts[contact_count].number, sep+1,12);
			 APP_LOG(APP_LOG_LEVEL_DEBUG, "Name: %s Number: %s", contacts[contact_count].name, contacts[contact_count].number);
			 if(contact_count == 0){
				APP_LOG(APP_LOG_LEVEL_DEBUG, "Name: %s Number: %s", contacts[0].name, contacts[0].number);
			 	initialPhoneConnection(contacts[0].name, contacts[0].number);
			 }
			 contact_count++;
			 if(contact_count < num_contacts){
			 	getContact();
				APP_LOG(APP_LOG_LEVEL_DEBUG, "Request next contact...");
			 }
			 return;
		 }
		 case 2: {
			APP_LOG(APP_LOG_LEVEL_DEBUG, "Message Confirmed!");
			successful();
			break;
			
		 }
		 case 3: {
			APP_LOG(APP_LOG_LEVEL_DEBUG, "Message Failure!");
			name_tuple = dict_find(iter, 1);
			char* reason = name_tuple->value->cstring;
			failure(reason);
			
			
			break;
			
		 }
	  }

}

void placeCall(int position){
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);
	Tuplet value = TupletInteger(0, 2);
	dict_write_tuplet(iter, &value);
	Tuplet numvalue = TupletInteger(1, position);
	dict_write_tuplet(iter, &numvalue);
	app_message_outbox_send();
	APP_LOG(APP_LOG_LEVEL_DEBUG, "outbox sent");
}

static void fetchContacts(){
	APP_LOG(APP_LOG_LEVEL_DEBUG, "In fetchContacts()");
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);
	Tuplet value = TupletInteger(0, 0);
	dict_write_tuplet(iter, &value);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Outbox send...");
	app_message_outbox_send();

}


void init_msg(void) {

   app_message_register_inbox_received(in_received_handler);
   app_message_register_inbox_dropped(in_dropped_handler);
   app_message_register_outbox_sent(out_sent_handler);
   app_message_register_outbox_failed(out_failed_handler);


   const uint32_t inbound_size = 300;
   const uint32_t outbound_size = 64;
   app_message_open(inbound_size, outbound_size);
   fetchContacts();

}
