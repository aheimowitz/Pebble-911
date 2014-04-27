/*
 * Copyright (C) 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.example.pebble911;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.util.ArrayList;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.content.Context;

public class Contacts {

    public static ArrayList<Contact> contactslist = new ArrayList<Contact>();
    public static Activity activ;
    
    public static void addContact(String name, String number){
    	contactslist.add(new Contact(name, number));
    }
    
    public static void readContacts(Activity act){
    	activ = act;
    	if(contactslist.size() == 0){
    		contactslist.add(new Contact("EMS", "7324078292"));
    	}
    	try {
    	      JSONArray jsonArray = new JSONArray("contacts.txt");
    	      for (int i = 0; i < jsonArray.length(); i++) {
    	        JSONObject jsonObject = jsonArray.getJSONObject(i);
    	        contactslist.add(new Contact(jsonObject.getString("name"), jsonObject.getString("phone")));
    	      }
    	    } catch (Exception e) {
    	      e.printStackTrace();
    	    }
    }

    public static void writeContacts() throws IOException{
    	JSONObject object = new JSONObject();
    	for(int x=0; x<contactslist.size(); x++){
    	  try {
    	    object.put("name", contactslist.get(x).getName());
    	    object.put("phone", contactslist.get(x).getNumber());
    	    writeObjectInInternalStorage((Context)activ,"contacts.txt",object);
    	  } catch (JSONException e) {
    	    e.printStackTrace();
    	  }
    	}
    }
    public static void writeObjectInInternalStorage(Context context, String filename, Object object) throws IOException {
        FileOutputStream fileOutputStream = context.openFileOutput(filename, Context.MODE_PRIVATE);
        ObjectOutputStream objectOutputStream = new ObjectOutputStream(fileOutputStream);
        objectOutputStream.writeObject(object);
        objectOutputStream.close();
        fileOutputStream.close();
    }

}
