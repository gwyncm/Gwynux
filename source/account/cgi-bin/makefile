FLAGS =  -DUNIX  

APOBJS = \
	acctcgi.o

all: acctcgi
$(APOBJS): %.o: %.cc 
	gcc -fPIC -c $(FLAGS) -g $< 
acctcgi: acctcgi.o  
	gcc -g -o acctcgi $(APOBJS) 
clean:
	rm *.o acctcgi
install: acctcgi
	./makedirs
	cp contact /usr/local/apache/cgi-bin
	cp acctcgi /usr/local/apache/cgi-bin/account
	cp enter /usr/local/apache/cgi-bin/account
	cp update /usr/local/apache/cgi-bin/account
	cp doedit /usr/local/apache/cgi-bin/account
	cp doselect /usr/local/apache/cgi-bin/account
	cp ltinv /usr/local/apache/cgi-bin/account
	cp expense /usr/local/apache/cgi-bin/account
	cp invoice /usr/local/apache/cgi-bin/account
	cp ledgcgi /usr/local/apache/cgi-bin/account
	cp joucgi /usr/local/apache/cgi-bin/account
	cp gstrep /usr/local/apache/cgi-bin/account
	cp pstrep /usr/local/apache/cgi-bin/account
	cp increp /usr/local/apache/cgi-bin/account
	cp balrep /usr/local/apache/cgi-bin/account
	cp prebal /usr/local/apache/cgi-bin/account
	cp ledger /usr/local/apache/cgi-bin/account
	cp journal /usr/local/apache/cgi-bin/account
	cp unwind /usr/local/apache/cgi-bin/account
	cp newexp /usr/local/apache/cgi-bin/account
	cp newinv /usr/local/apache/cgi-bin/account
	cp select /usr/local/apache/cgi-bin/account
	cp select.awk /usr/local/apache/cgi-bin/account
	cp ltinv.awk /usr/local/apache/cgi-bin/account
	cp income.awk /usr/local/apache/cgi-bin/account
	cp prebal.awk /usr/local/apache/cgi-bin/account
	cp balance.awk /usr/local/apache/cgi-bin/account
	cp expense.awk /usr/local/apache/cgi-bin/account
	cp invoice.awk /usr/local/apache/cgi-bin/account
	cp ledger.awk /usr/local/apache/cgi-bin/account
	cp enter.awk /usr/local/apache/cgi-bin/account
	cp journal.awk /usr/local/apache/cgi-bin/account
	cp unwind.awk /usr/local/apache/cgi-bin/account
	cp ltinv.html /usr/local/apache/htdocs/account
	cp expense.html /usr/local/apache/htdocs/account
	cp invoice.html /usr/local/apache/htdocs/account
	cp master.html /usr/local/apache/htdocs/account
	cp contact.html /usr/local/apache/htdocs
