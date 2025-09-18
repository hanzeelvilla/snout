# LoRa RYLR998
Long Range. Wireless radio frequency technology

## Pin Descriptions and Wiring
| **RYLR998 PIN** |  **FUNCTION** | **ESP32 PIN** |
|:---------------:|:-------------:|:-------------:|
|       VDD       |  Power Supply |      3.3V     |
|       GND       |     Ground    |      GND      |
|       TXD       | UART Transmit |    17 (TX)    |
|       RXD       |  UART Receive |    16 (RX)    |

## Received Message Format
When the RYLR998 module receives a message, it reports it through the UART port using the following format: 
`+RCV=<address>,<data_length>,<data>,<RSSI>,<SNR>`

| **POSITION** |                  **DESCRIPTION**                 |
|:------------:|:------------------------------------------------:|
| ADDRESS      | The 16-bit address of the transmitter (sender)   |
| DATA LENGTH  | The length of the received data in bytes.        |
| DATA         | The message data in ASCII format.                |
| RSSI         | The Received Signal Strength Indicator, in dBm.  |
| SNR          | The Signal-to-Noise Ratio of the message, in dB. |

## Documentation
- [REYAX](https://reyax.com/products/RYLR998)
- [AT COMMAND MANUAL](https://reyax.com//upload/products_download/download_file/LoRa_AT_Command_RYLR998_RYLR498_EN.pdf)
- [Cómo Usar Módulos LoRa con ESP32](https://proyectocorrecaminos.com/como-usar-modulos-lora-con-esp32/)
- [Cómo Usar Módulos LoRa RYLR998 con ESP32](https://www.youtube.com/watch?v=DOtZwD23ePQ)