FLAGS =  -DUNIX  -I../htmlgen -I../image -I../session  -I.

BASEOBJS = \
	pcgener.o pccomp.o hpcomp.o hpgener.o syslog.o \
	stcomp.o stgener.o trcomp.o trgener.o sqlcomp.o \
	file.o basemgr.o

INCOBJS = \
	basetab.cd syslog.cd baseclass.cd defclass.cd block.cd \
	scanner.cd sharemgr.cd string.cd 

all: pc osbase.pcp 
$(INCOBJS): %.cd: %.hpp
	./pc $< 
$(BASEOBJS): %.o: %.cc %.hpp
	g++ -fPIC -c $(FLAGS) -g $< 
pc: $(BASEOBJS) pc.o  
	g++ -g -L. -ldl -lm  -lhts4 -lodi -lodo -lods  \
	 -lodt -rdynamic -o pc $(BASEOBJS) pc.o
#	g++ -g -L/usr/X11R6/lib -L. -lX11 -ldl -lm -lacs4 -lhts4 -lodi -lodo -lods \
#	-luis4 -ldbs4 -lodt -rdynamic -o pc $(BASEOBJS) pc.o
osbase.pcp: $(INCOBJS)
	cat *.cd > osbase.pcp
basemgr.cc: basemgr.bt bt
	./bt basemgr.bt
bt: bt.o basetab.o file.o 
	g++ -g -L. -ldl -lodo -lods -lodi -lm -rdynamic -o bt basetab.o file.o bt.o 
clean:
	rm *.o *.cd osbase.pcp pc bt basemgr.cc 2>/dev/null
install: pc osbase.pcp 
	cp pc /usr/bin
