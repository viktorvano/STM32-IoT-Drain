package eu.cyberpunktech.drain;

import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;

public class Client extends Thread {
    // initialize socket and input output streams
    private Socket socket            = null;
    private DataOutputStream out     = null;

    private String address;
    private int port;
    private String message;

    // constructor to put ip address and port
    public Client(String address, int port, String message)
    {
        this.address = address;
        this.port = port;
        this.message = message;
    }

    @Override
    public void run() {
        super.run();

        // establish a connection
        try
        {
            socket = new Socket(address, port);
            System.out.println("Connected");

            // sends output to the socket
            out    = new DataOutputStream(socket.getOutputStream());
        }
        catch(UnknownHostException u)
        {
            System.out.println(u);
        }
        catch(IOException i)
        {
            System.out.println(i);
        }

        try
        {
            out.writeUTF(message);
        }
        catch(IOException i)
        {
            System.out.println(i);
        }


        // close the connection
        try
        {
            out.close();
            socket.close();
        }
        catch(IOException i)
        {
            System.out.println(i);
        }
    }
}
