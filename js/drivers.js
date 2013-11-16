if ((typeof process_token != "undefined") &&
        (typeof process_error != "undefined")) {
    driver = driverFactory.createInstance("TimeInfo");
    driver.setPortName("SLAB_USBtoUART");
    driver.setReaderId("0001");
    driver.tokenRead.connect(process_token);
    driver.error.connect(process_error);
    driver.open();
}
