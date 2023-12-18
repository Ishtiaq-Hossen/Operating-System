import java.io.*;
import java.net.*;
import java.util.*;

// Client class
class Client {

    // Driver code
    public static void main(String[] args) {
        // Establish a connection by providing host and port number
        try (Socket socket = new Socket("localhost", 1234)) {

            // Writing to the server
            PrintWriter out = new PrintWriter(socket.getOutputStream(), true);

            // Reading from the server
            BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));

            // Object of Scanner class for user input
            Scanner sc = new Scanner(System.in);
            String line = null;

            // Loop until the user inputs "exit"
            while (!"exit".equalsIgnoreCase(line)) {
                // Reading input from the user
                line = sc.nextLine();

                // Sending the user input to the server
                out.println(line);
                out.flush();

                // Displaying the server's reply
                System.out.println("Server replied: " + in.readLine());
            }

            // Closing the scanner object
            sc.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}



/*
Explanation:

The client establishes a connection to a server at the specified host ("localhost") and port number (1234).
It creates a PrintWriter for writing to the server and a BufferedReader for reading from the server.
It uses a Scanner object (sc) to read input from the user.
The client enters a loop where it continuously reads input from the user, sends it to the server, and prints the server's reply.
The loop continues until the user inputs "exit."
The Scanner object is closed, and any IOExceptions are caught and printed.

*/