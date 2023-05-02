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
public class ServerEx { 
    
    /**
     * @param args the command line arguments
     */
   
    
    public static void main(String[] args) {
        try{
            
            String addr ="127.0.0.1";//InetSocketAddress address = new InetSocketAddress("127.0.0.1", 30001);
            System.out.println("UdpServer");
            
           
            UDPConnection sUDP = new UDPConnection(30001, 40002, addr,5000);
             
           
            
            while (true)
            {     
                String data = sUDP.readRawDatastr();
                if (!data.equals(""))
                {
                System.out.println(data);
                sUDP.writeDatastr(data.toUpperCase());
                }
            }
            
            
        }
        catch (Exception e){
            System.err.println(e);
        }
        //while(true);
      
    }
    
}
