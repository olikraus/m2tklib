CFLAGS = -g -Wall
#CFLAGS = -O4 -Wall

SRC = $(shell ls *.c)

OBJ = $(SRC:.c=.o)

m2tk_v2: $(OBJ) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o m2tk_v2

clean:	
	-rm $(OBJ) m2tk_v2

zip:
	zip m2tk_v2_`date +%Y%m%d_%H%M` $(SRC) *.txt 

test:	m2tk_v2
	./m2tk_v2