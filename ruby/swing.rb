require "ffi"

class Engines < FFI::Struct
  layout :left, :float
  layout :right, :float
end

module Quadcopter
  extend FFI::Library
  ffi_lib("quadcopter")

  attach_function :calculate_engines, [:float], Engines.by_value
end

class Swing
  def initialize(angle)
    @angle = angle
  end

  def apply_power(engine1, engine2, for_time)
    @angle += 1
  end
end

swing = Swing.new(15)
loop do
  engine1, engine2 = calculate_engines(swing.angle)
  swing.apply_power(engine1, engine2, 0.01)
  sleep 0.01
  p swing.angle
end
