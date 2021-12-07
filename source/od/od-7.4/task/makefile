FLAGS =  -DUNIX  -I../htmlgen -I../ui -I../db -I../acct -I../image -I../session  

BASEOBJS = \
	taskmgr.o task.o

INCOBJS = \
	taskmgr.cd 
all: libodt.so exbase.pcp tt
$(INCOBJS): %.cd: %.hpp
	pc $< 
$(BASEOBJS): %.o: %.cc %.hpp
	g++ -fPIC -c $(FLAGS) -g $< 
libodt.so : $(BASEOBJS)
	g++ -shared -o libodt.so $(BASEOBJS)
tt: tt.o taskmgr.o task.o
	g++ -g -L. -ldl -lm -lodi -lodo -lods -lhts4 -rdynamic -o tt tt.o taskmgr.o \
	task.o ../comp/basemgr.o
exbase.pcp: $(INCOBJS)
	cat *.cd > exbase.pcp
clean:
	rm *.o *.so exbase.pcp 2>/dev/null
install: libodt.so exbase.pcp 
	cp libodt.so /usr/lib
