require "serialport"

port_str = "/dev/tty.usbmodemfa131"
baud_rate = 9600
data_bits = 8
stop_bits = 1
parity = SerialPort::NONE

SerialPort.open(port_str, baud_rate, data_bits, stop_bits, parity) do |sp|
  while true do
    n = gets.chomp.to_i
    sp.putc([n].pack("C"))
  end
end
