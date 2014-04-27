package com.example.pebble911;

import android.location.Address;
import android.location.Location;
import android.location.LocationListener;
import android.os.Bundle;
import android.util.Log;

public class LocationGetter implements LocationListener {
	private Main_Activity activity;
	private int tries;
	
	public LocationGetter(Main_Activity activity)
	{
		this.activity = activity;
		tries = 0;
	}

	@Override
	public void onLocationChanged(Location location)
	{
		Log.i("Location", "Location changed called");
		(new LocationState(activity, this)).execute(location);
	}

	@Override
	public void onProviderDisabled(String provider) {}
	@Override
	public void onProviderEnabled(String provider) {}
	@Override
	public void onStatusChanged(String provider, int status, Bundle extras) {}
	
	private String stringFromAddress(Address address)
	{
		StringBuilder sb = new StringBuilder();
		int maxLines = address.getMaxAddressLineIndex();
		
		if (maxLines > 0)
		{
			sb.append(address.getAddressLine(0));
			for(int i=1;i<maxLines;i++)
			{
				sb.append(", ");
				sb.append(address.getAddressLine(i));
			}
		}
		return sb.toString();
	}
	
	public void finalCheck()
	{
		activity.finishLocation();
		tries = activity.getSettings().getMaxTries()+1;
		Location loc = activity.getLastLocation();
		Log.i("Location", "Final location is " + loc);
		if (loc == null)
			activity.sendError("No location");
		else
			(new LocationState(activity, this)).execute(loc);
	}
	
	public void setAddress(Address address)
	{
		if (address == null)
		{
			tries++;
			int max = activity.getSettings().getMaxTries();
			Log.e("Location", address + "\nTry " + tries + "/" + max);
			if (tries >= max)
			{
				if (tries == max)
					this.finalCheck();
				else
					activity.sendError("No location");
			}
		}
		else
		{
			tries = 0;
			String addrStr = stringFromAddress(address);
			Log.i("Location", "Got address: " + addrStr);
			activity.sendMessage(addrStr);
		}
	}
}
