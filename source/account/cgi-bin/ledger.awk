# print ledger report for account "acct"
# input: $1=acct $2=num $3=date $4=debit $5=credit $6=who
BEGIN 	{ header=0; total=0; ctotal=0; dtotal=0 } 
header < 1	{ header=1
printf("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2//EN\">\n");
printf("<HTML><PLAINTEXT>\n");
	printf "                  Ledger for account %s\n",acct 
	printf "Date         Tran      Debit     Credit    Balance Company\n" 
	}
acct==$1 { total-=$5; total+=$4; dtotal+=$4; ctotal+=$5
	printf "%6s %10d %10.2f %10.2f %10.2f %s\n", $3,$2,$4,$5,total,$6 }
END	{
	printf "\nTotals:%10s %10.2f %10.2f\n","",dtotal,ctotal }
