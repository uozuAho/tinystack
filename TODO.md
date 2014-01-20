todo
====
- Write tests for cobs_decode()
 - can it correctly decode if src & dest buffers are the same?
 - this would be handy as it would negate the need for separate
   receive/decoded buffers.
- Make a network layer protocol
 - Sits on COBS, packets separated by 0x00 at start and end of packet
  - Before and after good since it will cause the receiver to discard any
    stray bytes received before a packet, or a packet with a corrupt
    length
 - 8 or 16 bit length at start of packet
 - 8 or 16 bit CRC at start/end of packet
 - That's it
- Make an app-layer serial bus protocol
