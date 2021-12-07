#! /usr/bin/python
# Calculate principal withdrawl schedual for investments
def sched ( years , amt ):
	year = years	# Number of years
	diff = amt	# Difference from last year
	intrate = 5	# Interest rate (percent)
	rtotal = 0	# Running withdrawl total
        print "%4s %10s %10s %10s %10s" % ( 
	"Year", "Ammount", "Interest", "Difference" , "Total" )
	print
	while year :
		inter = amt * intrate / 100		
       		print "%4d %10.2f %10.2f %10.2f %10.2f" % (  
		year , amt  , inter , diff - amt , diff - amt + inter )
		rtotal = rtotal + inter + diff - amt 
       		diff = amt
       		amt = amt - ( amt  / year )
       		year = year  - 1
	print
	# Calculate the averaged deductions including interest
	print "Total" , rtotal , "Average" , rtotal / years
	print
	return
sched ( 20 , 148570)
sched ( 20 , 113000)
#sched ( 20 , 80000)
