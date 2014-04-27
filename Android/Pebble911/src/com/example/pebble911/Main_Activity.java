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

import java.io.IOException;
import java.util.ArrayList;
import java.util.Timer;
import java.util.TimerTask;

import android.app.Activity;
import android.content.ActivityNotFoundException;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.database.Cursor;
import android.location.Location;
import android.location.LocationManager;
import android.location.LocationProvider;
import android.net.Uri;
import android.os.Bundle;
import android.provider.ContactsContract;
import android.provider.ContactsContract.CommonDataKinds.Phone;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.widget.ListView;

import com.example.android.Pebble911.R;
import com.getpebble.android.kit.PebbleKit;
import com.getpebble.android.kit.PebbleKit.PebbleAckReceiver;
import com.getpebble.android.kit.PebbleKit.PebbleNackReceiver;
import com.getpebble.android.kit.util.PebbleDictionary;

/**
 * This application creates a listview where the ordering of the data set
 * can be modified in response to user touch events.
 *
 * An item in the listview is selected via a long press event and is then
 * moved around by tracking and following the movement of the user's finger.
 * When the item is released, it animates to its new position within the listview.
 */
public class Main_Activity extends Activity {
	private final int PICK_CONTACT = 1;
public static final java.util.UUID PEBBLE_APP_UUID = java.util.UUID.fromString("7f480ee9-82d3-4df7-9c6b-6d2319159fc3");
	
	private ArrayList<Contact> contacts;
	private Settings settings;
	private LocationManager locationManager;
	private LocationGetter locationListener;
	private Contact currentContact;
	
	public Timer timer;
	
	private ArrayList<Contact> fakecontacts = new ArrayList<Contact>();
	private int current = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_list_view);

        ArrayList<String>contactlist = new ArrayList<String>();
        Contacts.readContacts(this);
        for (int i = 0; i < Contacts.contactslist.size(); ++i) {
            contactlist.add(Contacts.contactslist.get(i).getName() + " - " + Contacts.contactslist.get(i).getNumber());
        }
        contacts = Contacts.contactslist;
        StableArrayAdapter adapter = new StableArrayAdapter(this, R.layout.text_view, contactlist);
        DynamicListView listView = (DynamicListView) findViewById(R.id.listview);

        listView.setcontactlist(contactlist);
        listView.setAdapter(adapter);
        listView.setChoiceMode(ListView.CHOICE_MODE_SINGLE);
        
        settings = new Settings();
		settings.setConfirm(true);
		settings.setMessage("Hello world!");
		settings.setSendText(true);
		settings.setMaxTries(3);
		settings.setMessage("My name is Kevin. I am at <address>. Please send help.");
		
		fakecontacts.add(new Contact("Mom", "16077591448"));
		fakecontacts.add(new Contact("Adam", "17324078292"));
		
		locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);
		locationListener = new LocationGetter(this);
		
		
		PebbleKit.registerReceivedDataHandler(this, new PebbleReceiver(this));
        PebbleKit.registerReceivedAckHandler(getApplicationContext(), new PebbleAckReceiver(PEBBLE_APP_UUID) {
          @Override
          public void receiveAck(Context context, int transactionId) {
            Log.i(getLocalClassName(), "Received ack for transaction " + transactionId);
          }
        });

        PebbleKit.registerReceivedNackHandler(getApplicationContext(), new PebbleNackReceiver(PEBBLE_APP_UUID) {
          @Override
          public void receiveNack(Context context, int transactionId) {
            Log.i(getLocalClassName(), "Received nack for transaction " + transactionId);
          }
        });

        this.registerReceiver(new BroadcastReceiver(this), new IntentFilter(SMS.SENT));
        
    }
    
    public void addContacttoList(String name, String number) throws IOException{
    	Contacts.addContact(name, number);
    	ArrayList<String>contactlist = new ArrayList<String>();
        for (int i = 0; i < Contacts.contactslist.size(); ++i) {
            contactlist.add(Contacts.contactslist.get(i).getName() + " - " + Contacts.contactslist.get(i).getNumber());
        }
        //contactlist.add(name + " - " + number);
        contacts = Contacts.contactslist;

        StableArrayAdapter adapter = new StableArrayAdapter(this, R.layout.text_view, contactlist);
        DynamicListView listView = (DynamicListView) findViewById(R.id.listview);

        listView.setcontactlist(contactlist);
        listView.setAdapter(adapter);
        listView.setChoiceMode(ListView.CHOICE_MODE_SINGLE);
        Contacts.writeContacts();
    }
    
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu items for use in the action bar
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.layout.menu_activity, menu);
        return super.onCreateOptionsMenu(menu);
    }
    
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle presses on the action bar items
        switch (item.getItemId()) {
            case R.id.action_new:
            	Intent intent = new Intent(Intent.ACTION_PICK, ContactsContract.Contacts.CONTENT_URI);
            	startActivityForResult(intent, PICK_CONTACT);
            	
                return true;
            case R.id.action_settings:
                //OPEN SETTINGS WINDOW
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }
    
    @Override
	public void onActivityResult(int reqCode, int resultCode, Intent data) {
		super.onActivityResult(reqCode, resultCode, data);

		switch (reqCode) {
		case (PICK_CONTACT):
			String name = null;
			String number = null;

			if (resultCode == Activity.RESULT_OK) {
				name = fakecontacts.get(current).getName();
				number = fakecontacts.get(current).getNumber();
				current++;
				/*
Cursor cursor = null;
    try {
        cursor = getApplicationContext().getContentResolver().query(Phone.CONTENT_URI, null, null, null, null);
        int nameIdx = cursor.getColumnIndex(Phone.DISPLAY_NAME);
        int phoneNumberIdx = cursor.getColumnIndex(Phone.NUMBER);
        cursor.moveToFirst();
        	Log.i("Main", "Getting name");
            name = cursor.getString(nameIdx);
        	Log.i("Main", "Getting number");
            number = cursor.getString(phoneNumberIdx);
        	Log.i("Main", "Yay " + name + " " + number);
    } catch (Exception e) {
        e.printStackTrace();
    } finally {
        if (cursor != null) {
            cursor.close();
        }
    }
    */
	/*			Uri contactData = data.getData();
				Cursor c = getContentResolver().query(contactData, null, null,
						null, null);
				if (c.moveToFirst()) {
					name = c.getString(c
							.getColumnIndex(ContactsContract.Contacts.DISPLAY_NAME));
					Cursor pCur = this
							.getContentResolver()
							.query(ContactsContract.CommonDataKinds.Phone.CONTENT_URI,
									null,
									ContactsContract.CommonDataKinds.Phone.CONTACT_ID
											+ " = ?",
									new String[] { c.getString(c
											.getColumnIndex(ContactsContract.Contacts._ID)) },
									null);
					number = c
							.getString(c
									.getColumnIndex(ContactsContract.CommonDataKinds.Phone.NUMBER));
					number = c.getString(c.getColumnIndex(Phone.NUMBER));*/
					try {
						addContacttoList(name, number);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
			}

		}
//KEVINS CODE STARTS HERE ================================================
	
	public Location getLastLocation()
	{
		LocationProvider lp = locationManager.getProvider("gps");
		return locationManager.getLastKnownLocation(lp.getName());
	}
	
	public Settings getSettings()
	{
		return settings;
	}
	
	
	public void sendSMS(String phoneNumber, String message)
	{
		new SMS(this).sendSMS(phoneNumber, message);
	}
	public void call(String phoneNumber)
	{
        try {
                Intent callIntent = new Intent(Intent.ACTION_CALL);
                callIntent.setData(Uri.parse("tel:"+phoneNumber));
                startActivity(callIntent);
        } catch (ActivityNotFoundException activityException) {
        	Log.e("Calling a Phone Number", "Call failed", activityException);
        }
     }
	
	public void sendSettings()
	{
		Log.i(this.getLocalClassName(), "Sending settings...");
		PebbleDictionary dict = new PebbleDictionary();
		byte confirm = (byte) (settings.getConfirm() ? 1 : 0);
		byte size = (byte) contacts.size();

		dict.addInt8(0, (byte)0);
		dict.addInt8(1, (byte) ((size << 1) | confirm));

		PebbleKit.sendDataToPebble(getApplicationContext(), PEBBLE_APP_UUID, dict);
	}
	public boolean sendContact(int index)
	{
		Log.i(this.getLocalClassName(), "Sending contact " + index + "...");
		if (index < 0 || index >= contacts.size())
		{
			Log.e("MainActivity", "Invalid contact index: " + index);
			return false;
		}
		PebbleDictionary dict = new PebbleDictionary();
		
		dict.addInt8(0, (byte) 1);
		
		Contact contact = contacts.get(index);
		String str = contact.getName() + "|" + contact.getNumber();
		dict.addString(1, str);

		PebbleKit.sendDataToPebble(getApplicationContext(), PEBBLE_APP_UUID, dict);
		return true;
	}
	
	public void sendConfirmation()
	{
		Log.i(this.getLocalClassName(), "Sending message confirmation...");
		PebbleDictionary dict = new PebbleDictionary();
		dict.addInt8(0, (byte) 2);
		PebbleKit.sendDataToPebble(getApplicationContext(), PEBBLE_APP_UUID, dict);
	}
	
	public void sendError(String error)
	{
		//locationManager.removeUpdates(locationListener);
		Log.i(this.getLocalClassName(), "Sending message error...");
		PebbleDictionary dict = new PebbleDictionary();
		dict.addInt32(0, 3);
		dict.addString(1, error);
		PebbleKit.sendDataToPebble(getApplicationContext(), PEBBLE_APP_UUID, dict);
	}
	
	public boolean startMessage(int index)
	{
		Log.i(this.getLocalClassName(), "Attempting to send message...");
		if (index < 0 || index >= contacts.size())
		{
			Log.e(this.getLocalClassName(), "ERROR: Index " + index + " is invalid for contact!");
			return false;
		}
		else
		{
			currentContact = contacts.get(index);
			requestLocation();
			//sendMessage("ADDRESS");
			//text.setText("Getting location...\nPlease wait...");
			return true;
		}
	}
	
	public void requestLocation()
	{
		finishLocation();
		Log.i("Main", "Here");
		locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 5000, 10, locationListener);
		Log.i("Main", "And Here");
		timer = new Timer();
		timer.schedule(new TimerTask() {
			public void run()
			{
				Log.i("MainActivity", "Timer went off!");
				//text.setText("Timer went off!");
				finishLocation();
				locationListener.finalCheck();
			}
		}, 15000);
	}
	public void finishLocation()
	{
		locationManager.removeUpdates(locationListener);
		if (timer != null)
		{
			timer.cancel();
			timer = null;
		}
	}
	
	public void sendMessage(String address)
	{
		finishLocation();
		String def = settings.getMessage();
		String message = def.replace("<address>", address);
		Log.i("Message", message);
		sendSMS(currentContact.getNumber(), message);
	}

}

