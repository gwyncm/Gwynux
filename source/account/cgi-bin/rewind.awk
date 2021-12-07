# rewind compound input records to compound with GST/PST calculated
#  input: $1=N $2=type $3=date $4=who $5=fromac $6=amount $7=gst $8=pst $9=toac 
#  output: $1=N $2=type $3=date $4=who $5=fromac $6=amount $7=gst $8=pst $9=toac 
	{ ca=substr($5,1,4); cb=substr($9,1,4); GST=$7; PST=$8; NUM=1 }
$2=="I"	&& $7=="" && $8=="" { # Calculate GST and PST
	GST=($6/1.15)*.07 ; PST=($6/1.15)*.08}
$2=="I"	&& $7=="" && $8!="" { # Calculate GST Only
	PST=0 ; GST=($6/1.07)*.07 }
$2=="I"	&& $7!="" && $8=="" { # Calculate PST Only
	GST=0 ; PST=($6/1.08)*.08 }
$2=="I" { # reprint with GST/PST calculated
	printf "%s:%s:%s:%s:%s:%s:%s:%s:%s\n",NUM++,$2,$3,$4,$5,$6,GST,PST,$9  }
$2=="E" && $7==""  && $8==0 { # Calculate GST with no PST
	GST=($6/1.07)*.07 }
$2=="E" && $7=="" && $8=="" { # Calculate GST with PST included 
	GST=($6/1.15)*.07 }
$2=="E" { # reprint with GST/PST calculated
	printf "%s:%s:%s:%s:%s:%s:%s:%s:%s\n",NUM++,$2,$3,$4,$5,$6,GST,PST,$9  }
