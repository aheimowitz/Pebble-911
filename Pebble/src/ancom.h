#ifndef ANCOM_H
#define ANCOM_H


typedef struct Contact{
	char* name;
	char number[12];
}Contact;


enum {
            AKEY_NUMBER,
            AKEY_TEXT,
};


void init_msg(void);

#endif
