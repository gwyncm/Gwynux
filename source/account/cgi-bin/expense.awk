	{ 
	MAX_ITEMS = 5 
	location = $1
	name = $2
	fromdate = $3
	todate = $4
	days = $5
	perdiem = $6
	description[0] = $7  ; amount[0] = $8
	description[1] = $9  ; amount[1] = $10
	description[2] = $11 ; amount[2] = $12
	description[3] = $13 ; amount[3] = $14
	description[4] = $15 ; amount[4] = $16
	}
END 	{ 
total=perdiem*days
printf("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2//EN\">\n");
printf("<HTML>");
printf("<plaintext>\n");
printf("                               Expense Statement\n\n");
printf("    ---------------------------------------------------------------\n");
printf("    TOADWARE TECHNOLOGIES INC.          For:     %s\n",name);
printf("    50 Ridgefield Cres.                 Locn:    %s\n",location);
printf("    Nepean , Ont.  K2H 6S4              Date:    %s\n\n",todate);
printf("    Statement for the period from:      %-10s    To   %s\n",fromdate,todate);
printf("    ---------------------------------------------------------------\n\n");
printf("                      DESCRIPTION                          AMOUNT\n\n");
printf("    Per Diem       %2d Days at     %6.2f                %10.2f\n",
	days,perdiem,total);
for( i=0; i<MAX_ITEMS; i++)
 	{
	if (amount[i])
		{
		printf("    %-20s                                %10.2f\n",
                	description[i],amount[i]);
        	total+=amount[i];
        	}      
	}                          
printf("\n    Total                                               %10.2f\n",total);
	}

