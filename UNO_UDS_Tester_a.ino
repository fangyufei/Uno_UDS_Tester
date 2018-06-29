//Pika, 28.06.2018: inital erstellt, funktioniert. 
//Für 29Bit IDs muss in der iso-tp.cpp (lib) bei sendMsgBuf(xx, 1, yy, zz) 
//die 0 durch eine 1 für ext frames ersetzt werden. (Zeile 35)

#include <mcp_can.h>
#include <mcp_can_dfs.h>
#include <SPI.h>
#include <iso-tp.h>
#include <uds.h>

#define MCP_INT 2

MCP_CAN CAN0(9);
IsoTp isotp(&CAN0, MCP_INT);
UDS uds(&isotp);

struct Session_t session;

void setup()
{
  Serial.begin(9600);
  pinMode(MCP_INT, INPUT);
  CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ);
  CAN0.setMode(MCP_NORMAL);
  delay(5000);
}

void loop()
{
  //uint8_t ds[] = {0x01};
  uint8_t ds[] = {0x02, 0x17}; //LT Pump Speed
  struct Session_t diag;
  uint16_t retval = 0;

  Serial.println(F("Diag Session"));
  //iso_tp lib kann nur std frames (keine ext frames mit 29bit IDs)
  diag.tx_id = 0x18DA4EF1; //Tester=0xF1          //0x7E0;
  diag.rx_id = 0x80000000 | 0x18DAF14E; //CCU=0x4E     //0x7E8;
  //diag.sid = UDS_SID_DIAGNOSTIC_CONTROL;  //UDS_SID_TESTER_PRESENT;  //UDS_SID_DIAGNOSTIC_CONTROL;
  diag.sid = UDS_SID_READ_DATA_BY_ID; //0x22 PumpeLT: 0x 22 02 17
  diag.Data = ds;
  diag.len = 2;
  if (retval = uds.Session(&diag))
  {
    Serial.print(F("UDS Session Error "));
    Serial.print(retval); Serial.print(F(" NRC "));
    Serial.println(retval, HEX);
  }
  else
  {
    Serial.println(F("Established with "));
    uds.print_buffer(diag.Data, diag.len);
  }
  delay(1000);
}
