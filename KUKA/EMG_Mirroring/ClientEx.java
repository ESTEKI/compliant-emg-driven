/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package EMG_Mirroring;

//import java.net.*;
import java.util.Scanner;
import java.io.*;
import com.kuka.common.ThreadUtil;

/**
 *
 * @author Siavash
 */
public class ClientEx {
    
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        System.out.println("Client");// there is no difference in Client and server in UPD connection it is only a title 
        try{
        	
        String otherMachineIP ="172.31.1.147";//kuka controller IP

        File file =  new File("C:\\Users\\Students\\estekiKUKA\\emgdata\\finedata.txt");
    	  

              String command;
             // String sendme = "SENDME";
              String position ;
               
              UDPConnection cUDP = new UDPConnection(40003, 30002, otherMachineIP,10000);   
              Scanner sc = new Scanner(file); 
              command = cUDP.readRawDatastr();
              System.out.println(command);
                   
                  	 while (sc.hasNextLine()) 
                       {
                           position = sc.nextLine();
                           System.out.println(position); 
                            cUDP.writeDatastr(position);
                            if (position.equals("end"))
                          	  break;
                            ThreadUtil.milliSleep(30);
                       }
                  	 
                    
              
          }
          catch (Exception e){
              System.err.println(e);
          }
      
       
    }
}
