/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package EMG_Mirroring;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketTimeoutException;

/**
 * Creates a datagram socket with specified port and datagram packet for send and receive data 
 * from one computer to another.
 * when sending packets, we need to specify port and IP of reciever regardless of being Client or Server
 * so the port number can be different in Server and client BUT here for simplicity of codes enter the same 
 * port number on Server machine and Client machine and remember if you test both programs on one machine 
 * then you must use two different ports for each program(ser and cli)
 * remember using same buffer size on both c++ code and kuka default=512
 * @author Siavash
 * 
 */
public class UDPConnection {
    
    private int bufferSize = 512;
    private byte[] buffer = new byte[bufferSize];
    private int _soTimeOut = 1000;//milisec
    private InetAddress _otherMachineIP;
    private int _thisMachinePort;
    private int _otherMachinePort;
    //private boolean _Available = false;
    
    private DatagramPacket datagramPacket = null;
    private DatagramSocket datagramSocket ;
    
    /**
     * 
     * initialize the connection DatagramSocket using specified Port
     * default timeOut is 10 milisec
     * default input buffer size is 512 bytes
     * @param thisMachinePort  port number of DatagramSocket on your machine( in kuka use 30001 to 30009 only )
     * @param otherMachinePort port number in the machine you want to communicate with
     * @param otherMachineIP IP address of the machine you want to communicate with  in format of bytes
     *                  Ex: byte[] addr = {127,0,0,1}
     */
    public UDPConnection(int thisMachinePort,int otherMachinePort,String otherMachineIP) {
    
        init(thisMachinePort,otherMachinePort,otherMachineIP);
    }
    
    /**
     * 
     * initialize the connection DatagramSocket using specified Ports
     * 
     * default input buffer size is 512 bytes
     * @param thisMachinePort  port number of DatagramSocket on your machine( in kuka use 30001 to 30009 only )
     * @param otherMachinePort port number of DatagramSocket on the machine you want to communicate with
     * @param otherMachineIP IP address of the machine you want to communicate with  in format of bytes
     *                  Ex: byte[] addr = {127,0,0,1}
     * @param  tout maximum wait time on receive functions
     */
    public UDPConnection(int thisMachinePort,int otherMachinePort,String otherMachineIP,int tout) {
         _soTimeOut = tout;
         init(thisMachinePort,otherMachinePort,otherMachineIP);
    }
    
    private void init(int tmport,int omport,String ip)
    {
        _thisMachinePort = tmport;
        _otherMachinePort = omport;
        
        try{ 
           //_otherMachineIP = InetAddress.getByAddress(ip);
           _otherMachineIP = InetAddress.getByName(ip);
           datagramSocket = new DatagramSocket(_thisMachinePort); 
           datagramSocket.setSoTimeout(_soTimeOut);
        }
        catch(Exception e){
            e.printStackTrace();
           // System.out.println("hereee");
        }
    }
    /**
     * returns data in String format if there was no data returns ""
     * @return received buffer in String format
     */
    public String readRawDatastr()
    {
        String strBuff;
        byte[] data = readData();
        if (data.length==1 && data[0] == -1)
            return "";
        else
           return strBuff = new String(data).split("\0")[0];
        
    }
    
    /**
     * if time out exception happens and no data receives in set time then one byte of data containing -1 will be returned
     * @return returns buffer in byte array format
     */
    public byte[] readRawDatabyte()
    {
        return readData();
    }
    
    /**
     * if time out exception happens and no data receives in set time then one byte of data containing -1 will be returned
     * @return 
     */
    private byte[] readData ()
    {
        buffer = new byte[bufferSize];
        datagramPacket = new DatagramPacket(buffer, bufferSize);
        try{
             //datagramSocket.setSoTimeout(_soTimeOut);
             datagramSocket.receive(datagramPacket); 
             //buffer[20] = (byte) datagramPacket.getLength();//remember to remove this
              
           }
        catch (SocketTimeoutException e){
              buffer = new byte[1];   // clear buffer , time out exception happend (or anything else)
              buffer[0] = -1;
           }
        catch (IOException eio)
        {
             eio.printStackTrace();
        }
        return buffer;
    }
    
    private void writeData(byte[] outData)
    {
        try{
        datagramPacket = new DatagramPacket(outData, outData.length, _otherMachineIP, _otherMachinePort);  
        datagramSocket.send(datagramPacket);
        }
        catch (Exception e){
            e.printStackTrace();
        }
              
    }
    
    /**
     * sends data to datagram socket
     * @param inData String data to be written
     */
    public void writeDatastr(String inData)
    {
        
        byte[] sendbuffer = new byte[inData.length()]; 
        sendbuffer = inData.getBytes();
        writeData(sendbuffer);
  
    }
    
    /**
     * Set maximum input buffer size 
     * Ex: String "hello" needs 5+1 bytes of buffer
     * remember using same buffer size on both c++ code and kuka
     * @param bfSize Buffer Size default=512
     */
    public void setBufferSize(int bfSize)
    {
        bufferSize = bfSize;
    }
    
    /**
     * 
     * @return Buffer Size
     */
    public int getBufferSize()
    {
        return bufferSize;
    }
    
    /**
     * maximum time out for wait until receive data
     * @param t Time out for wait time on receive data in milisec
     */
    public void setTimeOut(int t) 
    {
        _soTimeOut = t;
        try{
            datagramSocket.setSoTimeout(_soTimeOut);
        }
        catch (Exception e)
        {
          e.printStackTrace();
        }
    }
    
    public int getTimeOut()
    {
       return _soTimeOut;
    }
    
    /**
     * closes datagram socket
     */
    public void closeUDP()
    {
        datagramSocket.close();
    }
}
