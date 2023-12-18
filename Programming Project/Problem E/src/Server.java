import java.io.*;
import java.net.*;

// Server class
class Server {
    public static void main(String[] args) {
        ServerSocket server = null;

        try {
            // Server is listening on port 1234
            server = new ServerSocket(1234);

            // Running an infinite loop for handling client requests
            while (true) {
                // Socket object to receive incoming client requests
                Socket clientSocket = server.accept();

                // Displaying that a new client is connected to the server
                System.out.println("New client connected " + clientSocket.getInetAddress().getHostAddress());

                // Create a new thread (ClientHandler) to handle the client separately
                ClientHandler clientHandler = new ClientHandler(clientSocket);

                // Start the thread to handle the client
                new Thread(clientHandler).start();
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (server != null) {
                try {
                    server.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    // ClientHandler class (Handles each client in a separate thread)
    private static class ClientHandler implements Runnable {
        private final Socket clientSocket;

        // Constructor
        public ClientHandler(Socket socket) {
            this.clientSocket = socket;
        }

        public void run() {
            PrintWriter out = null;
            BufferedReader in = null;

            try {
                // Get the output stream of the client
                out = new PrintWriter(clientSocket.getOutputStream(), true);

                // Get the input stream of the client
                in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));

                String line;
                while ((line = in.readLine()) != null) {
                    // Print the received message from the client
                    System.out.printf(" Sent from the client: %s\n", line);

                    // Check if the client wants to disconnect
                    if ("exit".equals(line)) {
                        System.out.println("Client Disconnected \n");
                        out.println("You are disconnected \n");
                        break; // Break the loop if the client wants to disconnect
                    } else {
                        // Echo the message back to the client
                        out.println(line);
                    }
                }
            } catch (IOException e) {
                e.printStackTrace();
            } finally {
                try {
                    // Close resources (output stream, input stream, and the client socket)
                    if (out != null) {
                        out.close();
                    }
                    if (in != null) {
                        in.close();
                    }
                    clientSocket.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}



/*
Explanation:

    The server listens for client connections on port 1234 in an infinite loop.
    When a new client connects, a new thread (ClientHandler) is created to handle that client separately.
    The ClientHandler reads messages from the client, prints them, and echoes them back to the client.
    If the client sends "exit," the server acknowledges the disconnection and breaks out of the loop to disconnect the client.
    The server continues listening for new client connections.
*/