#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTAsync.h"
#include "MQTTClient.h"
#include <locale.h>
#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ExemploClienteC"
#define QOS         1

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    char user_input[1000];
    char topic[500];

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if (MQTTClient_connect(client, &conn_opts) != MQTTCLIENT_SUCCESS) {
        printf("Falha ao conectar-se ao broker MQTT\n");
        return -1;
    }

    printf("Digite o topico MQTT que deseja criar, por favor: ");
    scanf("%500s", topic);

    int num_messages;
    printf("Quantas mensagens deseja enviar para o topico %s? ", topic);
    scanf("%d", &num_messages);

    for (int i = 0; i < num_messages; i++) {
        printf("Digite a mensagem %d: ", i + 1);
        scanf(" %[^\n]s", user_input);

        pubmsg.payload = user_input;
        pubmsg.payloadlen = strlen(user_input);
        pubmsg.qos = QOS;
        pubmsg.retained = 0;

        MQTTClient_publishMessage(client, topic, &pubmsg, &token);

        MQTTClient_waitForCompletion(client, token, 10000);
    }

    MQTTClient_disconnect(client, 10000);

    MQTTClient_destroy(&client);

    return 0;
}
