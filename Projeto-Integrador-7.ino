/*

  sistema adaptado por Paulo Furlan 12/2018
  Revisado para projeto integrador 7
  Univesp
  27/10/2021 -> Paulo Furlan
  Implementado opcoes para AP ou STA

*/
//=============declaracao includes==========================
#include <ESP8266WiFi.h>
#include <ESP8266LLMNR.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>

//=================== Escolher Modo de Operacao ==============
#ifndef MODE
#define MODE 1  ///1 para AP 0 para cliente
#endif
//============================================================
char* ssid = "";    //nome da rede que ira conectar
char* password = "";//senha da



void config_rede() {
  if (MODE) {//Mode AP
    ssid = "Projeto7";    //nome da rede que ira conectar
    password = "univesp";//senha da
    IPAddress ip(192, 168, 4, 1);
    IPAddress gateway(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);
    IPAddress primaryDNS(8, 8, 8, 2);
    IPAddress secondaryDNS(8, 8, 8, 3);
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(ip, gateway, subnet);
    delay(1000);
    Serial.println("===============================================");
    Serial.println("MODO AP");
    Serial.print("ssid: ");
    Serial.println(ssid);
    Serial.print("password: ");
    Serial.println(password);
    Serial.print("ip: ");
    Serial.println(ip);
    Serial.print("gateway: ");
    Serial.println(gateway);
    Serial.print("subnet: ");
    Serial.println(subnet);
    Serial.println("===============================================");

  } else {
    ssid = "Sua Rede";    //nome da rede que ira conectar
    password = "Sua Senha";//senha da rede
    IPAddress ip(192, 168, 0, 27);
    IPAddress gateway(192, 168, 0, 1);
    IPAddress subnet(255, 255, 255, 0);
    IPAddress primaryDNS(181, 213, 132, 2);
    IPAddress secondaryDNS(181, 213, 132, 3);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    WiFi.config(ip, gateway, subnet);
    delay(1000);
    Serial.println("===============================================");
    Serial.println("MODO CLIENTE");    
    Serial.print("ssid: ");
    Serial.println(ssid);
    Serial.print("password: ");
    Serial.println(password);
    Serial.print("ip: ");
    Serial.println(ip);
    Serial.print("gateway: ");
    Serial.println(gateway);
    Serial.print("subnet: ");
    Serial.println(subnet);
    Serial.println("===============================================");
  }

  return;
}

//====================== cria objeto servidor porta 80 =====================
ESP8266WebServer web_server(80);    //cria objeto para o servidor na porta 80

int pinoD5 = 4;
int pinoD8 = 5;
int led_online = 12;
int led_offline = 14;
String  est_lamp1 = "aguardando";
String  est_lamp2 = "aguardando";
int lamp1;
int lamp2;
long rssi;
boolean flag_lamp1 = false;
boolean flag_lamp2 = false;


//=============== criacao da pagina========================================================
String getPage() {
  String page = "<html lang=pt-BR><head><meta http-equiv='refresh' content='12'/>";
  page += "<title>Univesp</title>";
  page += "<script src=\"https://code.jquery.com/jquery-1.11.2.js\"></script>";
  page += "<script type=\"text/javascript\">jQuery(window).load(function($){atualizaRelogio();});</script>";
  page += "<style> body { background-color: #fffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style>";
  page += "<style> INPUT.MeuInput { width: 50%; height: 60px;font-family: Arial, Helvetica, Sans-Serif;font-size: 100%;Color: Green; background-color: #0ff0f;}</style>";
  page += "</head><body><h1>Projeto Integrador-7</h1>";
  page += "<HR size=\"8\" WIDTH=100% color=#000088 noshade >";
  page += "<div><h2>Data: <output id=\"data\" style=\"font-family: 'arial black', 'avant garde'; font-size: 24px;\"></output>";
  page += "->Hora: <output id=\"hora\" style=\"font-family: 'arial black', 'avant garde'; font-size: 24px;\"></output>";
  page += "->Sinal (RSSI):" + String(rssi) + "dBm</h2></div>";
  page += "<HR size=8 WIDTH=100% color=#000088>";
  page += "<HR size=10 WIDTH=100% color=#005000>";
  page += "<h1>L&acircmpadas:</h1>";
  page += "<form action='/' method='POST'>";
  //=================================================BOTAO GARAGEM==============================================================================
  page += "<ul><li><h1>";
  page += "<output id='led_status' style=\"font-family: 'arial black', 'avant garde'; font-size: 24px;\"></output>"; //Label aviso status
  page += "<INPUT type='submit'name='LED' id='led' onclick=\"mudarNome();\" value='Garagem'  class='MeuInput'>";//Botao comando
  page += "</h1></li></ul>";

  page += "<HR size=\"1\" WIDTH=100% color=#000088>";
  //===============================================BOTAO JARDIM==================================================================================
  page += "<ul><li><h1>";
  page += "<output id='led2_status' style=\"font-family: 'arial black', 'avant garde'; font-size: 24px;\"></output>"; //Label aviso status
  page += "<INPUT type='submit'name='LED2' id='led2' onclick=\"mudarNome2();\" value='Jardim'  class='MeuInput'>";//Botao comando
  page += "</h1></li></ul>";

  page += "<HR size=10 WIDTH=100% color=#005000>";
  page += "</body>";

  page += "  <script>";
  page += "  function atualizaRelogio(){ ";
  page += "    var momentoAtual = new Date();";
  page += "    var vhora = momentoAtual.getHours();";
  page += "    var vminuto = momentoAtual.getMinutes();";
  page += "    var vsegundo = momentoAtual.getSeconds();";

  page += "    var vdia = momentoAtual.getDate();";
  page += "    var vmes = momentoAtual.getMonth() + 1;";
  page += "    var vano = momentoAtual.getFullYear();";

  page += "    if (vdia < 10){ vdia = \"0\" + vdia;}";
  page += "    if (vmes < 10){ vmes = \"0\" + vmes;}";
  page += "    if (vhora < 10){ vhora = \"0\" + vhora;}";
  page += "    if (vminuto < 10){ vminuto = \"0\" + vminuto;}";
  page += "    if (vsegundo < 10){ vsegundo = \"0\" + vsegundo;}";

  page += "    dataFormat = vdia +\" / \" + vmes + \"/ \" + vano;";
  page += "    horaFormat = vhora + \": \" + vminuto +  \": \" + vsegundo;";

  page += "    document.getElementById(\"data\").innerHTML = dataFormat;";
  page += "    document.getElementById(\"hora\").innerHTML = horaFormat;";

  page += "    setTimeout(\"atualizaRelogio()\",1000);";
  page += "  }";
  page += "</script> ";

  page += "<script>";
  page += "function mudarNome()";
  page += "{";

  page += "if(document.getElementById(\"led\").value == \"Garagem\")";
  page += "{document.getElementById(\"led\").value = \"Aguardar\";document.getElementById(\"led_status\").innerHTML = \"Enviando\";} else";
  page += "{document.getElementById(\"led\").value = \"Garagem\";document.getElementById(\"led_status\").innerHTML =\"Aguardando\";}";

  page += "}";
  page += "</script>";


  page += "<script>";
  page += "function mudarNome2()";
  page += "{";

  page += "if(document.getElementById(\"led2\").value == \"Jardim\")";
  page += "{document.getElementById(\"led2\").value = \"Aguardar\";document.getElementById(\"led2_status\").innerHTML = \"Enviando\";} else";
  page += "{document.getElementById(\"led2\").value = \"Jardim\";document.getElementById(\"led2_status\").innerHTML =\"Aguardando\";}";

  page += "}";
  page += "</script>";




  page += "</html>";

  return page;
}

//========fucao erro no carregamento===================
void handle_http_not_found() {
  web_server.send(404, "text/plain", "Not Found");
  digitalWrite(led_online, 0);
  digitalWrite(led_offline, 1);
  Serial.println("erro carregamento da pagina");
  handle_http_root();
}

//=======funcao envio de pagina ========================
void handle_http_root() {
  rssi = WiFi.RSSI();
  handleRoot();
  String message = getPage();
  web_server.send(200, "text/html", message);
  digitalWrite(led_online, 1);
  digitalWrite(led_offline, 0);
}

//==========funcao1 teste================================
void funcao1() {
  handleRoot();
  web_server.send(200, "text/plain", "Led1 Ligado");
  digitalWrite(led_online, 1);
  digitalWrite(led_offline, 0);
}


void handleRoot() {

  if ( web_server.hasArg("LED") ) {
    handleSubmit();
  } else {
    web_server.send ( 200, "text/html", getPage() );
  }

  if ( web_server.hasArg("LED2") ) {
    handleSubmit();
  } else {
    web_server.send ( 200, "text/html", getPage() );
  }


}

void handleSubmit() {
  // Actualise le GPIO / Update GPIO
  digitalWrite(led_online, 1);
  digitalWrite(led_offline, 1);
  String LEDValue;
  String LEDValue2;
  LEDValue = web_server.arg("LED");
  LEDValue2 = web_server.arg("LED2");
  Serial.print("Set GPIO14: ");
  Serial.println(LEDValue);
  Serial.print("Set GPIO15: ");
  Serial.println(LEDValue2);
  Serial.print("Sinal (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  digitalWrite(led_online, 0);
  digitalWrite(led_offline, 0);

  //===========lamp1=======================
  /*
    if ( LEDValue == "1" ) {
     digitalWrite(pinoD5, 1);
     est_lamp1 = "ON";
     lamp1 = 1;
     Serial.print("est_lamp1:ON");
    // web_server.send ( 200, "text/html", getPage() );
    } else if ( LEDValue == "0" ) {
     digitalWrite(pinoD5, 0);
     est_lamp1 = "OFF";
     lamp1 = 0;
     Serial.print("est_lamp1:OFF");
    // web_server.send ( 200, "text/html", getPage() );
    }
  */
  if ( LEDValue == "Garagem" || LEDValue == "Aguardar" ) {
    digitalWrite(led_online, 0);
    digitalWrite(led_offline, 1);
    flag_lamp1 = !flag_lamp1;
    digitalWrite(pinoD5, flag_lamp1);
    if (flag_lamp1) {
      // est_lamp1 = "ON";
      lamp1 = 1;
    } else if (!flag_lamp1) {
      // est_lamp1 = "OFF";
      lamp1 = 0;
    }
    web_server.send ( 200, "text/html", getPage() );
  }
  //=============lamp2==============================
  /*
    if ( LEDValue2 == "1" ) {
     digitalWrite(pinoD8, 1);
     est_lamp2 = "ON";
     lamp2 = 1;
     Serial.print("est_lamp2:ON");
    // web_server.send ( 200, "text/html", getPage() );
    } else if ( LEDValue2 == "0" ) {
     digitalWrite(pinoD8, 0);
     est_lamp2 = "OFF";
     lamp2 = 0;
     Serial.print("est_lamp2:OFF");
     //web_server.send ( 200, "text/html", getPage() );
    }
  */
  if ( LEDValue2 == "Jardim" || LEDValue2 == "Aguardar" ) {
    digitalWrite(led_online, 0);
    digitalWrite(led_offline, 1);
    flag_lamp2 = !flag_lamp2;
    digitalWrite(pinoD8, flag_lamp2);
    if (flag_lamp2) {
      // est_lamp2 = "aguardando";
      lamp2 = 1;
    } else if (!flag_lamp2) {
      // est_lamp2 = "aguardando";
      lamp2 = 0;
    }
    web_server.send ( 200, "text/html", getPage() );
  }

  digitalWrite(led_online, 1);
  digitalWrite(led_offline, 0);
  web_server.send ( 200, "text/html", getPage() );
}


void setup(void) {
  pinMode(pinoD5, OUTPUT);
  pinMode(pinoD8, OUTPUT);
  pinMode(led_online, OUTPUT);
  pinMode(led_offline, OUTPUT);

  Serial.begin(115200);
  config_rede();//chama funcao para configurar em modo AP ou STA

  // Start LLMNR responder
  LLMNR.begin("esp8266");
  Serial.println("LLMNR responder started");
  // Start HTTP server
  web_server.on("", handle_http_root);
  web_server.onNotFound(handle_http_not_found);
  web_server.on(" ", handle_http_root);
  web_server.on("/", handle_http_root);
  web_server.on("/1", funcao1);
  web_server.begin();
  Serial.println("HTTP server started");
  rssi = WiFi.RSSI();
  Serial.print("Sinal (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  digitalWrite(led_online, 1);
  digitalWrite(led_offline, 0);
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
  Serial.print("Hostname: ");
  Serial.println(WiFi.hostname());
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

}

void loop(void) {
  web_server.handleClient();

  delay(1000);
}
