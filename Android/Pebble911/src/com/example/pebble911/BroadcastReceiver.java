package com.example.pebble911;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.telephony.SmsManager;
import android.util.Log;

public class BroadcastReceiver extends android.content.BroadcastReceiver {
	private Main_Activity activity;
	public BroadcastReceiver(Main_Activity activity)
	{
		this.activity = activity;
	}

	@Override
	public void onReceive(Context context, Intent intent) {
		String result = "";
		int code = getResultCode();
		Log.i("Broadcast", "Received result: " + code);

		switch (code)
		{
		case Activity.RESULT_OK:
			result = "Transmission successful";
			break;
		case SmsManager.RESULT_ERROR_GENERIC_FAILURE:
			result = "Failed";
			break;
		case SmsManager.RESULT_ERROR_RADIO_OFF:
			result = "Radio off";
			break;
		case SmsManager.RESULT_ERROR_NULL_PDU:
			result = "No PDU defined";
			break;
		case SmsManager.RESULT_ERROR_NO_SERVICE:
			result = "No service";
			break;
		default:
			result = "Error " + code;
			break;
		}
		Log.i("Broadcast", "Result: " + result);
		if (this.getResultCode() == Activity.RESULT_OK)
			activity.sendConfirmation();
		else
			activity.sendError(result);
	}
}
