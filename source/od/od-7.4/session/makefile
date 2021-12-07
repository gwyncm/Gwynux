FLAGS =  -DUNIX -I ../image

BASEOBJS = \
	sharemgr.o scanner.o socket.o remote.o sessmgr.o session.o

all: libods.so st
$(BASEOBJS): %.o: %.cc %.hpp
	g++ -fPIC -c $(FLAGS) -g $< 
libods.so : $(BASEOBJS)
	g++ -shared -o libods.so $(BASEOBJS)
st: st.o
	g++ -g -L. -ldl -lm -lodi -lodo -lods -rdynamic -o st st.o
clean:
	rm *.o *.so st 
install: libods.so st 
	cp libods.so /usr/lib
