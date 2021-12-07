# Generate file selection form
BEGIN {
print "<html>"
print "<head><title>Toadware Technologies Inc. -Accounting System</title></head>"
print "<body>" 
print "<H1>Accounting System - Select Files </H1>"
print "Select accounting file and click submit."	 
print "<p>"
print "<hr>"
print "<form action=\"/cgi-bin/account/select\" method=\"post\">"
print "<pre>"
print "Accounting File: <select name=afile size=1>"
	}
	{
	if ($9 == "transact.log") 
		print "<option selected > " $11 
	else 
		print "<option> " $9 
	}
END	{
print "</select>"
print "</pre>"
print "<input type=submit value=\"Submit entry\">"
print "<input type=reset value=\"Clear entry\">"
print "</form>"
print "<hr>"
print "</body></html>"
	}
