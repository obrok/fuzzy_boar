require "serialport"
require "timeout"
require "io/wait"

class FuzzyWindow
  def initialize(output, in_height, out_height, header = "")
    @output = output
    @in_height = in_height
    @out_height = out_height
    @input_lines = Array.new(@in_height, "")
    @output_lines = Array.new(@out_height, "")
    @header = header
  end

  def reputs(str = '')
    @output.puts "\e[0K" + str
  end

  def clear_screen!
    @output.print "\e[2J"
  end

  def move_to_home!
    @output.print "\e[H"
  end

  def flush!
    @output.flush
  end

  def tick(in_line, out_line)
    if in_line
      @input_lines.unshift(in_line)
      @input_lines.pop
    end
    if out_line
      @output_lines.unshift(out_line)
      @output_lines.pop
    end
    draw
  end

  def draw
    clear_screen!
    move_to_home!

    reputs @header
    reputs "=" * 80
    @input_lines.each do |line|
      reputs line
    end
    reputs "=" * 80
    @output_lines.each do |line|
      reputs line
    end
    reputs "=" * 80
    flush!
  end
end

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

  def ready?
    @serial_port.ready?
  end

  def get
    @serial_port.gets.chomp
  end

  def clear!
    Timeout.timeout(1) do
      while true
        com.get rescue nil
      end
    end
  rescue
    # noop
  end

  private
  def pack_message(msg)
    length = "fcom".size + 1 + msg.size + 1
    ["fcom", length, msg, 0].pack("A4CA#{msg.size}C")
  end
end

if __FILE__ == $0
  modems = Dir["/dev/ttyUSB*"]
  modem = modems[0]
  com = FuzzyCom.new(modem)
  com.clear!
  view = FuzzyWindow.new(STDOUT, 3, 40, "Connected to #{modem}")
  while true
    in_line = out_line = nil
    if STDIN.ready?
      in_line = STDIN.gets.chomp
      com.put(in_line)
    end
    out_line = com.get if com.ready?
    view.tick(in_line, out_line)
    sleep 0.03
  end
end

