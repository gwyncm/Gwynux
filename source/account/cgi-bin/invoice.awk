# Modified GST Aug 6 2006 GCM
	{ 
	MAX_ITEMS = 5 
	to[0] = $1
	date = $2
	to[1] = $3
	to[2] = $4
	to[3] = $5
	pst = $6
	gst = $7
	description[0] = $8  ; amount[0] = $9
	description[1] = $10 ; amount[1] = $11
	description[2] = $12 ; amount[2] = $13
	description[3] = $14 ; amount[3] = $15
	description[4] = $16 ; amount[4] = $17
	months["01"] = "Jan"
	months["02"] = "Feb"
	months["03"] = "Mar"
	months["04"] = "Apr"
	months["05"] = "May"
	months["06"] = "Jun"
	months["07"] = "Jul"
	months["08"] = "Aug"
	months["09"] = "Sep"
	months["10"] = "Oct"
	months["11"] = "Nov"
	months["12"] = "Dec"
	}
END 	{ 
total=0
for(i=0; i<MAX_ITEMS; i++) 
	{
       	total+=amount[i]
	}                          
if (pst == "") pst=total*0.08
if (gst == "") gst=total*0.06
gtotal=total+pst+gst;
printf("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2//EN\">\n");
printf("<HTML>");
printf("<BODY BGCOLOR=\"#FFFFFF\" TEXT=\"#000000\" LINK=\"#0000FF\" VLINK=\"#000099\">\n");
printf("<IMG src=\"http://www.toadware.ca/letterhead.jpg\" alt=\"banner\" border=\"0\"><BR>\n");
printf("<span style=\"width: 60px\"></span>");
printf("<plaintext>\n");
printf("                                Invoice %s\n\n",date);
printf("                                 %s %s, 20%s\n\n",
	months[substr(date,3,2)],substr(date,5,2),substr(date,1,2));
printf("    ---------------------------------------------------------------\n");
printf("    %s\n",to[0]);
printf("    %s\n",to[1]);
printf("    %s\n",to[2]);
printf("    %s\n",to[3]);
printf("    ---------------------------------------------------------------\n\n");
printf("                      DESCRIPTION                            AMOUNT\n\n");
for(i=0; i<MAX_ITEMS; i++)
	{
	if (amount[i])
		{
		printf("    %-40s             %10.2f\n",
                	description[i],amount[i]);
        	}      
	}                          
printf("\n    Subtotal                                             %10.2f\n",total);
printf("\n    PST                                                  %10.2f\n",pst);
printf("    GST                                                  %10.2f\n\n",gst);
printf("    Total                                                %10.2f\n",gtotal);
	}

