package com.example.pebble911;

public class Settings {
	private boolean confirm;
	private boolean sendText;
	private int maxTries;
	private String message;
	
	public boolean getConfirm()
	{
		return confirm;
	}
	public boolean getSendText()
	{
		return sendText;
	}
	public int getMaxTries()
	{
		return maxTries;
	}
	public String getMessage()
	{
		return message;
	}
	public void setConfirm(boolean confirm)
	{
		this.confirm = confirm;
	}
	public void setSendText(boolean sendText)
	{
		this.sendText = sendText;
	}
	public void setMaxTries(int maxTries)
	{
		this.maxTries = maxTries;
	}
	public void setMessage(String message)
	{
		this.message = message;
	}
}
