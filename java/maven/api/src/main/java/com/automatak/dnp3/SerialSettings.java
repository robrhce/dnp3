package com.automatak.dnp3;

/**
 * Settings class uses to configure a serial port
 */
public class SerialSettings {

    public final String port;
    public final int baudRate;
    public final int dataBits;
    public final int stopBits;
    public final Parity parity;
    public final FlowControl flowControl;

    /**
     * Constructs a SerialSettings class with a fully configurable set of options
     * @param port        i.e. COMX on windows or /dev/ttyX on Linux
     * @param baudRate    Baud rate for the port. Allowed values depend on the specific port
     * @param dataBits    Number of data bits. Typically 8.
     * @param parity      Parity, typically none
     * @param stopBits    Stop bits, typically 1
     * @param flowControl Flow control, typically none
     */
    public SerialSettings(String port, int baudRate, int dataBits, Parity parity, int stopBits, FlowControl flowControl)
    {
        this.port = port;
        this.baudRate = baudRate;
        this.dataBits = dataBits;
        this.stopBits = stopBits;
        this.parity = parity;
        this.flowControl = flowControl;
    }

    /**
     * Constructs the SerialSettings class with the typical 8/N/1 settings
     * @param port       i.e. COMX on windows or /dev/ttyX on Linux
     * @param baudRate   Baud rate for the port. Allowed values depend on the specific port
     */
    public SerialSettings(String port, int baudRate)
    {
        this(port, baudRate, 8, Parity.PAR_NONE, 1, FlowControl.FLOW_NONE);
    }



}