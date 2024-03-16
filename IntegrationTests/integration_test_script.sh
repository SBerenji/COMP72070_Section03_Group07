#!/bin/bash

# Start the server
echo "Starting server..."
./server &

# Store the PID of the server process
server_pid=$!

# Wait for the server to start (assuming it listens on port 27000)
sleep 2

# Start the client
echo "Starting client..."
./client

# Wait for the client to finish

# Kill the server process
echo "Stopping server..."
kill $server_pid

echo "Integration test completed."