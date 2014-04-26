#ifndef ANCOM_H
#define ANCOM_H


typedef struct Contact{
	char name[20];
	char number[12];
}Contact;


enum {
            AKEY_NUMBER,
            AKEY_TEXT,
};

int getFlagValue();
char* getContactName(int position);
char* getContactNumber(int position);
int getNumContacts();
void init_msg(void);
void in_received_handler(DictionaryIterator *iter, void *context);
void placeCall(int position);
#endif
