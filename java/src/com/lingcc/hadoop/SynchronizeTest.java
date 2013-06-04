package com.lingcc.hadoop;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;
import java.util.List;
import java.util.Random;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * This is a class for Java Synchronized key word with function call,
 *  switch-case statement test.
 *  
 * @author lkun.erlv@gmail.com
 *
 */
public class SynchronizeTest {

	public class LocalString {
		String local;
		
		public LocalString() {
			int randomInt = new Random(100).nextInt();
			local = Integer.toString(randomInt % 10);
		}
		public void setHardLink() {
			// TODO Auto-generated method stub
			System.out.print("set hardlink");
		}

	}


	public   List<LocalString> readyMapSpillFiles 
	= Collections.synchronizedList(new ArrayList<LocalString>());
	public boolean exitProducerThread;
	int numMap = 8;
	int currentmaps = 0;

	private static final Log LOG = LogFactory.getLog(SynchronizeTest.class.getName());

	public boolean consumer() throws InterruptedException {
		ProducerThread localproducer = null;
		localproducer = new ProducerThread();
		localproducer.start();

		while( currentmaps < numMap ) {
			LOG.info("Consumer: current finish " + currentmaps + ", totalmaps=" + numMap);
			synchronized(this.readyMapSpillFiles) {  // Throw ConcurrentModificationException here
				Iterator<LocalString> locItr = readyMapSpillFiles.iterator();

				while(locItr.hasNext()) {
					LocalString loc = locItr.next();
					//readyMapSpillFiles.remove(loc);
					if( loc != null) {
						loc.setHardLink();
					}
				}
			}
			//Thread.sleep(100);
		}
		return true;
	}

	public void  main( ) throws IOException, InterruptedException {
		LOG.info("start synchronized Test");
		if(!consumer()) {
			throw new IOException("Task: producer does not work");
		}
	}


	private class ProducerThread extends Thread {
		public ProducerThread() {
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
			for(int i = 0; i < 100; i++) {
				int randomInt = new Random().nextInt();
				randomInt = Math.abs(randomInt);
					synchronized(readyMapSpillFiles) {
						for( int j=0; j < (randomInt%7); j++) {
							LocalString tmp = new LocalString();
							LOG.info("Case 0/1 Operation on readyMapSpillFiles, add()");
							readyMapSpillFiles.add(tmp);
							LOG.info("Operation on readyMapSpillFiles, add() Done");
						}
						currentmaps ++;
					}
			}
			return 1;
		}
	}
}
