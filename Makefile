.PHONY:all
all:udpserver udpclient

udpserver:Main.cc
	g++ -o $@ $^ -std=c++11 -std=c++14
udpclient:UdpClient.cc
	g++ -o $@ $^ -std=c++11
	
.PHONY:clean
clean:
	rm -f udpserver udpclient