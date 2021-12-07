# print income statement report
# 1=C 2=L 3=C 4=R 5=E 6=E
# input: $1=acct $2=name $3=debit $4=credit 
BEGIN 	{ head=0; dtotal=0; ctotal=0 } 
head < 1{ head=1
	printf("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2//EN\">\n");
	printf("<HTML>");
	printf("<plaintext>\n")
	printf "             Income Statement Report\n" 
	printf "Acct                                  Debit     Credit\n" }
	{
	if ( substr($1,1,1) ~ "[456]" )
		{
		printf("%-8s%-30s%-10.2f%-10.2f\n",
		$1,$2,$3,$4)
		dtotal+=$3
		ctotal+=$4
		}
	}
END	{
	printf("\n%-8s%-20s%-10.2f%-10.2f%-10.2f\n","","",
	ctotal-dtotal,dtotal,ctotal)
	}
