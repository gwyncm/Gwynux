# convert cvs to accounting records
BEGIN		{ NUMBER = 1 }
		{ DATE = $1 ; DESC = $2 ; TYPE = "I" ; ACCT = "4000" }
$3 > 0 		{ TYPE = "E" ; AMT = $3 }
$4 > 0 		{ TYPE = "I" ; AMT = $4 }
$0 ~ "MASTER"	{ ACCT = "6450" }
$0 ~ "SERVICE"	{ ACCT = "6305" }
$0 ~ "MEMO"	{ ACCT = "6120" }
		{ printf "%d:%c:%s:%s:1000:%f:0:0:%s\n" , 
			NUMBER++ , TYPE , DATE , DESC , AMT , ACCT } 
