// #include "setup.h"

// /** MQTT broker URL */
// static const char * mqttBroker = "";
// /** MQTT connection id */
// static const char * mqttID = "";
// /** MQTT user name */
// static const char * mqttUser = "";
// /** MQTT password */
// static const char * mqttPwd = "";

// /** WiFi client class to receive messages from mqtt broker */
// WiFiClient mqttReceiver;
// /** Max number of messages in buffer */
// #define msgBufferSize 10

// void mqttTask(void *pvParameters);
// void messageReceived(String &topic, String &payload);

// /** Structure for MQTT message Buffer */
// struct mqttMsgStruct {
// 	String topic = "";
// 	String payload = "";
// 	bool retained = false;
// };

// /** Queued messages for MQTT */
// mqttMsgStruct mqttMsg[msgBufferSize];
// /** Task handle for the MQTT publisher task */
// TaskHandle_t mqttTaskHandle = NULL;

// /**
//  * initMqtt
//  * Initialize Meeo connection
//  */
// void initMqtt() {
// 	// Clear message structure
// 	for (int index = 0; index < msgBufferSize; index++) {
// 		mqttMsg[index].topic = "";
// 	}

// 	// Connect to MQTT broker
// 	if (!connectMQTT()) {
// 		Serial.println("Can't connect to MQTT broker");
// 		return;
// 	}

// 	// Start task for MEEO publishing
// 	xTaskCreatePinnedToCore(
// 			mqttTask,              /* Function to implement the task */
// 			"MQTT ",               /* Name of the task */
// 			2000,                  /* Stack size in words */
// 			NULL,                  /* Task input parameter */
// 			5,                     /* Priority of the task */
// 			&mqttTaskHandle,       /* Task handle. */
// 			1);                    /* Core where the task should run */

// 	// Tell MQTT broker we are alive
// 	// mqttClient.publish((char *)"/DEV/ESP32",(char *)"Office",6,true,0);
// 	addMqttMsg ("DEV/ESP32", devLoc, true);
// }

// /**
//  * addMqttMsg
//  * Adds a message to the buffer to be processed by meeoTask()
//  *
//  * @param topic
//  *		 String with the topic
//  * @param payload
//  *		String with the payload
//  * @return bool
//  *		true if message is added to the buffer
//  *		false if buffer was full
//  */
// bool addMqttMsg (String topic, String payload, bool retained) {
// 	bool queueResults = false;
// 	for (byte index = 0; index < msgBufferSize; index ++) {
// 		if (mqttMsg[index].topic.length() == 0) { // found an empty slot?
// 			mqttMsg[index].topic = topic;
// 			mqttMsg[index].payload = payload;
// 			mqttMsg[index].retained = retained;
// 			queueResults = true;
// 			break;
// 		}
// 	}
// 	if (tasksEnabled) {
// 		if (mqttTaskHandle != NULL) {
// 			vTaskResume(mqttTaskHandle);
// 		}
// 	}
// 	return queueResults;
// }

// /**
//  * Task to send data from meeoMsg buffer to Meeo.IO
//  * @param pvParameters
//  *		pointer to task parameters
//  */
// void mqttTask(void *pvParameters) {
// 	// Serial.println("mqttTask loop started");
// 	while (1) // mqttTask loop
// 	{
// 		if (otaRunning) {
// 			vTaskDelete(NULL);
// 		}
// 		if (connStatus != CON_GOTIP) {
// 			Serial.println("MQTT has message, but no WiFi connection");
// 			vTaskSuspend(NULL);
// 		}
// 		for (byte index = 0; index < msgBufferSize; index ++) {
// 			if (mqttMsg[index].topic.length() != 0) {
// 				if (mqttMsg[index].topic == "WEI") {
// 					// Broadcast weather status over UDP
// 					// IPAddress multiIP (192,	168, 0, 255);
// 					udpSendMessage(multiIP, mqttMsg[index].payload, 9997);
// 					mqttMsg[index].topic = "";
// 				} else if (mqttClient.publish("/"+mqttMsg[index].topic,mqttMsg[index].payload,mqttMsg[index].retained,0)) {
// 					mqttMsg[index].topic = "";
// 				} else { // Publishing error. Maybe we lost connection ???
// 					// Serial.println("Sending to MQTT broker failed");
// 					if (connectMQTT()) { // Try to reconnect and resend the message
// 						if (mqttClient.publish("/"+mqttMsg[index].topic,mqttMsg[index].payload,mqttMsg[index].retained,0)) {
// 							mqttMsg[index].topic = "";
// 							// mqttClient.publish((char *)"/DEV/ESP32",(char *)"Office",6,true,0);
// 							addMqttMsg ("DEV/ESP32", devLoc, true);
// 						}
// 					}
// 				}
// 			}
// 		}
// 		bool queueIsEmpty = true;
// 		for (byte index = 0; index < msgBufferSize; index ++) {
// 			if (mqttMsg[index].topic.length() != 0) {
// 				queueIsEmpty = false;
// 			}
// 		}
// 		if (queueIsEmpty) {
// 			vTaskSuspend(NULL);
// 		}
// 	}
// }

// /**
//  * Connect to MQTT broker
//  *
//  * @return <code>bool</code>
//  *			true if connection was successful
//  *			false if connection failed
//  **/
// bool connectMQTT() {
// 	// Serial.println("Connecting to MQTT broker");

// 	// Connect to MQTT broker
// 	mqttClient.begin(mqttBroker, mqttReceiver);
// 	// Setup callback function for messages from broker
// 	mqttClient.onMessage(messageReceived);

// 	int connectTimeout = 0;

// 	while (!mqttClient.connect(mqttID, mqttUser, mqttPwd)) {
// 		delay(100);
// 		connectTimeout++;
// 		if (connectTimeout > 10) { // Wait for 1 seconds to connect
// 			// Serial.println("Can't connect to MQTT broker");
// 			return false;
// 		}
// 	}
// 	// Serial.println("Connected to MQTT");

// 	// Set MQTT last will
// 	mqttClient.setWill("/DEV/ESP32", "Dead");

// 	return true;
// }

// /**
//  * messageReceived
//  * Called when subscribed message was received from MQTT broker
//  *
//  * @param topic
//  *			topic of received message
//  * @param payload
//  *			payload of message
// */
// void messageReceived(String &topic, String &payload) {
// 	// Serial.println("MQTT - received from topic: " + topic +	" payload: " + payload);
// }
