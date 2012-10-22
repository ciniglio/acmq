import json
from socket import socket, AF_INET, SOCK_STREAM


class ACMQ():
	def __init__(host,port):
		self.host = host
		self.port = port

	def push(body):
		string = json.dumps(body)
		string = "PUSH %s" % string
		return_value = transact(string)
		if return_value != "ACK\n":
			raise "Pushing failed %s" % return_value
		return return_value

	def pop():
		string = transact("POP\n")
		return json.loads(string)[0]

	def transact(string):
		s = socket(AF_INET, SOCK_STREAM)
		s.connect((self.host, self.port))
		s.send(string)
		data = s.recv(1024)