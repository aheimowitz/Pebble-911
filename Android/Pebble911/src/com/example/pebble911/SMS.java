package com.example.pebble911;

import android.app.PendingIntent;
import android.content.Intent;
import android.telephony.SmsManager;

public class SMS
{
	public static final String SENT = "sent";

	private Main_Activity activity;
	public SMS(Main_Activity activity)
	{
		this.activity = activity;
	}

	public void sendSMS(String phoneNumber, String message)
	{
		Intent sentIntent = new Intent(SENT);

		PendingIntent sentPI = PendingIntent.getBroadcast(
				activity.getApplicationContext(), 0, sentIntent,
				PendingIntent.FLAG_UPDATE_CURRENT);

        SmsManager sms = SmsManager.getDefault();
        message = sms.divideMessage(message).get(0);
        sms.sendTextMessage(phoneNumber, null, message, sentPI, null);
	}
}
