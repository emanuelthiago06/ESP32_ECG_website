#include <WiFi.h>
#include <HTTPClient.h>
#include "fileData.h" // Inclui o arquivo com o array fileData

const char* ssid = "brisa-2983954";      // Substitua pelo seu SSID
const char* password = "dtiu2ujf";       // Substitua pela senha do Wi-Fi
const char* serverUrl = "http://192.168.0.10:8000/upload-esp/";

const int LINES_PER_PACKET = 128; // Número de linhas por pacote

void setup() {
  Serial.begin(115200);

  // Conectando ao Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Conectando ao Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");

  // Enviar as linhas em pacotes
  enviarPacotes();
}

void enviarPacotes() {
  int totalLinhas = sizeof(fileData2) / sizeof(fileData2[0]); // Total de linhas no array

  for (int i = 0; i < totalLinhas; i += LINES_PER_PACKET) {
    String pacote = "[";
    String pos = "meio";

    // Construir o pacote com até LINES_PER_PACKET linhas
    for (int j = 0; j < LINES_PER_PACKET && (i + j) < totalLinhas; j++) {
      pacote += "\"" + String(fileData2[i + j]) + "\"";
      if (j < LINES_PER_PACKET - 1 && (i + j + 1) < totalLinhas) {
        pacote += ",";
      }
    }
    pacote += "]";

    // Determinar a posição (início, meio, fim)
    if (i == 0) {
      pos = "inicio";
    } else if ((i + LINES_PER_PACKET) >= totalLinhas) {
      pos = "fim";
    }

    enviarPacote(pacote, pos);
    delay(100); // Pequeno atraso para evitar sobrecarga
  }
}

void enviarPacote(String pacote, String pos) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Configura a URL do servidor
    http.begin(serverUrl);

    // Configura o tipo de conteúdo
    http.addHeader("Content-Type", "application/json");

    // Cria o payload em JSON
    String userName = "dev2";
    String jsonPayload = "{";
    jsonPayload += "\"user\": \"" + userName + "\",";
    jsonPayload += "\"lines\": " + pacote + ",";
    jsonPayload += "\"position\": \"" + pos + "\"";
    jsonPayload += "}";

    // Exibe o payload no Serial Monitor
    Serial.print("Enviando pacote: ");
    Serial.println(jsonPayload);

    // Envia os dados usando POST
    int httpResponseCode = http.POST(jsonPayload);

    // Verifica a resposta do servidor
    if (httpResponseCode > 0) {
      Serial.print("Resposta do servidor: ");
      Serial.println(httpResponseCode);
      Serial.println(http.getString());
    } else {
      Serial.print("Erro ao enviar dados: ");
      Serial.println(http.errorToString(httpResponseCode));
    }

    // Fecha a conexão
    http.end();
  } else {
    Serial.println("Wi-Fi desconectado!");
  }
}

void loop() {
  // O loop está vazio porque o envio é feito no setup
}
