/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package EMG_Mirroring;

//import udpconnection.UDPConnection;

/**
 *
 * @author Siavash
 */
public class testStringSplit { 
    
    /**
     * @param args the command line arguments
     */
   
    
    public static void main(String[] args) {
        try{
        	String command = "sa#df#OK";
        	String[] cmdvalues = command.split("#");
        	System.err.println(cmdvalues[0] + cmdvalues[1] + cmdvalues.length );
        }
        catch (Exception e){
            System.err.println(e);
        }
        //while(true);
      
    }
    
}
