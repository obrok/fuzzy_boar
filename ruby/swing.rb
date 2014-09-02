require "ffi"

class Engines < FFI::Struct
  layout(
    :left, :float,
    :right, :float,
  )

  def left
    self[:left]
  end

  def right
    self[:right]
  end

  def inspect
    "#<Engines left=#{format(left)} right=#{format(right)}>"
  end

  private
  def format(float)
    "%.5f" % [float.round(6)]
  end
end

module Quadcopter
  extend FFI::Library
  ffi_lib("quadcopter")

  attach_function :calculate_engines, [:float], Engines.by_value
  attach_function :init, [], :void
end

class Swing
  attr_reader :angle

  def initialize(angle)
    @angle = angle
    @angular_velocity = 0.0
  end

  def apply_power(engines, for_time)
    @angle = @angle + @angular_velocity * for_time
    @angular_velocity = @angular_velocity + torque(engines) * for_time / angular_momentum
  end

  def torque(engines)
    torque_left(engines.left) + torque_right(engines.right)
  end

  def torque_left(input_signal)
    power_left(input_signal) * distance_left
  end

  def torque_right(input_signal)
    - power_right(input_signal) * distance_right
  end

  def power_left(input_signal)
    input_signal * 1
  end

  def power_right(input_signal)
    input_signal * 1
  end

  def distance_left
    distance
  end

  def distance_right
    distance
  end

  def distance
    1.0
  end

  def angular_momentum
    0.1
  end
end

Quadcopter.init
swing = Swing.new(5)
loop do
  engines = Quadcopter.calculate_engines(swing.angle)
  swing.apply_power(engines, 0.01)
  sleep 0.01
  puts "#{engines.inspect} => #{swing.angle.round(2).to_s.ljust(6, "0")}"
end
