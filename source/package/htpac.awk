# Package | Class | Version | URL | Description
BEGIN	{
	printf "<HTML>\n"
	printf "<TABLE border=1 cellpadding=0 cellspacing=1>\n"
	printf "<TD bgcolor=aqua color=white width=120>\n"
	printf "Package                       </TD>\n"
	printf "<TD bgcolor=aqua color=white width=80>\n"
	printf "Class                       </TD>\n"
	printf "<TD bgcolor=aqua color=white width=200>\n"
	printf "Version                       </TD>\n"
	printf "<TD bgcolor=aqua color=white width=400>\n"
	printf "URL                       </TD>\n"
	printf "<TD bgcolor=aqua color=white width=400>\n"
	printf "Description                       </TD>\n"
	printf "<TR>\n" 
	}
	{
	printf "<TD bgcolor=white color=white width=120>\n"
	printf "%s </TD>\n",$1
	printf "<TD bgcolor=white color=white width=80>\n"
	printf "%s </TD>\n",$2
	printf "<TD bgcolor=white color=white width=200>\n"
	printf "%s </TD>\n",$3
	printf "<TD bgcolor=white color=white width=400>\n"
	printf "<a href=\"%s\">%s</a>\n</TD>\n",$4, $4
	printf "<TD bgcolor=white color=white width=400>\n"
	printf "%s </TD>\n",$5
	printf "</TR>\n" 
	}
