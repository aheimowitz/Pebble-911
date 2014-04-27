package com.example.pebble911;

public class Contact {
	private String name;
	private String phonenumber;
	
	public Contact(String namein, String phonein){
		name = namein;
		phonenumber = phonein;
	}
	
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getNumber() {
		return phonenumber;
	}
	public void setNumber(String phonenumber) {
		this.phonenumber = phonenumber;
	}

}
