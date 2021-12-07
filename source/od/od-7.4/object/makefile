FLAGS =  -DUNIX -I ../image

BASEOBJS = \
	string.o defclass.o attrib.o objmgr.o \
	symtab.o symbol.o block.o

all: libodo.so ot
$(BASEOBJS): %.o: %.cc %.hpp
	g++ -fPIC -c $(FLAGS) -g $< 
libodo.so : $(BASEOBJS)
	g++ -shared -o libodo.so $(BASEOBJS)
ot: ot.o
	g++ -g -L. -ldl -lm -lodi -lodo -rdynamic -o ot ot.o
clean:
	rm *.o *.so ot
install: libodo.so ot 
	cp libodo.so /usr/lib
