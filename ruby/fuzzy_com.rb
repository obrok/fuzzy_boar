require "serialport"

modems = Dir["/dev/tty.usb*"]
port_str = modems[0]
baud_rate = 115200
data_bits = 8
stop_bits = 1
parity = SerialPort::NONE

def message(msg)
  length = "fcom".size + 1 + msg.size + 1
  ["fcom", length, msg, 0].pack("A4CA#{msg.size}C")
end

SerialPort.open(port_str, baud_rate, data_bits, stop_bits, parity) do |sp|
  while true do
    print "Command: "
    sp.write message(gets.chomp)
    print "Output: "
    puts sp.gets
  end
end
