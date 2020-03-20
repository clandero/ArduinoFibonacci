

#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>

// Listen to the default port 5555, the Yún webserver
// will forward there all the HTTP requests you send
BridgeServer server;

void setup() {
  //Serial startup
  Serial.begin(9600);
  while(!Serial);
  
  // Bridge startup
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);
  // Listen for incoming connection only from localhost
  // (no one from the external network could connect)
  server.listenOnLocalhost();
  server.begin();
}

void loop() {
  // Get clients coming from server
  BridgeClient client = server.accept();
  // There is a new client?
  if (client) {
    // Process request
    process(client);
    // Close connection and free resources.
    client.stop();
  }
  delay(50); // Poll every 50ms
}

void process(BridgeClient client) {
  // read the command
  String command = client.readStringUntil('/');
  Serial.print(command);
  if(command == "fibonacci"){
    single_fibonacci(client);
  }
  if(command == "sequence"){
    fibonacci_sequence(client);
  }
}

void single_fibonacci(BridgeClient client){
  int n = client.parseInt();
  client.print("Fibonacci of ");
  client.print(n);
  client.print(": ");
  client.print(fibonacci(n));
}

void fibonacci_sequence(BridgeClient client){
  int n = client.parseInt();
  int i=0;
  for(i=0;i<n;i++){
    client.print("Fibonacci of ");
    client.print(i);
    client.print(": ");
    client.println(fibonacci(i));
  }
}

int fibonacci(int n){
  int n_menos_2 = 1;
  int n_menos_1 = 0;
  int actual = 1;
  int i = 0;
  for (i = 0; i < n; i++) {
    n_menos_2 = n_menos_1;
    n_menos_1 = actual;
    actual = n_menos_2 + n_menos_1;
  }
  return actual;
}
