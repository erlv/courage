package com.lingcc.hadoop;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;



/**
 * This is a class for Java Synchronized key word with function call,
 *  switch-case statement test.
 *  
 *  TODO: to test what is wrong with my synchronzed code to access an synchronized
 *  list, with one consumer, and one producer.
 * @author erlv
 *
 */
public class SynchronizeTest {
	  public   List<String> readyMapSpillFiles 
  		= Collections.synchronizedList(new ArrayList<String>());
	  
	  
	  public boolean consumer() {
		return false;
		  
	  }
	  
	  
	  
	  private class Producer extends Thread {
		  public Producer() {
			  setName("Thread for producer");
			  setDaemon(true);
		  }
	  }
	  
	  
}
