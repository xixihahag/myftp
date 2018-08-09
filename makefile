all: server client
.PHONY: all
objectser = myftpserver.o error.o network.o servftp.o
objectcli = myclient.o cliftp.o error.o network.o
server:$(objectser)
	gcc -o server $(objectser)
client:$(objectcli)
	gcc -o client $(objectcli)

error.o: error.h
network.o: network.h
myclient.o myftpserver.o: network.h ftp.h error.h
servftp.o cliftp.o: error.h network.h ftp.h
.PHONY : clean
clean:
	rm -rf *.o server client
