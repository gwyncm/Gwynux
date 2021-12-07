FLAGS =  -DUNIX

BASEOBJS = \
	imgmgr.o object.o segment.o oid.o image.o 

all: libodi.so it
$(BASEOBJS): %.o: %.cc %.hpp
	g++ -fPIC -c $(FLAGS) -g $< 
libodi.so : $(BASEOBJS)
	g++ -shared -o libodi.so $(BASEOBJS)
it: it.o
	g++ -g -L. -ldl -lm -lodi -rdynamic -o it it.o
clean:
	rm *.o *.so it  
install: libodi.so  it
	cp libodi.so /usr/lib
