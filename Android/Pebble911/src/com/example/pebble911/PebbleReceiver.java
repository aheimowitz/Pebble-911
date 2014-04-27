package com.example.pebble911;

import android.content.Context;
import android.util.Log;

import com.getpebble.android.kit.PebbleKit;
import com.getpebble.android.kit.util.PebbleDictionary;

public class PebbleReceiver extends PebbleKit.PebbleDataReceiver {
	private Main_Activity activity;
	
	public PebbleReceiver(Main_Activity activity)
	{
		super(Main_Activity.PEBBLE_APP_UUID);
		this.activity = activity;
	}
	
	public void reject(int id)
	{
		PebbleKit.sendNackToPebble(activity.getApplicationContext(), id);
	}
	public void reject(int id, String message)
	{
		Log.e(getClass().getSimpleName(), message);
		reject(id);
	}

	public void accept(int id)
	{
		PebbleKit.sendAckToPebble(activity.getApplicationContext(), id);
	}

	@Override
	public void receiveData(final Context context, final int transactionId, final PebbleDictionary data) {
		Long value = data.getInteger(0);
		if (value == null)
		{
			reject(transactionId, "No type sent");
			return;
		}
		int mode = (int) (long) value;
		Log.i("Receiver", "Id: " + transactionId + " Received value: " + mode + " for key: 0");
		
		switch(mode)
		{
		case 0:
			//Get settings
			activity.sendSettings();
			break;
		case 1:
		case 2:
			Long contactIndex = data.getInteger(1);
			if (contactIndex == null)
			{
				reject(transactionId, "No contact index");
				return;
			}
			int index = (int) (long) contactIndex;
			Log.i("Receiver", "Received index: " + index);

			boolean result = (mode == 1) ? activity.sendContact(index) : activity.startMessage(index);
			if (!result)
			{
				reject(transactionId, "Invalid contact index");
				return;
			}
			break;
		default:
			reject(transactionId, "Invalid mode");
		}
		accept(transactionId);
	}
}
