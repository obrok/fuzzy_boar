require "serialport"
require "timeout"

class FuzzyCom
  def initialize(port)
    baud_rate = 115200
    data_bits = 8
    stop_bits = 1
    parity = SerialPort::NONE
    @serial_port = SerialPort.new(port, baud_rate, data_bits, stop_bits, parity)
  end

  def put(msg)
    packed = pack_message(msg)
    @serial_port.write(packed)
  end

  def get
    @serial_port.gets.chomp
  end

  private
  def pack_message(msg)
    length = "fcom".size + 1 + msg.size + 1
    ["fcom", length, msg, 0].pack("A4CA#{msg.size}C")
  end
end

if __FILE__ == $0
  modems = Dir["/dev/tty.usb*"]
  modem = modems[0]
  com = FuzzyCom.new(modem)
  while true do
    print "< "
    command = gets.to_s.chomp.split(" ")
    case command[0]
    when "quit"
      exit(0)
    when "log"
      com.put(command.join(" "))
      while true
        msg = com.get
        break if msg == "endlog"
        puts "> #{msg}"
      end
    else
      com.put(command.join(" "))
      msg = com.get
      puts "> #{msg}"
    end
  end
end
