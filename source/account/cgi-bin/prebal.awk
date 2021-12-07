# pre balance processing - calculate account balances
#  input: $1=acct $2=num $3=date $4=debit $5=credit $6=who
# output: $1=acct $2=name $3=debit $4=credit
BEGIN	{
	account["1500"]="Furniture & Fixtures    "
	account["1510"]="Equipment               "
	account["1520"]="Reference Library       "
	account["1530"]="Vehicle(s)              "
	account["1540"]="Computer Hardware       "
	account["1550"]="Computer Software       "
	account["1560"]="Chattels                "
	account["5100"]="Purchases for Resale    "
	account["5110"]="Packaging Materials     "
	account["5000"]="Opening Inventory       "
	account["5200"]="Closing Inventory       "
	account["6000"]="Professional Services   "
	account["6010"]="Secretarial Services    "
	account["6030"]="Commissions             "
	account["6050"]="Sub-Contracted Services "
	account["6100"]="Rent                    "
	account["6105"]="Mortgage Interest       "
	account["6110"]="Condominium Fees        "
	account["6115"]="Property Taxes          "
	account["6120"]="Insurance               "
	account["6360"]="Miscellaneous           "
	account["6125"]="Utilities               "
	account["6125"]="Utilities               "
	account["6130"]="Repairs & Maintenance   "
	account["6150"]="Other Building Expenses "
	account["6195"]="Personal Use            "
	account["6200"]="Business Taxes & Permits"
	account["6205"]="Telecommunications      "
	account["6210"]="Postage, Courier & Frght"
	account["6215"]="Materials & Supplies    "
	account["6220"]="Equipment Rent & Repair "
	account["6225"]="Liab or Bus Insurance   "
	account["6230"]="Printing & Copying      "
	account["6235"]="Ref Books & Magazines   "
	account["6240"]="Stationery & Cards      "
	account["6250"]="Other Office Expenses   "
	account["6300"]="Loan Interest           "
	account["6305"]="Bank Charges            "
	account["6310"]="Accounting & Legal Fees "
	account["6315"]="Association Dues        "
	account["6320"]="Business Promotion      "
	account["6325"]="Travel                  "
	account["6330"]="Conferences & Seminars  "
	account["6335"]="Advertising             "
	account["6340"]="Small Tools             "
	account["6350"]="Bad Debts               "
	account["6400"]="Garaging & Parking      "
	account["6405"]="Leasing                 "
	account["6410"]="Gasoline                "
	account["6415"]="Insurance               "
	account["6420"]="Plates & Permits        "
	account["6425"]="Repairs & Maintenance   "
	account["6430"]="Financing Charges       "
	account["6435"]="Washing & Waxing        "
	account["6450"]="Other Vehicle Expenses  "
	account["6495"]="Personal Use            "
	account["1000"]="Bank General            "
	account["2000"]="Accrued Expenses        "
	account["2100"]="GST Collected           "
	account["1100"]="GST Paid                "
	account["1200"]="Accounts Receivable     "
	account["2300"]="Accounts Payable        "
	account["4000"]="Sales Services          "
	account["4100"]="Sales Products          "
	account["2200"]="PST Collected           "
	account["3300"]="Retained Earnings       "
	account["4200"]="Commissions             "
	account["4300"]="Partnership Alloc T1013 "
	account["4400"]="Dividends               "
	account["4500"]="Other Income            "
	account["1010"]="Cash on Hand            "
	account["1020"]="Prepaid Federal Taxes   "
	account["1030"]="Prepaid Provincial Taxes"
	account["1040"]="Inventory               "
	account["1300"]="Notes Receivable        "
	account["2400"]="Notes Payable           "
	account["2410"]="Bank Loans              "
	account["1600"]="Elig. Capital Property  "
	account["1610"]="Organizational Expenses "
	account["2420"]="Income Taxes Payable    "
	account["3600"]="Shareholders Advance    "
	account["2450"]="Salary Payable          "
	account["6900"]="Depreciation            "
	account["3400"]="Capital Stock           "
	account["1050"]="Prepaid Expenses        "
	}
	{ balance[$1]-=$4; balance[$1]+=$5; }
END 	{
	for ( ac in balance )
		if ( balance[ac] < 0 )
			{
			printf("%s:%s:%f:0\n",
			ac,account[ac],balance[ac]*-1)
			dtotal+=balance[ac]*-1
			}
		else
			{
			printf("%s:%s:0:%f\n",
			ac,account[ac],balance[ac])
			ctotal+=balance[ac]
			}
	}
