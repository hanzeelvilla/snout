# LoRa RYLR998

Long Range. Wireless radio frequency technology

## Pin Descriptions and Wiring

| **RYLR998 PIN** | **FUNCTION**  | **ESP32 PIN** |
|-----------------|---------------|---------------|
|       VDD       |  Power Supply |      3.3V     |
|       GND       |     Ground    |      GND      |
|       TXD       | UART Transmit | RX2 (GPIO 16) |
|       RXD       |  UART Receive | TX2 (GPIO 17) |

## LoRa Parameter Configuration (AT Commands)

The sequence of AT commands sent in the `setup()` function is crucial for defining the module's identity and network parameters

|    **COMMAND**   |       **FUNCTION**      | **EXPLANATION**                                                                                                                                                                                                                                              |
|:----------------:|:-----------------------:|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|  `AT+ADDRESS=1`  | Module Address (Sender) | Sets the unique 16-bit address for this transmitter module to `1`. The receiver must have its own distinct address, and this module will address messages to that receiver's address (in this case, `2`, as seen in the `loop()` function: `AT+SEND=2,...`). |
| `AT+NETWORKID=5` |     LoRa Network ID     | Establishes the LoRa Network ID as `5`. **Both the transmitter and the receiver modules must use the same** `NETWORKID` to be able to communicate and hear each other's transmissions.                                                                       |
|     `AT+BAND`    |      Read Frequency     | Requests the module to report its currently configured center frequency (e.g., 868000000 or 915000000 Hz). This command is used for **verification**.                                                                                                        |
|  `AT+PARAMETER`  |    Read RF Parameters   | Requests the module to report its Radio Frequency (RF) parameters (Spreading Factor, Bandwidth, and Coding Rate). This command is used for **verification**.                                                                                                 |
|     `AT+MODE`    |   Read Operation Mode   | Measure TemperatureRequests the module to report its current operating mode (usually the default, standard Transmit/Receive mode). This is for **verification**.                                                                                             |

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
