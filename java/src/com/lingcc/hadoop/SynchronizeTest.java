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
	public boolean exitProducerThread;


	public boolean consumer() {
		return false;

	}

	public static main() {
		
	}


	private class Producer extends Thread {
		public Producer() {
			setName("Thread for producer");
			setDaemon(true);
		}

		public void run() {
			do {
				int numNewMaps = getMapCompletionEvents();
				try {
					Thread.sleep(1000);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				System.out.println("numNewMaps=" + numNewMaps);
			} while(!exitProducerThread);
		}

		private int getMapCompletionEvents() {
			
			return 0;
		}
	}
}
