#Created by null, 2012

require 'socket'
require 'timeout'

class Networking
	def query_receive
		ret_value = nil
		begin
			if @query_socket
				results = select([@query_socket], nil, nil, 0.0)
				if results and results.length >= 1 and results[0].include?(@query_socket)
					data, receiver = @query_socket.recvfrom(1024)
					data_array = data.split("\0")
					ret_value = [data_array, receiver[3], receiver[1]]
				end
			end
		rescue
			ret_value = nil
			return ret_value
		end
		ret_value
	end
	
	def query_server(address, port)
		unless @query_socket
			@games = []
			begin
				@query_socket = UDPSocket.new
				@query_socket.bind('0.0.0.0', 0)
				@query_message = [0xFE, 0xFD, 0x00, 0x77, 0x6A, 0xBF, 0xBF, 0xFF, 0xFF, 0xFF, 0xFF].pack('c*')
			rescue
				@query_socket = nil
				return nil
			end
		end
		
		begin
			@query_socket.send(@query_message, 0, address, port)
		rescue
			return nil
		end
	end
end