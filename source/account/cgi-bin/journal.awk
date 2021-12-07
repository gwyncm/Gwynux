# print journal report
# input:$1=N $2=type $3=date $4=who $5=fromacct $6=amount $7=gst $8=pst $9=toacct
BEGIN 	{ header=0; } 
header < 1	{ header=1
printf("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2//EN\">\n");
printf("<HTML><PLAINTEXT>\n");
	printf "                      General Journal\n" 
printf "T Date   Tran To       Amount From    GST    PST Company\n" 
	}
	{ ca=substr($5,1,4); cb=substr($9,1,4);  
	printf "%s %6s %4d %s %10.2f %s %6.2f %6.2f %s\n",
	$2,$3,$1,ca,$6,cb,$7,$8,$4 }
