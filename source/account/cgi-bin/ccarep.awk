# input: $1=acct $2=num $3=date $4=debit $5=credit $6=who
# output: $1=acct $2=num $3=date $4=debit $5=credit $6=who
# Produce CCA report and output records
#
# Print class totals 
function prtotal ( ) {
printf("-----------------------------------------------------------------------------------------------------\n")
printf("Totals %10.2f%10.2f%10.2f%10.2f%10.2f%10.2f%10.2f    %10.2f%10.2f\n\n\n",
	tfval,tfcost,tfadded,tfsale,tfucca,tf50,tfrucca,tfcca,tnewval)
		tfval=0 ; tfcost=0 ; tfadded=0 ; tfsale=0 ; tfucca=0 ; tf50=0
		tfrucca=0 ; tfcca=0 ; tnewval=0
	}
BEGIN	{
	class["500"] = 8  ; rate["500"] = 20
	class["510"] = 8  ; rate["510"] = 20
	class["520"] = 8  ; rate["520"] = 20
	class["530"] = 8  ; rate["530"] = 20
	class["540"] = 10 ; rate["540"] = 30
	class["550"] = 10 ; rate["550"] = 30
	class["560"] = 8  ; rate["560"] = 20
	class["600"] = 0  ; rate["600"] = 7
	class["610"] = 0  ; rate["610"] = 7
	num = 600
	}
		{  abase = substr($1,2,3) } 
$1  ~  "^7"	{  amort[abase]+=$5 } 
$1  ~  "^8"	{  cost[abase]+=$4 } 
$1  ~  "^1"	{  added[abase]+=$4 } 
$1  ~  "^1"	{  sold[abase]+=$5 } 
$1  ~  "^[178]"	{  crate[abase]=rate[abase] } 
END	{
printf("CC Acct     Value      Cost     Added      Sale      Ucca       F50   Adjucca Rat       Cca    Nvalue\n")
	lastrate = "BEGIN"
	for ( i in crate )
		{	
		 if ( lastrate != class[i] && lastrate != "BEGIN" )  prtotal() 
		lastrate = class[i] 

		fcost = cost[i]
		fval = cost[i]-amort[i]
		fadded = added[i]
		fsale = sold[i]
		fucca=(fadded+fval)-fsale
		f50=(fadded-fsale)/2
		if (f50<0) f50=0
		frucca=fucca-f50
		frate=rate[i]
		fcca=(frucca*frate)/100
printf("%3d%4d%10.2f%10.2f%10.2f%10.2f%10.2f%10.2f%10.2f%4.0f%10.2f%10.2f\n",
		class[i], i, fval, fcost, fadded, fsale, fucca, f50,
		frucca, frate, fcca, fucca-fcca)
		tfval+=fval ; tfcost+=fcost ; tfadded+=fadded
		tfsale+=fsale ; tfucca+=fucca ; tf50+=f50
		tfrucca+= frucca ; tfcca+=fcca ; tnewval+=(fucca-fcca)
		ncost=fcost+fadded;
		namort=amort[i]+fcca;
	 if ( cost[i] > 0)
		print "@8" i ":" num++ ":000000:" ncost ":0:Previous Balance"
	 if ( amort[i] > 0)
		print "@7" i ":" num++ ":000000:0:" namort ":Acum Amort"
	 if ( fcca > 0 && substr(i,1,1) == "5" )
		print "@6900:" num++ ":000000:0:" fcca ":Tangable"
	 if ( fcca > 0 && substr(i,1,1) == "6" )
		print "@6800:" num++ ":000000:0:" fcca ":Intangable"
		}
	prtotal()
	}
