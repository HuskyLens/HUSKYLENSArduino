# HUSKYLENS Protocol

### Version:

0.1

### Designer:

Angelo (Angelo.qiao@dfrobot.com)

### Init parameter:

* Serial Mode: 9600 (bps) 8N1
* Address: 0x11

### Communication Command Frame Format:

| Header | Header 2 | Address | Data Length | Command | Data           | Checksum |
| :----- | :------- | :------ | :---------- | :------ | :------------- | :------- |
| 0x55   | 0xAA     | 1 byte  | 1 byte      | 1 byte  | Data 1～Data n | 1 byte   |

| Hex  | Function                                                     |
| ---- | ------------------------------------------------------------ |
| 0x55 | Header                                                       |
| 0xAA | Header 2                                                     |
| 0x11 | Address                                                      |
| 0x0A | Data Length                                                  |
| 0x2A | Command is 0x2A                                              |
| 0x2C | Data[0]                                                      |
| 0x01 | Data[1]                                                      |
| 0xC8 | Data[2]                                                      |
| 0x00 | Data[3]                                                      |
| 0x0A | Data[4]                                                      |
| 0x00 | Data[5]                                                      |
| 0x14 | Data[6]                                                      |
| 0x00 | Data[7]                                                      |
| 0x01 | Data[8]                                                      |
| 0x00 | Data[9]                                                      |
| 0x58 | CheckSum, Sum all and only use low byte (Low Byte of 0x258 = 0x55 + 0xAA + 0x11 + 0x0A + 0x2A + 0x2C + 0x01 + 0xC8 + 0x00 + 0x0A + 0x00 + 0x14 + 0x00 + 0x01 +0x00) |

### General concept:

#### ID Meaning:

| ID   | Means                                                        |
| ---- | ------------------------------------------------------------ |
| 1    | The first learned item is detected                           |
| 2    | The second learned item is detected                          |
| XXX  | The XXXth learned item is detected                           |
| 0    | Item is detected but not learned, like unlearned faces block in grey color. |

### Protocol flow:

| hosts                         | HUSKYLENS                 |
| ----------------------------- | ------------------------- |
| COMMAND_REQUEST_KNOCK===>     |                           |
|                               | <===COMMAND_RETURN_OK     |
|                               |                           |
| COMMAND_REQUEST===>           |                           |
|                               | <=== COMMAND_RETURN_INFO  |
|                               | <=== COMMAND_RETURN_BLOCK |
|                               | <=== COMMAND_RETURN_BLOCK |
|                               | <=== COMMAND_RETURN_ARROW |
|                               |                           |
| COMMAND_REQUEST_BLOCKS====>   |                           |
|                               | <=== COMMAND_RETURN_INFO  |
|                               | <=== COMMAND_RETURN_BLOCK |
|                               | <=== COMMAND_RETURN_BLOCK |
|                               |                           |
| COMMAND_REQUEST_ARROWS====>   |                           |
|                               | <=== COMMAND_RETURN_INFO  |
|                               | <=== COMMAND_RETURN_ARROW |
|                               |                           |
| COMMAND_REQUEST_ALGORITHM===> |                           |
|                               | <===COMMAND_RETURN_OK     |



## Commands List:

### COMMAND_REQUEST (0x20):

Request all blocks and arrows from HUSKYLENS.

| Header | Header 2 | Address | Data Length | Command | Checksum |
| :----- | :------- | :------ | :---------- | :------ | :------- |
| 0x55   | 0xAA     | 0x11    | 0x00        | 0x20    | 0x30     |

### COMMAND_REQUEST_BLOCKS (0x21):

Request all blocks from HUSKYLENS.

| Header | Header 2 | Address | Data Length | Command | Checksum |
| :----- | :------- | :------ | :---------- | :------ | :------- |
| 0x55   | 0xAA     | 0x11    | 0x00        | 0x21    | 0x31     |

### COMMAND_REQUEST_ARROWS (0x22):

Request all arrows from HUSKYLENS.

| Header | Header 2 | Address | Data Length | Command | Checksum |
| :----- | :------- | :------ | :---------- | :------ | :------- |
| 0x55   | 0xAA     | 0x11    | 0x00        | 0x22    | 0x32     |

### COMMAND_REQUEST_LEARNED (0x23):

Request all learned blocks and arrows (ID >=1) from HUSKYLENS.

| Header | Header 2 | Address | Data Length | Command | Checksum |
| :----- | :------- | :------ | :---------- | :------ | :------- |
| 0x55   | 0xAA     | 0x11    | 0x00        | 0x23    | 0x33     |

### COMMAND_REQUEST_BLOCKS_LEARNED (0x24):

Request all learned blocks (ID >=1) from HUSKYLENS.

| Header | Header 2 | Address | Data Length | Command | Checksum |
| :----- | :------- | :------ | :---------- | :------ | :------- |
| 0x55   | 0xAA     | 0x11    | 0x00        | 0x24    | 0x34     |

### COMMAND_REQUEST_ARROWS_LEARNED (0x25):

Request all learned arrows (ID >=1) from HUSKYLENS.

| Header | Header 2 | Address | Data Length | Command | Checksum |
| :----- | :------- | :------ | :---------- | :------ | :------- |
| 0x55   | 0xAA     | 0x11    | 0x00        | 0x25    | 0x35     |

### COMMAND_REQUEST_BY_ID (0x26):

Request all blocks and arrows by given ID (Here is 0x01) from HUSKYLENS.

| Header | Header 2 | Address | Data Length | Command | Data      | Checksum |
| :----- | :------- | :------ | :---------- | :------ | --------- | :------- |
| 0x55   | 0xAA     | 0x11    | 0x02        | 0x26    | 0x01 0x00 | 0x39     |

Data details:

| Data | Function                                                     |
| ---- | ------------------------------------------------------------ |
| 0x01 | Given ID, see **ID Details** above (Low Byte of 1 = 0x0001)  |
| 0x00 | Given ID, see **ID Details** above (High Byte of 1 = 0x0001) |

### COMMAND_REQUEST_BLOCKS_BY_ID (0x27):

Request all blocks by given ID (Here is 0x01) from HUSKYLENS.

| Header | Header 2 | Address | Data Length | Command | Data      | Checksum |
| :----- | :------- | :------ | :---------- | :------ | --------- | :------- |
| 0x55   | 0xAA     | 0x11    | 0x02        | 0x27    | 0x01 0x00 | 0x3A     |

Data details:

| Data | Function                                                     |
| ---- | ------------------------------------------------------------ |
| 0x01 | Given ID, see **ID Details** above (Low Byte of 1 = 0x0001)  |
| 0x00 | Given ID, see **ID Details** above (High Byte of 1 = 0x0001) |

### COMMAND_REQUEST_ARROWS_BY_ID (0x28):

Request all arrows by given ID (Here is 0x01) from HUSKYLENS.

| Header | Header 2 | Address | Data Length | Command | Data      | Checksum |
| :----- | :------- | :------ | :---------- | :------ | --------- | :------- |
| 0x55   | 0xAA     | 0x11    | 0x02        | 0x28    | 0x01 0x00 | 0x3B     |

 Data details:

| Data | Function                                                     |
| ---- | ------------------------------------------------------------ |
| 0x01 | Given ID, see **ID Details** above (Low Byte of 1 = 0x0001)  |
| 0x00 | Given ID, see **ID Details** above (High Byte of 1 = 0x0001) |

### COMMAND_RETURN_INFO (0x29):

When HUSKYLENS Receives the command above, HUSKYLENS will return this info first and then return the arrows and blocks.

| Header | Header 2 | Address | Data Length | Command | Data                                              | Checksum |
| :----- | :------- | :------ | :---------- | :------ | ------------------------------------------------- | :------- |
| 0x55   | 0xAA     | 0x11    | 0x0A        | 0x29    | 0x01 0x00 0x01 0x00 0x05 0x00 0x00 0x00 0x00 0x00 | 0x4A     |

Data details:

| Data | Function                                                     |
| ---- | ------------------------------------------------------------ |
| 0x01 | numbers of blocks and arrows from HUSKYLENS (Low Byte of 1 = 0x0001) |
| 0x00 | numbers of blocks and arrows from HUSKYLENS (High Byte of 1 = 0x0001) |
| 0x01 | numbers of IDs HUSKYLENS have learned (Low Byte of 1 = 0x0001) |
| 0x00 | numbers of IDs HUSKYLENS have learned (High Byte of 1 = 0x0001) |
| 0x05 | current frame number (Low Byte of 5 = 0x0005)                |
| 0x00 | current frame number (High Byte of 5 = 0x0005)               |
| 0x00 | reserved                                                     |
| 0x00 | reserved                                                     |
| 0x00 | reserved                                                     |
| 0x00 | reserved                                                     |

### COMMAND_RETURN_BLOCK(0x2A):

After HUSKYLENS returns info, HUSKYLENS will return the blocks like this:

| Header | Header 2 | Address | Data Length | Command | Data                                              | Checksum |
| :----- | :------- | :------ | :---------- | :------ | ------------------------------------------------- | :------- |
| 0x55   | 0xAA     | 0x11    | 0x0A        | 0x2A    | 0x2C 0x01 0xC8 0x00 0x0A 0x00 0x14 0x00 0x01 0x00 | 0x58     |

Data details:

| Hex  | Function                                                   |
| ---- | ---------------------------------------------------------- |
| 2C   | X Center of Block (Low Byte of 300 = 0x012C, range:0-319)  |
| 01   | X Center of Block (High Byte of 300 = 0x012C, range:0-319) |
| C8   | Y Center of Block (Low Byte of 200 = 0x00C8, range:0-239)  |
| 00   | Y Center of Block (High Byte of 200 = 0x00C8, range:0-239) |
| 0A   | Width of Block (Low Byte of 10 = 0x000A, range:0-319)      |
| 00   | Width of Block (High Byte of 10 = 0x000A, range:0-319)     |
| 14   | Height of Block (Low Byte of 20 = 0x0014, range:0-239)     |
| 00   | Height of Block (High Byte of 20 = 0x0014, range:0-239)    |
| 01   | ID, see **ID Meaning** above (Low Byte of 1 = 0x0001)      |
| 00   | ID, see **ID Meaning** above (High Byte of 1 = 0x0001)     |

### COMMAND_RETURN_ARROW(0x2B):

After HUSKYLENS returns info, HUSKYLENS will return the arrows like this:

| Header | Header 2 | Address | Data Length | Command | Data                                              | Checksum |
| :----- | :------- | :------ | :---------- | :------ | ------------------------------------------------- | :------- |
| 0x55   | 0xAA     | 0x11    | 0x0A        | 0x2B    | 0x2C 0x01 0xC8 0x00 0x0A 0x00 0x14 0x00 0x01 0x00 | 0x5A     |

Data details:

| Data | Function                                                   |
| ---- | ---------------------------------------------------------- |
| 2C   | X Origin of Arrow (Low Byte of 300 = 0x012C, range:0-319)  |
| 01   | X Origin of Arrow (High Byte of 300 = 0x012C, range:0-319) |
| C8   | Y Origin of Arrow (Low Byte of 200 = 0x00C8, range:0-239)  |
| 00   | Y Origin of Arrow (High Byte of 200 = 0x00C8, range:0-239) |
| 0A   | X Target of Arrow (Low Byte of 10 = 0x000A, range:0-319)   |
| 00   | X Target of Arrow (High Byte of 10 = 0x000A, range:0-319)  |
| 14   | Y Target of Arrow (Low Byte of 20 = 0x0014, range:0-239)   |
| 00   | Y Target of Arrow (High Byte of 20 = 0x0014, range:0-239)  |
| 01   | ID, see **ID Meaning** above (Low Byte of 1 = 0x0001)      |
| 00   | ID, see **ID Meaning** below (High Byte of 1 = 0x0001)     |

### COMMAND_REQUEST_KNOCK(0x2C):

Used for test connection with HUSKYLENS. When HUSKYLENS received this command, HUSKYLENS will return COMMAND_RETURN_OK.

| Header | Header 2 | Address | Data Length | Command | Checksum |
| :----- | :------- | :------ | :---------- | :------ | :------- |
| 0x55   | 0xAA     | 0x11    | 0x00        | 0x2C    | 0x3C     |

### COMMAND_REQUEST_ALGORITHM(0x2D):

When HUSKYLENS receives this command, HUSKYLENS will change the algorithm by the Data. And will return COMMAND_RETURN_OK.

| Header | Header 2 | Address | Data Length | Command | Data      | Checksum |
| :----- | :------- | :------ | :---------- | :------ | --------- | :------- |
| 0x55   | 0xAA     | 0x11    | 0x02        | 0x2D    | 0x01 0x00 | 0x40     |

 Data details:

| Data | Function                                                     |
| ---- | ------------------------------------------------------------ |
| 0x01 | Change to  ALGORITHM_OBJECT_TRACKING (Low Byte of 1 = 0x0001) |
| 0x00 | Change to  ALGORITHM_OBJECT_TRACKING (High Byte of 1 = 0x0001) |

Data's correspondent algorithm:

| Data      | Algorithm                       |
| --------- | ------------------------------- |
| 0x00 0x00 | ALGORITHM_FACE_RECOGNITION      |
| 0x01 0x00 | ALGORITHM_OBJECT_TRACKING       |
| 0x02 0x00 | ALGORITHM_OBJECT_RECOGNITION    |
| 0x03 0x00 | ALGORITHM_LINE_TRACKING         |
| 0x04 0x00 | ALGORITHM_COLOR_RECOGNITION     |
| 0x05 0x00 | ALGORITHM_TAG_RECOGNITION       |
| 0x06 0x00 | ALGORITHM_OBJECT_CLASSIFICATION |

### COMMAND_RETURN_OK(0x2E):

HUSKYLENS will return OK, if HUSKYLENS receives COMMAND_REQUEST_ALGORITHM, COMMAND_REQUEST_KNOCK.

| Header | Header 2 | Address | Data Length | Command | Checksum |
| :----- | :------- | :------ | :---------- | :------ | :------- |
| 0x55   | 0xAA     | 0x11    | 0x00        | 0x2E    | 0x3E     |

### 





