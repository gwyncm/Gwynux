# Print gifi financial statements
# input: $1=acct $2=num $3=date $4=debit $5=credit $6=who
# 1=10-20 2=26-29 3=30-39 4=80-82 5=83-85 6=86-92
#
function prtotal ( head, pat, neg ) {
# Print total by category 
# Print header, match pattern, negate total
t1=0
printf "%s\n",head
for(i=0; i<length(head); i++) printf "-"
printf "\n"
for ( i in gtotal )
	{
	if ( i ~ pat )
		{
		printf "%s     %s %10.2f\n",i,gname[i],neg*gtotal[i]
		t1+=neg*gtotal[i]
		}
	}
printf "                                      ------------\n"
return(t1)
}
#
BEGIN 	{
# Added
gtrans["7500"] = "1901" ; gname["1901"] = "AA Other tangable capital as  "
gtrans["7510"] = "1741" ; gname["1741"] = "AA Machinery and Equipment    "
gtrans["7520"] = "1901" ; gname["1901"] = "AA Other tangable capital as  "
gtrans["7530"] = "1901" ; gname["1901"] = "AA Other tangable capital as  "
gtrans["7540"] = "1741" ; gname["1741"] = "AA Machinery and Equipment    "
gtrans["7550"] = "1741" ; gname["1741"] = "AA Machinery and Equipment    "
gtrans["7560"] = "1741" ; gname["1741"] = "AA Machinery and Equipment    "
gtrans["7600"] = "2011" ; gname["2011"] = "AA Intangable assets          "
gtrans["7610"] = "2011" ; gname["2011"] = "AA Intangable assets          "
# Added 
gtrans["8500"] = "1900" ; gname["1900"] = "Other tangable capital assets "
gtrans["8510"] = "1740" ; gname["1740"] = "Machinery and Equipment       "
gtrans["8520"] = "1900" ; gname["1900"] = "Other tangable capital assets "
gtrans["8530"] = "1900" ; gname["1900"] = "Other tangable capital assets "
gtrans["8540"] = "1740" ; gname["1740"] = "Machinery and Equipment       "
gtrans["8550"] = "1740" ; gname["1740"] = "Machinery and Equipment       "
gtrans["8560"] = "1740" ; gname["1740"] = "Machinery and Equipment       "
gtrans["8600"] = "2010" ; gname["2010"] = "Intangable assets             "
gtrans["8610"] = "2010" ; gname["2010"] = "Intangable assets             "
# Generated
gtrans["1000"] = "1000" ; gname["1000"] = "Cash and deposits             "
gtrans["1010"] = "1000" ; gname["1000"] = "Cash and deposits             "
gtrans["1020"] = "1480" ; gname["1480"] = "Other current assets          "
gtrans["1030"] = "1480" ; gname["1480"] = "Other current assets          "
gtrans["1040"] = "1120" ; gname["1120"] = "Inventories                   "
gtrans["1050"] = "1480" ; gname["1480"] = "Other current assets          "
gtrans["1100"] = "1480" ; gname["1480"] = "Other current assets          "
gtrans["1200"] = "1060" ; gname["1060"] = "Accounts receivable           "
gtrans["1300"] = "1480" ; gname["1480"] = "Other current assets          "
gtrans["1500"] = "1900" ; gname["1900"] = "Other tangable capital assets "
gtrans["1510"] = "1740" ; gname["1740"] = "Machinery and Equipment       "
gtrans["1520"] = "1900" ; gname["1900"] = "Other tangable capital assets "
gtrans["1530"] = "1900" ; gname["1900"] = "Other tangable capital assets "
gtrans["1540"] = "1740" ; gname["1740"] = "Machinery and Equipment       "
gtrans["1550"] = "1740" ; gname["1740"] = "Machinery and Equipment       "
gtrans["1560"] = "1740" ; gname["1740"] = "Machinery and Equipment       "
gtrans["1600"] = "2010" ; gname["2010"] = "Intangable assets             "
gtrans["1610"] = "2010" ; gname["2010"] = "Intangable assets             "
gtrans["2000"] = "2620" ; gname["2620"] = "Ammounts pabl and accrued liab"
gtrans["2100"] = "2620" ; gname["2620"] = "Ammounts pabl and accrued liab"
gtrans["2200"] = "2620" ; gname["2620"] = "Ammounts pabl and accrued liab"
gtrans["2300"] = "2620" ; gname["2620"] = "Ammounts pabl and accrued liab"
gtrans["2400"] = "2620" ; gname["2620"] = "Ammounts pabl and accrued liab"
gtrans["2410"] = "2620" ; gname["2620"] = "Ammounts pabl and accrued liab"
gtrans["2420"] = "2620" ; gname["2620"] = "Ammounts pabl and accrued liab"
gtrans["2450"] = "2620" ; gname["2620"] = "Ammounts pabl and accrued liab"
gtrans["3300"] = "3849" ; gname["3849"] = "Retained earnings             "
gtrans["3400"] = "3540" ; gname["3540"] = "Contributed and other surplus "
gtrans["3600"] = "3540" ; gname["3540"] = "Contributed and other surplus "
gtrans["4000"] = "8000" ; gname["8000"] = "Trade sales of goods and serv "
gtrans["4100"] = "8000" ; gname["8000"] = "Trade sales of goods and serv "
gtrans["4200"] = "8120" ; gname["8120"] = "Commission income             "
gtrans["4300"] = "8235" ; gname["8235"] = "Income/loss on partnerships   "
gtrans["4400"] = "8095" ; gname["8095"] = "Dividend income               "
gtrans["4500"] = "8230" ; gname["8230"] = "Other Revenue                 "
gtrans["5000"] = "8300" ; gname["8300"] = "Opening inventory             "
gtrans["5100"] = "8320" ; gname["8320"] = "Purchases/cost of materials   "
gtrans["5110"] = "8450" ; gname["8450"] = "Other direct costs            "
gtrans["5200"] = "8500" ; gname["8500"] = "Closing inventory             "
gtrans["6000"] = "8860" ; gname["8860"] = "Professional fees             "
gtrans["6010"] = "8860" ; gname["8860"] = "Professional fees             "
gtrans["6030"] = "9060" ; gname["9060"] = "Salaries and wages            "
gtrans["6050"] = "9110" ; gname["9110"] = "Sub-contracts                 "
gtrans["6100"] = "8910" ; gname["8910"] = "Rental                        "
gtrans["6105"] = "8710" ; gname["8710"] = "Interest and bank charges     "
gtrans["6110"] = "8910" ; gname["8910"] = "Rental                        "
gtrans["6115"] = "8760" ; gname["8760"] = "Business tax                  "
gtrans["6120"] = "8910" ; gname["8910"] = "Rental                        "
gtrans["6125"] = "8810" ; gname["8810"] = "Office expenses               "
gtrans["6130"] = "8960" ; gname["8960"] = "Repairs and maintenance       "
gtrans["6150"] = "8810" ; gname["8810"] = "Office expenses               "
gtrans["6195"] = "9270" ; gname["9270"] = "Other expenses                "
gtrans["6200"] = "8760" ; gname["8760"] = "Business tax                  "
gtrans["6205"] = "8810" ; gname["8810"] = "Office expenses               "
gtrans["6210"] = "9270" ; gname["9270"] = "Other expenses                "
gtrans["6215"] = "9270" ; gname["9270"] = "Other expenses                "
gtrans["6220"] = "9270" ; gname["9270"] = "Other expenses                "
gtrans["6225"] = "8910" ; gname["8910"] = "Rental                        "
gtrans["6230"] = "8810" ; gname["8810"] = "Office expenses               "
gtrans["6235"] = "9270" ; gname["9270"] = "Other expenses                "
gtrans["6240"] = "8810" ; gname["8810"] = "Office expenses               "
gtrans["6250"] = "8810" ; gname["8810"] = "Office expenses               "
gtrans["6300"] = "8710" ; gname["8710"] = "Interest and bank charges     "
gtrans["6305"] = "8710" ; gname["8710"] = "Interest and bank charges     "
gtrans["6310"] = "8860" ; gname["8860"] = "Professional fees             "
gtrans["6315"] = "8760" ; gname["8760"] = "Business tax                  "
gtrans["6320"] = "8520" ; gname["8520"] = "Advertising and promotion     "
gtrans["6325"] = "9270" ; gname["9270"] = "Other expenses                "
gtrans["6330"] = "9270" ; gname["9270"] = "Other expenses                "
gtrans["6335"] = "8520" ; gname["8520"] = "Advertising and promotion     "
gtrans["6340"] = "9270" ; gname["9270"] = "Other expenses                "
gtrans["6350"] = "8590" ; gname["8590"] = "Bad debt expense              "
gtrans["6360"] = "9270" ; gname["9270"] = "Other expenses                "
gtrans["6400"] = "9270" ; gname["9270"] = "Other expenses                "
gtrans["6405"] = "9270" ; gname["9270"] = "Other expenses                "
gtrans["6410"] = "9270" ; gname["9270"] = "Other expenses                "
gtrans["6415"] = "9270" ; gname["9270"] = "Other expenses                "
gtrans["6420"] = "9270" ; gname["9270"] = "Other expenses                "
gtrans["6425"] = "8960" ; gname["8960"] = "Repairs and maintenance       "
gtrans["6430"] = "8710" ; gname["8710"] = "Interest and bank charges     "
gtrans["6435"] = "9270" ; gname["9270"] = "Other expenses                "
gtrans["6450"] = "9270" ; gname["9270"] = "Other expenses                "
gtrans["6495"] = "9270" ; gname["9270"] = "Other expenses                "
gtrans["6800"] = "8570" ; gname["8570"] = "Amortization intangable assets"
gtrans["6900"] = "8670" ; gname["8670"] = "Amortization tangable assets  "
	}
	{ 
	gtotal[gtrans[$1]] -= $4
	gtotal[gtrans[$1]] += $5
	}
END 	{
# Revenues
	printf "Revenues\n"
	printf "========\n\n"
	r1=prtotal("Sales And Services","^8[012]",1)
	printf "8089     Total Revenues                            %10.2f\n\n",r1
# Cost of goods
	c1=prtotal("Cost Of Goods Sold","^8[34]",-1)
	printf "8518     Cost Of Sales                             %10.2f\n",c1
	printf "                                                 ------------\n"
	printf "8519     Gross Profit/Loss                         %10.2f\n\n",r1-c1
# Expenses
	printf "Expenses\n"
	printf "========\n\n"
	e1=prtotal("Professional Services","^9[01]",-1)
	printf "Total Professional Services                        %10.2f\n\n",e1
	e2=prtotal("Operating Expenses","^8[789]",-1)
	printf "Total Operating Expenses                           %10.2f\n\n",e2
	e3=prtotal("Other Expenses","92",-1)
	printf "Total Other Expenses                               %10.2f\n\n",e3
# CCA
	a1=prtotal("Capital Cost Allowances","^8[56]7",1)
	printf "Total Capital Cost Allowances                      %10.2f\n\n",a1
# Total Expenses
	printf "                                                 ------------\n"
	t1=e1+e2+e3+a1
	printf "9368 Total Expenses                                %10.2f\n",t1
	printf "                                                 ------------\n\n"
	ni=r1-c1-t1;
	if (ni>=0)
	printf "9369 Net Income (Loss)                             %10.2f\n",ni
		else
	printf "9369 Net Income (Loss)                            (%10.2f)\n",-1*ni
	printf "                                                 ============\n\n"
# Assets
	printf "Assets\n"
	printf "======\n\n"
	a1=prtotal("Current Assets","^1[0-5]",-1)
	printf "1599 Total Current Assets                          %10.2f\n\n\n",a1
	a2=prtotal("Fixed Assets","^1[6-9]",-1)
	printf "1599 Total Fixed Assets                            %10.2f\n\n\n",a2
	a3=prtotal("Intangable Assets","^2[0-1]",-1)
	printf "1599 Total Intangable Assets                       %10.2f\n\n\n",a3
	printf "2599 Total Assets                                  %10.2f\n",a1+a2+a3
	printf "                                                 ------------\n\n"
	ta=a1+a2+a3
# Liabilities
	printf "Liabilities and Equities\n"
	printf "========================\n\n"
	l1=prtotal("Liabilities","^2[6-9]",1)
	printf "3499 Total Liabilities                             %10.2f\n\n",l1
	e1=prtotal("Equities","^3[0-9]",1)
	r2=ta-e1-ni-l1
	printf "3849     Retained Earnings (Capital)               %10.2f\n\n\n",r2
	printf "Total Equities                                     %10.2f\n\n\n",e1+r2
# Totals
	if (ni<0)
	printf "9369 Net Income                                   (%10.2f)\n",-1*ni
	else
	printf "9369 Net Income                                    %10.2f\n",ni
	printf "                                                 ------------\n"
	printf "3640 Total Liabilities and Equities                %10.2f\n",e1+l1+ni+r2
	printf "                                                 ------------\n\n"
	}
