package com.example.pebble911;

import java.io.IOException;
import java.util.List;
import java.util.Locale;

import android.location.Address;
import android.location.Geocoder;
import android.location.Location;
import android.os.AsyncTask;
import android.util.Log;

public class LocationState extends AsyncTask<Location, Void, Address>
{
	private Main_Activity activity;
	private LocationGetter getter;
	
	public LocationState(Main_Activity activity, LocationGetter getter)
	{
		this.activity = activity;
		this.getter = getter;
	}
	@Override
	protected Address doInBackground(Location... params)
	{
		Location location = params[0];
		String error = null;
		Geocoder gcd = new Geocoder(activity.getBaseContext(), Locale.getDefault());
        List<Address> addresses;
        try
        {
        	addresses = gcd.getFromLocation(location.getLatitude(), location.getLongitude(), 1);
        	if (!addresses.isEmpty())
        		return addresses.get(0);
        	else
        		error = "No address";
        }
        catch (IOException e)
        {
        	e.printStackTrace();
        	error = e.getMessage();
        }
        Log.e("Location", "Error: " + error);
		return null;
	}
	
	@Override
	protected void onPostExecute(Address address)
	{
		getter.setAddress(address);
	}
}
