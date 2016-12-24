






ch0 = (sbus_rx_buffer[0]| (sbus_rx_buffer[1] << 8)) & 0x07ff; //!< Channel 0
ch1 = ((sbus_rx_buffer[1] >> 3) | (sbus_rx_buffer[2] << 5)) & 0x07ff; //!< Channel 1
ch2 = ((sbus_rx_buffer[2] >> 6) | (sbus_rx_buffer[3] << 2) | (sbus_rx_buffer[4] << 10)) & 0x07ff;//!< Channel 2
ch3 = ((sbus_rx_buffer[4] >> 1) | (sbus_rx_buffer[5] << 7)) & 0x07ff; //!< Channel 3


s1 = ((sbus_rx_buffer[5] >> 4)& 0x000C) >> 2; //!< Switch left
s2 = ((sbus_rx_buffer[5] >> 4)& 0x0003); //!< Switch right
x = sbus_rx_buffer[6] | (sbus_rx_buffer[7] << 8); //!< Mouse X axis
y = sbus_rx_buffer[8] | (sbus_rx_buffer[9] << 8); //!< Mouse Y axis
z = sbus_rx_buffer[10] | (sbus_rx_buffer[11] << 8); //!< Mouse Z axis
press_l = sbus_rx_buffer[12]; //!< Mouse Left Is Press ?
press_r = sbus_rx_buffer[13]; //!< Mouse Right Is Press ?
v = sbus_rx_buffer[14] | (sbus_rx_buffer[15] << 8); //!< KeyBoard value