	{ 
	KMRATE = 0.40 
	gst = $1 
	instructor = $2
	course = $3
	location = $4
	date = $5
	airfare = $6
	kms = $7
	car = $8
	taxipark = $9 / 1.14 * 1.08
	hotel = $10
	tripdays = $11
	perdiem = $12
	other = $13
	rate = $14
	on = $15
	teachdays = $16
	teachrate = $17
	setup = $18
	}
END 	{ 
printf("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2//EN\">\n");
printf("<HTML>");
printf("<BODY BGCOLOR=\"#FFFFFF\" TEXT=\"#000000\" LINK=\"#0000FF\" VLINK=\"#000099\">\n");
printf("<IMG src=\"http://www.toadware.ca/letterhead.jpg\" alt=\"banner\" border=\"0\"><BR>\n");
printf("<span style=\"width: 60px\"></span>");
printf("<plaintext>\n");
printf("    GST Registration: %-10s          INVOICE\n",gst);
printf("    Instructor: %-25s Course: %s\n",instructor,course);
printf("    Location: %-25s   Date: %s\n\n",location,date);
printf("                      EXPENSES (Receipts must be submitted)\n\n");
printf("    Transportation:   Airfare                             ");
printf("%8.2f\n",airfare);
printf("                      Mileage      %4d kms @ %8.2f/km ",kms,KMRATE);
printf("%8.2f\n",KMRATE*kms);
printf("                      Car Rental                          ");
printf("%8.2f\n",car);
printf("                      Taxi/Parking                        ");
printf("%8.2f\n\n",taxipark);
printf("    Hotel:            (Room and tax only)                 ");
printf("%8.2f\n\n",hotel);
printf("  Travel Fee:               %2d Days @     $%6.2f",tripdays,perdiem);
printf("         %8.2f\n\n",tripdays*perdiem);
printf("    Other:            (please specify)                    ");
printf("%8.2f\n\n\n",other);
printf("    Exchange Rate:      %6.4f         On:   $%6.2f",rate,on);
printf("      %8.2f\n\n\n",rate*on);
printf("                                                 Subtotal ");
printf("%8.2f\n\n",airfare+(KMRATE*kms)+car+taxipark+hotel+(tripdays*perdiem)+other+(rate*on));
printf("                      FEES\n\n");
printf("    Teaching Fees:    Base Rate   %2.0f days @ $%8.2f",teachdays,teachrate);
printf("/day %8.2f\n\n",teachdays*teachrate);
printf("                      Setup Fee                           ");
printf("%8.2f (Hours)\n\n\n",setup);
printf("    Incentive:        GPAS:  Course:     Instructor:\n");
printf("                      Uplift:       x            %%       _________");
printf("\n\n");
printf("    Customization:    As per Customization Agreement     _________");
printf("\n\n");
printf("                                                Subtotal _________");
printf("\n\n");
printf("    GST:              Total Fees:               @ 6%%     _________");
printf("\n\n");
printf("                                                TOTAL    _________");
printf("\n\n");
printf("    Approval:________     Date:________      Seq #: ______________\n\n");
printf("    A/P Vendor # ______ Assigned Invoice # _______ Month: ________\n");
printf("    Account #(____)____ Amount ____ Account #(____)____ Amount____\n");
printf("    Account #(____)____ Amount ____ Account #(____)____ Amount____\n");
	}
