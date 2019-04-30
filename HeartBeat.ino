/*
 * Setup function to initialize the heartbeat LED
 * 
 * @param none
 * @return void
 */
void setupHeartBeat() {
  pinMode(78, OUTPUT);
}

/*
 * Function run periodically to toggle the heartbeat LED
 * 
 * @param none
 * @return void
 */
void loopHeartBeat() {
    pinToggle(78);
    delay(1000);
}
