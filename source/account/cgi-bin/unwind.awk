# unwind compound input records to standard entries
#  input: $1=N $2=type $3=date $4=who $5=fromac $6=amount $7=gst $8=pst $9=toac 
# output: $1=acct $2=num $3=date $4=debit $5=credit $6=who
	{ ca=substr($5,1,4); cb=substr($9,1,4); GST=$7; PST=$8 }
$2=="I"	&& $7=="" && $8=="" { # Calculate GST and PST
	GST=($6/1.14)*.06 ; PST=($6/1.14)*.08}
$2=="I"	&& $7=="" && $8!="" { # Calculate GST Only
	PST=0 ; GST=($6/1.06)*.06 }
$2=="I"	&& $7!="" && $8=="" { # Calculate PST Only
	GST=0 ; PST=($6/1.08)*.08 }
$2=="I" { # credit a debit b 
	printf "%s:%d:%s:%s:%s:%s\n",ca,$1,$3,$6,0,$4 
	printf "%s:%d:%s:%s:%g:%s\n",cb,$1,$3,0,$6-GST-PST,$4 } 
$2=="I" && GST > 0 { # GST Collected 
	printf "%s:%d:%s:%s:%g:%s\n","2100",$1,$3,0,GST,$4 }
$2=="I" && PST > 0 { # PST Collected 
	printf "%s:%d:%s:%s:%g:%s\n","2200",$1,$3,0,PST,$4 }
$2=="E" && $7==""  && $8==0 { # Calculate GST with no PST
	GST=($6/1.06)*.06 }
$2=="E" && $7=="" && $8=="" { # Calculate GST with PST included 
	GST=($6/1.14)*.06 }
$2=="E" { # credit b debit a
	printf "%s:%d:%s:%s:%s:%s\n",ca,$1,$3,0,$6,$4  
	printf "%s:%d:%s:%g:%s:%s\n",cb,$1,$3,$6-GST,0,$4 }
$2=="E" && GST > 0 { # GST Paid 
	printf "%s:%d:%s:%g:%s:%s\n","1100",$1,$3,GST,0,$4 }
