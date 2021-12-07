FLAGS =  -DUNIX  -I../image -I../object -I ../session

BASEOBJS = \
	htmldoc.o

INCOBJS = \
	htmldoc.cd

all: libhts4.so htbase.pcp
$(INCOBJS): %.cd: %.hpp
	pc $< 
$(BASEOBJS): %.o: %.cc %.hpp
	g++ -fPIC -c $(FLAGS) -g $< 
libhts4.so : $(BASEOBJS) 
	g++ -shared -o libhts4.so $(BASEOBJS) 
htbase.pcp: $(INCOBJS)
	cat *.cd > htbase.pcp
clean:
	rm *.o *.so htbase.pcp *.cd
install: libhts4.so htbase.pcp
	cp libhts4.so /usr/lib
