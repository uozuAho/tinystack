todo
====
- Make a network layer protocol
 - Sits on COBS, packets separated by 0x8000
   - see COBS paper for reason for 0x8000 separator
 - 8 or 16 bit length at start of packet
 - 8 or 16 bit CRC at start/end of packet
 - That's it
- Make an app-layer serial bus protocol
