require 'json'
require 'socket'

class ACMQ
  def initialize(host, port)
    @host = host
    @port = port
  end

  def push (body)
    str = JSON.dump [body]
    str = "PUSH " + str
    return_value = transact(str)
    if return_value != "ACK\n"
      raise "Pushing failed" + return_value
    end
  end

  def pop
    str = transact("POP\n")
    JSON.parse(str)[0]
  end

  def transact (str)
    s = TCPSocket.new @host, @port
    s.puts(str)
    s.gets
  end
end
