# process accounting record patterns
#  input: $1=date $2=who $3=DC $4=ac $5=amt, $6=DC $7=ac $8, $9=DC $10=ac $11 
#  output: $1=type $2=date $3=who $4=fromacct $5=amount $6=gst $7=pst $8=toacct 
	{ accepted = 0 }
# regular expense no GST
NF == 9  &&  $3 == "D"  &&  $6 == "C"  {
	accepted = 1 ; print "E:" $1 ":" $2 ":" $4 ":" $5 ":" "0"  ":0:" $7 }
# regular invoice without GST or PST
NF == 9  &&  $3 == "C"  &&  $6 == "D"  {
	accepted = 1 ; print "I:" $1 ":" $2 ":" $4 ":" $5 ":" "0"  ":0:" $7 }
# regular expense with GST
NF == 12  &&  $3 == "D"  &&  $6 == "C"  &&  $7 == "1100"  &&  $9 == "C"  {
	accepted = 1 ; print "E:" $1 ":" $2 ":" $4 ":" $5 ":" $8 ":0:" $10 }
# regular invoice with GST
NF == 12  &&  $3 == "C"  &&  $6 == "D"  &&  $9 == "D"  &&  $7 == "2100"  {
	accepted = 1 ; print "I:" $1 ":" $2 ":" $4 ":" $5 ":" $8 ":0:" $10 }
# regular invoice with GST
NF == 12  &&  $3 == "C"  &&  $6 == "D"  &&  $9 == "D"  &&  $10 == "2100"  {
	accepted = 1 ; print "I:" $1 ":" $2 ":" $4 ":" $5 ":" $11 ":0:" $7 }
# regular invoice with GST and PST
NF == 15  &&  $3 == "C"  &&  $6 == "D"  &&  $9 == "D"  &&  $10 ==  "2100"  &&  $12 == "D"  &&  $13 == "2200" {
	accepted = 1 ; print "I:" $1 ":" $2 ":" $4 ":" $5 ":" $11 ":" $14 ":" $7 }
accepted == 0 { print "*** Error ***", NF ,$1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14 }
